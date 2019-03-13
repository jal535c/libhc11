#include "ram.h"
#include <assert.h>
#include <stdlib.h>		//para el tipo NULL


RAM::RAM(word_t base, int mem_size) : MemoryObject() 		//llama al constructor de la clase base, que pone enable=true
{	
	base_addr = base;				//inicia atributos derivados de la clase base
    size = mem_size;
	top_addr = base_addr + size;
	
	mem = new byte_t[size];		//mem un puntero a un bloque del heap, de tamaño size bytes
    assert(mem!=NULL);			//comprueba que mem no es null
}


RAM::~RAM() {
    if (mem!=NULL)			//delete seguro
	    delete [] mem;		//al usar new con [], el delete tambien con []
}


byte_t RAM::Read(word_t address) {
    return mem[address - base_addr];
}

	
void RAM::Write(word_t address, byte_t data) {    
    mem[address - base_addr] = data;
}