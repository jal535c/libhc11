#ifndef __WORDREGISTER_H__
#define __WORDREGISTER_H__


#include "word.h"
#include "byteRegister.h"


class WordRegister {
private:
	
public:		
	ByteRegister low;			//composicion
	ByteRegister high;


	word_t Read() {				    
		return ((high<<8)|(low));	//el alto lo desplaza 8 a la izquierda y le suma el bajo
	}								

	void Write(word_t data) {		//entra uno de 16, y saco 2 de 8
		low = data & 0x00FF;		//mascara, me quedo con los 8 bits de menor peso, AND bit a bit, auto cast al guardar en low?
		high = (data & 0xFF00)>>8;	//me quedo con los 8 bits de mayor peso, y desplazo a la derecha 8, se pierde la parte alta (auto cast)
	}

	word_t operator++(int ) {		//Word++, post incremento, y dejo high y low con el valor correspondiente
		word_t val = ( ( ((word_t)high )<<8)|low ) + 1;		//desplazo high 8 posiciones a la izq, y hago un OR bit a bit con low
		high = (val >> 8);
		low = (val & 0x00FF);								//hago un AND bit a bit, mascara para la parte baja
		return val;
	}
	
	word_t operator++( ) {			//++Word, pre incremento, hago lo mismo k antes
		word_t val = ((((word_t)high)<<8)|low) + 1;
		high = (val >> 8);
		low = (val & 0x00FF);
		return val;
	}
};


#endif