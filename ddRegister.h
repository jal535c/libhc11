#ifndef DDREGISTER_H
#define DDREGISTER_H


#include "byteRegister.h"
class PortRegister;
#include "bytetype.h"


//esta clase afecta a los modos (direccion) de los pines de un puerto asociado (ej: DDRC esta asociado con PORTC)
//Los pines del PORTC pueden ser de entrada o salida, depende del valor del registro DDRC (data direction register)

//PORTA tiene algunos pines de entrada, y otros de salida. Se cambian usando esta clase, por eso PortARegister es amiga de ella
//realmente la amiga es la clase PortRegister de la que deriva PortARegister

class DDRegister: public ByteRegister {		
private:
	PortRegister* port_reg; 
 
public:	
	DDRegister();	//constructor, pone port_reg a NULL

	void Associate(PortRegister* port);		//vincula port a port_reg
		
	byte_t Read();			//redefine el metodo Read del padre ByteRegister, del abuelo MemoryRegister
	void Write(byte_t data);	//redefine el metodo Write del padre ByteRegister, del abuelo MemoryRegister
								//al escribir en un DDR lo que hago es cambiar la direccion de los pines del puerto asociado
								//segun el valor que escriba: 1 salida, 0 entrada
};


#endif