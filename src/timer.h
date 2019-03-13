#ifndef __TIMER_H__
#define __TIMER_H__


//#include "hc11.h"		
#include "word.h"	
class HC11;			


class TimerSubSystem {
protected:
	HC11* hc11;		
					
	word_t	rti_clock_divider;
	
public:
	word_t	main_clock_divider;		//es public, el opcode STOP necesita accederlo

    TimerSubSystem(HC11 * nhc11);	
	void ClockSystem();			//metodo principal con la logica del timer, del rti, y llama a los otros 2 metodos
	void DoOutputCompares();	//metodo con la logica de los comparadores de salida
	void DoPulseAccumulator_GatedTimeAccumulationMode();	//metodo con la logica del pulse acumulator en modo gated (duracion)
};


#endif
