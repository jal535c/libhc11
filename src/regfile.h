//contiene la definicion de todos los registros y sus bits
//accesibles individualmente (consultores y modificadores)

#ifndef __REGFILE_H__
#define __REGFILE_H__


//#include "hc11io.h"		
				
#include "portRegister.h"
#include "portARegister.h"
//#include "portBRegister.h"	//quitar portB
#include "ddRegister.h"

#include "byte.h"
#include "word.h"
//#include "register.h"		
#include "byteRegister.h"
#include "wordRegister.h"

//#include "memory.h"		


//mascaras de los bit field

#define MASK_01		0x3		//0000 0011
#define MASK_23		0xC		//0000 1100
#define MASK_45		0x30	//0011 0000
#define MASK_67		0xC0	//1100 0000
#define MASK_0123	0xF		//0000 1111
#define MASK_4567	0xF0	//1111 0000
#define MASK_34567	0xF8	//1111 1000
#define MASK_12		0x6		//0000 0110
#define MASK_012	0x7		//0000 0111


typedef enum { BOOTSTRAP, SPECIAL_TEST, SINGLE_CHIP, EXPANDED } Mode;


class RegisterFile {

public:		

	class TCTL1_C : public ByteRegister {
	public:
		void Write(byte_t data);	    		
		PortRegister* portA;	
	};

	class OC1M_C : public ByteRegister {
	public:
		void Write(byte_t data);
		PortRegister* portA;
		ByteRegister* pactl;
		ByteRegister* tctl1;
	};

	class PACTL_C : public ByteRegister {
	public:
		void Write(byte_t data);
		PortRegister* portA;
		ByteRegister* oc1m;
	};

	class INIT_C : public ByteRegister {
	public:
		void Write(byte_t data);
		void* regmap;				//en el constructor de HC11 se vinculan con HC11::regmap y HC11::chip_ram
		void* chip_ram;				
	};

	class HPRIO_C : public ByteRegister {
	public:
		void Write(byte_t data);
	//	word_t* pvector;
	};


	class TFLG_C: public ByteRegister {
	public:
		void Write(byte_t data);
	//	void Set(byte_t val);	
	};

	class CFORC_C: public ByteRegister {
	public:
		void Write(byte_t data);
		PortRegister* portA;
		ByteRegister* oc1m;
		ByteRegister* oc1d;
		ByteRegister* tctl1;
	};

	class SPCR_C : public ByteRegister {
	public:
		void Write(byte_t data);	    		
		DDRegister* ddrd;
		PortRegister* portD;
	};

	

	//variables miembro, de acceso publico, de la clase RegisterFile

	PortARegister PORTA;		
	// Reserved at 0x1001
	ByteRegister PIOC;
	PortRegister PORTC;
	PortRegister PORTB;		//quitar portB
	ByteRegister PORTCL;
	// Reserved at 0x1006
	DDRegister DDRC;
	PortRegister PORTD;
	DDRegister DDRD;
	PortRegister PORTE;
	CFORC_C			 CFORC;	// a custom reg
	OC1M_C			 OC1M;  // a custom reg
	ByteRegister OC1D;  // a custom reg
	WordRegister TCNT;
	
	WordRegister TIC1;
	WordRegister TIC2;
	WordRegister TIC3;
	WordRegister TOC1;
	WordRegister TOC2;
	WordRegister TOC3;
	WordRegister TOC4;
	WordRegister TI4O5;
	
	TCTL1_C			 TCTL1;  // a custom reg
	ByteRegister TCTL2;  // a custom reg
	ByteRegister TMSK1;  // a custom reg
	TFLG_C      TFLG1;  // a custom reg
	ByteRegister TMSK2;  // a custom reg
	TFLG_C      TFLG2;  // a custom reg
	PACTL_C			 PACTL;  // a custom reg
	ByteRegister PACNT;
	SPCR_C		 SPCR;  // a custom reg
	ByteRegister SPSR;  // a custom reg
	ByteRegister SPDR;
	ByteRegister BAUD;  // a custom reg
	ByteRegister SCCR1;  // a custom reg
	ByteRegister SCCR2;  // a custom reg
	ByteRegister SCSR;  // a custom reg
	ByteRegister SCDR;  // a custom reg

