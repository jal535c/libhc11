
//este fichero contiene el constructor, reset, y los 8 metodos write de las clases anidadas internas

//al activar o desactivar bits de ciertos registros, ocurren ciertas cosas al hc11
//por eso estan estas funciones aqui


#include "regfile.h"	
//#include "memory.h"		
#include "ram.h"
#include "registerMap.h"


///////////////////////////////////////////////////////////////
// RegisterFile functions


RegisterFile::RegisterFile()
{
	
	DDRC.Associate(&PORTC);		//asocia los DDR con sus puertos
	DDRD.Associate(&PORTD);

	
	TCTL1.portA = &PORTA;	//vincula Atributos (puertos) de las clases internas con Atributos de la clase contenedora RegisterFile. 
							//faltan las clases internas INIT_C, HPRIO_C, TFLG_C ya que no tienen atributos o no son registros
	OC1M.portA = &PORTA;
	OC1M.pactl = &PACTL;
	OC1M.tctl1 = &TCTL1;

	PACTL.portA = &PORTA;	
	PACTL.oc1m = &OC1M;

	CFORC.portA = &PORTA;
	CFORC.oc1m = &OC1M;
	CFORC.oc1d = &OC1D;
	CFORC.tctl1 = &TCTL1;

	SPCR.ddrd = &DDRD;
	SPCR.portD = &PORTD;
	
  	for(int i=0; i<8; i++)				//pone puerto E como de entrada
    	PORTE.SetMode(i, Pin::INPUT);

}


void RegisterFile::Reset(Mode mode /*=SPECIAL_TEST*/)	//se llama desde el doc, OnOpenDocument() y desde HC11::Reset()
{														
											
	PORTA.Write(0x0);	// despues de un reset tiene el valor: U000 UUUU (U es indeterminado)
	PIOC.Write(0x3);	// Bit 2 (PLS) is U, reset: $03
	PORTB.Write(0x0);	// Port B is an output port, reset: $00

	DDRC.Write(0x0);	//reset: $00
	DDRD.Write(0x0);	//reset: $00
	
	PORTC.Write(0x0);	//reset indeterminado
	PORTD.Write(0x0);	//reset indeterminado
	PORTCL.Write(0x0);	//reset indeterminado

	PORTE.Write(0x0);		// Port E is an input port, reset: indeterminado
	
	CFORC.Write(0x0);	//reset: $00
	OC1M.Write(0x0);	//reset: $00
	OC1D.Write(0x0);	//reset: $00

	TCNT.Write(0x0);	//reset: $0000

	// TICx registers no les afecta el reset

	TOC1.Write(0xffff);		//reset: $ffff
	TOC2.Write(0xffff);		//reset: $ffff
	TOC3.Write(0xffff);		//reset: $ffff
	TOC4.Write(0xffff);		//reset: $ffff
	TI4O5.Write(0xffff);	//reset: $ffff

	TCTL1.Write(0x0);	//reset: $00
	TCTL2.Write(0x0);	//reset: $00
	TMSK1.Write(0x0);	//reset: $00
	TFLG1.Write(0x0);	//reset: $00
	TMSK2.Write(0x0);	//reset: $00
	TFLG2.Write(0x0);	//reset: $00
	PACTL.Write(0x0);	//reset: $00
	PACNT.Write(0x0);		// realmente no es afectado por el reset

	SPCR.Write(0x4);  // Bits 0,1  Undefined
	SPSR.Write(0x0);

	BAUD.Write(0x0);	// Bits 0,1,2  U, los otros 0, por defecto $04, y en buffalo $30
	SCCR1.Write(0x0);	// Bits 6,7 are U, los otros 0
	SCCR2.Write(0x0);	//reset: $00
	SCSR.Write(0xc0);	//reset: $c0
	SCDR.Write(0x0);	//  U, reset: indeterminado

	ADCTL.Write(0x0); // Bits 0-5  U, reset: indeterminado
	ADR1.Write(0x0);	//  A/D input regs, reset $00
	ADR2.Write(0x0);	//reset $00
	ADR3.Write(0x0);	//reset $00
	ADR4.Write(0x0);	//reset $00

	OPTION.Write(0x0);	// normalmente con delay ($10)
	COPRST.Write(0x0);	// COP register, reset $00
	PPROG.Write(0x0);	//reset $00
	
	
	if (mode == SINGLE_CHIP)		//$06	el 5 es prioridad reserved (igual k en buffalo), el 6 por defecto es prioridad irq
		HPRIO.Write(0x5);
	else if (mode == EXPANDED)		//$26
		HPRIO.Write(0x25);
	else if (mode == BOOTSTRAP)		//$c6
		HPRIO.Write(0xc5);
	else if (mode == SPECIAL_TEST)	//$76
		HPRIO.Write(0x75);


	INIT.Write(0x1);	//reset $01

	TEST1.Write(0x0);	//normal mode $00, special mode $08
	CONFIG.Write(0xc);  //por defecto NOSEC=1 NOCOP=1 ROMON=0 EEON=0, pero en la placa es 0x0F ya que si tiene en cuenta la rom y eeprom	
					
}


