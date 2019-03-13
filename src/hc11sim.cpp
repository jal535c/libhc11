#include "hc11sim.h"	
#include <stdio.h>		//para fopen, fgets, sscanf, fclose de HC11Sim:LoadS19File, 
#include "ram.h"


HC11Sim::HC11Sim() 
{
    cfg = None;        
}


HC11Sim::~HC11Sim() {
    if(cfg != None) {		//si está en una configuracion diferente de none, tiene que eliminar la memoria expandida
   		hc11.memory.RemoveMemoryObject(expanded_ram);	//elimina la ram expandida
        delete expanded_ram;    
    }
		
	cfg = None;
}


//añade memoria y cosas, para emular varias configuraciones, como la placa briefcase o rugwarrior
//despues tambien se le pueden añadir dispositivos a la simulacion, como un display de 7 segmentos en el briefcase,
//o motores en el rugwarrior

void HC11Sim::Config(HC11SimConfig ncfg) {
	int i;

    if(cfg != None) {
        hc11.memory.RemoveMemoryObject(expanded_ram);	//borra lo anterior
        delete expanded_ram;
    }
    
    cfg = ncfg;
    
    switch(cfg) {
        
		case BriefCase:

            //hc11.mode = SPECIAL_TEST;            
            hc11.mode = EXPANDED;            
            expanded_ram = new RAM(0xC000, 0x3FFF);                
            hc11.memory.AddMemoryObject(expanded_ram);                        
		    		    
		    SetupPseudoVectors(0xFFC0);
            break;

        case RugWarriorBootstrap:

            hc11.mode = BOOTSTRAP;            
            expanded_ram = new RAM(0x8000, 0x7FFF);                
            hc11.memory.AddMemoryObject(expanded_ram);                        
			            
            for(i=0; i<8; i++)
                hc11.regfile.PORTB.SetMode(i, Pin::AUTO);            
            for(i=0; i<8; i++)
                hc11.regfile.PORTC.SetMode(i, Pin::AUTO);            
            break;

        case RugWarriorExpanded:        

            hc11.mode = EXPANDED;                                   
            expanded_ram = new RAM(0x8000, 0x7FFF);                
            hc11.memory.AddMemoryObject(expanded_ram);                        
                  
            for(i=0; i<8; i++)
                hc11.regfile.PORTB.SetMode(i, Pin::AUTO);            
            for(i=0; i<8; i++)
                hc11.regfile.PORTC.SetMode(i, Pin::AUTO);            
    }    
}


//este metodo establece los pseudo vectores (para el modo single_chip), lo cual es normalmente
//hecho por el programa bootloader

void HC11Sim::SetupPseudoVectors(word_t vector_base_addr) {			
	hc11.memory.Write((vector_base_addr + ResetVector),0x00);
	hc11.memory.Write((vector_base_addr + ResetVector+1),0x00);		//el reset va a bf40, donde está el bootloader, que va a 0000
	
	//falta el vector de clock monitor, 
	//cop fail, 
	//illegal op-code

	hc11.memory.Write((vector_base_addr + SWIVector),0x00);			
	hc11.memory.Write((vector_base_addr + SWIVector+1),0xF4);
	
	hc11.memory.Write((vector_base_addr + XIRQVector),0x00);
	hc11.memory.Write((vector_base_addr + XIRQVector+1),0xF1);
	
	hc11.memory.Write((vector_base_addr + IRQVector),0x00);
	hc11.memory.Write((vector_base_addr + IRQVector+1),0xEE);
	
	hc11.memory.Write((vector_base_addr + RTIVector),0x00);
	hc11.memory.Write((vector_base_addr + RTIVector+1),0xEB);

	hc11.memory.Write((vector_base_addr + IC1Vector),0x00);
	hc11.memory.Write((vector_base_addr + IC1Vector+1),0xE8);

	hc11.memory.Write((vector_base_addr + IC2Vector),0x00);
	hc11.memory.Write((vector_base_addr + IC2Vector+1),0xE5);

	hc11.memory.Write((vector_base_addr + IC3Vector),0x00);
	hc11.memory.Write((vector_base_addr + IC3Vector+1),0xE2);

	hc11.memory.Write((vector_base_addr + OC1Vector),0x00);
	hc11.memory.Write((vector_base_addr + OC1Vector+1),0xDF);

	hc11.memory.Write((vector_base_addr + OC2Vector),0x00);
	hc11.memory.Write((vector_base_addr + OC2Vector+1),0xDC);

	hc11.memory.Write((vector_base_addr + OC3Vector),0x00);
	hc11.memory.Write((vector_base_addr + OC3Vector+1),0xD9);

	hc11.memory.Write((vector_base_addr + OC4Vector),0x00);
	hc11.memory.Write((vector_base_addr + OC4Vector+1),0xD6);

	hc11.memory.Write((vector_base_addr + I4O5Vector),0x00);
	hc11.memory.Write((vector_base_addr + I4O5Vector+1),0xD3);

	hc11.memory.Write((vector_base_addr + TOVector),0x00);
	hc11.memory.Write((vector_base_addr + TOVector+1),0xD0);

	hc11.memory.Write((vector_base_addr + PAOVVector),0x00);
	hc11.memory.Write((vector_base_addr + PAOVVector+1),0xCD);

	hc11.memory.Write((vector_base_addr + PAIVector),0x00);
	hc11.memory.Write((vector_base_addr + PAIVector+1),0xCA);	
	
	//falta el spi y 
	//sci
}


