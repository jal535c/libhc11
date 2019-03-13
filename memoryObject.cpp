#include "memoryObject.h"


MemoryObject::MemoryObject() 
{
    enable = true;
}


void MemoryObject::Move(word_t new_base) {
    int new_top;
        
    new_top = new_base + size;
    
    if (new_top > 0xFFFF)
        new_top = 0xFFFF;

    base_addr = new_base;
    top_addr = new_top;
}
