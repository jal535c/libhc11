#include "hc11.h"

// Defino la variable static (debe ser fuera de la clase), y la inicializo con las direcciones de las funciones

interrupt_service_t HC11::psel_table[N_IBIT_INTERRUPTS] = { HC11::ServiceTO,	//tambien vale &HC11::ServiceTO
															HC11::ServicePAOV,
															HC11::ServicePAI,
															HC11::ServiceSPI,
															HC11::ServiceSCI,
															HC11::ServiceIRQ,	//reserved
															HC11::ServiceIRQIO,	//irq
															HC11::ServiceRTI,
															HC11::ServiceIC1,
															HC11::ServiceIC2,
															HC11::ServiceIC3,
															HC11::ServiceOC1,
															HC11::ServiceOC2,
															HC11::ServiceOC3,
															HC11::ServiceOC4,
															HC11::ServiceI4O5 };

//lo inicio con las direcciones de comienzo de las funciones
//para llamar o ejecutar una funcion: func()
//para usar su direccion: func o tambien &func
//para llamar a una funcion, psel_table[0](*hc11), elemento del array seguido del argumento de la funcion
//El orden en el k estan en el array es el orden de los 4 bits de psel, despues el orden de prioridad se sabe en ResolveInterrupt()
												

//Implementacion de las funciones

bool HC11::ServiceIRQ(HC11 *hc11) {
	bool status = false;

	if (hc11->IRQInterrupt()) {	//llama al metodo IRQInterrup(), devuelve 1 si hay peticion de int del pin externo IRQ
		
		if (hc11->regfile.IRQE())	//si es activo por flanco
			hc11->irq_edge = 0;		//reseteo la variable de flancos
		
		hc11->FetchVector(IRQVector);	//busco el vector: pone el valor del #define en PC
		status = true;
	
	}

	return (status);
};


bool HC11::ServiceIRQIO(HC11 *hc11)
{
	bool status = false;

	if (hc11->IRQInterrupt() ||	(hc11->regfile.STAF() && hc11->regfile.STAI()))   //bits del registro PIOC (modo handshake)
	{
		if (hc11->regfile.IRQE())
			hc11->irq_edge = 0;
		
		hc11->FetchVector(IRQVector);        
		status = true;
	}
	return (status);
};


bool HC11::ServiceRTI(HC11 *hc11) {
	bool status = false;

	if (hc11->regfile.RTII() && hc11->regfile.RTIF()) {	//si la mascara está activada y ocurre el flag
		
		hc11->FetchVector(RTIVector);					//va ha buscar el vector (pone el valor del #define en PC)
		status = true;
	
	}

	return (status);

};


bool HC11::ServiceIC1(HC11 *hc11)
{   
	bool status = false;

	if (hc11->regfile.IC1I() && hc11->regfile.IC1F())        
	{         
		hc11->FetchVector(IC1Vector);        
		status = true;
	}        

	return (status);
};


bool HC11::ServiceIC2(HC11 *hc11)
{   
	bool status = false;

	if (hc11->regfile.IC2I() && hc11->regfile.IC2F())        
	{                
		hc11->FetchVector(IC2Vector);        
		status = true;
	}        

	return (status);
};


bool HC11::ServiceIC3(HC11 *hc11)
{   
	bool status = false;

	if (hc11->regfile.IC3I() && hc11->regfile.IC3F())        
	{
		hc11->FetchVector(IC3Vector);        
		status = true;
	}        

	return (status);
};


bool HC11::ServiceOC1(HC11 *hc11)
{   
	bool status = false;

	if (hc11->regfile.OC1I() && hc11->regfile.OC1F())        
	{                
		hc11->FetchVector(OC1Vector);        
		status = true;
	}        

	return (status);
};


bool HC11::ServiceOC2(HC11 *hc11)
{   
	bool status = false;
	if (hc11->regfile.OC2I() && hc11->regfile.OC2F())        
	{         
		hc11->FetchVector(OC2Vector);        
		status = true;
	}
	return (status);
};


bool HC11::ServiceOC3(HC11 *hc11)
{   
	bool status = false;
	if (hc11->regfile.OC3I() && hc11->regfile.OC3F())        
	{         
		hc11->FetchVector(OC3Vector);        
		status = true;
	}        
	return (status);
};


bool HC11::ServiceOC4(HC11 *hc11)
{   
	bool status = false;
	if (hc11->regfile.OC4I() && hc11->regfile.OC4F())        
	{         
		hc11->FetchVector(OC4Vector);        
		status = true;
	}        
	return (status);
};


bool HC11::ServiceI4O5(HC11 *hc11)
{   
	bool status = false;
	if (hc11->regfile.I4O5I() && hc11->regfile.I4O5F())        
	{         
		hc11->FetchVector(I4O5Vector);        
		status = true;
	}        
	return (status);
};


bool HC11::ServiceTO(HC11 *hc11) {		//devuelve true si se ha ejecuta la rutina de interrupcion
   
	bool status = false;

	if (hc11->regfile.TOI() && hc11->regfile.TOF()) {      //si se dan la mascara y el flag  
			 
		hc11->FetchVector(TOVector);				//busca el vector, segun el define TOVector es 0x1E
		status = true;							
	
	}        
		
	return (status);

};


bool HC11::ServicePAOV(HC11 *hc11)
{   
	bool status = false;
	if (hc11->regfile.PAOVI() && hc11->regfile.PAOVF())        
	{         
		hc11->FetchVector(PAOVVector);        
		status = true;
	}        
	return (status);
};


bool HC11::ServicePAI(HC11 *hc11)
{   
	bool status = false;
	if (hc11->regfile.PAII() && hc11->regfile.PAIF())        
	{         
		hc11->FetchVector(PAIVector);        
		status = true;
	}        
	return (status);
};


bool HC11::ServiceSPI(HC11 *hc11)
{   
	bool status = false;

	return (status);
};


bool HC11::ServiceSCI(HC11 *hc11)	
{   
	bool status = false;

	return (status);
};

