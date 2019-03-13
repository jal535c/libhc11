#ifndef __PORTAREGISTER_H__
#define __PORTAREGISTER_H__


#include "portRegister.h"
class HC11;			
#include "byte.h"


//esta clase se usa principalmente para capturar entradas en los pines (capturador de entrada: bits 0,1,2 del puerto A)

class PortARegister : public PortRegister {	
private:
	HC11* hc11;		
				
public:	
    PortARegister();	//situa los bits 0,1 y 2 como entrada (para el capturador)
    
	void SetHC11(HC11* mcu) {	
		hc11 = mcu;				
	}							
  
	void PinInput(int PinNo, bit_t value);		//logica del capturador de entrada, y del acumulador de pulsos
								
	//friend class DDRegister;
};


#endif