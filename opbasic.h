
#ifndef OPBASIC_H
#define OPBASIC_H
 


//el *.cpp que incluya este *.h antes debe incluir a hc11.h, basetypes.h, hc11misc.h, sino dá error
//aunque la segunda y tercera ya estan incluidas en hc11.h

Byte addbyte(HC11* hc11, Byte acc, Byte num, CCRReg flag, bit_t carryinflag);
Word addword(HC11* hc11, Word acc, Word num, CCRReg flag, bit_t carryinflag);
Byte subbyte(HC11* hc11, Byte acc, Byte num, CCRReg flag);
Word subword(HC11* hc11, Word acc, Word num, CCRReg flag);
int round(double num);



#endif