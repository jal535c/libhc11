//#include "hc11.h"	//la pongo antes que el include timer.h, sino dá error

#include "timer.h"	//para acceder a su propia interfaz, 
					//tiene k ir debajo del include hc11.h, 
					//o poner class antes del tipo HC11 en timer.h

#include "hc11.h"	//al poner class HC11; en timer.h ya puedo poner éste include debajo de #include "timer.h"

#include <assert.h>		//la pongo yo
//#include <stdio.h>


//contiene toda la funcionalidad del sistema timer en el hc11
//maneja el divisor de reloj para el TCNT y para el RTI

TimerSubSystem::TimerSubSystem(HC11 *nhc11) {		//se llama desde el constructor de la clase HC11
       hc11 = nhc11;

       main_clock_divider = 0;
       rti_clock_divider = 0;
}


//esta funcion es llamada por el sistema timer en cada pulso de reloj
//maneja todos los divisores de reloj

void TimerSubSystem::ClockSystem() {	
	
	int PR = hc11->regfile.PR();	//PR son los bits de prescala, bits 0 y 1 de TMSK2, el valor varia de 0 a 3
	
    main_clock_divider++;	//es mi divisor de tiempos (para multiplos), señal E, empieza en 0, y se incrementa cada vez k 
							//se llame a esta funcion, 
							//si PR=0, main_clock_divider y TCNT tienen el mismo valor?

	//!(algo), busco que ese algo sea 0, para que valga 1 para &&
	//el resto es 0 si es multiplo de ese valor

    // TCNT rate
    if((PR == 0) ||									// && AND normal, & AND bit a bit
      ((PR == 1) && !(main_clock_divider%4)) ||		// el % se refiere al modulo, resto de una division entera, busco k sea 0
      ((PR == 2) && !(main_clock_divider%8)) ||     // si PR=2, solo si main_clock_divider es multiplo de 8 entrará en el if,
      ((PR == 3) && !(main_clock_divider%16))) {	//donde incrementa TCNT
        
		// Counter overflow
        if((++hc11->regfile.TCNT) == 0)		//incrementa TCNT, y si es cero
            hc11->regfile.TOF(1);			//activa el flag overflow del timer
    
	}

	//para activar el flag, antes ha tenido que entrar 65536 veces


	//el rti funciona diferente, entra en el if cada 8192 veces (2^13)-> 4ms
	//incrementando mi reloj rti, pero para el segundo if:
	//relacion: rti_clock =1, si RTR=0, mod entre 1 (cada vez k entra, cada 4ms, activa flag)
	//						, si RTR=1, mod entre 2 (cada 2 veces k entra, cada 8ms, activa flag)
	//						, si RTR=2, mod entre 4 (cada 4 veces k entra, cada 16ms, activa flag)
	//						, si RTR=3, mod entre 8 (cada 8 veces k entra, cada 32ms, activa flag)
    
    
    // RTI rate
	if(!(main_clock_divider % (1<<13))) {	//2^13 es mi referencia, es igual que desplazar un 1 hasta el bit 13

		rti_clock_divider++;	//mi referncia para el rti, mi frecuencia base
        
		if(!(rti_clock_divider % (1<<hc11->regfile.RTR())))		// RTI is further divided by RTR, 
															//1 los desplazo segun RTR, asi hago el mod entre 1,2,4,8
															//si es multiplo, activa el flag de rti
			hc11->regfile.RTIF(1);							
	
	}

	DoPulseAccumulator_GatedTimeAccumulationMode();
	DoOutputCompares();
};


//comprueba si un comparador de salida se ha disparado
//si lo ha hecho, realiza la accion apropiada en un pin o genera una interrupcion

