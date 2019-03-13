#ifndef __CCR_H__
#define __CCR_H__


#include "byte.h"			//usa la clase Byte, y el typedef bit_t y byte_t
#define log(a,b) //fprintf(stderr,a,b) 


struct CCRReg : public Byte {	//deriva de la clase Byte
    bit_t C() {
		return bit0();
	}
    bit_t V() {
		return bit1();
	}
	bit_t Z() {
		return bit2();
	}
	bit_t N() {
		return bit3();
	}
	bit_t I() {
		return bit4();
	}
	bit_t H() {
		return bit5();
	}
    bit_t X() {
		return bit6();
	}
    bit_t S() {
		return bit7();
	}
    
    
    void C(bit_t val) {
		bit0(val);
	}
    void V(bit_t val) {
		bit1(val);
	}
    void Z(bit_t val) {
		bit2(val);
	}
    void N(bit_t val) {
		bit3(val);
	}
    void I(bit_t val) {
		bit4(val);
	}
    void H(bit_t val) {
		bit5(val);
	}
    void X(bit_t val) {
		bit6(val);
	}
    void S(bit_t val) {
		bit7(val);
	}
	
	
	CCRReg() 	
	{	
		byte = 0;
	}
	
	CCRReg(byte_t data) 
	{	
		byte = data;
	}

    CCRReg& operator=(const byte_t& data) {		
		byte = data;	
		return *this;	
	}
};


#endif
