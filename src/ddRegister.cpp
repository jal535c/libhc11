#include "ddRegister.h"
#include "portRegister.h"
#include <assert.h>
#include <stdlib.h>


DDRegister::DDRegister() {
	port_reg = NULL;
}


void DDRegister::Associate(PortRegister* port) {
	assert(port != NULL);	//comprueba que el port que le paso no sea NULL
	port_reg = port;		//lo vincula al atributo 
}


byte_t DDRegister::Read() {		//lee el valor byte de un DDRegister
    return byte;	
}


//funcion para escribir un valor byte en el registro. Despues de que el byte es escrito,
//los modos de direccion de los pines del puerto con el que ese registro está asociado, son actualizados

void DDRegister::Write(byte_t data) {

	byte = data;	//byte es el atributo de la clase/struct Byte, de la k deriva ByteRegister, de la k deriva DDRegister

	if (port_reg != NULL) { //si está asociado, entra en el if
		
		if(port_reg->GetMode(0) != Pin::AUTO)						//obtiene el modo de ese pin, si es diferente de auto: entra en if
			port_reg->pins[0].SetMode((data&0x01) ? Pin::OUTPUT : Pin::INPUT);	//si el pin es 1 lo pone de salida, si es 0 de entrada
		
		if(port_reg->GetMode(1) != Pin::AUTO)
			port_reg->pins[1].SetMode((data&0x02) ? Pin::OUTPUT : Pin::INPUT);
		
		if(port_reg->GetMode(2) != Pin::AUTO)
			port_reg->pins[2].SetMode((data&0x04) ? Pin::OUTPUT : Pin::INPUT);
		
		if(port_reg->GetMode(3) != Pin::AUTO)
			port_reg->pins[3].SetMode((data&0x08) ? Pin::OUTPUT : Pin::INPUT);
		
		if(port_reg->GetMode(4) != Pin::AUTO)
			port_reg->pins[4].SetMode((data&0x10) ? Pin::OUTPUT : Pin::INPUT);
		
		if(port_reg->GetMode(5) != Pin::AUTO)
			port_reg->pins[5].SetMode((data&0x20) ? Pin::OUTPUT : Pin::INPUT);
		
		if(port_reg->GetMode(6) != Pin::AUTO)
			port_reg->pins[6].SetMode((data&0x40) ? Pin::OUTPUT : Pin::INPUT);
		
		if(port_reg->GetMode(7) != Pin::AUTO)
			port_reg->pins[7].SetMode((data&0x80) ? Pin::OUTPUT : Pin::INPUT);

	}

}