//#include "hc11.h"
#include <hc11sim.h>        
#include <portConnection.h> 
#include <stdio.h>


int main() {
	HC11Sim sim;
	
	sim.LoadS19File("LED.S19");	
	sim.hc11.Reset();    

	PortConnection pca;
	PortConnection pcb;
	PortConnection pcc;

	sim.hc11.regfile.PORTA.Attach(&pca);     
	sim.hc11.regfile.PORTB.Attach(&pcb);
	sim.hc11.regfile.PORTC.Attach(&pcc);

	for (int i=0; i<20; i++) {
		printf("%02X ", sim.hc11.memory.Read(i));
	}

	sim.hc11.PC = 0;

	for (int i=0; i<65; i++) {
 		sim.hc11.Step();

		printf("\nPC %04X, AA %02X, IY %04X ", (int)sim.hc11.PC, (unsigned)sim.hc11.ACCA, (unsigned)sim.hc11.IY);
	}

	printf("\n");
	
	return 0;
}
