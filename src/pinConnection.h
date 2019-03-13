#ifndef __PINCONNECTION_H__
#define __PINCONNECTION_H__


#include "byte.h"


//permite poner un valor (level 1 o 0) en un pin desde un modulo diferente, por medio de funciones virtuales
//se usa de vinculo con la GUI
//se implenta en la GUI (en una clase derivada)

class PinConnection {		
private:

public:
	virtual void Set(bit_t value);	//no es virtual pura, ya que en la LIB ninguna clase deriva de ella, sino que se crean objetos de ella
};


#endif