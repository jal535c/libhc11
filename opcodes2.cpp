#include "hc11.h"	//incluyo al general, k ya incluye a opcodes.h

//#include <stdio.h>	//no es necesaria

#include "timer.h" //para acceder a main_clock_divider

//estas instrucciones no necesitan las funciones de flag que estan en opbasic.h






//estaban ordenadas alfabeticamente, las voy a ordenar por codigo de operacion

//primero las 32 que faltaban de instru ext 0x18


/////////////////////////////////////////////////
//
//  TSY - Transfer from SP to IY
//  
//  Addr. Mode: Inherent
//  
//  Opcode: 18h
//	        30h
//
//  Clocks: 4	(menos 2 del fetch y op18h)
//
//  Flags:  no
//
////////////////////////////////////////////////

void HC11::opIY_30h(HC11 *hc11)
{
	hc11->_clock();
	hc11->_clock();
	hc11->IY = hc11->SP+1;		// Y = SP+1
}


/////////////////////////////////////////////////
//
//  TYS - Transfer from IY to SP
//  
//  Addr. Mode: Inherent
//  
//  Opcode: 18h
//			35h
//
//  Clocks: 4	(-2)
//
//  Flags:  no
//
////////////////////////////////////////////////

void HC11::opIY_35h(HC11 *hc11)
{
	hc11->_clock();
	hc11->_clock();
	hc11->SP = hc11->IY -1;		// SP= Y-1
}


/////////////////////////////////////////////////
//
//  PULY - pull IY from stack
//  
//  Addr. Mode: INH
//  
//  Opcodes: 18h
//           38h
//
//  Clocks: 6	(-2)
//
//  Flags:	no
//
////////////////////////////////////////////////

void HC11::opIY_38h(HC11 *hc11)
{
	hc11->_clock();
	hc11->_clock();
	hc11->IY.high(hc11->Pull());	//saca Y de la pila, la funcion Pull() consume 1 ciclo de reloj
	hc11->IY.low(hc11->Pull());        
}


/////////////////////////////////////////////////
//
//  PSHY - push IY to stack
//  
//  Addr. Mode: INH
//  
//  Opcodes: 18h
//           3Ch
//           
//  Clocks: 5
//
//  Flags:	no
//
////////////////////////////////////////////////

void HC11::opIY_3Ch(HC11 *hc11)
{
	hc11->_clock();
	hc11->Push(hc11->IY.low());        //mete Y en la pila, consume 1 ciclo
	hc11->Push(hc11->IY.high());        
}


/////////////////////////////////////////////////
//
//  NEG - Negate Memory Byte
//  
//  Addr. Mode: IND, Y
//  
//  Opcodes: 18h
//           60h
//           
//  Clocks: 7
//
//  Flags:	N, Z, V, C
//
////////////////////////////////////////////////

void HC11::opIY_60h(HC11 *hc11)
{
	byte_t data;
	word_t address;
	
	address = IndYAddr(hc11);		//consume 2 ciclos

	hc11->_clock();
	data = hc11->memory[address];

	hc11->CCR.V(data==0x80);    //solo hay overflow si vale $80, da igual antes o despues ya que no cambia            
	hc11->CCR.C(data != 0);		//siempre hay carry excepto si vale 0

	hc11->_clock();
	data = ~data + 1;    //complemento a 2, o lo que es lo mismo, M = 0-M

	hc11->_clock();
	hc11->memory.Write(address, data);
	
	hc11->CCR.Z(data == 0);    
	hc11->CCR.N((data & 0x80) ? 1 : 0);
}


/////////////////////////////////////////////////
//
//  ROR - rotate right
//  
//  Addr. Mode: IND, Y
//  
//  Opcodes:18h 
//          66h
//
//  Clocks: 7
//
//  Flags:	N, Z, V, C
//
////////////////////////////////////////////////

void HC11::opIY_66h(HC11 *hc11)
{   
	word_t address;
	Byte data;
	bit_t carry_old = hc11->CCR.C();	// Save the current carry bit

	address = IndYAddr(hc11);	//consume 2 ciclos
	
	hc11->_clock();               
	data = hc11->memory[address];
	

	hc11->CCR.C(data.bit0());                    

	hc11->_clock();               
	data = (data >> 1) | (carry_old << 7);	//desplaza a la derecha, y pone en el bit7 el carry_old
		
	hc11->CCR.Z(data == 0);                        
	hc11->CCR.N(data.bit7());                        
	hc11->CCR.V(hc11->CCR.C() ^ hc11->CCR.N());		//overflow: C exor N
	
	hc11->_clock();               
	hc11->memory.Write(address, data);
}


/////////////////////////////////////////////////
//
//  ROL - rotate left
//  
//  Addr. Mode: IND, Y
//  
//  Opcodes:18h 
//          69h
//
//  Clocks: 7
//
//  Flags:	N, Z, V, C
//
////////////////////////////////////////////////

void HC11::opIY_69h(HC11 *hc11)
{   
	word_t address;
	Byte data;
	byte_t carry;

	carry = hc11->CCR.C();	//guardo el carry actual

	address = IndYAddr(hc11);	//consume 2 ciclos
	
	hc11->_clock();               
	data = hc11->memory[address];
	
	hc11->CCR.C(data.bit7());	//el nuevo carry es el bit7

	hc11->_clock();               
	data = (data << 1) | carry;		//desplazo a la izquierda y le sumo el viejo carry
		
	hc11->CCR.Z(data == 0);                        
	hc11->CCR.N(data.bit7());                        
	hc11->CCR.V(hc11->CCR.C() ^ hc11->CCR.N());    //overflow: C exor N            
	
	hc11->_clock();               
	hc11->memory.Write(address, data);
}


/////////////////////////////////////////////////
//
//  TST - Test
//  
//  Addr. Mode: IND, Y
//  
//  Opcodes: 	18h
//				6Dh
//
//  Clocks: 7
//
//  Flags:  N, Z, V=0, C=0
//
////////////////////////////////////////////////

