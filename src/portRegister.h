#ifndef __PORTREGISTER_H__
#define __PORTREGISTER_H__


#include "registerObject.h"
#include "pin.h"		//ya lleva stdlib.h
class PortConnection;		//??????????	si algun fichero incluye portRegister.h, tambien debe incluir portConnection.h
#include "byte.h"
#include <stdlib.h>


//maneja todas las operaciones de puertos y pines

class PortRegister : public RegisterObject {		//del padre tengo los metodos virtuales Read y Write, aqui los implemento
protected:
	Pin pins[8];				//un puerto esta compuesto por un array de 8 pines
    PortConnection* connect;	//si esta conectado, puedo escribir, la GUI puede usarlo
							
public:	
	PortRegister();	
				
    byte_t Read();			//lee todo el puerto, metodo que redefine al Read usado en MemoryRegister
	bit_t  Read(int PinNo);		//lee solo un pin de un puerto
	
	void Write(byte_t data);	
									
 	void Attach(PortConnection* pConnect);				
	void Attach(int PinNo, PinConnection* pConnect);	
   	void UnAttach() {							
		connect = NULL;
	}
    void UnAttach(int PinNo);					
	
	void SetMode(int PinNo, Pin::PinDirection dir);	//pone el modo de direccion de un pin del puerto, llama a Pin::SetMode
	Pin::PinDirection GetMode(int PinNo);			//devuelve el modo de direccion de un pin del puerto
  
	virtual void PinInput(int PinNo, bit_t value);	
									  
    void Action(int PinNo, bit_t level);
	void Output(int PinNo, bit_t level);

	friend class DDRegister;	//necesito acceder a pins[] desde DDRegister::Write()
};


#endif