#include "portRegister.h"
#include "portConnection.h"		//en el .h hay un class PortConnection;
#include <assert.h>


PortRegister::PortRegister() {
	connect = NULL;
}


byte_t PortRegister::Read() {
	Byte value;
    
    value.bit0(pins[0].GetLevel());		//bit0() es un metodo de la clase Byte, y Getlevel() es un metodo de la clase Pin
    value.bit1(pins[1].GetLevel());
    value.bit2(pins[2].GetLevel());
    value.bit3(pins[3].GetLevel());
    value.bit4(pins[4].GetLevel());
    value.bit5(pins[5].GetLevel());
    value.bit6(pins[6].GetLevel());
    value.bit7(pins[7].GetLevel());
    
    return value;
}


bit_t PortRegister::Read(int PinNo) {
	assert((PinNo >=0) && (PinNo <= 7));	//comprueba que el pin a leer esta dentro del rango (entre 0 y 7)
	return pins[PinNo].GetLevel();			//devuelve el valor del pin
}


void PortRegister::Write(byte_t data) {
	Byte value;
    value = data;		//hace uso de la sobrecarga del operador de asignacion de la clase Byte

	pins[0].Output(value.bit0());	//modifico mi atributo pins con el metodo Pin::Output()
    pins[1].Output(value.bit1());
    pins[2].Output(value.bit2());
    pins[3].Output(value.bit3());
    pins[4].Output(value.bit4());
    pins[5].Output(value.bit5());
    pins[6].Output(value.bit6());
    pins[7].Output(value.bit7());

	if ((connect != NULL)) 
		connect->Write(Read());			
}


void PortRegister::Attach(PortConnection* pConnect) {	//conecta un objeto PortConnection a mi objeto PortRegister
	if (pConnect != NULL)		
		connect = pConnect;		
}


void PortRegister::Attach(int PinNo, PinConnection* pConnect) {		//conecta un objeto PinConnection a un pin de un puerto
	assert((PinNo >= 0) && (PinNo <= 7));	
	
	if (pConnect != NULL)				
		pins[PinNo].Attach(pConnect);	
}


void PortRegister::UnAttach(int PinNo) {	//libera la vinculacion de PinConnection del pin de un puerto
	assert((PinNo >= 0) && (PinNo <= 7));
	pins[PinNo].UnAttach();			
}


void PortRegister::SetMode(int PinNo, Pin::PinDirection dir) {	//para establecer el modo de direccion de un pin del puerto
	assert((PinNo >= 0) && (PinNo <= 7));	
	pins[PinNo].SetMode(dir);		
}


Pin::PinDirection PortRegister::GetMode(int PinNo) {	//devuelve el modo de direccion de un pin del puerto
	assert((PinNo >= 0) && (PinNo <= 7));
	return pins[PinNo].GetMode();	
}


//cambia el level de un pin del puerto si el modo de direccion es OUTPUT. Actualiza PortConnection si uno está vinculado

void PortRegister::Output(int PinNo, bit_t level) {
	assert((PinNo >=0) && (PinNo <= 7));
		
	pins[PinNo].Output(level);		

	if (connect != NULL)
		connect->Write(Read());		//actualiza GUI si esta conectado, atraves de PortConnection::Write()
}


//cambia el level de un pin del puerto si el modo de direccion es AUTO. Actualiza PortConnection si uno está vinculado

void PortRegister::Action(int PinNo, bit_t level) {
	assert((PinNo >=0) && (PinNo <= 7));
		
	pins[PinNo].Action(level);		//llama al metodo action (pasando el level) de la clase pin (nº de pin concreto pasado)

	if (connect != NULL)			//si esta conectado actualiza interfaz (CPort::Write())
		connect->Write(Read());
}


//cambia el level de un pin del puerto si el modo de direccion es INPUT. Actualiza PortConnection si uno está vinculado

void PortRegister::PinInput(int PinNo, bit_t value) {
	assert((PinNo >= 0) && (PinNo <= 7));	
 
	pins[PinNo].Input(value);		

	if (connect != NULL)			
		connect->Write(Read());		
}

