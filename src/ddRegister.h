#ifndef __DDREGISTER_H__
#define __DDREGISTER_H__


#include "byteRegister.h"
class PortRegister;
#include "byte.h"


//esta clase afecta a los modos (direccion) de los pines de un puerto asociado (ej: DDRC esta asociado con PORTC)
//Los pines del PORTC pueden ser de entrada o salida, depende del valor del registro DDRC (data direction register)

//PORTA tiene algunos pines de entrada, y otros de salida. Se cambian usando esta clase, por eso PortARegister es amiga de ella

class DDRegister: public ByteRegister {		
private:
	PortRegister* port_reg; 
 
public:	
	DDRegister();

	void Associate(PortRegister* port);		//vincula port a port_reg
		
	byte_t Read();			//redefine el metodo Read
	void Write(byte_t data);	//redefine el metodo Write
};


#endif