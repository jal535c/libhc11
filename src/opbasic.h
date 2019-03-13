#ifndef __OPBASIC_H__
#define __OPBASIC_H__
 

Byte addbyte(HC11* hc11, Byte acc, Byte num, CCRReg flag, bit_t carryinflag);
Word addword(HC11* hc11, Word acc, Word num, CCRReg flag, bit_t carryinflag);
Byte subbyte(HC11* hc11, Byte acc, Byte num, CCRReg flag);
Word subword(HC11* hc11, Word acc, Word num, CCRReg flag);
int round(double num);


#endif