#ifndef __REGISTEROBJECT_H__
#define __REGISTEROBJECT_H__


#include "bytetype.h"


class RegisterObject {	//todas las clases para registros tendran funciones read y write, las abstraigo, las pongo como virtuales puras
private:

public:
	virtual byte_t Read() = 0;		
	virtual void Write(byte_t data) = 0;	
};


#endif
