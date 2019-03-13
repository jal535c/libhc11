#ifndef __RAM_H__
#define __RAM_H__


#include "memoryObject.h"
#include "bytetype.h"
#include "wordtype.h"


class RAM : public MemoryObject {	
private: 	    
	byte_t* mem;
	
public:	        
    RAM(word_t base, int mem_size);
    ~RAM();
	byte_t Read(word_t address);	
    void Write(word_t address, byte_t data);
};


#endif