	ByteRegister ADCTL;  // a custom reg
	ByteRegister ADR1;
	ByteRegister ADR2;
	ByteRegister ADR3;
	ByteRegister ADR4;
	ByteRegister BPROT;  // a custom reg
	ByteRegister EPROG;  // a custom reg, este tambien es reservado
	// Reserved at 0x1037
	// Reserved at 0x1038
	ByteRegister OPTION;  // a custom reg
	ByteRegister COPRST;
	ByteRegister PPROG;  // a custom reg
	HPRIO_C     HPRIO;  // a custom reg
	INIT_C			 INIT;  // a custom reg
	ByteRegister TEST1;  // a custom reg
	ByteRegister CONFIG;  // a custom reg
	
	RegisterFile();
	void Reset(Mode mode= SPECIAL_TEST);	//esta funcion no está implementada


/*******************************************************************************

Register Bit Functions - Accessors and Mutators (consultores y modificadores)	
		
//funciones para acceder a los bits de los registros, leen o escriben

//pongo todos los bits, pero solo uso unos pocos
//normalmente los leo para consultar
//pero no los escribo, ya se hace al cargar un fichero en la memoria

//son como funciones set y get, metodos para leer o escribir los atributos bit o byte desde fuera
//ya que todas derivan de la clase ByteRegister, y esta de la clase Byte
//todas son inline: implementadas aquí

*******************************************************************************/


	////////////////////////////////////////////////////////
	// OPTION Register 
	// Accessors
	// CR (cop rate) son 2 bits, por tanto al leer o escribir hay que usar mascaras
	inline byte_t CR()	{ return (OPTION.byte & 0x3); }		//& AND bit a bit, mascara, devuelve un byte pero solo miro bit0 y bit1
	// Bit 2 is unused
	inline bit_t CME()	{ return OPTION.bit3(); }
	inline bit_t DLY()	{ return OPTION.bit4(); }
	inline bit_t IRQE()	{ return OPTION.bit5(); }
	inline bit_t CSEL()	{ return OPTION.bit6(); }
	inline bit_t ADPU()	{ return OPTION.bit7(); }

	// Mutators, para CR escribo un byte pero solo tengo en cuenta los 2 primeros bits

	//hago una AND bit a bit con el byte y 1111 1100, para preparar los 2 bits de CR y no tocar los otros
	//como el valor que meto es de tipo byte_t, le hago una mascara con 0000 0011, para solo tener en cuenta los 2 ultimos bits
	//hago una OR bit a bit entre el byte preparado y mi valor preparado

	inline void CR(byte_t val)	{ OPTION.byte = OPTION.byte & (~MASK_01)  |	 (val & MASK_01); }	// ~ es el complemento a 1
	
	inline void CME(bit_t val)	{ OPTION.bit3(val); }
	inline void DLY(bit_t val)	{ OPTION.bit4(val); }
	inline void IRQE(bit_t val)	{ OPTION.bit5(val); }
	inline void CSEL(bit_t val)	{ OPTION.bit6(val); }
	inline void ADPU(bit_t val)	{ OPTION.bit7(val); }


	////////////////////////////////////////////////////////
	// PPROG Register
	// Accessors
	inline bit_t EPGM()		{ return PPROG.bit0(); }
	inline bit_t EELAT()	{ return PPROG.bit1(); }
	inline bit_t ERASE()	{ return PPROG.bit2(); }
	inline bit_t ROW()		{ return PPROG.bit3(); }
	inline bit_t BYTE()		{ return PPROG.bit4(); }
	inline bit_t ELAT3()	{ return PPROG.bit5(); }	//solo en mc68hc711e9, pone ELAT3 ya que hay otro ELAT en el registro EPROG
	inline bit_t EVEN()		{ return PPROG.bit6(); }
	inline bit_t ODD()		{ return PPROG.bit7(); }

	// Mutators
	inline void EPGM(bit_t val)		{ PPROG.bit0(val); }
	inline void EELAT(bit_t val)	{ PPROG.bit1(val); }
	inline void ERASE(bit_t val)	{ PPROG.bit2(val); }
	inline void ROW(bit_t val)		{ PPROG.bit3(val); }
	inline void BYTE(bit_t val)		{ PPROG.bit4(val); }
	inline void ELAT3(bit_t val)	{ PPROG.bit5(val); }	//solo en mc68hc711e9
	inline void EVEN(bit_t val)		{ PPROG.bit6(val); }
	inline void ODD(bit_t val)		{ PPROG.bit7(val); }


	////////////////////////////////////////////////////////
	// HPRIO Register	
	// Accessors
	// PSEL is 4 bits
	inline byte_t PSEL() { return HPRIO.byte & 0xF; }
	inline bit_t IRVNE() { return HPRIO.bit4(); }
	inline bit_t MDA()	 { return HPRIO.bit5(); }
	inline bit_t SMOD()	 { return HPRIO.bit6(); }
	inline bit_t RBOOT() { return HPRIO.bit7(); }

