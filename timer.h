#ifndef TIMER_H
#define TIMER_H


//#include "hc11.h"		//tiene que ir en timer.cpp sino da error
#include "wordtype.h"	//para el tipo definido word_t
class HC11;			//referencias cruzadas, declaracion adelantada


class TimerSubSystem {
protected:
	HC11* hc11;		//guarda una direccion de memoria, la k le paso en el constructor de TimerSubSystem, 
					//k es la del constructor HC11, el objeto hc11 de HC11Sim (el primer objeto), de esta manera los vinculo
	word_t	rti_clock_divider;
	
public:
	word_t	main_clock_divider;		//es public, el opcode STOP necesita accederlo

    TimerSubSystem(HC11 * nhc11);	//constructor, vincula el atributo hc11 a nhc11, y los divisores a 0
	void ClockSystem();	//metodo principal con la logica del timer, del rti, y llama a los otros 2 metodos
	void DoOutputCompares();	//metodo con la logica de los comparadores de salida
	void DoPulseAccumulator_GatedTimeAccumulationMode();	//metodo con la logica del pulse acumulator en modo gated (duracion)
};


#endif
