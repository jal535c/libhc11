#ifndef __PIN_H__
#define __PIN_H__


#include <assert.h>
#include "byte.h"
class PinConnection;  //???????? si uso pin.h en algun sitio, su .cpp debe incluir pinConnection.h
#include <stdlib.h>


class Pin {		
public:	
	typedef enum  { INPUT = 0, OUTPUT, AUTO } PinDirection;		//auto = cuando un comparador esta activado y no permite usar el port
 
private:		
	bit_t level;				//nivel del pin: 1 o 0
	PinDirection mode;			//para el modo de direccion: entrada, salida o auto
	PinConnection* connect;		//connect guarda la dir de un objeto PinConnection que solo tiene un metodo (Set)
								
public:	
    Pin() 
    { 		
		connect = NULL; 
		level = 0; 
		mode = Pin::OUTPUT; 
	}

	PinDirection GetMode() {
		return mode;
	}

	void SetMode(PinDirection value) {
		mode = value; 
	}

	void Attach(PinConnection* pConnect) {	//vincula o conecta un objeto de la clase PinConnection con el atributo de ésta
		assert(pConnect != NULL); 	//comprueba que no es nulo: si se da la condicion ejecuta, sino aborta
	    connect = pConnect;		
	}

	void UnAttach() {
		connect = NULL;
	}
	
	bit_t GetLevel() {			
		return level;
	}

	void Input(bit_t value);	//dependiendo del modo, el SetLevel será diferente

protected:
	void Output(bit_t value); 
	void Action(bit_t value); 

	friend class PortRegister;		//los metodos Write, Output y Action acceden a los campos privados de la clase Pin
};


#endif