	// Mutators
	// PSEL is 4 bits
	inline void PSEL(bit_t val)	 { HPRIO.byte = HPRIO.byte & (~MASK_0123) |		(val & MASK_0123); }
	inline void IRVNE(bit_t val) { HPRIO.bit4(val); }
	inline void MDA(bit_t val)	 { HPRIO.bit5(val); }
	inline void SMOD(bit_t val)	 { HPRIO.bit6(val); }
	inline void RBOOT(bit_t val) { HPRIO.bit7(val); }


	////////////////////////////////////////////////////////
	// TEST1 Register
	// Accessors
	inline bit_t TCON()	{ return TEST1.bit0(); }
	inline bit_t FCOP()	{ return TEST1.bit1(); }
	inline bit_t FCM()	{ return TEST1.bit2(); }
	inline bit_t DISR()	{ return TEST1.bit3(); }
	inline bit_t CBYP()	{ return TEST1.bit4(); }
	inline bit_t OCCR()	{ return TEST1.bit5(); }
	
	inline bit_t TILOP(){ return TEST1.bit7(); }

	// Mutators
	inline void TCON(bit_t val)	{ TEST1.bit0(val); }
	inline void FCOP(bit_t val)	{ TEST1.bit1(val); }
	inline void FCM(bit_t val)	{ TEST1.bit2(val); }
	inline void DISR(bit_t val)	{ TEST1.bit3(val); }
	inline void CBYP(bit_t val)	{ TEST1.bit4(val); }
	inline void OCCR(bit_t val)	{ TEST1.bit5(val); }
	
	inline void TILOP(bit_t val){ TEST1.bit7(val); }
	

	////////////////////////////////////////////////////////
	// CONFIG Register
	// Accessors
	inline bit_t EEON()		{ return CONFIG.bit0(); }
	inline bit_t ROMON()	{ return CONFIG.bit1(); }
	inline bit_t NOCOP()	{ return CONFIG.bit2(); }
	inline bit_t NOSEC()	{ return CONFIG.bit3(); }
	
	inline byte_t EE()		{ return (CONFIG.byte & MASK_4567)>>4;	}
	
	// Mutators
	inline void EEON(bit_t val)		{ CONFIG.bit0(val); }
	inline void ROMON(bit_t val)	{ CONFIG.bit1(val); }
	inline void NOCOP(bit_t val)	{ CONFIG.bit2(val); }
	inline void NOSEC(bit_t val)	{ CONFIG.bit3(val); }
	
	inline void EE(byte_t val)		{ CONFIG.byte = CONFIG.byte & (~MASK_4567)|(val<<4) & MASK_4567; }	


	////////////////////////////////////////////////////////
	// TCTL1 Register
	// Accessors
	inline bit_t OL5()	{ return TCTL1.bit0(); }
	inline bit_t OM5()	{ return TCTL1.bit1(); }
	inline bit_t OL4()	{ return TCTL1.bit2(); }
	inline bit_t OM4()	{ return TCTL1.bit3(); }
	inline bit_t OL3()	{ return TCTL1.bit4(); }
	inline bit_t OM3()	{ return TCTL1.bit5(); }
	inline bit_t OL2()	{ return TCTL1.bit6(); }
	inline bit_t OM2()	{ return TCTL1.bit7(); }
	
	
	inline void OL5(bit_t val)	{ 
		
		TCTL1.bit0(val);		//escribo en el bit correspondiente del registro

		if (OL5() || OM5())		//funcion OL5() me devuelve el bit0 de TCTL1, hago OR normal con el otro bit
			PORTA.SetMode(3, Pin::AUTO);		//si alguno de los 2 es 1, el if es true: pone el bit3 del port A en modo auto
		else 
			PORTA.SetMode(3, Pin::OUTPUT);	//timer desconectado, puedo usar el puertoA de modo normal, pin3 de salida
	
	}

	inline void OM5(bit_t val)	{		//igual que para OL5
		TCTL1.bit1(val); 
		if (OL5() || OM5()) 
			PORTA.SetMode(3, Pin::AUTO); 
		else 
			PORTA.SetMode(3, Pin::OUTPUT); 
	}
	
	inline void OL4(bit_t val)	{ 
		TCTL1.bit2(val); 
		if (OL4() || OM4())														
			PORTA.SetMode(4, Pin::AUTO);
		else  
			PORTA.SetMode(4, Pin::OUTPUT); 
	}