/////////////////////////////////////////////////////////////////
// TCTL1 function overrides
//////////////////////////////////////////////////////////////////////////////

void RegisterFile::TCTL1_C::Write(byte_t val)
{
	byte = val;
	
	// OL2, OM2
	if (bit7() || bit6())
		portA->SetMode(6, Pin::AUTO);		//comparador 2 conectado, habilitado (modo auto?), no puedo usar el port a
	else
		portA->SetMode(6, Pin::OUTPUT);		//comparador 2 desconectado, no afecta al pin asociado, puedo usar port a


	// OL3, OM3
	if (bit5() || bit4())
		portA->SetMode(5, Pin::AUTO);		//comparador 3 habilitado
	else 
		portA->SetMode(5, Pin::OUTPUT);


	// OL4, OM4
	if (bit3() || bit2())					
		portA->SetMode(4, Pin::AUTO);		//comparador 4 habilitado
	else
		portA->SetMode(4, Pin::OUTPUT);


	// OL5, OM5
	if (bit1() || bit0())
		portA->SetMode(3, Pin::AUTO);		//comparador 5 habilitado
	else
		portA->SetMode(3, Pin::OUTPUT);
}


/////////////////////////////////////////////////////////////////
// OC1M function overrides
//////////////////////////////////////////////////////////////////////////////

void RegisterFile::OC1M_C::Write(byte_t val)
{
	byte = val;
	
	if (bit7())									//si el bit7 de OC1M es 1, 
	{
		if(portA->GetMode(7) == Pin::OUTPUT)	//y el modo del pin7 de port A es de salida
			portA->SetMode(7, Pin::AUTO);		//habilita el comparador 1 (pone bit7 de port A en modo auto)
	}
	else if (pactl->bit7())
		portA->SetMode(7, Pin::OUTPUT);			//si el bit7 de OC1M es 0, dependiendo del bit7 de PACTL pondrá el bit7 de port A
	else										//en modo de salida o entrada
		portA->SetMode(7, Pin::INPUT);

	
	if (bit6())
		portA->SetMode(6, Pin::AUTO);			//si el bit6 de OC1M es 1, pone el bit6 de port A en modo auto (comparador 1 enable)
	else if(!(tctl1->bit7() || tctl1->bit6()))	//sino, y si está desactivado el comparador 2, pone bit6 de port A como salida
		portA->SetMode(6, Pin::OUTPUT);


	if (bit5())
		portA->SetMode(5, Pin::AUTO);
	else if(!(tctl1->bit5() || tctl1->bit4()))
		portA->SetMode(5, Pin::OUTPUT);


	if (bit4())
		portA->SetMode(4, Pin::AUTO);
	else if(!(tctl1->bit3() || tctl1->bit2()))
		portA->SetMode(4, Pin::OUTPUT);


	if (bit3())
		portA->SetMode(3, Pin::AUTO);
	else if(!(tctl1->bit1() || tctl1->bit0()))
		portA->SetMode(3, Pin::OUTPUT);

}


/////////////////////////////////////////////////////////////////
// PACTL function overrides
//////////////////////////////////////////////////////////////////////////////

void RegisterFile::PACTL_C::Write(byte_t val)
{
	byte = val;
	
	if (!bit7())
		portA->SetMode(7, Pin::INPUT);		//si el bit7 de PACTL es cero, pone el pin 7 del puerto A como entrada
	
	else if (oc1m->bit7())
		portA->SetMode(7, Pin::AUTO);		//sino, si bit7 de oc1m es 1, el pin7 de port A es controlado por el comparador 1 (auto)
	else
		portA->SetMode(7, Pin::OUTPUT);		//sino, poner como pin de salida
}


/////////////////////////////////////////////////////////////////
// INIT function overrides
//////////////////////////////////////////////////////////////////////////////

void RegisterFile::INIT_C::Write(byte_t val)
{
	byte = val;
	
	byte_t temp = 0;
	
	if ((temp = byte<<4) != 0)
	{
		byte_t move_back = temp>>4;

		RegisterMap* pregmap = (RegisterMap*)regmap;	//regmap es el atributo, hace casting, ya que lo declaré como void*
														

		//pregmap apunta a regmap (de INIT_C) que apunta a regmap (de HC11)
		//todas son la misma, cambian (leen y escriben) la misma zona de memoria
			
		int new_base = move_back * 0x1000;
		
		pregmap->Move(new_base);        
	}
	
	if ((temp = byte>>4) != 0)
	{
		RAM* pram = (RAM*)chip_ram;
		int new_base = temp * 0x1000;
        pram->Move(new_base);
	}
}


