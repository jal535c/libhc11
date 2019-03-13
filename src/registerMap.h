#ifndef __REGISTERMAP_H__
#define __REGISTERMAP_H__


#include "memoryObject.h"
#include "byteRegister.h"
#include "byte.h"
#include "word.h"
class RegisterObject;		//forward declarations
class RegisterFile;			

#define NUM_REGISTERS	64


class RegisterMap : public MemoryObject {		
private:
	RegisterObject* regs[NUM_REGISTERS];	//el array tendrá elementos de diferentes tipos, para definirlo uso el tipo del padre
	ByteRegister reserved;					
 
public:
	RegisterMap(word_t base, RegisterFile* regfile);
	~RegisterMap();
	byte_t Read(word_t address);			
	void Write(word_t address, byte_t data);	
};


#endif