	inline void OM4(bit_t val)	{ 
		TCTL1.bit3(val); 
		if (OL4() || OM4()) 
			PORTA.SetMode(4, Pin::AUTO); 
		else 
			PORTA.SetMode(4, Pin::OUTPUT); 
	}

	inline void OL3(bit_t val)	{ 
		TCTL1.bit4(val); 
		if (OL3() || OM3()) 
			PORTA.SetMode(5, Pin::AUTO); 
		else 
			PORTA.SetMode(5, Pin::OUTPUT); 
	}

	inline void OM3(bit_t val)	{ 
		TCTL1.bit5(val); 
		if (OL3() || OM3()) 
			PORTA.SetMode(5, Pin::AUTO); 
		else 
			PORTA.SetMode(5, Pin::OUTPUT); 
	}

	inline void OL2(bit_t val)	{ 
		TCTL1.bit6(val); 
		if (OL2() || OM2()) 
			PORTA.SetMode(6, Pin::AUTO); 
		else 
			PORTA.SetMode(6, Pin::OUTPUT); 
	}

	inline void OM2(bit_t val)	{ 
		TCTL1.bit7(val); 
		if (OL2() || OM2()) 
			PORTA.SetMode(6,Pin::AUTO); 
		else 
			PORTA.SetMode(6,Pin::OUTPUT); 
	}


	////////////////////////////////////////////////////////
	// TCTL2 Register
	// Accessors
	
	inline byte_t EDG3() { return (TCTL2.byte & 0x3); }

	inline byte_t EDG2() { return (TCTL2.byte & 0xc)>>2; }

	inline byte_t EDG1() { return (TCTL2.byte & 0x30)>>4; }

	inline byte_t EDG4() { return (TCTL2.byte & 0xc0)>>6; }
	
	// Mutators
	
	inline void EDG3(byte_t val) { TCTL2.byte = TCTL2.byte & (~MASK_01)|(val & MASK_01); }

	inline void EDG2(byte_t val) { TCTL2.byte = TCTL2.byte & (~MASK_23)|(val<<2) & MASK_23;	}
	
	inline void EDG1(byte_t val) { TCTL2.byte = TCTL2.byte & (~MASK_45)|(val<<4) & MASK_45;	}
	
	inline void EDG4(byte_t val) { TCTL2.byte = TCTL2.byte & (~MASK_67)|(val<<6) & MASK_67;	}	


	////////////////////////////////////////////////////////
	// CFORC Register
	// Accessors
	// los 3 primeros bits no se usan
	inline bit_t FOC5()	{ return CFORC.bit3(); }
	inline bit_t FOC4()	{ return CFORC.bit4(); }
	inline bit_t FOC3()	{ return CFORC.bit5(); }
	inline bit_t FOC2()	{ return CFORC.bit6(); }
	inline bit_t FOC1()	{ return CFORC.bit7(); }


	////////////////////////////////////////////////////////
	// OC1M Register
	// Accessors
	
	inline bit_t OC1M3() { return OC1M.bit3(); }
	inline bit_t OC1M4() { return OC1M.bit4(); }
	inline bit_t OC1M5() { return OC1M.bit5(); }
	inline bit_t OC1M6() { return OC1M.bit6(); }
	inline bit_t OC1M7() { return OC1M.bit7(); }

	// Mutators
	
	inline void OC1M3(bit_t val) { 
		OC1M.bit3(val); 
		if (val) 
			PORTA.SetMode(3, Pin::AUTO);
		else 
			PORTA.SetMode(3, Pin::OUTPUT);
	}

	inline void OC1M4(bit_t val) { 
		OC1M.bit4(val); 
		if (val)
			PORTA.SetMode(4, Pin::AUTO);
		else 
			PORTA.SetMode(4, Pin::OUTPUT);
	}

	inline void OC1M5(bit_t val) { 
		OC1M.bit5(val); 
		if (val) 
			PORTA.SetMode(5, Pin::AUTO);
		else 
			PORTA.SetMode(5, Pin::OUTPUT);
	}

	inline void OC1M6(bit_t val) { 
		OC1M.bit6(val); 
		if (val)
			PORTA.SetMode(6, Pin::AUTO);
		else 
			PORTA.SetMode(6, Pin::OUTPUT);
	}

	inline void OC1M7(bit_t val) { 
		OC1M.bit7(val); 
		if (val) {
			if (PORTA.GetMode(7) == Pin::OUTPUT)
				PORTA.SetMode(7, Pin::AUTO);
		} 
		else if (PACTL.bit7())		//si está a 1 es de salida
			PORTA.SetMode(7, Pin::OUTPUT);
		else						//sino de entrada
			PORTA.SetMode(7, Pin::INPUT);
	}


