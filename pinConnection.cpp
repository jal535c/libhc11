#include "pinConnection.h"
#include "stdio.h"


void PinConnection::Set(bit_t value) {		//funcion virtual que es sobrecargada por una clase derivada
	printf("Pin Updated %d\n", value);		//para testear
}
