#ifndef PORTREGISTER_H
#define PORTREGISTER_H


#include "registerObject.h"
#include "pin.h"		//ya lleva stdlib.h
class PortConnection;		//??????????	si algun fichero incluye portRegister.h, tambien debe incluir portConnection.h
#include "bytetype.h"
#include <stdlib.h>


//maneja todas las operaciones de puertos y pines

class PortRegister : public RegisterObject {		//de MemoryRegister tengo los metodos virtuales Read y Write, aqui los implemento
protected:
	Pin pins[8];				//un puerto esta compuesto por un array de 8 pines
    PortConnection* connect;	//si esta conectado, puedo escribir, la GUI puede usarlo, sino no
							//connect guarda la dir de un objeto PortConnection (solo tiene un metodo Write),
							//si esta conectado (punteros vinculados), CPortIO puede escribir

public:	
	PortRegister();	//constructor, connect=NULL, inicialmente no puedo escribir en el puerto (estará desconectado de la GUI)
					//ya que para escribir se usa la funcion CPortIO::Write
	
    byte_t Read();			//lee todo el puerto, metodo que redefine al Read usado en MemoryRegister
	bit_t  Read(int PinNo);		//lee solo un pin de un puerto
	
	void Write(byte_t data);	//implementa el metodo virtual puro de la clase padre MemoryRegister
								//esta funcion llama a su vez a PortConnection::Write() que es la que actualiza la GUI
	
 	void Attach(PortConnection* pConnect);				//conecta un objeto PortConnection a mi PortRegister (para visualizar)
	void Attach(int PinNo, PinConnection* pConnect);	//conecta un objeto PinConnection a un pin de un puerto
   	void UnAttach() {							//desconecta el puerto
		connect = NULL;
	}
    void UnAttach(int PinNo);						//desconecta el pin del puerto
	
	void SetMode(int PinNo, Pin::PinDirection dir);	//pone el modo de direccion de un pin del puerto, llama a Pin::SetMode
	Pin::PinDirection GetMode(int PinNo);			//devuelve el modo de direccion de un pin del puerto
  
	virtual void PinInput(int PinNo, bit_t value);	//escribe un valor en un pin (de entrada) del puerto
												//es virtual ya que será redefinido en la clase PortARegister
  
    void Action(int PinNo, bit_t level);
	void Output(int PinNo, bit_t level);

	friend class DDRegister;	//necesito acceder a pins[] desde DDRegister::Write()
};


#endif