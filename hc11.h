#ifndef __HC11_H__		//compilacion condicional
#define __HC11_H__


#include "bytetype.h"	//bit_t, byte_t (solo en Fetch()), word_t, Byte, Word
#include "wordtype.h"
//#include "register.h"
#include "regfile.h"	//clase RegisterFile, enum Mode

//#include "hc11io.h"		//clase Pin
//#include "pin.h"

#include "memoryMap.h"		//clase MemoryMap, RegisterMap, RAM, ROM
class RegisterMap;
class RAM;
class ROM;

#include "ccr.h"	//clase CCRReg, macro log

//#include "timer.h"		//clase TimerSubSystem
//#include "iosystem.h"		//clase HandShakeIOSubSystem
class TimerSubSystem;
//class HandshakeIOSubSystem;


	
//mas adelante se incluye "opcode.h", desde dentro de la clase



#define N_OPCODES 256
#define N_IBIT_INTERRUPTS 16




//zona de vectores en modo Single chip empieza en: FFC0, pero reserva unas cuantas direcciones, el primer vector esta en FFD6
//zona de vectores modo Bootstrap empieza en: BFC0, pero reserva unas cuantas direcciones, el primer vector esta en BFD6

//trabajo con direcciones relativas, para no volver a cambiar los valores si cambia el modo

//sci
//spi
#define PAIVector   (0x1A)	//dir relativa (0x1A) = dir absoluta del vector (0xBFDA) - base que depende del modo (0xBFC0)
#define PAOVVector  (0x1C)
#define TOVector    (0x1E)
#define I4O5Vector  (0x20)
#define OC4Vector   (0x22)
#define OC3Vector   (0x24)
#define OC2Vector   (0x26)
#define OC1Vector   (0x28)
#define IC3Vector   (0x2A)
#define IC2Vector   (0x2C)
#define IC1Vector   (0x2E)
#define RTIVector   (0x30)
#define IRQVector   (0x32)
#define XIRQVector  (0x34)
#define SWIVector   (0x36)
#define IllegalVector (0x38)
//cop fail
//clock monitor
#define ResetVector (0x3E)




typedef void (*opcode_t)(HC11 *);				
typedef bool (*interrupt_service_t)(HC11 *); 


class HC11 {
public:
	Byte ACCA;	
	Byte ACCB;		
	
	CCRReg CCR;	        
	
	Word IX;
	Word IY;
	
	Word PC;
	Word SP;				//no se inicia, valor a 0, deberia ser 512 en decimal, realmente solo es un almacen, funciona bien
					

	long int timer_count;	//contador continuo de tiempo, indica pulsos en señal E, TCNT, se incrementa en la funcion _clock()
	
	//Pin STRA;       //pin nº4 STRA/AS, creo k no se usa, para los modos handshake?
	//Pin STRB;       //pin nº6 STRB/R-W, se usa en el display 7seg?      


	
	MemoryMap memory;         //no es puntero, su constructor no recibe parametros, puedo usar desde ya
	
	RegisterMap* regmap;        //son punteros: su constructor recibe parametros, se los envio al iniciarlos con new
	RAM* chip_ram;
	ROM* boot_rom;    

	
	RegisterFile regfile;

   
	TimerSubSystem* timer_system;  
	//HandshakeIOSubSystem *handshake_system;     //quitar portB
	// ADSubSystem  *ad_system;
	// SPISubSystem *spi_system;
	// SCISubSystem *sci_system;    




	HC11();     
	~HC11();    

	
	Mode mode;      //Mode es un enum de regfile.h
		
	void Reset();
	void Step(); 



	void XIRQLevel(bit_t);	//escribe y lee el atributo xirq_level
	bit_t XIRQLevel();

	void IRQLevel(bit_t);    //escribe y lee el atributo irq_level
	bit_t IRQLevel();


	bool IRQInterrupt();
  
	
	friend class TimerSubSystem;	//no hace falta
	friend class PortARegister; 	//no hace falta


private:        
	bool xbit_cleared;		//indica que CCR.X ha sido puesto ha cero 
	
	bool stopped;	// usadas en los opcodes STOP y WAIT    
	bool waiting;	//son puestas a false en el metodo Reset(), que es llamado desde el constructor

	bit_t xirq_level;	//nivel en el pin /xirq de entrada al MCU
	bit_t irq_level;	//nivel en el pin /irq de entrada al mcu
	bit_t irq_edge;		//variable k indica si ha habido flanco de bajada en /irq


	
	void _clock(); 	
	void LoadROMBootLoader(word_t vector_base_addr);  //para bootstrap mode     
 



protected:	//las clases derivadas pueden acceder
	static opcode_t opcode[N_OPCODES];		//se inicializa el array de punteros a funciones en opcodes.cpp
	static opcode_t opIYcode[N_OPCODES];	//array de punteros a funciones extendidas (empiezan por 18), para 1A y CD uso un switch
											

	word_t  vector_base_addr;

	
	static interrupt_service_t psel_table[N_IBIT_INTERRUPTS];   //array de 16 dir de funciones, la inicializacion está en interrup.cpp


	
	void Fetch();
	
	void Fetch(word_t address);
		
	
	#include "opcodes.h"	//contiene el prototipo de las funciones de las instrucciones

							
			 
	void FetchVector(word_t num);

	void ResolveInterrupt();	   

	
	//cada vez k termina de ejecutar una instruccion comprueba si hay interrupciones pendientes (si tiene CCR.I a 0)
	//realmente se hace al principio de la secuencia de instruccion
	//aqui compruebo las interrupciones de: irq, tmsk1 (comparador y capturador) y tmsk2 (timer, rti, acumulador de pulsos)
	inline bool InterruptPending() {            

		return (IRQInterrupt() || (regfile.TFLG1)&(regfile.TMSK1) || (regfile.TFLG2)&(regfile.TMSK2));
	
	}
	

	static bool ServiceTO(HC11 *hc11);		//la implementacion de estas funciones está en interrup.cpp, y no en hc11.cpp
	static bool ServicePAOV(HC11 *hc11);
	static bool ServicePAI(HC11 *hc11);
	static bool ServiceSPI(HC11 *hc11);
	static bool ServiceSCI(HC11 *hc11);
	static bool ServiceIRQ(HC11 *hc11);    //reserved?
	static bool ServiceIRQIO(HC11 *hc11);    //irq externa?
	static bool ServiceRTI(HC11 *hc11);
	static bool ServiceIC1(HC11 *hc11);
	static bool ServiceIC2(HC11 *hc11);
	static bool ServiceIC3(HC11 *hc11);
	static bool ServiceOC1(HC11 *hc11);
	static bool ServiceOC2(HC11 *hc11);
	static bool ServiceOC3(HC11 *hc11);
	static bool ServiceOC4(HC11 *hc11);
	static bool ServiceI4O5(HC11 *hc11);
 
};


#endif