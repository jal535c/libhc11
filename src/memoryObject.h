#ifndef __MEMORYOBJECT_H__
#define __MEMORYOBJECT_H__


#include "byte.h"
#include "word.h"


class MemoryObject {
protected:				//accesibles desde clases derivadas
	word_t size;		
	word_t base_addr;
	word_t top_addr;				
public:
    bit_t enable;		//indica si el MemoryObject está habilitado (algunos modelos de chip no tienen memoria rom)
						

    MemoryObject();				
  	
	virtual ~MemoryObject() {	//destructor virtual, llama a los destructores de las clases hijas antes
		
	}

	virtual byte_t Read(word_t address) = 0;				
	virtual void Write(word_t address, byte_t data) = 0;	
	
    bool Contains(word_t addr) {						
		return (addr>=base_addr)&&(addr<=top_addr);
	}

    void Move(word_t new_base);		//para implementar el registro INIT (cambia el mapa de memoria)
};


#endif