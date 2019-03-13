#ifndef PORTCONNECTION_H
#define PORTCONNECTION_H


#include "bytetype.h"


//permite a un objeto escribir un valor en un puerto desde un modulo diferente, atraves de funciones virtuales
//la implementacion de Write está en CPortIO (GUI MFC)

class PortConnection {	// Port writing to GUI, 
private:				
		
public:	
	virtual void Write(byte_t);	// GUI writes this function		
							//no uso ésta funcion (solo para testear) sino la que está definida en CPortIO
							//la clase CPortIO deriva de ésta, y es ahí donde se implementa este metodo
							//no se pone =0 (virtual pura) porque en la LIB ninguna clase deriva de ella, solo se crean objetos
};


#endif