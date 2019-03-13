#ifndef PORTAREGISTER_H
#define PORTAREGISTER_H


#include "portRegister.h"
class HC11;			//referencias cruzadas, si incluyo "hc11.h" daria error, todavia no está creado el objeto HC11
#include "bytetype.h"


//la sobrecarga de un PortRegister con eventos tan importantes puede ocurrir cuando Port A es cambiado
//esta clase se usa principalmente para capturar entradas en los pines (capturador de entrada: bits 0,1,2 del puerto A)

class PortARegister : public PortRegister {	
private:
	HC11* hc11;		
				
public:	
    PortARegister();	//situa los bits 0,1 y 2 como entrada (para el capturador)
    
	void SetHC11(HC11* mcu) {	//se llama en el constructor de HC11, atraves de regfile, es necesario? SI. 
		hc11 = mcu;				//vincula (punteros) un objeto hc11 con el mio (el de aqui, ke es un puntero del original de hc11sim)
	}							//es como la segunda parte del constructor PortARegister
  
	void PinInput(int PinNo, bit_t value);	//que pasa al cambiar el valor de un pin de entrada?, 
											//redefine el metodo de la clase base
											//Tiene la logica del capturador de entrada, y del acumulador de pulsos
	
	//esta clase amiga no es necesaria
	//friend class DDRegister;	//DDRegister tiene permiso para acceder a los campos privados de PortARegister
};


#endif