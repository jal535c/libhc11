#include "rom.h"
#include <assert.h>
#include <stdlib.h>


ROM::ROM(word_t base, int mem_size) : MemoryObject() 	//llama al constructor de la clase base: enable = true
{	
	assert((mem_size >= 0) && (mem_size <= 0xFFFF));		//comprueba que el tamaño es el adecuado

	base_addr = base;
    size = mem_size;
	top_addr = base_addr + size;
	
	mem = new byte_t[size];
    assert(mem!=NULL);
}


ROM::~ROM() {
	if (mem!=NULL)
	    delete [] mem;
}


byte_t ROM::Read(word_t address) {
	return mem[address-base_addr];
}


void ROM::Program(word_t address, byte_t data) {
	mem[address-base_addr] = data;
}