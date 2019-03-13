#include "memoryMap.h"
#include "memoryObject.h"


MemoryObject* MemoryMap::GetObject(word_t address) {	//le paso una direccion y busca el MemoryObject que contiene, si no devuelve NULL
	MemoryObject* mo;				//mo puede ser un RegisterMap, RAM o ROM, depende de lo k añadí con AddMemoryObject
	list_iter<MemoryObject*> index;
      
	index = map.first();
	while (index.is_valid()) {
		mo = map[index];
		if (mo->Contains(address) && mo->enable) {	//si está dentro de los rangos, y está habilitado
            return mo;			//devuelve ese valor
        }
		index++;				//sino sigue buscando hasta encontrar uno valido
	}	
	
	//log("INVALID MEMORY ACCESS @ %X!\n",address);		//macro fprintf(stderr, a, b), no lo uso
	return NULL;
}


MemoryMap::MemoryMap() 
{
 	//no inicia el atributo map, se hace desde el metodo AddMemoryObject
}


MemoryMap::~MemoryMap() {		//elimina todos los elementos de la lista
	MemoryObject* mo;
	list_iter<MemoryObject*> index;
	
    index = map.first();
	while (index.is_valid()) {
		mo = map[index];	//usa la sobrecarga del operador [] de list
		delete mo;
		index++;
	}	
}


byte_t MemoryMap::Read(word_t address) {
    byte_t data;
    MemoryObject* mo = GetObject(address);		//si voy a leer en ram usa RAM::Read, en registro usa RegisterMap::Read
									
	if (mo)
	    data = mo->Read(address);		//mo apunta a un objeto MemoryObject concreto, como: RegisterMap, RAM, ROM

    return data;
}


void MemoryMap::Write(word_t address, byte_t data) {
    MemoryObject* mo = GetObject(address);

    if (mo)
	    mo->Write(address, data);
}


void MemoryMap::AddMemoryObject(MemoryObject* mo) {
	map.add(mo);		//map es de mi tipo list, usa su metodo add para añadir el elemento mo a la lista
}


bool MemoryMap::RemoveMemoryObject(MemoryObject* mo) {		//devuelve true si se elimina el objeto con exito
	list_iter<MemoryObject*> index;

    index = map.first();
	while (index.is_valid()) {
		if (mo == map[index]) {
             map.remove(index);            
            return true;			
		}
		index++;
	}		

	return false;
}