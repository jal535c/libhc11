#ifndef __ROM_H__
#define __ROM_H__


#include "memoryObject.h"
#include "bytetype.h"
#include "wordtype.h"


class ROM : public MemoryObject {
private:
	byte_t* mem;
	
public:	
    ROM(word_t base, int mem_size);
    ~ROM();
	byte_t Read(word_t address);
	void Write(word_t address, byte_t data) {	//no hace nada, las roms no se escriben, se programan
	}
    void Program(word_t address, byte_t data);	//funcion para escribir en la rom
};


#endif