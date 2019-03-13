#include "hc11.h"	


//suma un byte al acumulador, 
//flag: un 1 indica los flags tenidos en cuenta (flags que son afectados)
//carryinflag: un 1 indica que la suma es con carry y que lo hay (me llevo carry)

Byte addbyte(HC11 *hc11, Byte acc, Byte num, CCRReg flag, bit_t carryinflag) {

	Byte result;
	
	result = acc + num + carryinflag;
	
	//logica para los indicadores que entran en juego

	if(flag.N())		//Negative. Si el bit mas significativo es 1, activa el flag N
		if(result.bit7())  
			hc11->CCR.N(1);
		else
			hc11->CCR.N(0);
	
	if(flag.Z())		//Zero. Si el resultado es cero, activa el flag Z
		if(!(result))   
			hc11->CCR.Z(1);
		else	
			hc11->CCR.Z(0);
	
	if(flag.V())		//Overflow
		if((acc.bit7()&num.bit7()&!(result.bit7())) | (!(acc.bit7())&!(num.bit7())&result.bit7()))	
			hc11->CCR.V(1);
		else
			hc11->CCR.V(0);
	
	if(flag.C()==1)		//carry
	{
		if((acc.bit7()&num.bit7()) | (num.bit7()&!(result.bit7())) | (!(result.bit7())&acc.bit7())) 
			hc11->CCR.C(1);
		else
			hc11->CCR.C(0);
	}

	if(flag.H())		//half carry
		if((acc.bit3()&num.bit3()) | (num.bit3()&!(result.bit3())) | (!(result.bit3())&acc.bit3()))  
			hc11->CCR.H(1);
		else	
			hc11->CCR.H(0);

	
	return(result);

}


Word addword(HC11* hc11, Word acc, Word num, CCRReg flag, bit_t carryinflag)
{
	flag.H(0);		//indicador de half carry
	
	Word result;
	
	result.low(addbyte(hc11, acc.low(), num.low(), 1, carryinflag)); 
	result.high(addbyte(hc11, acc.high(), num.high(), flag, hc11->CCR.C()));
	
	return(result);
}


Byte subbyte(HC11* hc11, Byte acc, Byte num, CCRReg flag) {		//no tiene en cuenta el borrow

	Byte result;

	result = acc - num;

	if(flag.N())		//Negative
		if(result.bit7())  
			hc11->CCR.N(1);
		else
			hc11->CCR.N(0);
	
	if(flag.Z())		//Zero
		if(result==0)		
			hc11->CCR.Z(1);
		else	
			hc11->CCR.Z(0);
	
	if(flag.V())		//Overflow
		if((acc.bit7()&!(num.bit7())&!(result.bit7())) | (!(acc.bit7())&num.bit7()&result.bit7()))	
			hc11->CCR.V(1);
		else
			hc11->CCR.V(0);
	
	if(flag.C()==1)		//carry
	{
		if((!(acc.bit7())&num.bit7()) | (num.bit7()&result.bit7()) | (result.bit7()&!(acc.bit7()))) 
			hc11->CCR.C(1);
		else
			hc11->CCR.C(0);
	}

	return(result);

}


Word subword(HC11* hc11, Word acc, Word num, CCRReg flag)
{
	Word result;
	Byte highX;
	Byte highM;
	Byte highR;
	
	result = acc - num;

	highX=acc.high();		//cojo los 8 bits altos, así trabajo con el bit 7 en vez del 15
	highM=num.high();		
	highR=result.high();	


	if(flag.N())		//Negative
		if(highR.bit7())  
			hc11->CCR.N(1);
		else
			hc11->CCR.N(0);
	
	if(flag.Z())		//Zero
		if(result == 0)   
			hc11->CCR.Z(1);
		else	
			hc11->CCR.Z(0);

	//fallo: usa num.bit7(), y debe usar highM.bit7(), probado al ejecutar y debugear 93 02 00 f0 y D=7777, no deberia activarse carry
	//arreglado

	if(flag.V())		//Overflow
		if((highX.bit7()&!(highM.bit7())&!(highR.bit7())) | (!(highX.bit7())&highM.bit7()&highR.bit7()))	
			hc11->CCR.V(1);
		else
			hc11->CCR.V(0);
	
	if(flag.C()==1)		//carry
	{
		if((!(highX.bit7())&highM.bit7()) | (highM.bit7()&highR.bit7()) | (highR.bit7()&!(highX.bit7()))) 
			hc11->CCR.C(1);
		else
			hc11->CCR.C(0);
	}

	return(result);
}


int round(double num) {
	return ((int)(num+0.5));		//redondea por arriba
}