void HC11::opIY_6Dh(HC11 *hc11)		//M-0
{
	Word data;
	word_t address;

	address = IndYAddr(hc11);		//consume 2 ciclos
	data = hc11->memory[address];	
	
	hc11->_clock();		    
	hc11->_clock();		
	hc11->CCR.N(data.bit7());
	hc11->CCR.Z(data == 0);
	hc11->CCR.V(0);
	hc11->CCR.C(0);		

	hc11->_clock(); 
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
//
//  JMP - Jump
//  
//  Addr. Mode: IND, Y
//  
//  Opcodes: 18h
//           6Eh
//
//  Clocks:	4
//
//  Flags:	no
//
////////////////////////////////////////////////

void HC11::opIY_6Eh(HC11 *hc11)
{	
	byte_t offset;    
	
	hc11->_clock();	
	offset = hc11->memory[hc11->PC];
	hc11->PC++;

	hc11->_clock();
	hc11->PC = hc11->IY + offset;	//carga PC
}


/////////////////////////////////////////////////
//
//  CPY - Compare Index Register Y
//  
//  Addr. Mode: IMM
//  
//  Opcodes: 18h
//           8Ch
//
//  Clocks: 5
//
//  Flags:	N, Z, V, C
//
////////////////////////////////////////////////

void HC11::opIY_8Ch(HC11 *hc11)
{	
	CCRReg mask;    
	Word data;
	Word result;

	hc11->_clock();	
	data.high(hc11->memory[hc11->PC]);
	hc11->PC++;   
	
	hc11->_clock();	
	data.low(hc11->memory[hc11->PC]);
	hc11->PC++;    

	result = hc11->IY - data;		// Y-M
	
	hc11->CCR.N(result.bit15());
	hc11->CCR.Z(result == 0);
	
	hc11->CCR.V(hc11->IY.bit15() && !data.bit15() && !result.bit15() ||
				!hc11->IY.bit15() && data.bit15() && result.bit15());                
	
	hc11->CCR.C((!hc11->IY.bit15()) && data.bit15() ||
				data.bit15() && result.bit15() ||
				result.bit15() && (!hc11->IY.bit15()));


	hc11->_clock();		//faltaba
}


/////////////////////////////////////////////////
//
//  XGDY - Exchange ACCD and IY
//  
//  Addr. Mode: Inherent/Y
//  
//  Opcode: 18h
//	        8Fh 
//
//  Clocks:	4
//
//  Flags:  no
//
////////////////////////////////////////////////

void HC11::opIY_8Fh(HC11 *hc11)
{
	word_t temp;

	temp = ((hc11->ACCA)<<8) | hc11->ACCB;		//mete D en temp

	hc11->_clock();
	hc11->ACCB = hc11->IY.low();		//mete Y en D
	hc11->ACCA = hc11->IY.high();

	hc11->_clock();
	hc11->IY = temp;					//mete temp en Y
}


/////////////////////////////////////////////////
//
//  CPY - Compare Index Register Y
//  
//  Addr. Mode: DIR
//  
//  Opcodes:18h
//          9Ch
//
//  Clocks: 6
//
//  Flags:	N, Z, V, C
//
////////////////////////////////////////////////

void HC11::opIY_9Ch(HC11 *hc11)
{	
	CCRReg mask;    
	Word data;
	Word result;
	word_t address;

	address = DirAddr(hc11);	//consume 1 ciclo

	hc11->_clock();	
	data.high(hc11->memory[address]);

	hc11->_clock();	
	data.low(hc11->memory[address+1]);

	result = hc11->IY - data;		// Y-M

	hc11->CCR.N(result.bit15());
	hc11->CCR.Z(result == 0);
	
	hc11->CCR.V(hc11->IY.bit15() && !data.bit15() && !result.bit15() ||
				!hc11->IY.bit15() && data.bit15() && result.bit15());                
	
	hc11->CCR.C((!hc11->IY.bit15()) && data.bit15() ||
				data.bit15() && result.bit15() ||
				result.bit15() && (!hc11->IY.bit15()));


	hc11->_clock();		//faltaba
}


/////////////////////////////////////////////////
//
//  LDAA - load ACCA
//  
//  Addr. Mode: IND, Y
//  
//  Opcodes: 18h
//           A6h
//
//  Clocks: 5
//
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::opIY_A6h(HC11 *hc11)
{	
	word_t address;    
	
	address = IndYAddr(hc11);		//consume 2 ciclos

	hc11->_clock();	
	hc11->ACCA = hc11->memory[address];    //A=M


	hc11->CCR.N(hc11->ACCA.bit7());
	hc11->CCR.Z(hc11->ACCA == 0);    
	hc11->CCR.V(0);    
		
}


/////////////////////////////////////////////////
//
//  STAA - Store ACCA
//  
//  Addr. Mode: IND, Y
//  
//  Opcodes: 18h
//           A7h
//
//  Clocks: 5
//
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::opIY_A7h(HC11 *hc11)
{	
	word_t address;

	address = IndYAddr(hc11);		//consume 2 ciclos

	hc11->_clock();	
	hc11->memory.Write(address, hc11->ACCA);	//M=A


	hc11->CCR.N(hc11->ACCA.bit7());
	hc11->CCR.Z(hc11->ACCA == 0);    
	hc11->CCR.V(0);
		
}


/////////////////////////////////////////////////
//
//  ORAA - Inclusize OR
//  
//  Addr. Mode: IND, Y
//  
//  Opcodes: 18h 
//           AAh
//           
//  Clocks: 5
//
//  Flags:	N, Z, V=0
//
////////////////////////////////////////////////

void HC11::opIY_AAh(HC11 *hc11)
{
	byte_t data;
	word_t address;    
	
	address = IndYAddr(hc11);		//consume 2 ciclos

	hc11->_clock();  
	data = hc11->memory[address];    

	hc11->ACCA = hc11->ACCA | data;		//A= A + M

	
	hc11->CCR.N(hc11->ACCA.bit7());
	hc11->CCR.Z(hc11->ACCA == 0);
	hc11->CCR.V(0);    

}


/////////////////////////////////////////////////
//
//  CPY - Compare Index Register Y
//  
//  Addr. Mode: IND, Y
//  
//  Opcodes: 18h
//           ACh
//
//  Clocks: 7
//
//  Flags:	N, Z, V, C
//
////////////////////////////////////////////////

void HC11::opIY_ACh(HC11 *hc11)
{	
	CCRReg mask;    
	Word data;
	Word result;
	word_t address;


	address = IndYAddr(hc11);	//consume 2 ciclos

	hc11->_clock();	
	data.high(hc11->memory[address]);

	hc11->_clock();	
	data.low(hc11->memory[address+1]);

	result = hc11->IY - data;		//Y-M

	hc11->CCR.N(result.bit15());
	hc11->CCR.Z(result == 0);
	
	hc11->CCR.V(hc11->IY.bit15() && !data.bit15() && !result.bit15() ||
				!hc11->IY.bit15() && data.bit15() && result.bit15());                
	
	hc11->CCR.C((!hc11->IY.bit15()) && data.bit15() ||
				data.bit15() && result.bit15() ||
				result.bit15() && (!hc11->IY.bit15()));


	hc11->_clock();		//faltaba

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
//
//  JSR - Jump to subroutine
//  
//  Addr. Mode: IND, Y
//  
//  Opcodes: 18h
//           ADh
//
//  Clocks:	7
//
//  Flags:	no
//
////////////////////////////////////////////////

void HC11::opIY_ADh(HC11 *hc11)
{	
	byte_t offset;    
	
	hc11->_clock();	
	offset = hc11->memory[hc11->PC];
	hc11->PC++;
	
	hc11->Push(hc11->PC.low());		//consume 1 ciclo
	hc11->Push(hc11->PC.high());

	hc11->_clock();	
	hc11->_clock();
	hc11->PC = hc11->IY + offset;
}


/////////////////////////////////////////////////
//
//  LDS - Load SP
//  
//  Addr. Mode: IND, Y
//  
//  Opcodes: 18h
//           AEh
//
//  Clocks: 6
//
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::opIY_AEh(HC11 *hc11)
{	
	word_t address;
	
	address = IndYAddr(hc11);		//consume 2 ciclos

	hc11->_clock();	
	hc11->SP.high(hc11->memory[address]);		//SP = M

	hc11->_clock();	
	hc11->SP.low(hc11->memory[address+1]);


	hc11->CCR.N((hc11->SP & 0x8000)>>15);
	hc11->CCR.Z(hc11->SP == 0);
	hc11->CCR.V(0);    
}


/////////////////////////////////////////////////
//
//  STS - Store SP
//  
//  Addr. Mode: IND, Y
//  
//  Opcodes: 18h
//           AFh
//
//  Clocks: 6
//
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::opIY_AFh(HC11 *hc11)
{	
	word_t address;

	address = IndYAddr(hc11);		//consume 2 ciclos

	hc11->_clock();	
	hc11->memory.Write(address, hc11->SP.high());		//M = SP
	
	hc11->_clock();	
	hc11->memory.Write(address+1, hc11->SP.low());


	hc11->CCR.N((hc11->SP & 0x8000)?1:0);
	hc11->CCR.Z(hc11->SP == 0);    
	hc11->CCR.V(0);
}


/////////////////////////////////////////////////
//
//  CPY - Compare Index Register Y
//  
//  Addr. Mode:Ext
//  
//  Opcodes: 18h BCh
//
//  Clocks:	7
//
//  Flags:	N, Z, V, C
//
////////////////////////////////////////////////

void HC11::opIY_BCh(HC11 *hc11)
{	
	CCRReg mask;    
	Word data;
	Word result;
	word_t address;

	address = ExtAddr(hc11);	//consume 2 ciclos

	hc11->_clock();	
	data.high(hc11->memory[address]);

	hc11->_clock();	
	data.low(hc11->memory[address+1]);

	result = hc11->IY - data;    //Y - M
	

	hc11->CCR.N(result.bit15());
	hc11->CCR.Z(result == 0);
	
	hc11->CCR.V(hc11->IY.bit15() && !data.bit15() && !result.bit15() ||
				!hc11->IY.bit15() && data.bit15() && result.bit15());                
	
	hc11->CCR.C((!hc11->IY.bit15()) && data.bit15() ||
				data.bit15() && result.bit15() ||
				result.bit15() && (!hc11->IY.bit15()));

	
	hc11->_clock();		//faltaba
}


/////////////////////////////////////////////////
//
//  LDY - Load IY
//  
//  Addr. Mode: IMM
//  
//  Opcodes: 18h
//           CEh
//
//  Clocks: 4
//
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::opIY_CEh(HC11 *hc11)
{	    
	hc11->_clock();	
	hc11->IY.high(hc11->memory[hc11->PC]);		// Y = M
	hc11->PC++;
	
	hc11->_clock();	
	hc11->IY.low(hc11->memory[hc11->PC]);
	hc11->PC++;


	hc11->CCR.N((hc11->IY & 0x8000)>>15);
	hc11->CCR.Z(hc11->IY == 0);
	hc11->CCR.V(0);    
}


/////////////////////////////////////////////////
//
//  LDY - Load IY
//  
//  Addr. Mode: DIR
//  
//  Opcodes: 18h
//           DEh
//
//  Clocks: 5
//
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::opIY_DEh(HC11 *hc11)
{	
	byte_t address;
	
	address = DirAddr(hc11);		//consume 1 ciclo

	hc11->_clock();	
	hc11->IY.high(hc11->memory[(word_t)address]);		//Y=M

	hc11->_clock();	
	hc11->IY.low(hc11->memory[(word_t)address+1]);


	hc11->CCR.N((hc11->IY & 0x8000)>>15);
	hc11->CCR.Z(hc11->IY == 0);
	hc11->CCR.V(0);    
}


/////////////////////////////////////////////////
//
//  STY - Store Y
//  
//  Addr. Mode: DIR
//  
//  Opcodes: 18h
//           DFh
//
//  Clocks: 5
//
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::opIY_DFh(HC11 *hc11)
{	
	byte_t address;
	
	address = DirAddr(hc11);	//consume 1 ciclo

	hc11->_clock();	
	hc11->memory.Write((word_t)address, hc11->IY.high());		// M = Y
	
	hc11->_clock();	
	hc11->memory.Write((word_t)address+1, hc11->IY.low());


	hc11->CCR.N((hc11->IY & 0x8000) ? 1 : 0);
	hc11->CCR.Z(hc11->IY == 0);
	hc11->CCR.V(0);
}


/////////////////////////////////////////////////
//
//  LDAB - load ACCB
//  
//  Addr. Mode: IND, Y
//  
//  Opcodes: 18h
//           E6h
//
//  Clocks: 5
//
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::opIY_E6h(HC11 *hc11)
{	
	word_t address;
	
	address = IndYAddr(hc11);		//consume 2 ciclos

	hc11->_clock();	
	hc11->ACCB = hc11->memory[address];		// B = M

	
	hc11->CCR.N(hc11->ACCB.bit7());
	hc11->CCR.Z(hc11->ACCB == 0);    
	hc11->CCR.V(0);    
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
//
//  STAB - Store ACCB
//  
//  Addr. Mode: IND, Y
//  
//  Opcodes: 18h
//           E7h
//
//  Clocks: 5
//
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::opIY_E7h(HC11 *hc11)
{	
	word_t address;

	address = IndYAddr(hc11);		//consume 2 ciclos

	hc11->_clock();	
	hc11->memory.Write(address, hc11->ACCB);	// M = B


	hc11->CCR.N(hc11->ACCB.bit7());
	hc11->CCR.Z(hc11->ACCB == 0);    
	hc11->CCR.V(0);
}


/////////////////////////////////////////////////
//
//  ORAB - Inclusize OR
//  
//  Addr. Mode: IND, Y
//  
//  Opcodes: 18h 
//           EAh
//           
//  Clocks: 5
//
//  Flags:	N, Z, V=0
//
////////////////////////////////////////////////

void HC11::opIY_EAh(HC11 *hc11)
{
	byte_t data;    
	word_t address;
	
	address = IndYAddr(hc11);		//consume 2 ciclos

	hc11->_clock();  
	data = hc11->memory[address];    

	hc11->ACCB = hc11->ACCB | data;		//B = B + M


	hc11->CCR.N(hc11->ACCB.bit7());
	hc11->CCR.Z(hc11->ACCB == 0);
	hc11->CCR.V(0);    
}


/////////////////////////////////////////////////
//
//  LDD - load ACCD
//  
//  Addr. Mode: IND, Y
//  
//  Opcodes: 18h
//           ECh
//
//  Clocks: 6
//
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::opIY_ECh(HC11 *hc11)
{	
	word_t address;
	
	address = IndYAddr(hc11);		//consume 2 ciclos

	hc11->_clock();	
	hc11->ACCA = hc11->memory[address];		// D = M

	hc11->_clock();	
	hc11->ACCB = hc11->memory[address+1];


	hc11->CCR.N(hc11->ACCA.bit7());				//el bit7 de A es el bit15 de D
	hc11->CCR.Z((hc11->ACCB | hc11->ACCA) == 0);    
	hc11->CCR.V(0);
}


/////////////////////////////////////////////////
//
//  STD - Store ACCD
//  
//  Addr. Mode: IND, Y
//  
//  Opcodes: 18h
//           EDh
//
//  Clocks: 6
//
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::opIY_EDh(HC11 *hc11)
{	
	word_t address;
	
	address = IndYAddr(hc11);		//consume 2 ciclos

	hc11->_clock();	
	hc11->memory.Write(address, hc11->ACCA);	// M = D
	
	hc11->_clock();	
	hc11->memory.Write(address+1, hc11->ACCB);


	hc11->CCR.N(hc11->ACCA.bit7());			//el bit7 de A es el bit15 de D
	hc11->CCR.Z((hc11->ACCB | hc11->ACCA) == 0);    
	hc11->CCR.V(0);
}


////////////////////////////////////////////////
//
//  LDY - Load IY
//  
//  Addr. Mode: IND, Y
//  
//  Opcodes: 18h
//           EEh
//
//  Clocks: 6
//
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::opIY_EEh(HC11 *hc11)
{	
	word_t address;
	
	address = IndYAddr(hc11);		//consume 2 ciclos

	hc11->_clock();	
	hc11->IY.high(hc11->memory[address]);	// Y = M

	hc11->_clock();	
	hc11->IY.low(hc11->memory[address+1]);


	hc11->CCR.N((hc11->IY & 0x8000)>>15);
	hc11->CCR.Z(hc11->IY == 0);
	hc11->CCR.V(0);    
}


/////////////////////////////////////////////////
//
//  STY - Store Y
//  
//  Addr. Mode: IND, Y
//  
//  Opcodes: 18h
//           EFh
//
//  Clocks: 6
//
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::opIY_EFh(HC11 *hc11)
{	
	word_t address;
	
	address = IndYAddr(hc11);		//consume 2 ciclos

	hc11->_clock();	
	hc11->memory.Write(address, hc11->IY.high());    // M = Y

	hc11->_clock();	
	hc11->memory.Write(address+1, hc11->IY.low());


	hc11->CCR.N((hc11->IY & 0x8000)?1:0);
	hc11->CCR.Z(hc11->IY == 0);    
	hc11->CCR.V(0);
}


/////////////////////////////////////////////////
//
//  LDY - Load IY
//  
//  Addr. Mode: EXT
//  
//  Opcodes: 18h
//           FEh
//
//  Clocks: 6
//
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::opIY_FEh(HC11 *hc11)
{	
	word_t address;
	
	address = ExtAddr(hc11);	//consume 2 ciclos

	hc11->_clock();	
	hc11->IY.high(hc11->memory[address]);	//Y=M

	hc11->_clock();	
	hc11->IY.low(hc11->memory[address+1]);


	hc11->CCR.N((hc11->IY & 0x8000)>>15);
	hc11->CCR.Z(hc11->IY == 0);
	hc11->CCR.V(0);    
}


/////////////////////////////////////////////////
//
//  STY - Store Y
//  
//  Addr. Mode: EXT
//  
//  Opcodes: 18h
//           FFh
//
//  Clocks: 6
//
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::opIY_FFh(HC11 *hc11)
{	
	word_t address;

	address = ExtAddr(hc11);	//consume 2 ciclos

	hc11->_clock();	
	hc11->memory.Write(address, hc11->IY.high());		// M = Y
	
	hc11->_clock();	
	hc11->memory.Write(address+1, hc11->IY.low());

	
	hc11->CCR.N((hc11->IY & 0x8000)?1:0);
	hc11->CCR.Z(hc11->IY == 0);    
	hc11->CCR.V(0);    
}





















///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//ahora las instrucciones que faltaban de opcodes.cpp, estaban ordenadas alfabeticamente, ahora por codigo de operacion


/////////////////////////////////////////////////
//
//  TEST - TEST Operation. Solo funciona en modo Test, incrementa el PC continuamente hasta recibir un reset
//  
//  Addr. Mode: Inherent
//  
//  Opcodes: 00h
//
//  Clocks: ?	puede que muchos, hasta recibir un reset
//
//  Flags:  ninguno
//
////////////////////////////////////////////////

void HC11::op_0h(HC11 *hc11)
{	
	if(hc11->mode != SPECIAL_TEST)
		HC11::op_illegal(hc11);				
	else 
		for(;;)
		{
			hc11->_clock();	
			hc11->PC = hc11->PC+1;
		}		
}


/////////////////////////////////////////////////
//
//  NOP - no operation. No hace nada, solo consume 2 ciclos de reloj
//  
//  Addr. Mode: INH
//  
//  Opcodes: 01h
//           
//  Clocks: 2
//
//  Flags: ninguno
//
////////////////////////////////////////////////

void HC11::op_1h(HC11 *hc11)
{
	hc11->_clock();    
}


/////////////////////////////////////////////////
//
//  TAP - Transfer ACCA to CCR
//  
//  Addr. Mode: Inherent
//  
//  Opcodes: 06h
//
//  Clocks: 2 (uno el fetch y otro el que esta aqui)
//
//  Flags:  todos cambian segun A excepto X (si estaba a 0 debe seguir a 0, si estaba a 1 puede cambiar a 0 o seguir en 1)
//
////////////////////////////////////////////////

void HC11::op_6h(HC11 *hc11)
{	
	hc11->_clock();
	
	if(!hc11->CCR.X())	//cannot set the X bit in CCR
		hc11->CCR = hc11->ACCA & 0xBF;	//copio todo menos el bit6
	else {
		hc11->CCR = hc11->ACCA;		//copio todo

		if (hc11->CCR.X() == 0)			//si es 0
			hc11->xbit_cleared = true;	//activo la variable que indica que el flag X ha sido puesto a 0
	
	}
}


/////////////////////////////////////////////////
//
//  TPA - Transfer CCR to ACCA
//  
//  Addr. Mode: Inherent
//  
//  Opcodes: 07h
//
//  Clocks: 2
//
//  Flags:  ninguno
//
////////////////////////////////////////////////

void HC11::op_7h(HC11 *hc11)
{	
	hc11->_clock();	
	hc11->ACCA = hc11->CCR;
}


/////////////////////////////////////////////////
//
//  CLV - Clear Twos Complement Overflow Bit
//  
//  Addr. Mode: INH
//  
//  Opcodes: 0A
//
//  Clocks: 2
//
//  Flags: V=0
//
////////////////////////////////////////////////

void HC11::op_Ah(HC11 *hc11)
{	
	hc11->_clock();	
	hc11->CCR.V(0);
}


/////////////////////////////////////////////////
//
//  SEV - Set Twos Complement Overflow Bit
//  
//  Addr. Mode: INH
//  
//  Opcodes: 0B
//
//  Clocks: 2
//
//  Flags: V=1
//
////////////////////////////////////////////////

void HC11::op_Bh(HC11 *hc11)
{	
	hc11->_clock();	
	hc11->CCR.V(1);
}


/////////////////////////////////////////////////
//
//  CLC - clear Carry Flag
//  
//  Addr. Mode: INH
//  
//  Opcodes: 0C
//
//  Clocks: 2
//
//  Flags: C=0
//
////////////////////////////////////////////////

void HC11::op_Ch(HC11 *hc11)
{	
	hc11->_clock();	
	hc11->CCR.C(0);
}


/////////////////////////////////////////////////
//
//  SEC - Set Carry Flag
//  
//  Addr. Mode: INH
//  
//  Opcodes: 0D
//
//  Clocks: 2
//
//  Flags: C=1
//
////////////////////////////////////////////////

void HC11::op_Dh(HC11 *hc11)
{	
	hc11->_clock();	
	hc11->CCR.C(1);
}


/////////////////////////////////////////////////
//
//  CLI - clear Interrupt Mask
//  
//  Addr. Mode: INH
//  
//  Opcodes: 0E
//
//  Clocks: 2
//
//  Flags: I=0
//
////////////////////////////////////////////////

void HC11::op_Eh(HC11 *hc11)
{	
	hc11->_clock();	
	hc11->CCR.I(0);
}


/////////////////////////////////////////////////
//
//  SEI - Set Interrupt Mask
//  
//  Addr. Mode: INH
//  
//  Opcodes: 0F
//
//  Clocks: 2
//
//  Flags: I=1
//
////////////////////////////////////////////////

void HC11::op_Fh(HC11 *hc11)
{	
	hc11->_clock();	
	hc11->CCR.I(1);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
//
//  TAB - Transfer ACCA to ACCB
//  
//  Addr. Mode: Inherent
//  
//  Opcodes: 16h
//
//  Clocks: 2
//
//  Flags:  N, Z  v=0
//
////////////////////////////////////////////////

void HC11::op_16h(HC11 *hc11)
{	
	hc11->_clock();	
	hc11->ACCB = hc11->ACCA;


	if(hc11->ACCB.bit7())	//indicador N
		hc11->CCR.N(1);	
	else
		hc11->CCR.N(0);		//faltaba el else
	

	if(hc11->ACCB == 0)		//indicador de Z
		hc11->CCR.Z(1);
	else 
		hc11->CCR.Z(0);		//faltaba el else

	
	hc11->CCR.V(0);			//indicador V
}


/////////////////////////////////////////////////
//
//  TBA - Transfer ACCB to ACCA
//  
//  Addr. Mode: Inherent
//  
//  Opcodes: 17h
//
//  Clocks: 2
//
//  Flags:  n, z, v=0
//
////////////////////////////////////////////////

void HC11::op_17h(HC11 *hc11)
{	
	hc11->_clock();	
	hc11->ACCA = hc11->ACCB;


	if(hc11->ACCA.bit7())	//flag N
		hc11->CCR.N(1);		
	else
		hc11->CCR.N(0);		//faltaba el else

	
	if(hc11->ACCA == 0)		//flag Z
		hc11->CCR.Z(1);
	else 
		hc11->CCR.Z(0);		//faltaba el else


	hc11->CCR.V(0);			//flag V
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////
//
//  BRA - Branch Always
//  
//  Addr. Mode: REL
//  
//  Opcodes: 20h
//
//  Clocks: 3 (uno el fetch y 2 de aqui)
//	
//  Flags: ninguno
//
////////////////////////////////////////////////

void HC11::op_20h(HC11 *hc11)
{
	signed char offset;		//rango desde -128 a 127 en decimal
	
	hc11->_clock();	
	offset = (signed)hc11->memory[hc11->PC];	//tengo en cuenta el signo
	hc11->PC++;

	hc11->_clock();	
	hc11->PC = hc11->PC + offset;	//salto relativo: escribo en PC lo que ya habia mas un offset
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
//
//  TSX - Transfer from SP to IX
//  
//  Addr. Mode: Inherent
//  
//  Opcodes: 30h
//
//  Clocks: 3
//
//  Flags:  ninguno
//
////////////////////////////////////////////////

void HC11::op_30h(HC11 *hc11)
{
	hc11->_clock();
	hc11->_clock();
	hc11->IX = hc11->SP+1;	
}


/////////////////////////////////////////////////
//
//  PULA - pull ACCA from stack
//  
//  Addr. Mode: INH
//  
//  Opcodes: 32h
//           
//  Clocks: 4
//
//  Flags: ninguno
//
////////////////////////////////////////////////

void HC11::op_32h(HC11 *hc11)
{
	hc11->_clock();
	hc11->_clock();
	hc11->ACCA = hc11->Pull();    //la funcion Pull() lleva un _clock(), lo que hace es leer de la mem segun SP (o sea saca de la pila)
}


/////////////////////////////////////////////////
//
//  PULB - pull ACCB from stack
//  
//  Addr. Mode: INH
//  
//  Opcodes: 33h
//           
//  Clocks: 4
//
//  Flags: ninguno
//
////////////////////////////////////////////////

void HC11::op_33h(HC11 *hc11)
{
	hc11->_clock();
	hc11->_clock();
	hc11->ACCB = hc11->Pull();    //saca de la pila, la funcion Pull() ya lleva un _clock()
}


/////////////////////////////////////////////////
//
//  TXS - Transfer from IX to SP
//  
//  Addr. Mode: Inherent
//  
//  Opcode: 35h
//
//  Clocks: 3
//
//  Flags:  
//
////////////////////////////////////////////////

void HC11::op_35h(HC11 *hc11)
{
	hc11->_clock();
	hc11->_clock();	
	hc11->SP = hc11->IX-1;		//copia IX-1 en SP
}


/////////////////////////////////////////////////
//
//  PSHA - push ACCA to stack
//  
//  Addr. Mode: INH
//  
//  Opcodes: 36h
//           
//  Clocks: 3
//
//  Flags: ninguno
//
////////////////////////////////////////////////

void HC11::op_36h(HC11 *hc11)
{
	hc11->_clock();
	hc11->Push(hc11->ACCA);        //la funcion Push() llama a _clock(), escribe en el parametro en mem segun SP, y decrementa SP
}


/////////////////////////////////////////////////
//
//  PSHB - push ACCB to stack
//  
//  Addr. Mode: INH
//  
//  Opcodes: 37h
//           
//  Clocks: 3
//
//  Flags: ninguno
//
////////////////////////////////////////////////

void HC11::op_37h(HC11 *hc11)
{
	hc11->_clock();
	hc11->Push(hc11->ACCB);        //la funcion Push() llama a _clock(), escribe en el parametro en mem segun SP, y decrementa SP
}


/////////////////////////////////////////////////
//
//  PULX - pull IX from stack
//  
//  Addr. Mode: INH
//  
//  Opcodes: 38h
//           
//  Clocks: 5
//
//  Flags: ninguno
//
////////////////////////////////////////////////

void HC11::op_38h(HC11 *hc11)
{
	hc11->_clock();
	hc11->_clock();
	hc11->IX.high(hc11->Pull());	//motorola alreves de intel, primero high y despues low
	hc11->IX.low(hc11->Pull());		//Pull() llama a _clock(), incrementa SP, lee de la mem segun SP
}


/////////////////////////////////////////////////
//
//  RTS - return from interrupt
//  
//  Addr. Mode: INH
//  
//  Opcodes: 39h
//
//  Clocks: 5
//
//  Flags: ninguno
//
////////////////////////////////////////////////

void HC11::op_39h(HC11 *hc11)
{
	hc11->_clock();
	hc11->_clock();
	hc11->PC.high(hc11->Pull());	//Pull() llama a _clock(), incrementa SP, lee de la mem segun SP
	hc11->PC.low(hc11->Pull());    
}


/////////////////////////////////////////////////
//
//  RTI - return from interrupt
//  
//  Addr. Mode: INH
//  
//  Opcodes: 3Bh
//
//  Clocks: 12
//
//  Flags: todos son afectados menos X que depende de si estaba antes a 0 (no puede cambiar) o a 1 (si puede cambiar)
//
////////////////////////////////////////////////

void HC11::op_3Bh(HC11 *hc11) {

	hc11->_clock();
	hc11->_clock();
	
	hc11->UnStackRegisters();	//desapila los registros, tarda 9 ciclos uno por cada funcion pull()
	
	//el bit CCR.X no puede cambiar de 0 a 1 despues de ser desapilado CCR
	//solo se pone a 1 despues de un reset o al aceptar xirq,
	//xbit_cleared a true indica que CCR.X valía 0 

	if(hc11->xbit_cleared)			//si estaba a 0, ahora tambien 0
		hc11->CCR.X(0);          
	else if(!hc11->CCR.X())			//si estaba a 1, y ahora vale 0, pongo xbit_cleared a true
		hc11->xbit_cleared = true;

	//    log("rti\n",0);
}


/////////////////////////////////////////////////
//
//  PSHX - push IX to stack
//  
//  Addr. Mode: INH
//  
//  Opcodes: 3Ch
//           
//  Clocks: 4
//
//  Flags: ninguno
//
////////////////////////////////////////////////

void HC11::op_3Ch(HC11 *hc11)
{
	hc11->_clock();
	hc11->Push(hc11->IX.low());        //mete primero low (motorola alreves de intel), Push() llama a _clock()
	hc11->Push(hc11->IX.high());        
}


/////////////////////////////////////////////////
//
//  WAI - Wait for Interrupt. Espera a que ocurra una int. Mirar el grafico del libro
//  
//  Addr. Mode: Inherint
//  
//  Opcode: 3E
//	    
//  Clocks: 14 + n
//
//  Flags:  None
//
////////////////////////////////////////////////

void HC11::op_3Eh(HC11 *hc11)
{
	static CCRReg real_CCR;    
	// disable interrupts
		
	if(!hc11->waiting)		//si la variable waiting es false (por defecto despues de un reset), y se ejecuta esta instru
	{
		hc11->_clock();
		hc11->StackRegisters();    
		hc11->waiting = true;
		
		real_CCR = hc11->CCR;

		// mask these cause this will do interrupt checking from now on
		hc11->CCR.X(1);		//no permite interrupciones
		hc11->CCR.I(1);
	}
	
	if((!real_CCR.X() && !hc11->XIRQLevel())  ||  (!real_CCR.I() && hc11->InterruptPending()))
	{        	  
		hc11->CCR = real_CCR;
		hc11->waiting = false;
		hc11->ResolveInterrupt();            
	}   
	else 
		hc11->PC--;       //vuelve a ejecutar este opcode, con lo cual, se queda en bucle hasta que entre una int     
}


/////////////////////////////////////////////////
//
//  SWI - Software Interrupt
//  
//  Addr. Mode: Inherent
//  
//  Opcodes: 3Fh
//
//  Clocks: 14
//
//  Flags: I=1
//
////////////////////////////////////////////////

void HC11::op_3Fh(HC11 *hc11)
{	
	hc11->_clock();	

	hc11->StackRegisters();		//consume 9 ciclos de reloj
	hc11->CCR.I(1);				//inhibe las interrupciones
	
	hc11->_clock();		//añadido

	hc11->FetchVector(SWIVector);	//consume 2, busca el vector de interrupcion
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
//
//  NEGA - Negate ACCA
//  
//  Addr. Mode: INH
//  
//  Opcodes: 40h
//           
//  Clocks: 2
//
//  Flags: n,z,v,c
//
////////////////////////////////////////////////

void HC11::op_40h(HC11 *hc11)
{         
	hc11->_clock();

	hc11->CCR.C(hc11->ACCA != 0);		//si no es 0, siempre se dá el carry
	hc11->CCR.V(hc11->ACCA == 0x80);     //solo hay overflow si vale $80 (dá igual antes o despues ya k el resultado no cambia)

	hc11->ACCA = ~(hc11->ACCA) + 1;		//complemento a 2: invierte con ~ y suma 1 (A=0-A)
	
	hc11->CCR.Z(hc11->ACCA == 0);			//zero: si el resultado es 0
	hc11->CCR.N(hc11->ACCA.bit7() ? 1 : 0);		//negative: si el bit7 es 1
}


/////////////////////////////////////////////////
//
//  RORA - rotate right el acumulador A atraves del carry
//  
//  Addr. Mode: INH
//  
//  Opcodes: 46h
//
//  Clocks: 2
//
//  Flags: n,z,v,c
//
////////////////////////////////////////////////

void HC11::op_46h(HC11 *hc11)
{    
	hc11->_clock();               

	// Save the current carry bit
	bit_t carry_old = hc11->CCR.C();	//rotacion siempre es atraves del carry: carry antiguo va a bit7, y el bit0 es el carry nuevo

	hc11->CCR.C(hc11->ACCA.bit0());		//carry nuevo es bit0
	hc11->ACCA = (hc11->ACCA >> 1);		//desplaza 1 a la derecha
	hc11->ACCA.bit7(carry_old);			//bit7 es el carry viejo

	hc11->CCR.Z(hc11->ACCA == 0);                   //flag zero
	hc11->CCR.N(hc11->ACCA.bit7());                 //flag negative   
	hc11->CCR.V(hc11->CCR.N() ^ hc11->CCR.C());		//flag overflow: N exor C                    
}


/////////////////////////////////////////////////
//
//  ROLA - rotate left acumulador A
//  
//  Addr. Mode: INH
//  
//  Opcodes: 49h
//
//  Clocks: 2
//
//  Flags: n,z,v,c
//
////////////////////////////////////////////////

void HC11::op_49h(HC11 *hc11)
{    
	byte_t carry;

	carry = hc11->CCR.C();		//carry viejo, va a bit0
	hc11->_clock();               

	hc11->CCR.C(hc11->ACCA.bit7());				//carry nuevo es el bit7
	
	hc11->ACCA = (hc11->ACCA << 1) | carry;		//desplaza 1 a la izquierda y suma el carry viejo
	
	hc11->CCR.Z(hc11->ACCA == 0);                        
	hc11->CCR.N(hc11->ACCA.bit7());                        
	hc11->CCR.V(hc11->CCR.C() ^ hc11->CCR.N());		//N exor C        
}


/////////////////////////////////////////////////
//
//  TSTA - Test		A-0
//  
//  Addr. Mode: Inherent
//  
//  Opcodes: 4Dh
//
//  Clocks: 2
//
//  Flags:  n,z, v=0, c=0
//
////////////////////////////////////////////////

void HC11::op_4Dh(HC11 *hc11)	//no hace nada, resta 0 al acumulador A, consume 2 ciclos y pone flag
{
	hc11->_clock();		
		
	hc11->CCR.N(hc11->ACCA.bit7());
	hc11->CCR.Z(hc11->ACCA == 0);
	hc11->CCR.V(0);
	hc11->CCR.C(0);		
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
//
//  NEGB - Negate ACCB     B=0-B
//  
//  Addr. Mode: INH
//  
//  Opcodes: 50h
//           
//  Clocks: 2
//
//  Flags: n,z,v,c
//
////////////////////////////////////////////////

void HC11::op_50h(HC11 *hc11)
{         
	hc11->_clock();

	hc11->CCR.C(hc11->ACCB != 0);		//siempre hay carry, a no ser k el valor del registro sea 0
	hc11->CCR.V(hc11->ACCB==0x80);		//solo hay overflow si vale $80 (dá igual antes o despues ya k el resultado no cambia)         

	hc11->ACCB = ~(hc11->ACCB) + 1;		//complemento a 2: invierte con ~ y suma 1
	
	hc11->CCR.Z(hc11->ACCB == 0);
	hc11->CCR.N(hc11->ACCB.bit7() ? 1 : 0);
}


/////////////////////////////////////////////////
//
//  RORB - rotate right B
//  
//  Addr. Mode: INH
//  
//  Opcodes: 56h
//
//  Clocks: 2
//
//  Flags: n,z,v,c
//
////////////////////////////////////////////////

void HC11::op_56h(HC11 *hc11)
{    
	hc11->_clock();               
	
//		hc11->Lock();
		// Save the current carry bit
		
	bit_t carry_old = hc11->CCR.C();
			
	hc11->CCR.C(hc11->ACCB.bit0());		//el carry es el LSB
	hc11->ACCB = (hc11->ACCB >> 1);		//deplaza
	hc11->ACCB.bit7(carry_old);			//el MSB será el old carry
		
	hc11->CCR.Z(hc11->ACCB == 0);                    
	hc11->CCR.N(hc11->ACCB.bit7());                    
	hc11->CCR.V(hc11->CCR.N() ^ hc11->CCR.C());		//overflow: N exor C                    
//		hc11->Unlock();
}


/////////////////////////////////////////////////
//
//  ROLB - rotate left B
//  
//  Addr. Mode: INH
//  
//  Opcodes: 59h
//
//  Clocks: 2
//
//  Flags: n,z,v,c
//
////////////////////////////////////////////////

void HC11::op_59h(HC11 *hc11)
{    
	hc11->_clock();               
	
	byte_t carry;	//es el carry old

	carry=hc11->CCR.C();
	
	hc11->CCR.C(hc11->ACCB.bit7());                    
	hc11->ACCB = (hc11->ACCB << 1) | carry;		//desplazo a la izq y sumo el carry old

	hc11->CCR.Z(hc11->ACCB == 0);				//zero
	hc11->CCR.N(hc11->ACCB.bit7());             //negative     
	hc11->CCR.V(hc11->CCR.N() ^ hc11->CCR.C());		//overflow: N exor C
}


/////////////////////////////////////////////////
//
//  TSTB - Test		B-0
//  
//  Addr. Mode: Inherent
//  
//  Opcodes: 5Dh
//
//  Clocks: 2
//
//  Flags:  n,z,v=0, c=0
//
////////////////////////////////////////////////

void HC11::op_5Dh(HC11 *hc11)		//no hace nada, resta 0 al acumulador B, consume 2 ciclos y pone flag
{
	hc11->_clock();		

	hc11->CCR.N(hc11->ACCB.bit7());		
	hc11->CCR.Z(hc11->ACCB == 0);
	hc11->CCR.V(0);
	hc11->CCR.C(0);		
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
//
//  NEG - Negate Memory Byte
//  
//  Addr. Mode: IND, X
//  
//  Opcodes: 60h
//           
//  Clocks: 6
//	
//  Flags: n,z,v,c
//
////////////////////////////////////////////////

void HC11::op_60h(HC11 *hc11)
{
	byte_t data;
	word_t address;

	address = IndXAddr(hc11);		//funcion del fichero opcodes.h, devuelve la direccion que busco (contenido de IX + offset)
									//consume 2 ciclos

	hc11->_clock();
	data = hc11->memory[address];

	hc11->CCR.V(data==0x80);	//hay overflow solo si vale $80, ya que el complemento a 2 de $80 es $80
	hc11->CCR.C(data != 0);		//siempre hay carry excepto cuando vale 0

	hc11->_clock();
	data = ~data + 1;		//complemento a 2: inverso + 1

	hc11->_clock();
	hc11->memory.Write(address, data);
	
	hc11->CCR.Z(data == 0);					//zero
	hc11->CCR.N((data & 0x80) ? 1 : 0);		//hago mascara con el bit7 de data: si es 1, pongo N a 1, sino pongo N a 0 
}


/////////////////////////////////////////////////
//
//  ROR - rotate right
//  
//  Addr. Mode: IND, X
//  
//  Opcodes: 66h
//
//  Clocks: 6
//	lenght: 2
//  Flags: n,z,v,c
//
////////////////////////////////////////////////

void HC11::op_66h(HC11 *hc11)
{   
	word_t address;
	Byte data;

	address = IndXAddr(hc11);			//devuelve la direccion que busco. Consume 2 ciclos
	
	hc11->_clock();               
	data = hc11->memory[address];
	
	// Save the current carry bit
	bit_t carry_old = hc11->CCR.C();

	hc11->CCR.C(data.bit0());                    //el carry será el LSB

	hc11->_clock();               
	data = (data >> 1) | (carry_old << 7);		//desplaza a la derecha, y le suma el carry old al bit 7
	
	hc11->CCR.Z(data == 0);							//zero
	hc11->CCR.N(data.bit7());                       //negative
	hc11->CCR.V(hc11->CCR.C() ^ hc11->CCR.N());		//overflow: N exor C
	
	hc11->_clock();               
	hc11->memory.Write(address, data);
}


/////////////////////////////////////////////////
//
//  ROL - rotate left
//  
//  Addr. Mode: IND, X
//  
//  Opcodes: 69h
//
//  Clocks: 6
//	lenght: 2
//  Flags: n,z,v,c
//
////////////////////////////////////////////////

void HC11::op_69h(HC11 *hc11)
{   
	word_t address;
	Byte data;
	byte_t carry;

	carry = hc11->CCR.C();		//carry old

	address = IndXAddr(hc11);		//devuelve la direccion. Consume 2 ciclos
	
	hc11->_clock();               
	data = hc11->memory[address];

	hc11->CCR.C(data.bit7());         //el carry será el bit7           

	hc11->_clock();               
	data = (data << 1) | carry;		//desplaza y le suma el carry old al bit0
		   
	hc11->CCR.Z(data == 0);                         //zero
	hc11->CCR.N(data.bit7());                       //negative  
	hc11->CCR.V(hc11->CCR.C() ^ hc11->CCR.N());		//overflow: N exor C                
	
	hc11->_clock();               
	hc11->memory.Write(address, data);
}


/////////////////////////////////////////////////
//
//  TST - Test		M-0
//  
//  Addr. Mode: IND, X
//  
//  Opcodes: 6Dh
//
//  Clocks: 6
//	lenght: 2
//  Flags:  n,z, v=0, c=0
//
////////////////////////////////////////////////

void HC11::op_6Dh(HC11 *hc11)
{
	Word data;
	word_t address;
	
	address = IndXAddr(hc11);	//devuelve la direccion buscada. Consume 2 ciclos
	
	hc11->_clock();
	data = hc11->memory[address];	

	hc11->_clock();		    
	hc11->_clock();		
	hc11->CCR.N(data.bit7());	//negative
	hc11->CCR.Z(data == 0);		//zero
	hc11->CCR.V(0);				//overflow
	hc11->CCR.C(0);				//carry
}


/////////////////////////////////////////////////
//
//  JMP - Jump
//  
//  Addr. Mode: IND, X
//  
//  Opcodes: 6E
//
//  Clocks: 3
//	lenght: 2
//  Flags:	ninguno
//
////////////////////////////////////////////////

void HC11::op_6Eh(HC11 *hc11)
{	
	byte_t offset;            
	hc11->_clock();	
	offset = hc11->memory[hc11->PC];	//lee de memoria el offset
	hc11->PC++;
 
	hc11->_clock();
	hc11->PC = hc11->IX + offset;	//carga el PC con el contenido de X mas el offset
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
//
//  NEG - Negate Memory Byte
//  
//  Addr. Mode: EXT
//  
//  Opcodes: 70h
//           
//  Clocks: 6
//	lenght: 3
//  Flags: n,z,v,c
//
////////////////////////////////////////////////

void HC11::op_70h(HC11 *hc11)
{
	byte_t data;
	word_t address;

	address = ExtAddr(hc11);	//funcion k lee 2 veces en memoria para formar la dir. Consume 2 ciclos

	hc11->_clock();
	data = hc11->memory[address];

	hc11->CCR.V(data==0x80);		//si es $80 activa overflow, da igual antes o despues porque no cambia          
	hc11->CCR.C(data != 0);			//siempre activa carry excepto si es 0

	hc11->_clock();
	data = ~data + 1;			// M= 0-M, complemento a 2 (inverso + 1)

	hc11->_clock();
	hc11->memory.Write(address, data);
			
	hc11->CCR.Z(data == 0);					//zero
	hc11->CCR.N((data & 0x80) ? 1 : 0);		//si el bit7 de data es 1, entonces N es 1, sino N es 0
}


/////////////////////////////////////////////////
//
//  ROR - rotate right
//  
//  Addr. Mode: EXT
//  
//  Opcodes: 76h
//
//  Clocks: 6
//			3
//  Flags: n,z,v,c
//
////////////////////////////////////////////////

void HC11::op_76h(HC11 *hc11)
{   
	word_t address;
	Byte data;

	address = ExtAddr(hc11);		//funcion k lee 2 veces en memoria para formar la dir. Consume 2 ciclos
	
	hc11->_clock();               
	data = hc11->memory[address];
	
	// Save the current carry bit
	bit_t carry_old = hc11->CCR.C();	//el carry actual será el bit7

	hc11->CCR.C(data.bit0());			//el LSB será el nuevo carry         

	hc11->_clock();               
	data = (data >> 1) | (carry_old << 7);	//desplazo y sumo el carry old al bit7
	
	hc11->CCR.Z(data == 0);                     //zero
	hc11->CCR.N(data.bit7());					//negative
	hc11->CCR.V(hc11->CCR.C() ^ hc11->CCR.N());		//overflow: N exor C 
	
	hc11->_clock();               
	hc11->memory.Write(address, data);
}


/////////////////////////////////////////////////
//
//  ROL - rotate left
//  
//  Addr. Mode: EXT
//  
//  Opcodes: 79h
//
//  Clocks: 6
//			3
//  Flags: n,z,v,c
//
////////////////////////////////////////////////

void HC11::op_79h(HC11 *hc11)
{   
	word_t address;
	Byte data;
	byte_t carry;

	carry = hc11->CCR.C();
	
	address = ExtAddr(hc11);    //funcion k lee 2 veces en memoria para formar la dir. Consume 2 ciclos

	hc11->_clock();               
	data = hc11->memory[address];

	hc11->CCR.C(data.bit7());	//el nuevo carry es el bit7

	hc11->_clock();               
	data = (data << 1) | carry;		//desplazo y sumo el carry old al bit0

	hc11->CCR.Z(data == 0);						//zero       
	hc11->CCR.N(data.bit7());                   //negative     
	hc11->CCR.V(hc11->CCR.C() ^ hc11->CCR.N());		//overflow: N exor C     

	
	hc11->_clock();               
	hc11->memory.Write(address, data);
}


/////////////////////////////////////////////////
//
//  TST - Test		M-0
//  
//  Addr. Mode: EXT
//  
//  Opcodes: 7Dh
//
//  Clocks: 6
//			3
//  Flags:  n,z, v=0, c=0
//
////////////////////////////////////////////////

void HC11::op_7Dh(HC11 *hc11)
{
	Word data;
	word_t address;
	
	address = ExtAddr(hc11);		//funcion k lee 2 veces en memoria para formar la dir. Consume 2 ciclos
	
	hc11->_clock();
	data = hc11->memory[address];	

	hc11->_clock();		    
	hc11->_clock();		
	hc11->CCR.N(data.bit7());
	hc11->CCR.Z(data == 0);
	hc11->CCR.V(0);
	hc11->CCR.C(0);		
}


/////////////////////////////////////////////////
//
//  JMP - Jump 
//  
//  Addr. Mode: EXT
//  
//  Opcodes: 7E
//
//  Clocks: 3
//			3
//  Flags:	ninguno
//
////////////////////////////////////////////////

void HC11::op_7Eh(HC11 *hc11)
{	
	byte_t data;
	
	hc11->_clock();	
	data = hc11->memory[hc11->PC];
	hc11->PC++;
	
	hc11->_clock();	
	hc11->PC.low(hc11->memory[hc11->PC]);	//sobreescribe PC 
	hc11->PC.high(data);    
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
//
//  LDAA - Load ACCA
//  
//  Addr. Mode: IMM
//  
//  Opcodes: 86
//
//  Clocks: 2
//			2
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_86h(HC11 *hc11)
{	
	hc11->_clock();	
	hc11->ACCA = hc11->memory[hc11->PC];	//lo que hay en memoria lo carga en acumulador A
	hc11->PC++;
	
	hc11->CCR.V(0);						//flag V a 0
	hc11->CCR.Z(hc11->ACCA == 0);		//si el dato cargado es 0, activa flag Z
	hc11->CCR.N(hc11->ACCA.bit7());		//si el bit mas significativo es 1, activa flag N
}


/////////////////////////////////////////////////
//
//  ORAA - Inclusive OR 
//  
//  Addr. Mode: IMM
//  
//  Opcodes: 8Ah
//           
//  Clocks: 2
//			2
//  Flags: n,z, v=0
//
////////////////////////////////////////////////

void HC11::op_8Ah(HC11 *hc11)
{
	byte_t data;

	hc11->_clock();  
	data = hc11->memory[hc11->PC];
	hc11->PC++;

	hc11->ACCA = hc11->ACCA | data;		//A = M or A  (or bit a bit)

	hc11->CCR.V(0);					//overflow
	hc11->CCR.Z(hc11->ACCA == 0);		//zero
	hc11->CCR.N(hc11->ACCA.bit7());		//negative

}


/////////////////////////////////////////////////
//
//  CPX - Compare Index Register X
//  
//  Addr. Mode: IMM
//  
//  Opcodes: 8C
//
//  Clocks: 4
//			3
//  Flags: n,z, V, c
//
////////////////////////////////////////////////

void HC11::op_8Ch(HC11 *hc11)
{	    
	Word data;
	Word result;

	hc11->_clock();	
	data.high(hc11->memory[hc11->PC]);
	hc11->PC++;
	
	hc11->_clock();	
	data.low(hc11->memory[hc11->PC]);
	hc11->PC++;
  
	result = hc11->IX - data;		// X-M
	
	hc11->CCR.N(result.bit15());
	hc11->CCR.Z(result == 0);
	
	hc11->CCR.V(hc11->IX.bit15() && !data.bit15() && !result.bit15() ||
				!hc11->IX.bit15() && data.bit15() && result.bit15());                
	
	hc11->CCR.C((!hc11->IX.bit15()) && data.bit15() ||
				data.bit15() && result.bit15() ||
				result.bit15() && (!hc11->IX.bit15()));


	hc11->_clock();		//faltaba
}


/////////////////////////////////////////////////
//
//  LDS - Load SP
//  
//  Addr. Mode: IMM
//  
//  Opcodes: 8E
//
//  Clocks: 3
//			3
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_8Eh(HC11 *hc11)
{	
	hc11->_clock();	
	hc11->SP.high(hc11->memory[hc11->PC]);		//lee de memoria y guarda en SP
	hc11->PC++;
	
	hc11->_clock();	
	hc11->SP.low(hc11->memory[hc11->PC]);
	hc11->PC++;

	hc11->CCR.V(0);						//overflow
	hc11->CCR.Z(hc11->SP == 0);				//zero
	hc11->CCR.N((hc11->SP & 0x8000)>>15);	//negative, tambien valdria: hc11->CCR.N(hc11->SP.bit15());
}


/////////////////////////////////////////////////
//
//  XGDX - Exchange ACCD and IX
//  
//  Addr. Mode: Inherint
//  
//  Opcode: 8Fh
//	    
//
//  Clocks: 3 
//
//  Flags:  ninguno
//
////////////////////////////////////////////////

void HC11::op_8Fh(HC11 *hc11)
{
	word_t temp;

	temp = ((hc11->ACCA)<<8) | hc11->ACCB;

	hc11->_clock();
	hc11->ACCB = hc11->IX.low();
	hc11->ACCA = hc11->IX.high();
	
	hc11->_clock();
	hc11->IX = temp;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
//
//  LDAA - load ACCA
//  
//  Addr. Mode: DIR
//  
//  Opcodes: 96
//
//  Clocks: 3
//			2
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_96h(HC11 *hc11)
{	
	byte_t address;

	address = DirAddr(hc11);    //DirAddr() es una funcion inline de opcodes.h, consume un ciclo

	hc11->_clock();	
	hc11->ACCA = hc11->memory[address];		//A=M

	hc11->CCR.V(0);					//overflow
	hc11->CCR.Z(hc11->ACCA == 0);		//zero
	hc11->CCR.N(hc11->ACCA.bit7());		//negative
}


/////////////////////////////////////////////////
//
//  STAA - Store ACCA
//  
//  Addr. Mode: DIR
//  
//  Opcodes: 97h
//
//  Clocks: 3
//			2
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_97h(HC11 *hc11)
{	
	byte_t address;
	address = DirAddr(hc11);		//consume 1 ciclo

	hc11->_clock();	
	hc11->memory.Write((word_t)address, hc11->ACCA);	//M=A

	hc11->CCR.V(0);					//overflow
	hc11->CCR.Z(hc11->ACCA == 0);		//zero
	hc11->CCR.N(hc11->ACCA.bit7());		//negative
}


/////////////////////////////////////////////////
//
//  ORAA - Inclusize OR
//  
//  Addr. Mode: DIR
//  
//  Opcodes: 9Ah
//           
//  Clocks: 3
//			2
//  Flags: n, z, v=0
//
////////////////////////////////////////////////

void HC11::op_9Ah(HC11 *hc11)
{
	byte_t data;
	byte_t address;

	address = DirAddr(hc11);		//consume 1 ciclo

	hc11->_clock();  
	data = hc11->memory[(word_t)address];

	hc11->ACCA = hc11->ACCA | data;		//A = A or M (bit a bit)

	hc11->CCR.V(0);					//overflow
	hc11->CCR.Z(hc11->ACCA == 0);		//zero
	hc11->CCR.N(hc11->ACCA.bit7());		//negative
}


/////////////////////////////////////////////////
//
//  CPX - Compare Index Register X
//  
//  Addr. Mode: DIR
//  
//  Opcodes: 9C
//
//  Clocks: 5
//			2
//  Flags: n, z, V, c
//
////////////////////////////////////////////////

void HC11::op_9Ch(HC11 *hc11)
{	
	CCRReg mask;    
	Word data;
	Word result;
	word_t address;

	address = DirAddr(hc11);		//consume 1 ciclo

	hc11->_clock();	
	data.high(hc11->memory[address]);
	hc11->_clock();	
	data.low(hc11->memory[address+1]);

	result = hc11->IX - data;		//X-M

	hc11->CCR.N(result.bit15());
	hc11->CCR.Z(result == 0);

	hc11->CCR.V(hc11->IX.bit15() && !data.bit15() && !result.bit15() ||
				!hc11->IX.bit15() && data.bit15() && result.bit15()); 
	
	hc11->CCR.C((!hc11->IX.bit15()) && data.bit15() ||
				data.bit15() && result.bit15() ||
				result.bit15() && (!hc11->IX.bit15()));


	hc11->_clock();		//faltaba
}


/////////////////////////////////////////////////
//
//  JSR - Jump to subroutine
//  
//  Addr. Mode: DIR
//  
//  Opcodes: 9D
//
//  Clocks: 5
//			2
//  Flags:	ninguno
//
////////////////////////////////////////////////

void HC11::op_9Dh(HC11 *hc11)
{	
	byte_t address;
	
	hc11->_clock();	
	address = hc11->memory[hc11->PC];	//leo la parte baja, la parte alta es 0 por ser direccionamiento directo
	hc11->PC++;

	hc11->Push(hc11->PC.low());		//consume 1 ciclo
	hc11->Push(hc11->PC.high());
	
	hc11->_clock();	
	hc11->PC = (word_t)address;		//cargo PC con la direccion
}


/////////////////////////////////////////////////
//
//  LDS - Load SP
//  
//  Addr. Mode: DIR
//  
//  Opcodes: 9E
//
//  Clocks: 4
//			2
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_9Eh(HC11 *hc11)
{	
	byte_t address; 
	
	address = DirAddr(hc11);		//consume 1 ciclo

	hc11->_clock();	
	hc11->SP.high(hc11->memory[(word_t)address]);	//SP = M

	hc11->_clock();	
	hc11->SP.low(hc11->memory[(word_t)address+1]);

	hc11->CCR.V(0);						//overflow
	hc11->CCR.Z(hc11->SP == 0);				//zero
	hc11->CCR.N((hc11->SP & 0x8000)>>15);	//negative: hago mascara con MSB y desplazo 15 posiciones a la derecha
}


/////////////////////////////////////////////////
//
//  STS - Store SP
//  
//  Addr. Mode: DIR
//  
//  Opcodes: 9Fh
//
//  Clocks: 4
//			2
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_9Fh(HC11 *hc11)
{	
	byte_t address;

	address = DirAddr(hc11);	//consume 1 ciclo

	hc11->_clock();	
	hc11->memory.Write((word_t)address, hc11->SP.high());    //M = SP
	hc11->_clock();	
	hc11->memory.Write((word_t)address+1, hc11->SP.low());

	hc11->CCR.V(0);						//overflow
	hc11->CCR.Z(hc11->SP == 0);				//zero
	hc11->CCR.N((hc11->SP & 0x8000)?1:0);	//negative: si se dá pongo 1, sino 0
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
//
//  LDAA - load ACCA
//  
//  Addr. Mode: IND, X
//  
//  Opcodes: A6h
//
//  Clocks: 4
//			2
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_A6h(HC11 *hc11)
{	
	word_t address;
	
	address = IndXAddr(hc11);	//obtiene la direccion. Consume 2 ciclos

	hc11->_clock();	
	hc11->ACCA = hc11->memory[address];		//A = M

	hc11->CCR.V(0);					//overflow
	hc11->CCR.Z(hc11->ACCA == 0);	//zero
	hc11->CCR.N(hc11->ACCA.bit7());		//negative
}


/////////////////////////////////////////////////
//
//  STAA - Store ACCA
//  
//  Addr. Mode: IND, X
//  
//  Opcodes: A7h
//
//  Clocks: 4
//			2
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_A7h(HC11 *hc11)
{	
	word_t address;

	address = IndXAddr(hc11);	//funcion inline de opcodes.h, consume 2 ciclos

	hc11->_clock();				//el cuarto ciclo, accede a memoria para escribir
	hc11->memory.Write(address, hc11->ACCA);	//M = A

	hc11->CCR.V(0);
	hc11->CCR.Z(hc11->ACCA == 0);    
	hc11->CCR.N(hc11->ACCA.bit7());
}


/////////////////////////////////////////////////
//
//  ORAA - Inclusize OR
//  
//  Addr. Mode: IND, X
//  
//  Opcodes: AAh
//           
//  Clocks: 4
//			2
//  Flags:	N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_AAh(HC11 *hc11)
{
	byte_t data;
	word_t address;

	address = IndXAddr(hc11);	//consume 2 ciclos

	hc11->_clock();  
	data = hc11->memory[address];    

	hc11->ACCA = hc11->ACCA | data;		//A = A or M (bit a bit)

	hc11->CCR.V(0);    
	hc11->CCR.Z(hc11->ACCA == 0);
	hc11->CCR.N(hc11->ACCA.bit7());
}


/////////////////////////////////////////////////
//
//  CPX - Compare Index Register X
//  
//  Addr. Mode: IND, X
//  
//  Opcodes: AC
//
//  Clocks: 6
//			2
//  Flags:	N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_ACh(HC11 *hc11)
{	
	CCRReg mask;    
	Word data;
	Word result;
	word_t address;

	address = IndXAddr(hc11);	//consume 2 ciclos

	hc11->_clock();	
	data.high(hc11->memory[address]);
	hc11->_clock();	
	data.low(hc11->memory[address+1]);

	result = hc11->IX - data;    // X - M

	hc11->CCR.N(result.bit15());
	hc11->CCR.Z(result == 0);

	hc11->CCR.V(hc11->IX.bit15() && !data.bit15() && !result.bit15() ||
				!hc11->IX.bit15() && data.bit15() && result.bit15());  
	
	hc11->CCR.C((!hc11->IX.bit15()) && data.bit15() ||
				data.bit15() && result.bit15() ||
				result.bit15() && (!hc11->IX.bit15()));


	hc11->_clock();		//faltaba
}


/////////////////////////////////////////////////
//
//  JSR - Jump to subroutine
//  
//  Addr. Mode: IND, X
//  
//  Opcodes: AD
//
//  Clocks: 6
//			2
//  Flags:	ninguno
//
////////////////////////////////////////////////

void HC11::op_ADh(HC11 *hc11)
{	
	byte_t offset;    
	
	hc11->_clock();	
	offset = hc11->memory[hc11->PC];
	hc11->PC++;
	
	hc11->Push(hc11->PC.low());		//consume 1 ciclo
	hc11->Push(hc11->PC.high());

	hc11->_clock();	
	hc11->_clock();
	hc11->PC = hc11->IX + offset;	//sobreescribe PC
}


/////////////////////////////////////////////////
//
//  LDS - load SP
//  
//  Addr. Mode: IND, X
//  
//  Opcodes: AEh
//
//  Clocks: 5
//			2
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_AEh(HC11 *hc11)
{	
	word_t address;

	address = IndXAddr(hc11);	//consume 2 ciclos

	hc11->_clock();	
	hc11->SP.high(hc11->memory[address]);	// SP = M

	hc11->_clock();	
	hc11->SP.low(hc11->memory[address+1]);
	
	hc11->CCR.V(0);						//overflow
	hc11->CCR.Z(hc11->SP == 0);				//zero
	hc11->CCR.N((hc11->SP & 0x8000)>>15);	//negative: hago mascara con MSB y desplazo 15 posiciones a la derecha
}


/////////////////////////////////////////////////
//
//  STS - Store SP
//  
//  Addr. Mode: IND, X
//  
//  Opcodes: AFh
//
//  Clocks: 5
//			2
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_AFh(HC11 *hc11)
{	
	word_t address;

	address = IndXAddr(hc11);		//consume 2 ciclos

	hc11->_clock();	
	hc11->memory.Write(address, hc11->SP.high());	// M = SP
	
	hc11->_clock();	
	hc11->memory.Write(address+1, hc11->SP.low());

	hc11->CCR.V(0);						//overflow
	hc11->CCR.Z(hc11->SP == 0);				//zero
	hc11->CCR.N((hc11->SP & 0x8000)?1:0);	//negative: si el bit15 es 1, pongo N a 1, sino a 0
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
//
//  LDAA - load ACCA
//  
//  Addr. Mode: EXT
//  
//  Opcodes: B6h
//
//  Clocks: 4
//			3
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_B6h(HC11 *hc11)
{	
	word_t address;
	
	address = ExtAddr(hc11);		//lee 2 veces de la memoria para formar la direccion. Consume 2 ciclos

	hc11->_clock();	
	hc11->ACCA = hc11->memory[address];    // A = M

	hc11->CCR.V(0);					//overflow
	hc11->CCR.Z(hc11->ACCA == 0);		//zero
	hc11->CCR.N(hc11->ACCA.bit7());		//negative
}


/////////////////////////////////////////////////
//
//  STAA - Store ACCA
//  
//  Addr. Mode: EXT
//  
//  Opcodes: B7h
//
//  Clocks: 4
//			3
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_B7h(HC11 *hc11)
{	
	word_t address;

	address = ExtAddr(hc11);		//lee 2 veces de la memoria para formar la direccion. Consume 2 ciclos
	
	hc11->_clock();	
	hc11->memory.Write(address, hc11->ACCA);	// M = A

	hc11->CCR.V(0);					//overflow
	hc11->CCR.Z(hc11->ACCA == 0);		//zero
	hc11->CCR.N(hc11->ACCA.bit7());		//negative
}


/////////////////////////////////////////////////
//
//  ORAA - Inclusize OR
//  
//  Addr. Mode: EXT
//  
//  Opcodes: BAh
//           
//  Clocks: 4
//			3
//  Flags:	N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_BAh(HC11 *hc11)
{
	byte_t data;
	word_t address;
	
	address = ExtAddr(hc11);	//lee 2 veces de la memoria para formar la direccion. Consume 2 ciclos

	hc11->_clock();  
	data = hc11->memory[address];    

	hc11->ACCA = hc11->ACCA | data;		//A = A or M (bit a bit)

	hc11->CCR.V(0);				//overflow
	hc11->CCR.Z(hc11->ACCA == 0);		//zero
	hc11->CCR.N(hc11->ACCA.bit7());		//negative
}


/////////////////////////////////////////////////
//
//  CPX - Compare Index Register X
//  
//  Addr. Mode:Ext
//  
//  Opcodes: BC
//
//  Clocks: 6
//			3
//  Flags: N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_BCh(HC11 *hc11)
{	
	CCRReg mask;    
	Word data;
	Word result;
	word_t address;

	address = ExtAddr(hc11);	//lee 2 veces de la memoria para formar la direccion. Consume 2 ciclos

	hc11->_clock();	
	data.high(hc11->memory[address]);
	hc11->_clock();	
	data.low(hc11->memory[address+1]);

	result = hc11->IX - data;		// X - M
	
	hc11->CCR.N(result.bit15());
	hc11->CCR.Z(result == 0);

	hc11->CCR.V(hc11->IX.bit15() && !data.bit15() && !result.bit15() ||
				!hc11->IX.bit15() && data.bit15() && result.bit15());   
	
	hc11->CCR.C((!hc11->IX.bit15()) && data.bit15() ||
				data.bit15() && result.bit15() ||
				result.bit15() && (!hc11->IX.bit15()));


	hc11->_clock();		//faltaba
}


/////////////////////////////////////////////////
//
//  JSR - Jump to subroutine
//  
//  Addr. Mode: EXT
//  
//  Opcodes: BD
//
//  Clocks: 6
//			3
//  Flags:	ninguno
//
////////////////////////////////////////////////

void HC11::op_BDh(HC11 *hc11)
{	
	Word address;
	
	hc11->_clock();	
	address.high(hc11->memory[hc11->PC]);
	hc11->PC++;
	
	hc11->_clock();	
	address.low(hc11->memory[hc11->PC]);
	hc11->PC++;

	hc11->Push(hc11->PC.low());		//consume 1 ciclo
	hc11->Push(hc11->PC.high());
	
	hc11->_clock();	
	hc11->PC = address;			//sobreescribe PC con la dir
}


/////////////////////////////////////////////////
//
//  LDS - load SP
//  
//  Addr. Mode: EXT
//  
//  Opcodes: BEh
//
//  Clocks: 5
//			3
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_BEh(HC11 *hc11)
{	
	word_t address;
	
	address = ExtAddr(hc11);		//consume 2 ciclos

	hc11->_clock();	
	hc11->SP.high(hc11->memory[address]);    //SP = M

	hc11->_clock();	
	hc11->SP.low(hc11->memory[address+1]);    

	hc11->CCR.V(0);					//overflow
	hc11->CCR.Z(hc11->SP == 0);				//zero
	hc11->CCR.N((hc11->SP & 0x8000)>>15);	//negative
}


/////////////////////////////////////////////////
//
//  STS - Store SP
//  
//  Addr. Mode: EXT
//  
//  Opcodes: BFh
//
//  Clocks: 5
//			3
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_BFh(HC11 *hc11)
{	
	word_t address;
	
	address = ExtAddr(hc11);		//consume 2 ciclos

	hc11->_clock();	
	hc11->memory.Write(address, hc11->SP.high());	// M = SP
	
	hc11->_clock();	
	hc11->memory.Write(address+1, hc11->SP.low());

	hc11->CCR.V(0);					//overflow
	hc11->CCR.Z(hc11->SP == 0);				//zero
	hc11->CCR.N((hc11->SP & 0x8000)?1:0);	//negative: si el bit15 es 1, pongo N a 1, sino N a 0
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
//
//  LDAB - Load ACCB
//  
//  Addr. Mode: IMM
//  
//  Opcodes: C6
//
//  Clocks: 2
//			2
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_C6h(HC11 *hc11)
{	
	hc11->_clock();	
	hc11->ACCB = hc11->memory[hc11->PC];	// B = M
	hc11->PC++;
	
	hc11->CCR.V(0);    
	hc11->CCR.Z(hc11->ACCB == 0);    
	hc11->CCR.N(hc11->ACCB.bit7());
}


/////////////////////////////////////////////////
//
//  ORAB - Inclusize OR
//  
//  Addr. Mode: IMM
//  
//  Opcodes: CAh
//           
//  Clocks: 2
//			2
//  Flags:	N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_CAh(HC11 *hc11)
{
	byte_t data;
	
	hc11->_clock();  
	data = hc11->memory[hc11->PC];
	hc11->PC++;

	hc11->ACCB = hc11->ACCB | data;		//B = B or M

	hc11->CCR.V(0);    
	hc11->CCR.Z(hc11->ACCB == 0);
	hc11->CCR.N(hc11->ACCB.bit7());
}


/////////////////////////////////////////////////
//
//  LDD - Load ACCD
//  
//  Addr. Mode: IMM
//  
//  Opcodes: CC
//
//  Clocks: 3
//			3
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_CCh(HC11 *hc11)
{	
	hc11->_clock();	
	hc11->ACCA = hc11->memory[hc11->PC];	//D=M
	hc11->PC++;
	
	hc11->_clock();	
	hc11->ACCB = hc11->memory[hc11->PC];	
	hc11->PC++;

	hc11->CCR.V(0);
	hc11->CCR.Z((hc11->ACCB | hc11->ACCA) == 0);    
	hc11->CCR.N(hc11->ACCA.bit7());				//el bit7 de A es el bit15 de D
}


/////////////////////////////////////////////////
//
//  LDX - Load IX
//  
//  Addr. Mode: IMM
//  
//  Opcodes: CE
//
//  Clocks: 3
//			3
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_CEh(HC11 *hc11)
{	
	hc11->_clock();	
	hc11->IX.high(hc11->memory[hc11->PC]);		// X = M
	hc11->PC++;
	
	hc11->_clock();	
	hc11->IX.low(hc11->memory[hc11->PC]);
	hc11->PC++;

	hc11->CCR.V(0);    
	hc11->CCR.Z(hc11->IX == 0);
	hc11->CCR.N((hc11->IX & 0x8000)>>15);
}


/////////////////////////////////////////////////
//
//  STOP - Stop Processing
//  
//  Addr. Mode:INH
//  
//  Opcodes: CFh
//
//  Clocks: 2
//
//  Flags:	ninguno
//
////////////////////////////////////////////////

void HC11::op_CFh(HC11 *hc11)
{	
	static CCRReg real_CCR;

	if(!hc11->stopped)		//si stopped es false
	{   
		if(!hc11->CCR.S())	//solo entra si el flag S es 0
		{	        
			hc11->stopped = true;		//activa el estado de stop (por defecto en HC11::Reset() se pone a false)
			real_CCR = hc11->CCR;		//copia el valor actual       
	
			hc11->CCR.I(1);			//inhibe las interrupciones
			hc11->CCR.X(1);                        
		}    
	} 
	
	if( !hc11->XIRQLevel()   ||   (!hc11->CCR.I() && !hc11->IRQLevel()) )	//si se dá una int externa, sale del estado de stop
	{
		hc11->stopped = false;		//desactiva el estado de stop
		hc11->CCR = real_CCR;		//recupera de la copia
		
	}
	else
	{
		hc11->timer_system->main_clock_divider--;
		hc11->PC--;
	}
	
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
//
//  LDAB - load ACCB
//  
//  Addr. Mode: DIR
//  
//  Opcodes: D6
//
//  Clocks: 3
//			2
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_D6h(HC11 *hc11)
{	
	byte_t address;
	
	address = DirAddr(hc11);		//consume 1 ciclo

	hc11->_clock();	
	hc11->ACCB = hc11->memory[(word_t)address];		//B=M

	hc11->CCR.V(0);    
	hc11->CCR.Z(hc11->ACCB == 0);    
	hc11->CCR.N(hc11->ACCB.bit7());
}


/////////////////////////////////////////////////
//
//  STAB - Store ACCB
//  
//  Addr. Mode: DIR
//  
//  Opcodes: D7h
//
//  Clocks: 3
//			2
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_D7h(HC11 *hc11)
{	
	byte_t address;
	
	address = DirAddr(hc11);	//consume 1 ciclo

	hc11->_clock();	
	hc11->memory.Write((word_t)address, hc11->ACCB);	//M=B

	hc11->CCR.V(0);    
	hc11->CCR.Z(hc11->ACCB == 0);    
	hc11->CCR.N(hc11->ACCB.bit7());
}


/////////////////////////////////////////////////
//
//  ORAB - Inclusize OR
//  
//  Addr. Mode: DIR
//  
//  Opcodes: DAh
//           
//  Clocks: 3
//			2
//  Flags:	N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_DAh(HC11 *hc11)
{
	byte_t data;
	byte_t address;

	address = DirAddr(hc11);		//consume 1 ciclo

	hc11->_clock();  
	data = hc11->memory[(word_t)address];

	hc11->ACCB = hc11->ACCB | data;		//B = B or M 

	hc11->CCR.V(0);    
	hc11->CCR.Z(hc11->ACCB == 0);
	hc11->CCR.N(hc11->ACCB.bit7());
}


/////////////////////////////////////////////////
//
//  LDD - load ACCD
//  
//  Addr. Mode: DIR
//  
//  Opcodes: DC
//
//  Clocks: 4
//			2
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_DCh(HC11 *hc11)
{	
	byte_t address;
	
	address = DirAddr(hc11);		//consume 1 ciclo
	
	hc11->_clock();	
	hc11->ACCA = hc11->memory[(word_t)address];		// D = M

	hc11->_clock();	
	hc11->ACCB = hc11->memory[(word_t)address+1];

	hc11->CCR.V(0);
	hc11->CCR.Z((hc11->ACCB | hc11->ACCA) == 0);    
	hc11->CCR.N(hc11->ACCA.bit7());					//bit7 de A es el bit15 de D
}


/////////////////////////////////////////////////
//
//  STD - Store ACCD
//  
//  Addr. Mode: DIR
//  
//  Opcodes: DDh
//
//  Clocks: 4
//			2
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_DDh(HC11 *hc11)
{	
	byte_t address;

	address = DirAddr(hc11);	//consume 1 ciclo

	hc11->_clock();	
	hc11->memory.Write((word_t)address, hc11->ACCA);		//M=D
	
	hc11->_clock();	
	hc11->memory.Write((word_t)address+1, hc11->ACCB);

	hc11->CCR.V(0);
	hc11->CCR.Z((hc11->ACCB | hc11->ACCA) == 0);    
	hc11->CCR.N(hc11->ACCA.bit7());					//bit7 de A es el bit15 de D
}


/////////////////////////////////////////////////
//
//  LDX - Load IX
//  
//  Addr. Mode: DIR
//  
//  Opcodes: DE
//
//  Clocks: 4
//			2
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_DEh(HC11 *hc11)
{	
	byte_t address;
	
	address = DirAddr(hc11);		//consume 1 ciclo

	hc11->_clock();	
	hc11->IX.high(hc11->memory[(word_t)address]);	//X=M

	hc11->_clock();	
	hc11->IX.low(hc11->memory[(word_t)address+1]);

	hc11->CCR.V(0);    
	hc11->CCR.Z(hc11->IX == 0);
	hc11->CCR.N((hc11->IX & 0x8000)>>15);
}


/////////////////////////////////////////////////
//
//  STX - Store X
//  
//  Addr. Mode: DIR
//  
//  Opcodes: DFh
//
//  Clocks: 4
//			2
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_DFh(HC11 *hc11)
{	
	byte_t address;
	
	address = DirAddr(hc11);	//consume 1 ciclo

	hc11->_clock();	
	hc11->memory.Write((word_t)address, hc11->IX.high());    // M = X
	
	hc11->_clock();	
	hc11->memory.Write((word_t)address+1, hc11->IX.low());

	hc11->CCR.V(0);
	hc11->CCR.Z(hc11->IX == 0);    
	hc11->CCR.N((hc11->IX & 0x8000)?1:0);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
//
//  LDAB - load ACCB
//  
//  Addr. Mode: IND, X
//  
//  Opcodes: E6h
//
//  Clocks: 4
//			2
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_E6h(HC11 *hc11)
{	
	word_t address;
	
	address = IndXAddr(hc11);		//consume 2 ciclos

	hc11->_clock();	
	hc11->ACCB = hc11->memory[address];		//B=M

	hc11->CCR.V(0);    
	hc11->CCR.Z(hc11->ACCB == 0);    
	hc11->CCR.N(hc11->ACCB.bit7());
}


/////////////////////////////////////////////////
//
//  STAB - Store ACCB
//  
//  Addr. Mode: IND, X
//  
//  Opcodes: E7h
//
//  Clocks: 4
//			2
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_E7h(HC11 *hc11)
{	
	word_t address;

	address = IndXAddr(hc11);	//consume 2 ciclos

	hc11->_clock();	
	hc11->memory.Write(address, hc11->ACCB);	//M=B

	hc11->CCR.V(0);
	hc11->CCR.Z(hc11->ACCB == 0);    
	hc11->CCR.N(hc11->ACCB.bit7());
}


/////////////////////////////////////////////////
//
//  ORAB - Inclusize OR
//  
//  Addr. Mode: IND, X
//  
//  Opcodes: EAh
//           
//  Clocks: 4
//			2
//  Flags:	N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_EAh(HC11 *hc11)
{
	byte_t data;
	word_t address;

	address = IndXAddr(hc11);	//consume 2 ciclos

	hc11->_clock();  
	data = hc11->memory[address];    

	hc11->ACCB = hc11->ACCB | data;		//B = B or M

	hc11->CCR.V(0);    
	hc11->CCR.Z(hc11->ACCB == 0);
	hc11->CCR.N(hc11->ACCB.bit7());
}


/////////////////////////////////////////////////
//
//  LDD - load ACCD
//  
//  Addr. Mode: IND, X
//  
//  Opcodes: ECh
//
//  Clocks: 5
//			2
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_ECh(HC11 *hc11)
{	
	word_t address;
	
	address = IndXAddr(hc11);	//consume 2 ciclos

	hc11->_clock();	
	hc11->ACCA = hc11->memory[address];		//D=M

	hc11->_clock();	
	hc11->ACCB = hc11->memory[address+1];

	hc11->CCR.V(0);
	hc11->CCR.Z((hc11->ACCB | hc11->ACCA) == 0);    
	hc11->CCR.N(hc11->ACCA.bit7());			//bit7 de A es el bit15 de D
}


/////////////////////////////////////////////////
//
//  STD - Store ACCD
//  
//  Addr. Mode: IND, X
//  
//  Opcodes: EDh
//
//  Clocks: 5
//			2
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_EDh(HC11 *hc11)
{	
	word_t address;
	
	address = IndXAddr(hc11);		//consume 2 ciclos

	hc11->_clock();	
	hc11->memory.Write(address, hc11->ACCA);	//M=D
	
	hc11->_clock();	
	hc11->memory.Write(address+1, hc11->ACCB);

	hc11->CCR.V(0);
	hc11->CCR.Z((hc11->ACCB | hc11->ACCA) == 0);    
	hc11->CCR.N(hc11->ACCA.bit7());
}


/////////////////////////////////////////////////
//
//  LDX - load X
//  
//  Addr. Mode: IND, X
//  
//  Opcodes: EEh
//
//  Clocks: 5
//			2
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_EEh(HC11 *hc11)
{	
	word_t address;
	
	address = IndXAddr(hc11);		//consume 2 ciclos

	hc11->_clock();	
	hc11->IX.high(hc11->memory[address]);	//X=M

	hc11->_clock();	
	hc11->IX.low(hc11->memory[address+1]);

	hc11->CCR.V(0);    
	hc11->CCR.Z(hc11->IX == 0);
	hc11->CCR.N((hc11->IX & 0x8000)>>15);
}


/////////////////////////////////////////////////
//
//  STX - Store IX
//  
//  Addr. Mode: IND, X
//  
//  Opcodes: EFh
//
//  Clocks: 5
//			2
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_EFh(HC11 *hc11)
{	
	word_t address;
	
	address = IndXAddr(hc11);		//consume 2 ciclos

	hc11->_clock();	
	hc11->memory.Write(address, hc11->IX.high());	//M=X
	
	hc11->_clock();	
	hc11->memory.Write(address+1, hc11->IX.low());

	hc11->CCR.V(0);
	hc11->CCR.Z(hc11->IX == 0);    
	hc11->CCR.N((hc11->IX & 0x8000)?1:0);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
//
//  LDAB - load ACCB
//  
//  Addr. Mode: EXT
//  
//  Opcodes: F6h
//
//  Clocks: 4
//			3
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_F6h(HC11 *hc11)
{	
	word_t address;
	
	address = ExtAddr(hc11);		//consume 2 ciclos

	hc11->_clock();	
	hc11->ACCB = hc11->memory[address];		//B=M

	hc11->CCR.V(0);    
	hc11->CCR.Z(hc11->ACCB == 0);    
	hc11->CCR.N(hc11->ACCB.bit7());
}


/////////////////////////////////////////////////
//
//  STAB - Store ACCB
//  
//  Addr. Mode: EXT
//  
//  Opcodes: F7h
//
//  Clocks: 4
//			3
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_F7h(HC11 *hc11)
{	
	word_t address;

	address = ExtAddr(hc11);	//consume 2 ciclos
	
	hc11->_clock();	
	hc11->memory.Write(address, hc11->ACCB);	//M=B

	hc11->CCR.V(0);
	hc11->CCR.Z(hc11->ACCB == 0);    
	hc11->CCR.N(hc11->ACCB.bit7());
}


/////////////////////////////////////////////////
//
//  ORAB - Inclusize OR
//  
//  Addr. Mode: EXT
//  
//  Opcodes: FAh
//           
//  Clocks: 4
//			3
//  Flags:	N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_FAh(HC11 *hc11)
{
	byte_t data;
	word_t address;    
	
	address = ExtAddr(hc11);    //consume 2 ciclos

	hc11->_clock();  
	data = hc11->memory[address];    

	hc11->ACCB = hc11->ACCB | data;		//B = B or M

	hc11->CCR.V(0);    
	hc11->CCR.Z(hc11->ACCB == 0);
	hc11->CCR.N(hc11->ACCB.bit7());
}


/////////////////////////////////////////////////
//
//  LDD - load ACCD
//  
//  Addr. Mode: EXT
//  
//  Opcodes: FCh
//
//  Clocks: 5
//			3
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_FCh(HC11 *hc11)
{	
	word_t address;
	
	address = ExtAddr(hc11);	//consume 2 ciclos

	hc11->_clock();	
	hc11->ACCA = hc11->memory[address];		//D=M

	hc11->_clock();	
	hc11->ACCB = hc11->memory[address+1];

	hc11->CCR.V(0);
	hc11->CCR.Z((hc11->ACCB | hc11->ACCA) == 0);    
	hc11->CCR.N(hc11->ACCA.bit7());			//el bit7 de A es el bit15 de D
}


/////////////////////////////////////////////////
//
//  STD - Store ACCD
//  
//  Addr. Mode: EXT
//  
//  Opcodes: FDh
//
//  Clocks: 5
//			3
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_FDh(HC11 *hc11)
{	
	word_t address;

	address = ExtAddr(hc11);	//consume 2 ciclos
	
	hc11->_clock();	
	hc11->memory.Write(address, hc11->ACCA);	//M=D
	
	hc11->_clock();	
	hc11->memory.Write(address+1, hc11->ACCB);

	hc11->CCR.V(0);
	hc11->CCR.Z((hc11->ACCB | hc11->ACCA) == 0);    
	hc11->CCR.N(hc11->ACCA.bit7());
}


////////////////////////////////////////////////
//
//  LDX - load IX
//  
//  Addr. Mode: EXT
//  
//  Opcodes: FEh
//
//  Clocks: 5
//			3
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_FEh(HC11 *hc11)
{	
	word_t address;
	
	address = ExtAddr(hc11);	//consume 2 ciclos

	hc11->_clock();	
	hc11->IX.high(hc11->memory[address]);	//X=M

	hc11->_clock();	
	hc11->IX.low(hc11->memory[address+1]);    

	hc11->CCR.V(0);    
	hc11->CCR.Z(hc11->IX == 0);
	hc11->CCR.N((hc11->IX & 0x8000)>>15);
}


/////////////////////////////////////////////////
//
//  STX - Store X
//  
//  Addr. Mode: EXT
//  
//  Opcodes: FFh
//
//  Clocks: 5
//			3
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_FFh(HC11 *hc11)
{	
	word_t address;
	
	address = ExtAddr(hc11);	//consume 2 ciclos

	hc11->_clock();	
	hc11->memory.Write(address, hc11->IX.high());	//M=X
	
	hc11->_clock();	
	hc11->memory.Write(address+1, hc11->IX.low());

	hc11->CCR.V(0);
	hc11->CCR.Z(hc11->IX == 0);    
	hc11->CCR.N((hc11->IX & 0x8000)?1:0);
}



















///////////////////////////////////////////////////////////////////////////////////////////////////////

//serian 7 funciones para instrucciones extendidas 0x1A
//y 4 funciones para 0xCD, se usan gracias a un switch case


/////////////////////////////////////////////////
//
//  Opcodes that deal with IX and IY use these codes
//  to precede the actually opcode
//  
//  Opcode: 1A/CD
//          ---
//  
////////////////////////////////////////////////

void HC11::op_1Ah(HC11 *hc11)
{	
	byte_t num;

	hc11->_clock();					//consume 1 ciclo
	num = hc11->memory[hc11->PC];	//lee el siguiente byte de la memoria
	hc11->PC++;

	switch(num)
	{
		case 0x83: HC11::op1A_83h(hc11);
			break;
		case 0x93: HC11::op1A_93h(hc11);
			break;
		case 0xA3: HC11::op1A_A3h(hc11);
			break;
		case 0xAC: HC11::op1A_ACh(hc11);
			break;
		case 0xB3: HC11::op1A_B3h(hc11);
			break;
		case 0xEE: HC11::op1A_EEh(hc11);
			break;
		case 0xEF: HC11::op1A_EFh(hc11);
			break;
		default: HC11::op_illegal(hc11);
	}
}              


void HC11::op_CDh(HC11 *hc11)
{
	byte_t num;
	
	hc11->_clock();					//consume 1 ciclo
	num = hc11->memory[hc11->PC];	//lee el siguiente byte de la memoria
	hc11->PC++;

	switch(num)
	{
		case 0xA3: HC11::opCD_A3h(hc11);break;
		case 0xAC: HC11::opCD_ACh(hc11);break;
		case 0xEE: HC11::opCD_EEh(hc11);break;
		case 0xEF: HC11::opCD_EFh(hc11);break;
		default: HC11::op_illegal(hc11);
	}
}              


/////////////////////////////////////////////////
//
//  CPD - Compare Accumulator D
//  
//  Addr. Mode: IMM
//  
//  Opcode: 1A 83 
//	    
//
//  Clocks:	5
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op1A_83h(HC11* hc11)
{
	Word temp;
	Word result;
	Word accd;
	
	accd = (hc11->ACCA << 8) | hc11->ACCB;		//formo el registro D

	hc11->_clock();
	temp.high(hc11->memory[hc11->PC]);
	hc11->PC++;

	hc11->_clock();
	temp.low(hc11->memory[hc11->PC]);
	hc11->PC++;

	result = accd - temp;			//D-M

	hc11->CCR.N(result.bit15());
	hc11->CCR.Z(result == 0);

	hc11->CCR.V((accd.bit15() && !temp.bit15() && !result.bit15()) ||
				(!accd.bit15() && temp.bit15() && result.bit15()));

	hc11->CCR.C((!accd.bit15() && temp.bit15()) || (temp.bit15() && result.bit15()) || (result.bit15() && !accd.bit15()));

	
	hc11->_clock();
	//en total son 5 menos 2, una del fetch y otra de llamar a op_1Ah()
}


/////////////////////////////////////////////////
//
//  CPD - Compare Accumulator D
//  
//  Addr. Mode: DIR
//  
//  Opcode: 1Ah 93h
//	    
//
//  Clocks:	6
//
//  Flags:	N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op1A_93h(HC11* hc11)
{
	byte_t address;
	Word temp;
	Word result;
	Word accd;

	accd = (hc11->ACCA << 8) | hc11->ACCB;

	address = DirAddr(hc11);	//consume 1 ciclo

	hc11->_clock();

	temp.high(hc11->memory[address]);
	temp.low(hc11->memory[address+1]);
	
	hc11->_clock();

	result = accd - temp;		//D-M

	hc11->CCR.N(result.bit15());
	hc11->CCR.Z(result == 0);

	hc11->CCR.V((accd.bit15() && !temp.bit15() && !result.bit15()) ||
				(!accd.bit15() && temp.bit15() && result.bit15()));
	
	hc11->CCR.C((!accd.bit15() && temp.bit15()) || (temp.bit15() && result.bit15()) || (result.bit15() && !accd.bit15()));


	hc11->_clock();
	//en total son 6 menos 2, una del fetch y otra de llamar a op_1Ah()
}


/////////////////////////////////////////////////
//
//  CPD - Compare Accumulator D
//  
//  Addr. Mode: IND, X
//  
//  Opcode: 1Ah A3h
//	    
//
//  Clocks:	7
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op1A_A3h(HC11* hc11)
{
	word_t address;
	Word temp;
	Word result;
	Word accd;

	accd = (hc11->ACCA << 8) | hc11->ACCB;

	address = IndXAddr(hc11);		//consume 2 ciclos

	hc11->_clock();

	temp.high(hc11->memory[address]);
	temp.low(hc11->memory[address+1]);

	hc11->_clock();

	result = accd - temp;		//D-M

	hc11->_clock();

	hc11->CCR.N(result.bit15());
	hc11->CCR.Z(result == 0);

	hc11->CCR.V((accd.bit15() && !temp.bit15() && !result.bit15()) ||
				(!accd.bit15() && temp.bit15() && result.bit15()));

	hc11->CCR.C((!accd.bit15() && temp.bit15()) || (temp.bit15() && result.bit15()) || (result.bit15() && !accd.bit15()));

	//en total son 7 menos 2, una del fetch y otra de llamar a op_1Ah()
}


/////////////////////////////////////////////////
//
//  CPY - Compare Index Register Y
//  
//  Addr. Mode: IND, X
//  
//  Opcodes:1Ah 
//          AC
//
//  Clocks: 7
//
//  Flags:	N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op1A_ACh(HC11 *hc11)
{	
	CCRReg mask;    
	Word data;
	Word result;
	word_t address;

	address = IndXAddr(hc11);	//consume 2 ciclos

	hc11->_clock();	
	data.high(hc11->memory[address]);
	hc11->_clock();	
	data.low(hc11->memory[address+1]);

	result = hc11->IY - data;		//Y-M
	
	hc11->CCR.N(result.bit15());
	hc11->CCR.Z(result == 0);

	hc11->CCR.V(hc11->IY.bit15() && !data.bit15() && !result.bit15() ||
				!hc11->IY.bit15() && data.bit15() && result.bit15());
	
	hc11->CCR.C((!hc11->IY.bit15()) && data.bit15() ||
				data.bit15() && result.bit15() ||
				result.bit15() && (!hc11->IY.bit15()));


	hc11->_clock();
	//en total son 7 menos 2, una del fetch y otra de llamar a op_1Ah()
}


/////////////////////////////////////////////////
//
//  CPD - Compare Accumulator D
//  
//  Addr. Mode: EXT
//  
//  Opcode: 1Ah B3h
//	    
//
//  Clocks:	7
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op1A_B3h(HC11* hc11)
{
	word_t address;
	Word temp;
	Word result;
	Word accd;

	accd = (hc11->ACCA << 8) | hc11->ACCB;

	address = ExtAddr(hc11);	//consume 2 ciclos

	hc11->_clock();

	temp.high(hc11->memory[address]);
	temp.low(hc11->memory[address+1]);

	hc11->_clock();

	result = accd - temp;		//D-M
	
	hc11->_clock();

	hc11->CCR.N(result.bit15());
	hc11->CCR.Z(result == 0);

	hc11->CCR.V((accd.bit15() && !temp.bit15() && !result.bit15()) ||
				(!accd.bit15() && temp.bit15() && result.bit15()));

	hc11->CCR.C((!accd.bit15() && temp.bit15()) || (temp.bit15() && result.bit15()) || (result.bit15() && !accd.bit15()));

	//en total son 7 menos 2, una del fetch y otra de llamar a op_1Ah()
}


/////////////////////////////////////////////////
//
//  LDY - Load IY
//  
//  Addr. Mode: IND, X
//  
//  Opcodes: 1Ah
//           EEh
//
//  Clocks: 6
//
//  Flags:	N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op1A_EEh(HC11 *hc11)
{	
	word_t address;
	
	address = IndXAddr(hc11);	//consume 2 ciclos

	hc11->_clock();	
	hc11->IY.high(hc11->memory[address]);	// Y = M

	hc11->_clock();	
	hc11->IY.low(hc11->memory[address+1]);

	hc11->CCR.V(0);    
	hc11->CCR.Z(hc11->IY == 0);
	hc11->CCR.N((hc11->IY & 0x8000)?1:0);//>>15);	//cualquiera de las 2 vale

	//en total son 6 menos 2, una del fetch y otra de llamar a op_1Ah()
}


/////////////////////////////////////////////////
//
//  STY - Store Y
//  
//  Addr. Mode: IND, X
//  
//  Opcodes: 1Ah
//           EFh
//
//  Clocks: 6
//
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op1A_EFh(HC11 *hc11)
{	
	word_t address;

	address = IndXAddr(hc11);		//consume 2 ciclos

	hc11->_clock();	
	hc11->memory.Write(address, hc11->IY.high());    // M = Y
	
	hc11->_clock();	
	hc11->memory.Write(address+1, hc11->IY.low());

	hc11->CCR.V(0);
	hc11->CCR.Z(hc11->IY == 0);    
	hc11->CCR.N((hc11->IY & 0x8000)?1:0);


	//en total son 6 menos 2, una del fetch y otra de llamar a op_1Ah()
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
//
//  CPD - Compare Accumulator D
//  
//  Addr. Mode: Indexed Y
//  
//  Opcode: CDh A3h
//	    
//  Clocks:	7
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::opCD_A3h(HC11* hc11)
{
	word_t address;
	Word temp;
	Word result;
	Word accd;

	accd = (hc11->ACCA << 8) | hc11->ACCB;		//formo el registro D con los valores de A y B

	address = IndYAddr(hc11);		//consume 2 ciclos

	hc11->_clock();
	temp.high(hc11->memory[address]);
	temp.low(hc11->memory[address+1]);
	hc11->_clock();

	result = accd - temp;		// D-M

	hc11->_clock();


	hc11->CCR.N(result.bit15());
	hc11->CCR.Z(result == 0);

	hc11->CCR.V((accd.bit15() && !temp.bit15() && !result.bit15()) || 
				(!accd.bit15() && temp.bit15() && result.bit15()));

	hc11->CCR.C( !accd.bit15() && temp.bit15() || temp.bit15() && result.bit15() || result.bit15() && !accd.bit15() );

	
	//en total son 7 menos 2, una del fetch y otra de llamar a op_CDh()
}


/////////////////////////////////////////////////
//
//  CPX - Compare Index Register X
//  
//  Addr. Mode: IND, Y
//  
//  Opcodes: CD AC
//
//  Clocks: 7
//
//  Flags:	N, Z, V, C
//
////////////////////////////////////////////////

void HC11::opCD_ACh(HC11 *hc11)
{	
	CCRReg mask;    
	Word data;
	Word result;
	word_t address;

	address = IndYAddr(hc11);	//consume 2 ciclos

	hc11->_clock();	
	data.high(hc11->memory[address]);
	hc11->_clock();	
	data.low(hc11->memory[address+1]);

	result = hc11->IX - data;	// X - M
	

	hc11->CCR.N(result.bit15());
	hc11->CCR.Z(result == 0);

	hc11->CCR.V( hc11->IX.bit15() && !data.bit15() && !result.bit15() || 
				!hc11->IX.bit15() && data.bit15() && result.bit15() );  
	
	hc11->CCR.C( !hc11->IX.bit15() && data.bit15() || data.bit15() && result.bit15() || result.bit15() && !hc11->IX.bit15() );


	hc11->_clock();		//faltaba
	//en total son 7 menos 2, una del fetch y otra de llamar a op_CDh() 
}


////////////////////////////////////////////////
//
//  LDX - Load IX
//  
//  Addr. Mode: IND, Y
//  
//  Opcodes: CDh
//           EEh
//
//  Clocks: 6
//
//  Flags: N, Z, V=0
//
////////////////////////////////////////////////

void HC11::opCD_EEh(HC11 *hc11)
{	
	word_t address;
	
	address = IndYAddr(hc11);	//consume 2 ciclos

	hc11->_clock();	
	hc11->IX.high(hc11->memory[address]);	// X = M

	hc11->_clock();	
	hc11->IX.low(hc11->memory[address+1]);

	hc11->CCR.V(0);    
	hc11->CCR.Z(hc11->IX == 0);
	hc11->CCR.N((hc11->IX & 0x8000)>>15);

	//en total son 6 menos 2, una del fetch y otra de llamar a op_CDh()
}


/////////////////////////////////////////////////
//
//  STX - Store IX
//  
//  Addr. Mode: IND, Y
//  
//  Opcodes: CDh
//           EFh
//
//  Clocks: 6
//
//  Flags:	N, Z, V=0
//
////////////////////////////////////////////////

void HC11::opCD_EFh(HC11 *hc11)
{	
	word_t address;
	
	address = IndYAddr(hc11);	//consume 2 ciclos

	hc11->_clock();	
	hc11->memory.Write(address, hc11->IX.high());		// M = X
	
	hc11->_clock();	
	hc11->memory.Write(address+1, hc11->IX.low());

	hc11->CCR.V(0);
	hc11->CCR.Z(hc11->IX == 0);    
	hc11->CCR.N((hc11->IX & 0x8000)?1:0);

	//en total son 6 menos 2, una del fetch y otra de llamar a op_CDh()
}