void TimerSubSystem::DoOutputCompares() {
	
	word_t tcnt = hc11->regfile.TCNT.Read();	//guarda el valor de TCNT en tcnt

	bit_t has_control2 = 0;	//indica si OC2 está controlado por el OC1 (1) o esta libre (0)
	bit_t has_control3 = 0;
	bit_t has_control4 = 0;
	bit_t has_control5 = 0;

	
	if (tcnt == hc11->regfile.TOC1.Read()) {	//si coinciden el valor de TCNT y de TOC1 (el programa del usuario escribe en TOC1)

		
		hc11->regfile.OC1F(1);	//activa el flag de OC1, bit 7 de TFLG1

		
	    if (hc11->regfile.OC1M3()) {						//si la maskara 3 está activa (el programa de usuario es el que la pone)
		    hc11->regfile.PORTA.Action(3, hc11->regfile.OC1D3());	//escribe el valor de OC1D3 en el pin 3 de PORTA
            has_control5 = 1;								//el OC1 controla a OC5, el 5 no se podria usar por separado ahora
        }

	    if (hc11->regfile.OC1M4()) {
		    hc11->regfile.PORTA.Action(4, hc11->regfile.OC1D4());
            has_control4 = 1;
        }

	    if (hc11->regfile.OC1M5()) {
		    hc11->regfile.PORTA.Action(5, hc11->regfile.OC1D5());
            has_control3 = 1;
        }

	    if (hc11->regfile.OC1M6()) {
			hc11->regfile.PORTA.Action(6, hc11->regfile.OC1D6());
            has_control2 = 1;
        }

	    if (hc11->regfile.OC1M7())
			hc11->regfile.PORTA.Action(7, hc11->regfile.OC1D7());
	
	}


	// OC2, PA6
	if (tcnt == hc11->regfile.TOC2.Read()) {	//si coinciden el valor de TCNT y TOC2, entra en el if
		
		hc11->regfile.OC2F(1);
		
		bit_t ol6 = hc11->regfile.OL2();	//bits del registro TCLT1, mode y level, accion a tomar en caso de comparacion exitosa
		bit_t om6 = hc11->regfile.OM2();
			
        if (!has_control2) {		//comprueba los bits de habilitacion del comparador de salida, si está libre entra en el if

			switch((om6<<1)|ol6) {	//desplaza a la izquierda 1 posicion, y suma bit a bit el otro, asi forma el valor: 0,1,2 o3
				case 0x0:		// does not affect pin
					break;
				case 0x1:		// toggles pin
					hc11->regfile.PORTA.Action(6, !hc11->regfile.PORTA.Read(6));	//escribe lo contrario de lo k haya en el bit6
					break;
				case 0x2:		// clears pin
					hc11->regfile.PORTA.Action(6, 0);	//escribe un 0 en el bit 6 del Port A
					break;
				case 0x3:		// sets pin
					hc11->regfile.PORTA.Action(6, 1);	//escribe un 1 en el bit 6 del Port A
					break;
				default:	
					assert(false);	//comprueba que la condicion es false (no es ni 0,1,2,3)
			}
			
		}
	
	}


	// OC3, PA5
	if (tcnt == hc11->regfile.TOC3.Read()) {
	
		hc11->regfile.OC3F(1);
		
		bit_t ol5 = hc11->regfile.OL3();
		bit_t om5 = hc11->regfile.OM3();
			
        if (!has_control3) {

			switch((om5<<1)|ol5) {
				case 0x0:		// no afecta 
					break;
				case 0x1:		// toggles pin
					hc11->regfile.PORTA.Action(5, !hc11->regfile.PORTA.Read(5));
					break;
				case 0x2:		// clears pin
					hc11->regfile.PORTA.Action(5, 0);
					break;
				case 0x3:		// sets pin
					hc11->regfile.PORTA.Action(5, 1);
					break;
				default:	
					assert(false);
			}

        }
	
	}
	

	// OC4, PA4
	if (tcnt == hc11->regfile.TOC4.Read()) {
		
		hc11->regfile.OC4F(1);
				
		bit_t ol4 = hc11->regfile.OL4();
		bit_t om4 = hc11->regfile.OM4();
			
		if (!has_control4) {

			switch((om4<<1)|ol4) {
				case 0x0:		// no afecta
					break;
				case 0x1:		// toggles pin
					hc11->regfile.PORTA.Action(4, !hc11->regfile.PORTA.Read(4));
					break;
				case 0x2:		// clears pin
					hc11->regfile.PORTA.Action(4, 0);
					break;
				case 0x3:		// sets pin
					hc11->regfile.PORTA.Action(4, 1);
					break;
				default:	
					assert(false);
		  
			}
        
		}		
	
	}


	// OC5, PA3
	if (tcnt == hc11->regfile.TI4O5.Read()) {
		
		hc11->regfile.I4O5F(1);
		
		bit_t ol3 = hc11->regfile.OL5();
		bit_t om3 = hc11->regfile.OM5();
			
		if (!has_control5) {

			switch((om3<<1)|ol3) {
				case 0x0:		// no afecta
					break;
				case 0x1:		// toggles pin
					hc11->regfile.PORTA.Action(3, !hc11->regfile.PORTA.Read(3));
					break;
				case 0x2:		// clears pin
					hc11->regfile.PORTA.Action(3, 0);
					break;
				case 0x3:		// sets pin
					hc11->regfile.PORTA.Action(3, 1);
					break;
				default:	
					assert(false);
			}
        
		}
	
	}


}


//maneja la funcion del acumulador de pulsos cuando está en modo Gated time accumulation (duracion, puerteado)

void TimerSubSystem::DoPulseAccumulator_GatedTimeAccumulationMode() {	

	if(hc11->regfile.PAEN() & hc11->regfile.PAMOD()) {	//si PAEN es 1, y PAMOD es 1, entra en el if
		
		if(!(main_clock_divider%64)) {	//E clock/64
										//para entrar en el if, main_clock_divider debe ser multipo de 64 (mod 0, inverso 1)
			
			if(hc11->regfile.PORTA.Read(7) == hc11->regfile.PEDGE()) {	
				
				//si al leer PA7 es 1 y el flanco esta configurado como de subida, entra en el if, o
				//si al leer PA7 es 0 y el flanco esta configurado como de bajada, tambien entra en if
				
				if(!(++hc11->regfile.PACNT))	//incrementa PACNT, y si llega a cero, entra en el if
					hc11->regfile.PAOVF(1);		//activa flag de overflow de pulse acumulator	
			
			}	
		}
	}

}
