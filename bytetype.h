#ifndef __BYTETYPE_H__
#define __BYTETYPE_H__


typedef unsigned char bit_t;	//0000 0001, solo miro el bit0 aunque ocupe 8, asi puedo comparar bits y bytes, y usar mascaras
typedef unsigned char byte_t;	// 8 bits


struct Byte {		

	byte_t byte;   	//atributo 


    bit_t bit0() {
		return (byte & 0x01);	//cojo el bit 0 haciendo mascara (AND bit a bit)
	}
    
	bit_t bit1() {
		return (byte & 0x02)>>1;	//cojo el bit 1 haciendo mascara, y desplazo una posicion a la derecha, solo miro el bit0 de bit_t
	}
    
	bit_t bit2() {
		return (byte & 0x04)>>2;	// bit a bit con 00000100, resultado lo desplaza 2 posiciones a la derecha
	}
    
	bit_t bit3() {
		return (byte & 0x08)>>3;
	}
    
	bit_t bit4() {
		return (byte & 0x10)>>4;
	}
    
	bit_t bit5() {
		return (byte & 0x20)>>5;
	}
    
	bit_t bit6() {
		return (byte & 0x40)>>6;
	}
    
	bit_t bit7() {
		return (byte & 0x80)>>7;
	}


    void bit0(bit_t value) {			//escribo un valor en el bit 0 del byte
		byte = (byte & 0xFE) | value;	
	}
    
	void bit1(bit_t value) {
		byte = (byte & 0xFD) | value<<1;	//desplaza el valor en una posicion a la izquierda (la del bit1)
	}
    
	void bit2(bit_t value) {
		byte = (byte & 0xFB) | value<<2;
	}
    
	void bit3(bit_t value) {
		byte = (byte & 0xF7) | value<<3;
	}
    
	void bit4(bit_t value) {
		byte = (byte & 0xEF) | value<<4;
	}
    
	void bit5(bit_t value) {
		byte = (byte & 0xDF) | value<<5;
	}
    
	void bit6(bit_t value) {
		byte = (byte & 0xBF) | value<<6;
	}
    
	void bit7(bit_t value) {
		byte = (byte & 0x7F) | value<<7;
	}


    Byte() 		//constructor por defecto
	{

	}
    
	Byte(byte_t data) 	//constructor extendido (con parametros)
	{	
		byte = data;
	}

	operator byte_t () {	//sobrecarga del operador de conversion de tipo: convierte un objeto de esta clase en un tipo byte_t
		return byte;	
	} 
	
	Byte& operator=(const byte_t& data) {		//cuando a un objeto Byte le asigno un byte_t
		byte = data;	
		return *this;	
	}
};


#endif
