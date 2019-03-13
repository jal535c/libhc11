#include "portConnection.h"

#include "stdio.h"

//usado para testear la funcionalidad de PortConnection, 
//esta funcion nunca se llama, es sobrecargada por una clase derivada

void PortConnection::Write(byte_t data) {	//escribe 1 si el bit esta a 1, usado para TESTEAR, la GUI usa la implementacion de CPortIO

	printf("Port Updated: %d %d %d %d %d %d %d %d\n",
					(data&0x80) ? 1 : 0,
					(data&0x40) ? 1 : 0,
					(data&0x20) ? 1 : 0,
					(data&0x10) ? 1 : 0,
					(data&0x08) ? 1 : 0,
					(data&0x04) ? 1 : 0,
					(data&0x02) ? 1 : 0,
					(data&0x01) ? 1 : 0); 

}