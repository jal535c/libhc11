#ifndef __WORDTYPE_H__
#define __WORDTYPE_H__


#include "bytetype.h"


typedef unsigned short word_t;	// 16 bits
typedef unsigned long dword_t;	// 32 bits


struct Word {

	word_t word;	//atributo


    byte_t low() {				//devuelve la parte baja del atributo en un byte_t
		return (word & 0x00FF);
	}

    byte_t high() {				//devuelve la parte alta del atributo en un byte_t
		return (word & 0xFF00)>>8;
	}

    void low(byte_t value) {		//escribe un byte_t en la parte baja del atributo word_t, respeta la parte alta
		word = (word & 0xFF00) | value;
	}

    void high(byte_t value) {		//escribe un byte_t desplazado en la parte alta del atributo, respeta la parte baja
		word = (word & 0x00FF) | (value<<8);
	}


    bit_t bit0() {
		return (word & 0x0001);
	}
    bit_t bit1() {
		return (word & 0x0002)>>1;
	}
    bit_t bit2() {
		return (word & 0x0004)>>2;
	}
    bit_t bit3() {
		return (word & 0x0008)>>3;
	}
    bit_t bit4() {
		return (word & 0x0010)>>4;
	}
    bit_t bit5() {
		return (word & 0x0020)>>5;
	}
    bit_t bit6() {
		return (word & 0x0040)>>6;
	}
    bit_t bit7() {
		return (word & 0x0080)>>7;
	}
    bit_t bit8() {
		return (word & 0x0100)>>8;
	}
    bit_t bit9() {
		return (word & 0x0200)>>9;
	}
    bit_t bit10() {
		return (word & 0x0400)>>10;
	}
    bit_t bit11() {
		return (word & 0x0800)>>11;
	}
    bit_t bit12() {
		return (word & 0x1000)>>12;
	}
    bit_t bit13() {
		return (word & 0x2000)>>13;
	}
    bit_t bit14() {
		return (word & 0x4000)>>14;
	}
    bit_t bit15() {
		return (word & 0x8000)>>15;
	}


    void bit0(bit_t value) {
		word = (word & 0xFFFE) | (word_t)value;
	}
    void bit1(bit_t value) {
		word = (word & 0xFFFD) | (word_t)value<<1;	//respeto todos los bits menos bit1, paso value a word_t y desplazo, los sumo
	}
    void bit2(bit_t value) {
		word = (word & 0xFFFB) | (word_t)value<<2;
	}
    void bit3(bit_t value) {
		word = (word & 0xFFF7) | (word_t)value<<3;
	}
    void bit4(bit_t value) {
		word = (word & 0xFFEF) | (word_t)value<<4;
	}
    void bit5(bit_t value) {
		word = (word & 0xFFDF) | (word_t)value<<5;
	}
    void bit6(bit_t value) {
		word = (word & 0xFFBF) | (word_t)value<<6;
	}
    void bit7(bit_t value) {
		word = (word & 0xFF7F) | (word_t)value<<7;
	}
    void bit8(bit_t value) {
		word = (word & 0xFEFF) | (word_t)value<<8;
	}
    void bit9(bit_t value) {
		word = (word & 0xFDFF) | (word_t)value<<9;
	}
    void bit10(bit_t value) {
		word = (word & 0xFBFF) | (word_t)value<<10;
	}
    void bit11(bit_t value) {
		word = (word & 0xF7FF) | (word_t)value<<11;
	}
    void bit12(bit_t value) {
		word = (word & 0xEFFF) | (word_t)value<<12;
	}
    void bit13(bit_t value) {
		word = (word & 0xDFFF) | (word_t)value<<13;
	}
    void bit14(bit_t value) {
		word = (word & 0xBFFF) | (word_t)value<<14;
	}
    void bit15(bit_t value) {
		word = (word & 0x7FFF) | (word_t)value<<15;
	}


	Word() 			//constructor
	{

	}

    Word(word_t data) 		//constructor extendido
	{	
		word = data;
	}

    operator word_t () {		//operador de conversion de tipo: pasa objeto Word a tipo word_t
		return word;
	}

    Word operator=(const word_t& data) {	//sobrecarga del operador de asignacion
		word = data;
		return *this;
	}    
  
    Word& operator++(int ) {		//sobrecarga del operador de post incremento
		word++;
		return *this;
	}

    Word& operator--(int ) {		//sobrecarga del operador de post decremento
		word--;
		return *this;
	}
};


#endif