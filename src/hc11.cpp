#include "hc11.h"

//#include "hc11misc.h"
#include "timer.h"
//#include "iosystem.h"

#include "registerMap.h"
#include "ram.h"
#include "rom.h"


HC11::HC11() {
    timer_system = new TimerSubSystem(this);		
        
    chip_ram = new RAM(0, 512);					//le paso la base y el tamaño
    regmap = new RegisterMap(0x1000, &regfile);  //le paso la base y la direccion de mi RegisterFile 
    
	RAM* vector_ram = new RAM(0xFF00, 0xFF);		//indica donde comienzan los vectores en modo single chip

    boot_rom = new ROM(0xBF00, 0xFF);
    boot_rom->enable = 0;	//inicialmente la rom esta deshabilitada hasta k sepamos el modo

	regfile.INIT.regmap = (void*)regmap;		//vincula atributos de la clase init_c que está dentro de la clase registerfile
    regfile.INIT.chip_ram = (void*)chip_ram;	

    memory.AddMemoryObject(regmap);			//añade los elementos a la lista (atributo del objeto memory)
    memory.AddMemoryObject(vector_ram);
    memory.AddMemoryObject(chip_ram);
    memory.AddMemoryObject(boot_rom);
            
    xirq_level = 1;	//reposo a nivel alto
    irq_level = 1;
    irq_edge = 0;		//no hay flanco

  	regfile.PORTA.SetHC11(this);
	
	//mode = BOOTSTRAP;
    mode = SPECIAL_TEST;	//inicia el modo como special test, igual que bootstrap pero sin rom, mirar Reset()
    Reset();
}


//elimina los punteros
//los de los MemoryObject no hace falta, ya que se destruyen al llamar al detructor de memory (es de tipo MemoryMap)   
    
HC11::~HC11() {
    delete timer_system;
}


void HC11::Reset() {
	CCR.I(1);	//bit de interrupcion enmascarable a 1 (no permite int), si quiero detectar int uso instruccion CLI en mi programa
	CCR.X(1);	//bit de interrupcion no enmascarable a 1 (no permite int NMI)
	
	//despues de un reset, el valor de los registros de la cpu es indeterminado, excepto para ccr
	//es en el bootloader donde se pone SP=0x1FF, IX=0x1000
	SP = 0x01FF;
	IX = 0x1000;

	regfile.Reset(mode);   //reset de regfile, resetea el bloque de registros segun el modo
		
	timer_count = 0;
        
    stopped = false;
    waiting = false;

	if (mode == BOOTSTRAP) {  
		regfile.DWOM(1);	//es del registro SPCR de SPI

		vector_base_addr = 0xBFC0;		//pone la direccion base de comienzo de los vectores en modo bootstrap
	    boot_rom->enable = 1;			//ahora si habilita la rom
	
		LoadROMBootLoader(vector_base_addr);	
	}
	else if (mode == SPECIAL_TEST) {
		vector_base_addr = 0xBFC0;
	}
	else if (mode == EXPANDED || mode == SINGLE_CHIP) {
		vector_base_addr = 0xFFC0;
	}
		
    FetchVector(ResetVector);
    xbit_cleared = false;
}


//esta funcion es llamada desde el Doc::OnSimmulatorStep
//hará la ejecucion paso a paso mediante un opcode entero, o resuelve interrupcion

void HC11::Step() {			
	if ( !(CCR.X() || XIRQLevel()) ) {		// si la mascara está a 0 y se dá el pin (activos a nivel bajo), entra en if
        xbit_cleared = true;	
        StackRegisters();       //meto los registros en la pila, es un metodo del fichero opcodes.h
                
        _clock();
        CCR.X(1);			//no permito int, para evitar anidamiento, cuando salga con instru RTI retorna los registros (inclusive CCR)
                
        _clock();
        CCR.I(1);           
                                
        FetchVector(XIRQVector);        //busca el vector de XIRQ directamente
    } 
    else if (!CCR.I() && InterruptPending()) {   // Si I es 0 y hay int pendiente 
        
		//I lo pongo a 0 por programa con instruccion CLI, ya que por defecto despues de un reset es 1
		StackRegisters();	//despues al ejecutar la instruccion RTI se retornan los registros (incluido el valor de CCR.I que era 0)
        ResolveInterrupt();
  	}
    else 
		Fetch();	// Si no hay peticion de int, busca el siguiente codigo de operacion
}


void HC11::XIRQLevel(bit_t level) {
    xirq_level = level;
}


void HC11::IRQLevel(bit_t level) {		//para escribir un nivel uso un boton en la interfaz
    if (regfile.IRQE() && irq_level && (level == 0))		//si está activado bit config por flanco, el nivel actual es 1, y el k meto es 0
		irq_edge = 1;								//hay flanco
    
    irq_level = level;		//escribe el valor
}


bit_t HC11::XIRQLevel() {
    return (xirq_level);
}


bit_t HC11::IRQLevel() {
    return (irq_level);
}


bool HC11::IRQInterrupt() {
	return ( (regfile.IRQE() && irq_edge)  ||  (!regfile.IRQE() && !irq_level) );
}


//es llamada en cada ciclo de E internamente por el simulador
//mas tarde puede contener un call back externo que se puede usar para mas precision en el timer

void HC11::_clock() {
    timer_system->ClockSystem();	
    timer_count++;					
}


//carga la Rom con los vectores (valores de direccion espejo en ram)

