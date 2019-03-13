#include "portARegister.h"
#include "hc11.h"
#include <assert.h>
#include <stdlib.h>

#include "portConnection.h"		//tengo el atributo connect heredado de PortRegister, pero 
									// en portRegister.h solo tengo la declaracion, debo incluir aqui portConnection.h 


//Funcion para introducir un nivel a un pin del Port A y ver si una captura de entrada o acumulador de pulso ha ocurrido
//El nivel del pin solo cambiara si el modo de direccion del pin es INPUT
//PortConnection se actualizara si uno esta conectado

//para el Input Capture (pines 0,1,2) y para el pulse acumulator (pin 7) del Port A

void PortARegister::PinInput(int PinNo, bit_t value) {	

	// Input just one pin
	assert((PinNo >= 0) && (PinNo <= 7));	//comprueba que el pin esta dentro del rango correcto

  
	bit_t previous_level = 0;	//usado en el Pulse Acumulator

	byte_t edge_mode = 0;	// Edge mode determines what edges the IC's are sensitive to.	Configuracion del capturador en TCTL2
	byte_t edge_type = 0;	// Edge type determines what edge (if any) has occurred.		Tipo de flanco detectado en PORTA

	
	// Edge configurations on EDGx, registro TCTL2
	//	EDGxA				EDGxB
	//		0						0					ICx's disabled
	//		0						1					Detect rising edges
	//		1						0					Detect falling edges
	//		1						1					Detect any edge


	switch(PinNo) {

		case 0:	// IC3, input capture 3, bit 0 de PortA
			
			// If the IC is enabled
			if (edge_mode = hc11->regfile.EDG3()) {	//asigna a edge_mode el valor de TCTL2 con mascara de EDG3, si no es 0 entra en if	

				// If an edge occurred, a 0x3 does not represent an edge
				if (  (edge_type = (Read(PinNo)<<1) | (value))	  &&    (edge_type != 0x3)  ) {	

					//forma de detectar flancos:
					//si edge_type vale 0 (lee 0 y value 0) o 3 (lee 1 y value 1), no entra al if ya que no hay flanco

					//lee el pin (valor actual), desplaza su valor 1 a la izq, hace OR bit a bit (suma) con el valor k kiero escribir
					//eso se lo asigno a egde_type
					//si edge_type es true (no es 0) y no es 3, entonces entra en el if



					// If the edge mode and type are equal, the right edge occurred (except 0x3)
					if ((edge_mode == edge_type) || (edge_mode == 0x3)) {	//al estar aquí ya ha habido flanco, ahora:
																			//si coinciden el flanco configurado y el que ocurre 
																			//o si el configurado es 3 (ambos)
																			//entra en el if

						// Set flag
						hc11->regfile.IC3F(1);	//se activa el flag IC3F

						// Generate an interrupt (maybe)  ????	Si las mascaras estan activas; en otro sitio se miran las mascaras

						// Set the TICx register
						hc11->regfile.TIC3.Write(hc11->regfile.TCNT.Read());	//guarda el valor de TCNT en TIC3
						//printf("Input compare %d occurred at %d\n",3-PinNo,hc11->regfile.TCNT.Read());
						//printf("TIC%d now holds %d\n",3-PinNo,hc11->regfile.TIC3.Read());
					}
				}		
			}

			pins[0].Input(value);	//escribe el valor en el pin 0 y sale del switch
			break;

      
		case 1:	// IC2, bit 1 de PortA
					
			// If the IC is enabled
			if (edge_mode = hc11->regfile.EDG2()) {

				// If an edge occurred, a 0x3 does not represent an edge
				if ((edge_type = (Read(PinNo)<<1) | (value))	&&    (edge_type != 0x3)) {

					// If the edge mode and type are equal, the right edge occurred (except 0x3)
					if ((edge_mode == edge_type) || (edge_mode == 0x3)) {

						// Set flag
						hc11->regfile.IC2F(1);

						// Generate an interrupt (maybe)  ????

						// Set the TICx register
						hc11->regfile.TIC2.Write(hc11->regfile.TCNT.Read());
						//printf("Input compare %d occurred at %d\n",3-PinNo,hc11->regfile.TCNT.Read());
						//printf("TIC%d now holds %d\n",3-PinNo,hc11->regfile.TIC2.Read());
					}
				}		
			}
          
			pins[1].Input(value);
			break;


		case 2:   // IC1, bit 2 de PortA
          
			// If the IC is enabled
			if (edge_mode = hc11->regfile.EDG1()) {

				// If an edge occurred, a 0x3 does not represent an edge
				if ((edge_type = (Read(PinNo)<<1) | (value)) && (edge_type != 0x3)) {

					// If the edge mode and type are equal, the right edge occurred (except 0x3)
					if ((edge_mode == edge_type) || (edge_mode == 0x3)) {

						// Set flag
						hc11->regfile.IC1F(1);

						// Generate an interrupt (maybe)  ????

						// Set the TICx register
						hc11->regfile.TIC1.Write(hc11->regfile.TCNT.Read());
						//printf("Input compare %d occurred at %d\n",3-PinNo,hc11->regfile.TCNT.Read());
						//printf("TIC%d now holds %d\n",3-PinNo,hc11->regfile.TIC1.Read());
					}
				}		
			}  
			
			pins[2].Input(value);
			break;


		case 7:	// PAI, pulse acumulator input en modo event counter
			
			previous_level = Read(PinNo);	//antes de escribir, guardo el valor que tenia       
			pins[PinNo].Input(value);		//escribo el nuevo valor que le paso como parametro

			// si está habilitado y el modo es cuenta de pulso, 
			//si hay flanco, 
			//y si el flanco es de subida (PEDGE 1 y previo 0) o bajada (PEDGE 0 previo 1)
			if ( (hc11->regfile.PAEN() && !hc11->regfile.PAMOD()) &&		
								(previous_level != Read(PinNo)) && 
								(hc11->regfile.PEDGE() != previous_level) ) {                                   
              
				hc11->regfile.PAIF(1);	//activa el flag del pulse acumulator input                         
              
				if(++hc11->regfile.PACNT == 0) {	//incrementa PACNT, y si llega a 0 (desborda)
					hc11->regfile.PAOVF(1);			//activa el flag de overflow del pulse acumulator
				}
          
			}
			break;
  
	}
	


	if (connect != NULL)			//si esta conectado
		connect->Write(Read());		//actualiza (escribe en la interfaz CPortIO::Write()) con el valor leido

}


PortARegister::PortARegister() {		//configura los pines como entrada del puerto A, para el capturador de entrada
	SetMode(0, Pin::INPUT);		//SetMode es un metodo de la clase padre PortRegister, pone un pin de un puerto en un modo determinado
	SetMode(1, Pin::INPUT);
	SetMode(2, Pin::INPUT);    
}

