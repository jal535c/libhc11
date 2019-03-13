#ifndef __PORTCONNECTION_H__
#define __PORTCONNECTION_H__


#include "byte.h"


//permite a un objeto escribir un valor en un puerto desde un modulo diferente, atraves de funciones virtuales
//la implementacion de Write está en CPortIO (GUI MFC)

class PortConnection {
private:				
		
public:	
	virtual void Write(byte_t);								
};


#endif