void HC11Sim::SetStartAddress(Word address) {	//este metodo pega la direccion en la localizacion del vector de reset del hc11
    switch(cfg) {
        case BriefCase:
        
		case RugWarriorExpanded:        
            hc11.memory.Write(0xFFFE, address.high());	//para briefcase o rugwarriorexpanded
            hc11.memory.Write(0xFFFF, address.low());
            break;
        
		case RugWarriorBootstrap:
            hc11.memory.Write(0xBFFE, address.high());
            hc11.memory.Write(0xBFFF, address.low());
            break;
    }
}


bool HC11Sim::LoadS19File(const char *fname) {		//carga un fichero .s19 en la memoria, le paso como parametro el path
    char line[256];					//va guardando las lineas del fichero .s19
    int bc, val, cksm, fcksm, i;	//byte count, value, checksum (lo voy calculando), final checksum (del fichero), i para bucle
    long addr;						//direccion inicial

    bool retval = true;				//indica si se cargó con exito o no

    FILE *fin;
    
    fin = fopen(fname, "r");	//fopen devuelve un puntero a la estructura FILE, le paso "r" que es para read only

    if(!fin) {
        log("couldn't open s19 file!\n", 0);
        return false;
    }

    while (fgets(line, 256, fin) != NULL) {		//lee caracteres de fin y los mete en line, hasta 255 o CR
		
    	if (line[0] == 'S') {
			 
            switch(line[1]) {

                case '1':
                    sscanf(&line[2], "%2x", &bc);    //empieza a cojer desde el tercer caracter, y guarda 2 en bc
					sscanf(&line[4], "%4x", &addr);  //empieza a cojer desde el quinto caracter, y guarda 4 en addr
					cksm = bc + (addr & 0xFF) + ((addr >> 8) & 0xFF);   
								                    
					for (i=0; i<(bc-3); i++) {    //menos 3 es por los 2 bytes de la direccion y 1 byte del checksum
						sscanf(&line[i*2 + 8], "%2x", &val);	//va saltando de 2 en 2, empieza por el octavo, y guarda 2 en val
						hc11.memory.Write(addr+i, val);	
						cksm += val;	
                    }
                    
                    sscanf(&line[(bc-3)*2 + 8], "%2x", &fcksm);		//lee el ultimo byte de la linea, el final checksum
						                   
					cksm = ~cksm & 0xFF;	//complemento a 1 (invierte)
														        
                    if (cksm != fcksm) {	
						log("s19 file checksum failed!\n", 0);
						retval = false;
					}
                    break;                                           
                
				case '9':
                    break;                       
			 } 
		 
		 }  
         else 
         	log("unknown line type in s19 file!\n", 0);    

	} 	   
	
    fclose(fin);
	return retval;
}