///////////////////////////////////////////////////////////////////////////////////
// HPRIO
//////////////////////////////////////////////////////////////////////////////////

void RegisterFile::HPRIO_C::Write(byte_t val)		//no es necesario, es igual que la clase ByteRegister
{
    byte = val;
}


/////////////////////////////////////////////////////////////////
// TFLG1 and TFLG2
//////////////////////////////////////////////////////////////////////////////

//escribe en un registro TFLG, un bit es limpiado en este registro si es escrito un uno

void RegisterFile::TFLG_C::Write(byte_t mask)
{
	byte &= ~mask;	//es lo mismo que byte = byte & ~mask
}

	
/////////////////////////////////////////////////////////////////
// CFORC function overrides
//////////////////////////////////////////////////////////////////////////////

void RegisterFile::CFORC_C::Write(byte_t val)
{
	
	Byte b = val;
	
	//los bits siempre se leen como 0, nunca cambian el nivel. Aunque yo escriba un 1, realmente no se escribe nada
	//un 1 escrito en un bit activa el comparador de salida. Un 0 escrito no hace nada
	//Realiza la accion como si fuera una comparacion exitosa pero sin activar el flag

	if (byte != 0)		//byte es el atributo de Byte::ByteRegister::CFORC_C
		byte = 0;
	
	bit_t has_control2 = 0;
	bit_t has_control3 = 0;
	bit_t has_control4 = 0;
	bit_t has_control5 = 0;

	//b es un tipo Byte al que le paso val (valor con el que quiero escribir este registro)

	if (b.bit7())	// FOC1
	{
		
		if (oc1m->bit3())		//oc1m es el atributo de CFORC_C
		{
			portA->Action(3, oc1d->bit3());		//portA es el atributo de CFORC_C
			has_control5 = 1;
		}
		if (oc1m->bit4())
		{
			portA->Action(4, oc1d->bit4());
			has_control4 = 1;
		}
		if (oc1m->bit5())
		{
			portA->Action(5, oc1d->bit5());
			has_control3 = 1;
		}
		if (oc1m->bit6())
		{
			portA->Action(6, oc1d->bit6());
			has_control2 = 1;
		}
		if (oc1m->bit7())
			portA->Action(7, oc1d->bit7());
	}

	if (b.bit6() && !has_control2)	// FOC2, si quiero escribir 1 en bit6 (aunque no se escribe nada) y oc2 está libre
	{
		bit_t ol2 = tctl1->bit6();
		bit_t om2 = tctl1->bit7();
		
		switch((om2<<1)|ol2)
		{
			case 0x0:		// no afecta
				break;
			case 0x1:		// toggles pin
				portA->Action(6, !portA->Read(6));
				break;
			case 0x2:		// clears pin
				portA->Action(6, 0);
				break;
			case 0x3:		// sets pin
				portA->Action(6, 1);
				break;
			default:	assert(false);	//comprueba que la condicion es false
		}	
	}

	if (b.bit5() && !has_control3)	// FOC3
	{
		bit_t ol3 = tctl1->bit4();
		bit_t om3 = tctl1->bit5();
		
		switch((om3<<1)|ol3)
		{
			case 0x0:		// no afecta
				break;
			case 0x1:		// toggles pin
				portA->Action(5, !portA->Read(5));
				break;
			case 0x2:		// clears pin
				portA->Action(5, 0);
				break;
			case 0x3:		// sets pin
				portA->Action(5, 1);
				break;
			default:	assert(false);
		}
	}

	if (b.bit4() && !has_control4)	// FOC4
	{
		bit_t ol4 = tctl1->bit2();
		bit_t om4 = tctl1->bit3();
		
		switch((om4<<1)|ol4)
		{
			case 0x0:		// no afecta
				break;
			case 0x1:		// toggles pin
				portA->Action(4, !portA->Read(4));
				break;
			case 0x2:		// clears pin
				portA->Action(4, 0);
				break;
			case 0x3:		// sets pin
				portA->Action(4, 1);
				break;
			default:	assert(false);
		}
	}

	if (b.bit3() && !has_control5)	// FOC5
	{
		bit_t ol5 = tctl1->bit0();
		bit_t om5 = tctl1->bit1();
		
		switch((om5<<1)|ol5)
		{
			case 0x0:		// no afecta
				break;
			case 0x1:		// toggles pin
				portA->Action(3, !portA->Read(3));
				break;
			case 0x2:		// clears pin
				portA->Action(3, 0);
				break;
			case 0x3:		// sets pin
				portA->Action(3, 1);
				break;
			default:	assert(false);
		}
	}
}


/////////////////////////////////////////////////////////////////
// SPCR function overrides
//////////////////////////////////////////////////////////////////////////////

void RegisterFile::SPCR_C::Write(byte_t val)
{
	byte = val;
}