void HC11::LoadROMBootLoader(word_t vector_base_addr) {		
	boot_rom->Program((vector_base_addr + ResetVector), 0x00);	//en realidad es bf4c, pero es la dir del bootloader (programa
	boot_rom->Program((vector_base_addr + ResetVector+1), 0x00);	//que carga codigo en ram, y despues va a la dir 0000)
																	
	//clock monitor
	//cop fail
	//illegal opcode

	boot_rom->Program((vector_base_addr + SWIVector), 0x00);
	boot_rom->Program((vector_base_addr + SWIVector+1), 0xF4);
	
	boot_rom->Program((vector_base_addr + XIRQVector), 0x00);
	boot_rom->Program((vector_base_addr + XIRQVector), 0xF1);
	
	boot_rom->Program((vector_base_addr + IRQVector), 0x00);
	boot_rom->Program((vector_base_addr + IRQVector+1), 0xEE);
	
	boot_rom->Program((vector_base_addr + RTIVector), 0x00);
	boot_rom->Program((vector_base_addr + RTIVector+1), 0xEB);

	boot_rom->Program((vector_base_addr + IC1Vector), 0x00);
	boot_rom->Program((vector_base_addr + IC1Vector+1), 0xE8);

	boot_rom->Program((vector_base_addr + IC2Vector), 0x00);
	boot_rom->Program((vector_base_addr + IC2Vector+1), 0xE5);

	boot_rom->Program((vector_base_addr + IC3Vector), 0x00);
	boot_rom->Program((vector_base_addr + IC3Vector+1), 0xE2);

	boot_rom->Program((vector_base_addr + OC1Vector),0x00);
	boot_rom->Program((vector_base_addr + OC1Vector+1),0xDF);

	boot_rom->Program((vector_base_addr + OC2Vector), 0x00);
	boot_rom->Program((vector_base_addr + OC2Vector+1), 0xDC);

	boot_rom->Program((vector_base_addr + OC3Vector), 0x00);
	boot_rom->Program((vector_base_addr + OC3Vector+1), 0xD9);

	boot_rom->Program((vector_base_addr + OC4Vector), 0x00);
	boot_rom->Program((vector_base_addr + OC4Vector+1), 0xD6);

	boot_rom->Program((vector_base_addr + I4O5Vector), 0x00);
	boot_rom->Program((vector_base_addr + I4O5Vector+1), 0xD3);

	boot_rom->Program((vector_base_addr + TOVector), 0x00);
	boot_rom->Program((vector_base_addr + TOVector+1), 0xD0);

	boot_rom->Program((vector_base_addr + PAOVVector), 0x00);
	boot_rom->Program((vector_base_addr + PAOVVector+1), 0xCD);

	boot_rom->Program((vector_base_addr + PAIVector), 0x00);
	boot_rom->Program((vector_base_addr + PAIVector+1), 0xCA);

	//spi
	//sci
}


//coge el opcode apuntado por PC y ejecuta (llama a esa funcion-instruccion). PC es incrementado antes de que el opcode sea llamado
//consume un ciclo de reloj

void HC11::Fetch() {
	byte_t num;    
   
    _clock();		//consume el primer ciclo (del total k use la instruccion) al buscar el opcode
		    
    num=memory[PC];
    PC++;

    opcode[num](this);	//ejecuta la instruccion. EJ: LDAA usa 2 ciclos, pero 1 ya esta consumido en esta funcion
}


//carga PC con una direccion (de 16bits) pasada como parametro, y despues llama a Fetch()

void HC11::Fetch(word_t address) {
    Word addr;
    
	addr = address;

    _clock();
    PC.low(addr.low());
    _clock();
    PC.high(addr.high());    

    Fetch();
}


//carga PC con una direccion, pero no llama al opcode (no ejecuta la instruccion)
//esto se hará en el siguiente ciclo de reloj
//la direccion es un offset de la direccion del vector base
//se llama usando los #define del fichero hc11.h

void HC11::FetchVector(word_t address) {	//ej: para TOVector address es 0x1E
	word_t vec;
   
    vec = address + vector_base_addr;		//vector_base_addr es un word_t, se inicia en el metodo Reset(), para Bootstrap es 0xbfc0
	
	log("fetching vector 0x%X\n", (int)vec);	//define macroinstruccion log, está en hc11misc.h
	
    _clock();
    PC.high(memory[vec]);		//carga el Program Counter con el contenido de la dir del vector, 
    _clock();
    PC.low(memory[vec+1]);		//para TOVector debe ser 0x00D0, se carga en la funcion LoadROMBootLoader()
}


//cuando una interrupcion es detectada en la rutina HC11::Step(), se llama a esta funcion para 
//resolverla y cargar PC con la direccion de comienzo de la rutina (o servicio) de interrupcion

void HC11::ResolveInterrupt() {		//resuelve prioridad de las int
	if ( !(CCR.X() || XIRQLevel()) ) {                                
		xbit_cleared = true;

        _clock();
        CCR.X(1);
        CCR.I(1);

        FetchVector(XIRQVector);                      
    }
    else {
        CCR.I(1);	//inhibe las interrupciones para evitar anidamiento, 
        
		//seria la primera, codigo 0110 despues de un reset, ServiceIRQ(hc11), se puede cambiar escribiendo en el registro HPRIO
        if (psel_table[regfile.PSEL()](this))	
			;	
        else if (ServiceIRQIO(this))		//las vá mirando todas (hace muestreo, polling)
			;							
        else if (ServiceRTI(this))
			;
        else if (ServiceIC1(this))
			;                    
        else if (ServiceIC2(this))
			;                    
        else if (ServiceIC3(this))
			;            
        else if (ServiceOC1(this))
			;           
        else if (ServiceOC2(this))
			;           
        else if (ServiceOC3(this))
			;          
        else if (ServiceOC4(this))
			;           
        else if (ServiceI4O5(this))
			;
        else if (ServiceTO(this))
			;
        else if (ServicePAOV(this))
			;
        else if (ServicePAI(this))
			;
        
        else 
			FetchVector(IRQVector);        //interrupcion falsa alarma
    }        
}

