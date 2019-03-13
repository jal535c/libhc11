#ifndef __MEMORYMAP_H__
#define __MEMORYMAP_H__


#include "list.h"
#include "bytetype.h"
#include "wordtype.h"
class MemoryObject;


class MemoryMap {	
private:
	list<MemoryObject*> map;	//map es una lista de punteros a MemoryObject, template al k le paso tipos punteros MemoryObject
	
	MemoryObject* GetObject(word_t address);	//me dice el MemoryObject que hay en esa dir. Solo se usa desde aqui, por eso privado
												
public:	
	MemoryMap();		
	~MemoryMap();		

	byte_t Read(word_t address);
	void Write(word_t address, byte_t data);	

	void AddMemoryObject(MemoryObject* mo);    
    bool RemoveMemoryObject(MemoryObject* mo);
    
	byte_t operator[](const word_t& address) {	
		return Read(address);
	}
};


#endif