	////////////////////////////////////////////////////////
	// OC1D Register
	// Accessors
	
	inline bit_t OC1D3() { return OC1D.bit3(); }
	inline bit_t OC1D4() { return OC1D.bit4(); }
	inline bit_t OC1D5() { return OC1D.bit5(); }
	inline bit_t OC1D6() { return OC1D.bit6(); }
	inline bit_t OC1D7() { return OC1D.bit7(); }
	
	// Mutators
	
	inline void OC1D3(bit_t val) { OC1D.bit3(val); }
	inline void OC1D4(bit_t val) { OC1D.bit4(val); }
	inline void OC1D5(bit_t val) { OC1D.bit5(val); }
	inline void OC1D6(bit_t val) { OC1D.bit6(val); }
	inline void OC1D7(bit_t val) { OC1D.bit7(val); }


	////////////////////////////////////////////////////////
	// TMSK1 Register
	// Accessors
	inline bit_t IC3I()	{ return TMSK1.bit0(); }
	inline bit_t IC2I()	{ return TMSK1.bit1(); }
	inline bit_t IC1I()	{ return TMSK1.bit2(); }
	inline bit_t I4O5I(){ return TMSK1.bit3(); }
	inline bit_t OC4I()	{ return TMSK1.bit4(); }
	inline bit_t OC3I()	{ return TMSK1.bit5(); }
	inline bit_t OC2I()	{ return TMSK1.bit6(); }
	inline bit_t OC1I()	{ return TMSK1.bit7(); }

	// Mutators
	inline void IC3I(bit_t val)	{ TMSK1.bit0(val); }
	inline void IC2I(bit_t val)	{ TMSK1.bit1(val); }
	inline void IC1I(bit_t val)	{ TMSK1.bit2(val); }
	inline void I4O5I(bit_t val){ TMSK1.bit3(val); }
	inline void OC4I(bit_t val)	{ TMSK1.bit4(val); }
	inline void OC3I(bit_t val)	{ TMSK1.bit5(val); }
	inline void OC2I(bit_t val)	{ TMSK1.bit6(val); }
	inline void OC1I(bit_t val)	{ TMSK1.bit7(val); }


	////////////////////////////////////////////////////////
	// TFLG1 Register
	// Accessors
	inline bit_t IC3F()	{ return TFLG1.bit0(); }
	inline bit_t IC2F()	{ return TFLG1.bit1(); }
	inline bit_t IC1F()	{ return TFLG1.bit2(); }
	inline bit_t I4O5F(){ return TFLG1.bit3(); }
	inline bit_t OC4F()	{ return TFLG1.bit4(); }
	inline bit_t OC3F()	{ return TFLG1.bit5(); }
	inline bit_t OC2F()	{ return TFLG1.bit6(); }
	inline bit_t OC1F()	{ return TFLG1.bit7(); }
	
	// Mutators
	inline void IC3F(bit_t val)	{ TFLG1.bit0(val); }
	inline void IC2F(bit_t val)	{ TFLG1.bit1(val); }
	inline void IC1F(bit_t val)	{ TFLG1.bit2(val); }
	inline void I4O5F(bit_t val){ TFLG1.bit3(val); }
	inline void OC4F(bit_t val)	{ TFLG1.bit4(val); }
	inline void OC3F(bit_t val)	{ TFLG1.bit5(val); }
	inline void OC2F(bit_t val)	{ TFLG1.bit6(val); }
	inline void OC1F(bit_t val)	{ TFLG1.bit7(val); }


	////////////////////////////////////////////////////////
	// TMSK2 Register
	// Accessors
	
	inline byte_t PR()	{ return (TMSK2.byte & MASK_01); }	//& and bit a bit, mascara 0x3

	inline bit_t PAII()	{ return TMSK2.bit4(); }
	inline bit_t PAOVI(){ return TMSK2.bit5(); }
	inline bit_t RTII()	{ return TMSK2.bit6(); }
	inline bit_t TOI()	{ return TMSK2.bit7(); }
	
	// Mutators
	
	inline void PR(byte_t val) { TMSK2.byte = TMSK2.byte & (~MASK_01)|	(val & MASK_01); }
	
	inline void PAII(bit_t val)	{ TMSK2.bit4(val); }
	inline void PAOVI(bit_t val){ TMSK2.bit5(val); }
	inline void RTII(bit_t val)	{ TMSK2.bit6(val); }
	inline void TOI(bit_t val)	{ TMSK2.bit7(val); }


	////////////////////////////////////////////////////////
	// TFLG2 Register
	// Accessors
	
