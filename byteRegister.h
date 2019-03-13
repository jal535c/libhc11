#ifndef __BYTEREGISTER_H__
#define __BYTEREGISTER_H__


#include "bytetype.h"
#include "registerObject.h"


class ByteRegister : public Byte, public RegisterObject {    
private:

public:	
	byte_t Read() {			//aqui se implementan los metodos virtuales Read y Write de RegisterObject
		return byte;			//byte es atributo derivado de la clase-struct Byte
	}
    
	void Write(byte_t data) {
		byte = data;		
	}
	
    ByteRegister& operator=(const byte_t& data) { 
		byte = data;
		return *this;
	}  

    byte_t operator++(int ) {	//sobrecarga del operador ++, post incremento (pasa un parametro)
		return ++byte;			
	}

    byte_t operator++() {		//pre incremento (no pasa parametro)
		return ++byte;			
	}
};


#endif