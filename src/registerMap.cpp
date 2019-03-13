#include "registerMap.h"
#include "registerObject.h"
#include "regfile.h"

#define SIZE_OF_REGISTER_BLOCK	0x3F


RegisterMap::RegisterMap(word_t base, RegisterFile* regfile) : MemoryObject() 		//llama al constructor de la clase base
{	
	base_addr = base;									
	top_addr = base_addr + SIZE_OF_REGISTER_BLOCK;
    size = SIZE_OF_REGISTER_BLOCK;

	
	regs[0x00] = &regfile->PORTA;	
	regs[0x01] = &reserved;
	regs[0x02] = &regfile->PIOC;
	regs[0x03] = &regfile->PORTC;	
	regs[0x04] = &regfile->PORTB;
	regs[0x05] = &regfile->PORTCL;
	regs[0x06] = &reserved;	
	regs[0x07] = &regfile->DDRC;
	regs[0x08] = &regfile->PORTD;
	regs[0x09] = &regfile->DDRD;
	regs[0x0A] = &regfile->PORTE;
	regs[0x0B] = &regfile->CFORC;
	regs[0x0C] = &regfile->OC1M;
	regs[0x0D] = &regfile->OC1D;
	regs[0x0E] = &regfile->TCNT.high;
	regs[0x0F] = &regfile->TCNT.low;

	regs[0x10] = &regfile->TIC1.high;
	regs[0x11] = &regfile->TIC1.low;
	regs[0x12] = &regfile->TIC2.high;
	regs[0x13] = &regfile->TIC2.low;
	regs[0x14] = &regfile->TIC3.high;
	regs[0x15] = &regfile->TIC3.low;
	regs[0x16] = &regfile->TOC1.high;
	regs[0x17] = &regfile->TOC1.low;
	regs[0x18] = &regfile->TOC2.high;
	regs[0x19] = &regfile->TOC2.low;
	regs[0x1A] = &regfile->TOC3.high;
	regs[0x1B] = &regfile->TOC3.low;
	regs[0x1C] = &regfile->TOC4.high;
	regs[0x1D] = &regfile->TOC4.low;
	regs[0x1E] = &regfile->TI4O5.high;
	regs[0x1F] = &regfile->TI4O5.low;
	
	regs[0x20] = &regfile->TCTL1;
	regs[0x21] = &regfile->TCTL2;
	regs[0x22] = &regfile->TMSK1;
	regs[0x23] = &regfile->TFLG1;
	regs[0x24] = &regfile->TMSK2;
	regs[0x25] = &regfile->TFLG2;
	regs[0x26] = &regfile->PACTL;
	regs[0x27] = &regfile->PACNT;
	regs[0x28] = &regfile->SPCR;
	regs[0x29] = &regfile->SPSR;
	regs[0x2A] = &regfile->SPDR;
	regs[0x2B] = &regfile->BAUD;
	regs[0x2C] = &regfile->SCCR1;
	regs[0x2D] = &regfile->SCCR2;
	regs[0x2E] = &regfile->SCSR;
	regs[0x2F] = &regfile->SCDR;
	
	regs[0x30] = &regfile->ADCTL;
	regs[0x31] = &regfile->ADR1;
	regs[0x32] = &regfile->ADR2;
	regs[0x33] = &regfile->ADR3;
	regs[0x34] = &regfile->ADR4;
	regs[0x35] = &regfile->BPROT;
	regs[0x36] = &regfile->EPROG;
	regs[0x37] = &reserved;	
	regs[0x38] = &reserved;	
	regs[0x39] = &regfile->OPTION;
	regs[0x3A] = &regfile->COPRST;
	regs[0x3B] = &regfile->PPROG;
	regs[0x3C] = &regfile->HPRIO;
	regs[0x3D] = &regfile->INIT;
	regs[0x3E] = &regfile->TEST1;
	regs[0x3F] = &regfile->CONFIG;
}


RegisterMap::~RegisterMap() {

}


byte_t RegisterMap::Read(word_t address) {
	return regs[address - base_addr]->Read();
}


void RegisterMap::Write(word_t address, byte_t data) {
	assert(address >= base_addr);				//comprueba que la direccion pertenece al bloque
	regs[address - base_addr]->Write(data);
}
