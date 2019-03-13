#include "pin.h"
#include "pinConnection.h"


void Pin::Input(bit_t value) {
	if (mode == Pin::INPUT) {		//si el modo es de entrada, puede cambiar su valor, sino no
		level = value;
		if (connect != NULL)	
			connect->Set(level);	//actualiza valor, C7SegDlg::Set (se muestra el cambio en la interfaz)
	}
}


void Pin::Output(bit_t value) {
	if (mode == Pin::OUTPUT) {		//si el modo es de salida, puede cambiar su valor
		level=value;
		if (connect != NULL)		//si esta conectado, actualiza
			connect->Set(level);	//Set se implementa en la GUI (C7SegDlg::Set)
	}
}


void Pin::Action(bit_t value) {		
	if (mode == AUTO) {
		level = value;
		if (connect != NULL)
			connect->Set(level);
	}
}