	inline bit_t PAIF()	{ return TFLG2.bit4(); }
	inline bit_t PAOVF(){ return TFLG2.bit5(); }
	inline bit_t RTIF()	{ return TFLG2.bit6(); }
	inline bit_t TOF()	{ return TFLG2.bit7(); }

	// Mutators
	
	inline void PAIF(bit_t val)	{ TFLG2.bit4(val); }
	inline void PAOVF(bit_t val){ TFLG2.bit5(val); }
	inline void RTIF(bit_t val)	{ TFLG2.bit6(val); }
	inline void TOF(bit_t val)	{ TFLG2.bit7(val); }


	////////////////////////////////////////////////////////
	// PACTL Register
	// Accessors
	
	inline byte_t RTR()  { return PACTL.byte & MASK_01; }	//& and bit a bit, mascara 0x3
	inline bit_t I4O5()	 { return PACTL.bit2(); }
	inline bit_t DDRA3() { return PACTL.bit3(); }
	inline bit_t PEDGE() { return PACTL.bit4(); }
	inline bit_t PAMOD() { return PACTL.bit5(); }
	inline bit_t PAEN()	 { return PACTL.bit6(); }
	inline bit_t DDRA7() { return PACTL.bit7(); }

	// Mutators

	inline void RTR(byte_t val)	 { PACTL.byte = PACTL.byte & (~MASK_01) | (val & MASK_01); }
	inline void I4O5(bit_t val)	 { PACTL.bit2(val); }
	inline void DDRA3(bit_t val) { PACTL.bit3(val); }
	inline void PEDGE(bit_t val) { PACTL.bit4(val); }
	inline void PAMOD(bit_t val) { PACTL.bit5(val); }
	inline void PAEN(bit_t val)	 { PACTL.bit6(val); }
	
	inline void DDRA7(bit_t val) { 
		PACTL.bit7(val);	
		if (val) {
			if (OC1M7())
				PORTA.SetMode(7, Pin::AUTO);
			else
				PORTA.SetMode(7, Pin::OUTPUT); 
		}
		else 
			PORTA.SetMode(7, Pin::INPUT); 
	}


	////////////////////////////////////////////////////////
	// BPROT Register
	// Accessors
	
	inline byte_t BPRT ()	{ return BPROT.byte&MASK_0123; }
	inline bit_t PTCON()	{ return BPROT.bit4(); }
	

	// Mutators
	
	inline void BPRT(byte_t val) { BPROT.byte = BPROT.byte&(~MASK_0123)|(val&MASK_0123); }
	inline void PTCON(bit_t val) { BPROT.bit4(val); }
	

	////////////////////////////////////////////////////////
	// EPROG Register
	// Accessors
	inline bit_t PGM ()  { return EPROG.bit0(); }
	
	inline byte_t T()	 { return (EPROG.byte&MASK_12)>>1; }
	inline bit_t EXROW() { return EPROG.bit3(); }
	inline bit_t EXCOL() { return EPROG.bit4(); }
	inline bit_t ELAT()	 { return EPROG.bit5(); }
	
	inline bit_t MBE()	 { return EPROG.bit7(); }

	// Mutators
	inline void PGM(bit_t val)	 { EPROG.bit0(val); }
	
	inline void T(byte_t val)	 { EPROG.byte = EPROG.byte&(~MASK_12)|(val<<1)&MASK_12; }
	inline void EXROW(bit_t val) { EPROG.bit3(val); }
	inline void EXCOL(bit_t val) { EPROG.bit4(val); }
	inline void ELAT(bit_t val)	 { EPROG.bit5(val); }
	
	inline void MBE(bit_t val)	 { EPROG.bit7(val); }


	////////////////////////////////////////////////////////
	// SPCR Register
	// Accessors
	
	inline byte_t SPR()	{ return SPCR.byte&MASK_01; }
	inline bit_t CPHA()	{ return SPCR.bit2(); }
	inline bit_t CPOL()	{ return SPCR.bit3(); }
	inline bit_t MSTR()	{ return SPCR.bit4(); }
	inline bit_t DWOM()	{ return SPCR.bit5(); }
	inline bit_t SPE()	{ return SPCR.bit6(); }
	inline bit_t SPIE()	{ return SPCR.bit7(); }
	
	// Mutators
	
	inline void SPR(byte_t val)	{ SPCR.byte = SPCR.byte&(~MASK_01)|(val&MASK_01); }
	inline void CPHA(bit_t val)	{ SPCR.bit2(val); }
	inline void CPOL(bit_t val)	{ SPCR.bit3(val); }
	inline void MSTR(bit_t val)	{ SPCR.bit4(val); }
	
	inline void DWOM(bit_t val)	{ 
		SPCR.bit5(val);
		if(val)		
			
			for(int i=0;i<=5;i++)  
				
				PORTD.SetMode(i,Pin::AUTO);  
				
		
			else {
									
				for(int x=0;x<=5;x++)
					PORTD.SetMode(x,Pin::OUTPUT);
									
				DDRD.Write(DDRD.Read());
			}
								
	}

	inline void SPE(bit_t val)	{ SPCR.bit6(val); }
	inline void SPIE(bit_t val)	{ SPCR.bit7(val); }


	////////////////////////////////////////////////////////
	// SPSR Register
	// Accessors
	
	inline bit_t MODF()	{ return SPSR.bit4(); }
	
	inline bit_t WCOL()	{ return SPSR.bit6(); }
	inline bit_t SPIF()	{ return SPSR.bit7(); }

	// Mutators
	
	inline void MODF(bit_t val)	{ SPSR.bit4(val); }
	
	inline void WCOL(bit_t val)	{ SPSR.bit6(val); }
	inline void SPIF(bit_t val)	{ SPSR.bit7(val); }


	////////////////////////////////////////////////////////
	// BAUD Register
	// Accessors
	
	inline byte_t SCR()	{ return BAUD.byte & MASK_012; }	//mascara 0x07
	inline bit_t RCKB()	{ return BAUD.bit3(); }
	
	inline bit_t SCP()	{ return (BAUD.byte & MASK_45)>>4; }	//mascar 0x30
	inline bit_t SCP2()	{ return BAUD.bit6(); }
	inline bit_t TCLR()	{ return BAUD.bit7(); }
	
	// Mutators
	
	inline void SCR(byte_t val)	{ BAUD.byte = BAUD.byte & (~MASK_012) | (val & MASK_012); } 
	inline void RCKB(bit_t val)	{ BAUD.bit3(val); }
	
	inline void SCP(byte_t val)	{ BAUD.byte = BAUD.byte & (~MASK_45) | (val<<4) & MASK_45; }
	inline void SCP2(bit_t val)	{ BAUD.bit6(val); }
	inline void TCLR(bit_t val)	{ BAUD.bit7(val); }


	////////////////////////////////////////////////////////
	// PIOC Register
	
	inline bit_t INVB() { return PIOC.bit0(); }
	inline bit_t EGA()	{ return PIOC.bit1(); }
	inline bit_t PLS()	{ return PIOC.bit2(); }
	inline bit_t OIN()	{ return PIOC.bit3(); }
	inline bit_t HNDS()	{ return PIOC.bit4(); }
	inline bit_t CWOM()	{ return PIOC.bit5(); }
	inline bit_t STAI()	{ return PIOC.bit6(); }
	inline bit_t STAF()	{ return PIOC.bit7(); }
  
	// Mutators
	inline void INVB(bit_t val) { PIOC.bit0(val);	}
	inline void EGA(bit_t val)	{ PIOC.bit1(val);	}
	inline void PLS(bit_t val)	{ PIOC.bit2(val);	}
	inline void OIN(bit_t val)	{ PIOC.bit3(val);	}
	inline void HNDS(bit_t val)	{ PIOC.bit4(val);	}
	inline void CWOM(bit_t val)	{ PIOC.bit5(val);	}
	inline void STAI(bit_t val)	{ PIOC.bit6(val);	}
	inline void STAF(bit_t val)	{ PIOC.bit7(val);	}


	////////////////////////////////////////////////////////
	// SCCR1 Register
	// Accessors
	
	inline bit_t WAKE()	{ return SCCR1.bit3(); }
	inline bit_t M()	{ return SCCR1.bit4(); }
	// Bit 5 is unused
	inline bit_t T8()	{ return SCCR1.bit6(); }
	inline bit_t R8()	{ return SCCR1.bit7(); }

	// Mutators
	
	inline void WAKE(bit_t val)	{ SCCR1.bit3(val); }
	inline void M(bit_t val)	{ SCCR1.bit4(val); }
	
	inline void T8(bit_t val)	{ SCCR1.bit6(val); }
	inline void R8(bit_t val)	{ SCCR1.bit7(val); }


	////////////////////////////////////////////////////////
	// SCCR2 Register
	
	inline bit_t SBK()	{ return SCCR2.bit0(); }
	inline bit_t RWU()	{ return SCCR2.bit1(); }
	inline bit_t RE()	{ return SCCR2.bit2(); }
	inline bit_t TE()	{ return SCCR2.bit3(); }
	inline bit_t ILIE()	{ return SCCR2.bit4(); }
	inline bit_t RIE()	{ return SCCR2.bit5(); }
	inline bit_t TCIE()	{ return SCCR2.bit6(); }
	inline bit_t TIE()	{ return SCCR2.bit7(); }
  
	// Mutators
	inline void SBK(bit_t val)  { SCCR2.bit0(val); }
	inline void RWU(bit_t val)	{ SCCR2.bit1(val); }
	inline void RE(bit_t val)	{ SCCR2.bit2(val); }
	inline void TE(bit_t val)	{ SCCR2.bit3(val); }
	inline void ILIE(bit_t val)	{ SCCR2.bit4(val); }
	inline void RIE(bit_t val)	{ SCCR2.bit5(val); }
	inline void TCIE(bit_t val)	{ SCCR2.bit6(val); }
	inline void TIE(bit_t val)	{ SCCR2.bit7(val); }


	////////////////////////////////////////////////////////
	// SCSR Register
	// Accessors
	
	inline bit_t FE()	{ return SCSR.bit1(); }
	inline bit_t NF()	{ return SCSR.bit2(); }
	inline bit_t OR()	{ return SCSR.bit3(); }
	inline bit_t IDLE()	{ return SCSR.bit4(); }
	inline bit_t RDRF()	{ return SCSR.bit5(); }
	inline bit_t TC()	{ return SCSR.bit6(); }
	inline bit_t TDRE()	{ return SCSR.bit7(); }
  
	// Mutators
	
	inline void FE(bit_t val)	{ SCSR.bit1(val); }
	inline void NF(bit_t val)	{ SCSR.bit2(val); }
	inline void OR(bit_t val)	{ SCSR.bit3(val); }
	inline void IDLE(bit_t val)	{ SCSR.bit4(val); }
	inline void RDRF(bit_t val)	{ SCSR.bit5(val); }
	inline void TC(bit_t val)	{ SCSR.bit6(val); }
	inline void TDRE(bit_t val)	{ SCSR.bit7(val); }


	////////////////////////////////////////////////////////
	// SCDR Register
	// Accessors
	inline bit_t R0T0()	{ return SCDR.bit0(); }
	inline bit_t R1T1()	{ return SCDR.bit1(); }
	inline bit_t R2T2()	{ return SCDR.bit2(); }
	inline bit_t R3T3()	{ return SCDR.bit3(); }
	inline bit_t R4T4()	{ return SCDR.bit4(); }
	inline bit_t R5T5()	{ return SCDR.bit5(); }
	inline bit_t R6T6()	{ return SCDR.bit6(); }
	inline bit_t R7T7()	{ return SCDR.bit7(); }
  
	// Mutators
	inline void R0T0(bit_t val)	{ SCDR.bit0(val); }
	inline void R1T1(bit_t val)	{ SCDR.bit1(val); }
	inline void R2T2(bit_t val)	{ SCDR.bit2(val); }
	inline void R3T3(bit_t val)	{ SCDR.bit3(val); }
	inline void R4T4(bit_t val)	{ SCDR.bit4(val); }
	inline void R5T5(bit_t val)	{ SCDR.bit5(val); }
	inline void R6T6(bit_t val)	{ SCDR.bit6(val); }
	inline void R7T7(bit_t val)	{ SCDR.bit7(val); }


	////////////////////////////////////////////////////////
	// ADCTL Register
	// Accessors
	inline bit_t CA()	{ return ADCTL.bit0(); }
	inline bit_t CB()	{ return ADCTL.bit1(); }
	inline bit_t CC()	{ return ADCTL.bit2(); }
	inline bit_t CD()	{ return ADCTL.bit3(); }
	inline bit_t MULT()	{ return ADCTL.bit4(); }
	inline bit_t SCAN()	{ return ADCTL.bit5(); }
	
	inline bit_t CCF()	{ return ADCTL.bit7(); }		
  
	// Mutators
	inline void CA(bit_t val)	{ ADCTL.bit0(val); }
	inline void CB(bit_t val)	{ ADCTL.bit1(val); }
	inline void CC(bit_t val)	{ ADCTL.bit2(val); }
	inline void CD(bit_t val)	{ ADCTL.bit3(val); }
	inline void MULT(bit_t val)	{ ADCTL.bit4(val); }
	inline void SCAN(bit_t val)	{ ADCTL.bit5(val); }
	
	inline void CCF(bit_t val)	{ ADCTL.bit7(val); }

};


#endif // REGFILE_H