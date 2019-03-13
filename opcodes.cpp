#include "hc11.h"	//incluyo al general donde está incluido opcodes.h

#include "opbasic.h"	//para acceder a 5 funciones de manejo de flags: addbyte, addword, subbyte, subword, round


//las instrucciones se llaman desde el metodo HC11::Fetch(), el cual lee de la memoria el codigo que usa como indice 
//del array de funciones, (hay que inicializar el array como aquí se muestra) 
//así llama a las diferentes funciones que implementan la funcionalidad de cada instruccion

opcode_t HC11::opcode[N_OPCODES] =  {   
	HC11::op_0h,		
	HC11::op_1h,
	HC11::op_2h,
	HC11::op_3h,
	HC11::op_4h,
	HC11::op_5h,
	HC11::op_6h,
	HC11::op_7h,
	HC11::op_8h,
	HC11::op_9h,
	HC11::op_Ah,
	HC11::op_Bh,
	HC11::op_Ch,
	HC11::op_Dh,
	HC11::op_Eh,
	HC11::op_Fh,

	HC11::op_10h,		
	HC11::op_11h,
	HC11::op_12h,
	HC11::op_13h,
	HC11::op_14h,
	HC11::op_15h,
	HC11::op_16h,
	HC11::op_17h,
	HC11::op_18h,
	HC11::op_19h,
	HC11::op_1Ah,
	HC11::op_1Bh,
	HC11::op_1Ch,
	HC11::op_1Dh,
	HC11::op_1Eh,
	HC11::op_1Fh,

	HC11::op_20h,		
	HC11::op_21h,
	HC11::op_22h,
	HC11::op_23h,
	HC11::op_24h,
	HC11::op_25h,
	HC11::op_26h,
	HC11::op_27h,
	HC11::op_28h,
	HC11::op_29h,
	HC11::op_2Ah,
	HC11::op_2Bh,
	HC11::op_2Ch,
	HC11::op_2Dh,
	HC11::op_2Eh,
	HC11::op_2Fh,

	HC11::op_30h,		
	HC11::op_31h,
	HC11::op_32h,
	HC11::op_33h,
	HC11::op_34h,
	HC11::op_35h,
	HC11::op_36h,
	HC11::op_37h,
	HC11::op_38h,
	HC11::op_39h,
	HC11::op_3Ah,
	HC11::op_3Bh,
	HC11::op_3Ch,
	HC11::op_3Dh,
	HC11::op_3Eh,
	HC11::op_3Fh,

	HC11::op_40h,		
	op_illegal,	//	HC11::op_41h,  not valid opcode		no tiene instrucion asociada, hay 20 así
	op_illegal,	//	HC11::op_42h,  not valid opcode
	HC11::op_43h,
	HC11::op_44h,
	op_illegal,	//	HC11::op_45h,  not valid opcode
	HC11::op_46h,
	HC11::op_47h,
	HC11::op_48h,
	HC11::op_49h,
	HC11::op_4Ah,
	op_illegal,	//	HC11::op_4Bh,  not valid opcode
	HC11::op_4Ch,
	HC11::op_4Dh,
	op_illegal,	//	HC11::op_4Eh,  not valid opcode
	HC11::op_4Fh,

	HC11::op_50h,		
	op_illegal,	//	HC11::op_51h,  not valid opcode
	op_illegal,	//	HC11::op_52h,  not valid opcode
	HC11::op_53h,
	HC11::op_54h,
	op_illegal,	//	HC11::op_55h,  not valid opcode
	HC11::op_56h,
	HC11::op_57h,
	HC11::op_58h,
	HC11::op_59h,
	HC11::op_5Ah,
	op_illegal,	//	HC11::op_5Bh,  not valid opcode
	HC11::op_5Ch,
	HC11::op_5Dh,
	op_illegal,	//	HC11::op_5Eh,  not valid opcode
	HC11::op_5Fh,

	HC11::op_60h,		
	op_illegal,	//	HC11::op_61h,  not valid opcode
	op_illegal,	//	HC11::op_62h,  not valid opcode
	HC11::op_63h,
	HC11::op_64h,
	op_illegal,	//	HC11::op_65h,  not valid opcode
	HC11::op_66h,
	HC11::op_67h,
	HC11::op_68h,
	HC11::op_69h,
	HC11::op_6Ah,
	op_illegal,	//	HC11::op_6Bh,
	HC11::op_6Ch,
	HC11::op_6Dh,
	HC11::op_6Eh,
	HC11::op_6Fh,

	HC11::op_70h,		
	op_illegal,	//	HC11::op_71h,  not valid opcode
	op_illegal,	//	HC11::op_72h,  not valid opcode
	HC11::op_73h,
	HC11::op_74h,
	op_illegal,	//	HC11::op_75h,  not valid opcode
	HC11::op_76h,
	HC11::op_77h,
	HC11::op_78h,
	HC11::op_79h,
	HC11::op_7Ah,
	op_illegal, //	HC11::op_7Bh,  not valid opcode
	HC11::op_7Ch,
    HC11::op_7Dh,
	HC11::op_7Eh,
	HC11::op_7Fh,

	HC11::op_80h,		
    HC11::op_81h,
	HC11::op_82h,
	HC11::op_83h,
	HC11::op_84h,
	HC11::op_85h,
	HC11::op_86h,
	op_illegal, //	HC11::op_87h,  not valid opcode
	HC11::op_88h,
	HC11::op_89h,
	HC11::op_8Ah,
	HC11::op_8Bh,
    HC11::op_8Ch,
	HC11::op_8Dh,
	HC11::op_8Eh,
    HC11::op_8Fh,

	HC11::op_90h,		
    HC11::op_91h,
	HC11::op_92h,
	HC11::op_93h,
	HC11::op_94h,
	HC11::op_95h,
	HC11::op_96h,
	HC11::op_97h,
	HC11::op_98h,
	HC11::op_99h,
	HC11::op_9Ah,
	HC11::op_9Bh,
    HC11::op_9Ch,
	HC11::op_9Dh,
	HC11::op_9Eh,
	HC11::op_9Fh,

	HC11::op_A0h,		
    HC11::op_A1h,
	HC11::op_A2h,
	HC11::op_A3h,
	HC11::op_A4h,
	HC11::op_A5h,
	HC11::op_A6h,
	HC11::op_A7h,
	HC11::op_A8h,
	HC11::op_A9h,
	HC11::op_AAh,
	HC11::op_ABh,
	HC11::op_ACh,
	HC11::op_ADh,
	HC11::op_AEh,
	HC11::op_AFh,

	HC11::op_B0h,		
    HC11::op_B1h,
	HC11::op_B2h,
	HC11::op_B3h,
	HC11::op_B4h,
	HC11::op_B5h,
	HC11::op_B6h,
	HC11::op_B7h,
	HC11::op_B8h,
	HC11::op_B9h,
	HC11::op_BAh,
	HC11::op_BBh,
    HC11::op_BCh,
	HC11::op_BDh,
	HC11::op_BEh,
	HC11::op_BFh,

	HC11::op_C0h,		
    HC11::op_C1h,
	HC11::op_C2h,	
    HC11::op_C3h,
	HC11::op_C4h,
	HC11::op_C5h,
	HC11::op_C6h,
	op_illegal, //	HC11::op_C7h,
	HC11::op_C8h,
	HC11::op_C9h,
	HC11::op_CAh,
	HC11::op_CBh,
	HC11::op_CCh,
	HC11::op_CDh,
	HC11::op_CEh,
    HC11::op_CFh,

	HC11::op_D0h,		
    HC11::op_D1h,
	HC11::op_D2h,
	HC11::op_D3h,
	HC11::op_D4h,
	HC11::op_D5h,
	HC11::op_D6h,
    HC11::op_D7h,
	HC11::op_D8h,
	HC11::op_D9h,
	HC11::op_DAh,
	HC11::op_DBh,
	HC11::op_DCh,
	HC11::op_DDh,
	HC11::op_DEh,
    HC11::op_DFh,

	HC11::op_E0h,		
    HC11::op_E1h,
	HC11::op_E2h,
	HC11::op_E3h,
	HC11::op_E4h,
	HC11::op_E5h,
	HC11::op_E6h,
	HC11::op_E7h,
	HC11::op_E8h,
	HC11::op_E9h,
	HC11::op_EAh,
	HC11::op_EBh,
	HC11::op_ECh,
	HC11::op_EDh,
	HC11::op_EEh,
	HC11::op_EFh,

	HC11::op_F0h,		
    HC11::op_F1h,
	HC11::op_F2h,
	HC11::op_F3h,
	HC11::op_F4h,
	HC11::op_F5h,
	HC11::op_F6h,
	HC11::op_F7h,
	HC11::op_F8h,
	HC11::op_F9h,
	HC11::op_FAh,
	HC11::op_FBh,
	HC11::op_FCh,
	HC11::op_FDh,
	HC11::op_FEh,
	HC11::op_FFh
};	





//array de 256, pero solo para las 64 instrucciones extendidas que empiezan por 0x18
//aqui se vé el codigo del segundo operando

opcode_t HC11::opIYcode[N_OPCODES] =  { 		
	op_illegal, //	HC11::opIY_0h,		
    op_illegal, //	HC11::opIY_1h,
	op_illegal, //	HC11::opIY_2h,
	op_illegal, //	HC11::opIY_3h,
	op_illegal, //	HC11::opIY_4h,
	op_illegal, //	HC11::opIY_5h,
	op_illegal, //	HC11::opIY_6h,
	op_illegal, //  HC11::opIY_7h,
	HC11::opIY_8h,
	HC11::opIY_9h,
	op_illegal, //	HC11::opIY_Ah,
	op_illegal, //	HC11::opIY_Bh,
	op_illegal, //	HC11::opIY_Ch,
	op_illegal, //	HC11::opIY_Dh,
	op_illegal, //	HC11::opIY_Eh,
	op_illegal, //	HC11::opIY_Fh,

	op_illegal, //	HC11::opIY_10h,		
    op_illegal, //	HC11::opIY_11h,
	op_illegal, //	HC11::opIY_12h,
	op_illegal, //	HC11::opIY_13h,
	op_illegal, //	HC11::opIY_14h,
	op_illegal, //	HC11::opIY_15h,
	op_illegal, //	HC11::opIY_16h,
	op_illegal, //	HC11::opIY_17h,
	op_illegal, //	HC11::opIY_18h,
	op_illegal, //	HC11::opIY_19h,
	op_illegal, //	HC11::opIY_1Ah,
	op_illegal, //	HC11::opIY_1Bh,
	HC11::opIY_1Ch,
	HC11::opIY_1Dh,
	HC11::opIY_1Eh,
	HC11::opIY_1Fh,

	op_illegal, //	HC11::opIY_20h,		
    op_illegal, //	HC11::opIY_21h,
	op_illegal, //	HC11::opIY_22h,
	op_illegal, //	HC11::opIY_23h,
	op_illegal, //	HC11::opIY_24h,
	op_illegal, //	HC11::opIY_25h,
	op_illegal, //	HC11::opIY_26h,
	op_illegal, //	HC11::opIY_27h,
	op_illegal, //	HC11::opIY_28h,
	op_illegal, //	HC11::opIY_29h,
	op_illegal, //	HC11::opIY_2Ah,
	op_illegal, //	HC11::opIY_2Bh,
	op_illegal, //	HC11::opIY_2Ch,
	op_illegal, //	HC11::opIY_2Dh,
	op_illegal, //	HC11::opIY_2Eh,
	op_illegal, //	HC11::opIY_2Fh,

    HC11::opIY_30h,		
    op_illegal, //	HC11::opIY_31h,
	op_illegal, //	HC11::opIY_32h,
	op_illegal, //	HC11::opIY_33h,
	op_illegal, //	HC11::opIY_34h,
    HC11::opIY_35h,
	op_illegal, //	HC11::opIY_36h,
	op_illegal, //	HC11::opIY_37h,
	HC11::opIY_38h,
	op_illegal, //	HC11::opIY_39h,
	HC11::opIY_3Ah,
	op_illegal, //	HC11::opIY_3Bh,
	HC11::opIY_3Ch,
	op_illegal, //	HC11::opIY_3Dh,
	op_illegal, //	HC11::opIY_3Eh,
	op_illegal, //	HC11::opIY_3Fh,

	op_illegal, //	HC11::opIY_40h,		
    op_illegal, //	HC11::opIY_41h,
	op_illegal, //	HC11::opIY_42h,
	op_illegal, //	HC11::opIY_43h,
	op_illegal, //	HC11::opIY_44h,
	op_illegal, //	HC11::opIY_45h,
	op_illegal, //	HC11::opIY_46h,
	op_illegal, //	HC11::opIY_47h,
	op_illegal, //	HC11::opIY_48h,
	op_illegal, //	HC11::opIY_49h,
	op_illegal, //	HC11::opIY_4Ah,
	op_illegal, //	HC11::opIY_4Bh,
	op_illegal, //	HC11::opIY_4Ch,
	op_illegal, //	HC11::opIY_4Dh,
	op_illegal, //	HC11::opIY_4Eh,
	op_illegal, //	HC11::opIY_4Fh,

	op_illegal, //	HC11::opIY_50h,		
    op_illegal, //	HC11::opIY_51h,
	op_illegal, //	HC11::opIY_52h,
	op_illegal, //	HC11::opIY_53h,
	op_illegal, //	HC11::opIY_54h,
	op_illegal, //	HC11::opIY_55h,
	op_illegal, //	HC11::opIY_56h,
	op_illegal, //	HC11::opIY_57h,
	op_illegal, //	HC11::opIY_58h,
	op_illegal, //	HC11::opIY_59h,
	op_illegal, //	HC11::opIY_5Ah,
	op_illegal, //	HC11::opIY_5Bh,
	op_illegal, //	HC11::opIY_5Ch,
	op_illegal, //	HC11::opIY_5Dh,
	op_illegal, //	HC11::opIY_5Eh,
	op_illegal, //	HC11::opIY_5Fh,

	HC11::opIY_60h,		
    op_illegal, //	HC11::opIY_61h,
	op_illegal, //	HC11::opIY_62h,
	HC11::opIY_63h,
	HC11::opIY_64h,
	op_illegal, //	HC11::opIY_65h,
	HC11::opIY_66h,
	HC11::opIY_67h,
	HC11::opIY_68h,
	HC11::opIY_69h,
	HC11::opIY_6Ah,
	op_illegal, //	HC11::opIY_6Bh,
	HC11::opIY_6Ch,
	HC11::opIY_6Dh,
	HC11::opIY_6Eh,
	HC11::opIY_6Fh,

	op_illegal, //	HC11::opIY_70h,		
    op_illegal, //	HC11::opIY_71h,
	op_illegal, //	HC11::opIY_72h,
	op_illegal, //	HC11::opIY_73h,
	op_illegal, //	HC11::opIY_74h,
	op_illegal, //	HC11::opIY_75h,
	op_illegal, //	HC11::opIY_76h,
	op_illegal, //	HC11::opIY_77h,
	op_illegal, //	HC11::opIY_78h,
	op_illegal, //	HC11::opIY_79h,
	op_illegal, //	HC11::opIY_7Ah,
	op_illegal, //	HC11::opIY_7Bh,
	op_illegal, //	HC11::opIY_7Ch,
	op_illegal, //	HC11::opIY_7Dh,
	op_illegal, //	HC11::opIY_7Eh,
	op_illegal, //	HC11::opIY_7Fh,

	op_illegal, //	HC11::opIY_80h,		
    op_illegal, //	HC11::opIY_81h,
	op_illegal, //	HC11::opIY_82h,
	op_illegal, //	HC11::opIY_83h,
	op_illegal, //	HC11::opIY_84h,
	op_illegal, //	HC11::opIY_85h,
	op_illegal, //	HC11::opIY_86h,
	op_illegal, //	HC11::opIY_87h,
	op_illegal, //	HC11::opIY_88h,
	op_illegal, //	HC11::opIY_89h,
	op_illegal, //	HC11::opIY_8Ah,
	op_illegal, //	HC11::opIY_8Bh,
    HC11::opIY_8Ch,
	op_illegal, //	HC11::opIY_8Dh,
	op_illegal, //	HC11::opIY_8Eh,
	HC11::opIY_8Fh,

	op_illegal, //	HC11::opIY_90h,		
    op_illegal, //	HC11::opIY_91h,
	op_illegal, //	HC11::opIY_92h,
	op_illegal, //	HC11::opIY_93h,
	op_illegal, //	HC11::opIY_94h,
	op_illegal, //	HC11::opIY_95h,
	op_illegal, //	HC11::opIY_96h,
	op_illegal, //	HC11::opIY_97h,
	op_illegal, //	HC11::opIY_98h,
	op_illegal, //	HC11::opIY_99h,
	op_illegal, //	HC11::opIY_9Ah,
	op_illegal, //	HC11::opIY_9Bh,
    HC11::opIY_9Ch,
	op_illegal, //	HC11::opIY_9Dh,
	op_illegal, //	HC11::opIY_9Eh,
	op_illegal, //	HC11::opIY_9Fh,

	HC11::opIY_A0h,		
    HC11::opIY_A1h,
	//op_illegal, //	HC11::opIY_A2h,	DUDA, creo k si es extendido y aki no aparece, SBCA
	HC11::opIY_A2h,
	HC11::opIY_A3h,
	HC11::opIY_A4h,
	HC11::opIY_A5h,
	HC11::opIY_A6h,
    HC11::opIY_A7h,
	HC11::opIY_A8h,
	HC11::opIY_A9h,
	HC11::opIY_AAh,
	HC11::opIY_ABh,
    HC11::opIY_ACh,
	HC11::opIY_ADh,
	HC11::opIY_AEh,
    HC11::opIY_AFh,

	op_illegal, //	HC11::opIY_B0h,		
    op_illegal, //	HC11::opIY_B1h,
	op_illegal, //	HC11::opIY_B2h,
	op_illegal, //	HC11::opIY_B3h,
	op_illegal, //	HC11::opIY_B4h,
	op_illegal, //	HC11::opIY_B5h,
	op_illegal, //	HC11::opIY_B6h,
	op_illegal, //	HC11::opIY_B7h,
	op_illegal, //	HC11::opIY_B8h,
	op_illegal, //	HC11::opIY_B9h,
	op_illegal, //	HC11::opIY_BAh,
	op_illegal, //	HC11::opIY_BBh,
    HC11::opIY_BCh,
	op_illegal, //	HC11::opIY_BDh,
	op_illegal, //	HC11::opIY_BEh,
	op_illegal, //	HC11::opIY_BFh,

	op_illegal, //	HC11::opIY_C0h,		
    op_illegal, //	HC11::opIY_C1h,
	op_illegal, //	HC11::opIY_C2h,
	op_illegal, //	HC11::opIY_C3h,
	op_illegal, //	HC11::opIY_C4h,
	op_illegal, //	HC11::opIY_C5h,
	op_illegal, //	HC11::opIY_C6h,
	op_illegal, //	HC11::opIY_C7h,
	op_illegal, //	HC11::opIY_C8h,
	op_illegal, //	HC11::opIY_C9h,
	op_illegal, //	HC11::opIY_CAh,
	op_illegal, //	HC11::opIY_CBh,
	op_illegal, //	HC11::opIY_CCh,
	op_illegal, //	HC11::opIY_CDh,
	HC11::opIY_CEh,
	op_illegal, //	HC11::opIY_CFh,

	op_illegal, //	HC11::opIY_D0h,		
    op_illegal, //	HC11::opIY_D1h,
	op_illegal, //	HC11::opIY_D2h,
	op_illegal, //	HC11::opIY_D3h,
	op_illegal, //	HC11::opIY_D4h,
	op_illegal, //	HC11::opIY_D5h,
	op_illegal, //	HC11::opIY_D6h,
	op_illegal, //	HC11::opIY_D7h,
	op_illegal, //	HC11::opIY_D8h,
	op_illegal, //	HC11::opIY_D9h,
	op_illegal, //	HC11::opIY_DAh,
	op_illegal, //	HC11::opIY_DBh,
	op_illegal, //	HC11::opIY_DCh,
	op_illegal, //	HC11::opIY_DDh,
	HC11::opIY_DEh,
	HC11::opIY_DFh,

	HC11::opIY_E0h,		
    HC11::opIY_E1h,
	//op_illegal, //	HC11::opIY_E2h, es extendido y no ilegal, no aparece, SBCB
	HC11::opIY_E2h,
	HC11::opIY_E3h,
	HC11::opIY_E4h,
	HC11::opIY_E5h,
	HC11::opIY_E6h,
    HC11::opIY_E7h,
	HC11::opIY_E8h,
	HC11::opIY_E9h,
	HC11::opIY_EAh,
	HC11::opIY_EBh,
	HC11::opIY_ECh,
    HC11::opIY_EDh,
	HC11::opIY_EEh,
	HC11::opIY_EFh,

	op_illegal, //  HC11::opIY_F0h,		
    op_illegal, //	HC11::opIY_F1h,
	op_illegal, //	HC11::opIY_F2h,
	op_illegal, //	HC11::opIY_F3h,
	op_illegal, //	HC11::opIY_F4h,
	op_illegal, //	HC11::opIY_F5h,
	op_illegal, //	HC11::opIY_F6h,
	op_illegal, //	HC11::opIY_F7h,
	op_illegal, //	HC11::opIY_F8h,
	op_illegal, //	HC11::opIY_F9h,
	op_illegal, //	HC11::opIY_FAh,
	op_illegal, //	HC11::opIY_FBh,
	op_illegal, //	HC11::opIY_FCh,
	op_illegal, //	HC11::opIY_FDh,
	HC11::opIY_FEh,
	HC11::opIY_FFh
};	






/////////////////////////////////////////////////
//
//  op_illegal - the opcode that is called when an
//               opcode is called that does not exist. 
//
/////////////////////////////////////////////////

void HC11::op_illegal(HC11 *hc11)
{
	log("illegal opcode 0x%X!\n", (int)hc11->memory[hc11->PC-1]);
  
  // Cause an interrupt!
  // Decrement the PC before stacking the interrupt
  hc11->PC--;
  hc11->FetchVector(IllegalVector);
}



//entrada para las extendidas 0x18

/////////////////////////////////////////////////
//
//  Opcodes that deal with IY (they always start
//  with 18h then the opcode).  This function gets
//  the next opcode, then calls the right function.
//  
//  Opcode: 18h
//          24d 
//
////////////////////////////////////////////////

void HC11::op_18h(HC11 *hc11) {

	byte_t num;

	hc11->_clock();
    num = hc11->memory[hc11->PC];	//obtiene el segundo operando del opcode
    hc11->PC++;

	hc11->opIYcode[num](hc11);    //call the function needed. (hc11) es el parametro de la funcion opIYcode[num]
    
}              


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//las que faltan estan en opcodes2.cpp


/////////////////////////////////////////////////
//
//  IDIV - Divides ACCD by IX.  The quotient goes into
//         IX, and the remander goes into ACCD
//  
//  Addr. Mode: Inherent
//  
//  Opcode: 02h
//          02d        
//
//  Clocks: 41  (-1 for fetching opcode)
//
//  OpLength: 1 (-1 for fetching opcode)
//
//  Flags:  Z, V=0, C
//
////////////////////////////////////////////////

void HC11::op_2h(HC11 *hc11)
{
	int i;
	Word ACCD;
	Word tempIX;	//guarda el cociente temporal

	if(hc11->IX==0)  //divide by zero, el denominador es 0
	{
		hc11->CCR.C(1);	//si el denominador es 0: carry es 1, el cociente $FFFF y el resto indeterminado
		hc11->CCR.Z(0);	//como el resultado es diferente de 0, el flag zero =0
		hc11->CCR.V(0);	//V siempre 0
		hc11->IX=0xFFFF;
	}
	else
	{
		ACCD.low(hc11->ACCB);
		ACCD.high(hc11->ACCA);

		tempIX = (unsigned int)(ACCD/(hc11->IX));	//cociente temporal = D/X
		ACCD = (ACCD%(hc11->IX));		//resto

		hc11->IX=tempIX;		//guarda cociente temporal en X
		hc11->ACCB=ACCD.low();	//guarda parte baja del resto en B
		hc11->ACCA=ACCD.high();	//guarda parte alta del resto en A

		hc11->CCR.C(0);		//flags: carry es 0, ya que solo es 1 si el denominador es 0
		hc11->CCR.V(0);		//V siempre 0
		if(hc11->IX==0)		//flag zero depende de si el cociente es 0
			hc11->CCR.Z(1);
		else
			hc11->CCR.Z(0);
	}

	for(i=2; i<=41; i++)	//ciclos consumidos, de 1 a 41, pero el primero se consume al traer el opcode
		hc11->_clock();
}


/////////////////////////////////////////////////
//
//  FDIV - Fractionally Divides ACCD by IX.  The 
//		   quotient goes into IX, and the remander
//         goes into ACCD.  The numerator must be
//         less than the denominator.
//  
//  Addr. Mode: Inherent
//  
//  Opcode: 03h
//          03d        
//
//  Clocks: 41  (-1 for fetching opcode)
//
//  OpLength: 1 (-1 for fetching opcode)
//
//  Flags:  Z, V, C
//
////////////////////////////////////////////////

void HC11::op_3h(HC11 *hc11)
{
	int i;
	dword_t ACCD;	//de 32 bits
	float result;

	ACCD=hc11->ACCB;
	ACCD=ACCD + (hc11->ACCA << 8);

	if(hc11->IX==0)  //divide by zero, denominador es 0
	{
		hc11->CCR.C(1);
		hc11->CCR.Z(0);
		hc11->CCR.V(0);
		hc11->IX=0xFFFF;
	}
	else if(ACCD >= hc11->IX)	//el numerador debe ser menor k el denominador, sino igual k arriba pero carry 0 y V 1
	{
		hc11->CCR.C(0);
		hc11->CCR.Z(0);
		hc11->CCR.V(1);
		hc11->IX=0xFFFF;
	}
	else
	{
		ACCD=ACCD << 16;	//desplazo a la parte alta del registro de 32bits (es como multiplicar por 2^16)
		result = (float)ACCD/(float)hc11->IX;	//hago la division del reg 32 / reg 16
		ACCD=(ACCD%(hc11->IX));

		hc11->IX=round(result/0.000015258255 + 0.000015);	//escala: $0001 corresponde a 0.000015 (2^-16), $ffff a 0.99998		
		hc11->ACCB=(word_t)ACCD & 0xFFFF;	//resto: de los 16bits de abajo, solo coge la parte baja
		hc11->ACCA=(word_t)ACCD >> 16;		//de los 16 de arriba, solo coge la parte baja

		hc11->CCR.C(0);		//carry
		hc11->CCR.V(0);		//overflow
		if(hc11->IX==0)		//zero
			hc11->CCR.Z(1);
		else
			hc11->CCR.Z(0);
	}

	for(i=2; i<=41; i++)		//consume 41 ciclos, el primero en el fetch
		hc11->_clock();
}


/////////////////////////////////////////////////
//
//  LSRD - Logical shift right ACCD
//
//  Addr. Mode: Inherent
//  
//  Opcode: 04h
//          04d        
//
//  Clocks: 3  (-1 for fetching opcode)
//
//  OpLength: 1 (-1 for fetching opcode)
//
//  Flags:  N=0, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_4h(HC11 *hc11)
{
	byte_t carry_ACCA;
	byte_t carry_ACCB;
	
	carry_ACCA = hc11->ACCA & 1;	//coge el LSB de A para sumarselo a B (antes de desplazar)
	carry_ACCB = hc11->ACCB & 1;    //coge el LSB de B para meterlo en el carry (antes de desplazar)

	hc11->ACCA = hc11->ACCA >> 1;	//desplazo A a la derecha 1 posicion
	hc11->ACCB = hc11->ACCB >> 1;	//desplazo B a la derecha 1 posicion
	hc11->_clock();
	
	hc11->ACCB = hc11->ACCB | carry_ACCA << 7;	//al B hay que sumarle bit a bit (solo en bit 7) el carry de A
	

	hc11->CCR.C(carry_ACCB);	//carry
	hc11->CCR.V(carry_ACCB);	//overflow, se activa si despues del desplazamiento C vale 1, por tanto la condicion es igual a la de C
	hc11->CCR.N(0);				//negative

	if(hc11->ACCA==0 && hc11->ACCB==0)	//zero
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);

	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  ASLD - Shifts all bits to the left.  The MSB shifts			mismo opcode que LSLD
//         into the carry flag, and a zero shifts into LSB.
//  
//  Addr. Mode: Inherent
//  
//  Opcode: 05h
//          05d        
//
//  Clocks: 3  (-1 for fetching opcode)
//
//  OpLength: 1 (-1 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_5h(HC11 *hc11)
{
	if(hc11->ACCA>=128)		//carry si el bit 7 es 1 (numero mayor de $80) antes de desplazar (es igual k mirar el bit 15 de ACCD)
		hc11->CCR.C(1);
	else
		hc11->CCR.C(0);

	hc11->ACCA = hc11->ACCA << 1;
	
	if(hc11->ACCB>=128)
		hc11->ACCA = hc11->ACCA | 0x01;		//si MSB de B es 1, le sumo 1 al A
	
	hc11->ACCB = hc11->ACCB << 1;


	if(hc11->ACCA==0 && hc11->ACCB==0)	//flag zero
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);
	
	if(hc11->ACCA>=128)		//flag Negative
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);

	if(hc11->CCR.N() ^ hc11->CCR.C())	//flag overflow: N exor C
		hc11->CCR.V(1);
	else
		hc11->CCR.V(0);


	hc11->_clock();
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  INX - Increment the X register
//  
//  Addr. Mode: Inherent
//  
//  Opcode: 08h
//          08d        
//
//  Clocks: 3  (-1 for fetching opcode)
//
//  OpLength: 1 (-1 for fetching opcode)
//
//  Flags:  Z
//
////////////////////////////////////////////////

void HC11::op_8h(HC11 *hc11)
{
	//CCRReg flag=0x04;
	
	// INX only affects the Z reg
	//hc11->IX=addword(hc11,hc11->IX,1,flag,0);		//se puede hacer de las 2 formas

	hc11->IX++;		//incrementa el registro IX

	if (hc11->IX == 0)		//si vale 0, activa el flag Z,
		hc11->CCR.Z(1);
	else					//sino, no
		hc11->CCR.Z(0);

	hc11->_clock();		//en total tarda 3 ciclos, estos 2 internos (de espera), mas el primero de fetch
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  DEX - Decrement the X register
//  
//  Addr. Mode: Inherent
//  
//  Opcode: 09h
//          09d        
//
//  Clocks: 3  (-1 for fetching opcode)
//
//  OpLength: 1 (-1 for fetching opcode)
//
//  Flags:  Z
//
////////////////////////////////////////////////

void HC11::op_9h(HC11 *hc11)
{
	CCRReg flag=0x04;
	
	hc11->IX = subword(hc11, hc11->IX, 1, flag);	//la funcion subword() ya tiene en cuenta los flags

	hc11->_clock();
	hc11->_clock();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
//
//  SBA - Subtracts ACCB from ACCA
//  
//  Addr. Mode: Inherent
//  
//  Opcode: 10h
//          16d        
//
//  Clocks: 2  (-1 for fetching opcode)
//
//  OpLength: 1 (-1 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_10h(HC11 *hc11)
{
	CCRReg flag=0x0F;
	
	hc11->ACCA = subbyte(hc11, hc11->ACCA, hc11->ACCB, flag);	//la funcion subbyte() ya tiene en cuenta los flags
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  CBA - Compares ACCA and ACCB
//  
//  Addr. Mode: Inherent
//  
//  Opcode: 11h
//          17d        
//
//  Clocks: 2  (-1 for fetching opcode)
//
//  OpLength: 1 (-1 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_11h(HC11 *hc11)
{
	CCRReg flag=0x0F;
	
	subbyte(hc11, hc11->ACCA, hc11->ACCB, flag);	//A-B, la funcion subbyte() ya tiene en cuenta los flags
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  BRSET - Branches mask bits are set, salta si index & mascara=1
//
//  Addr. Mode: Direct
//  
//  Opcode: 12h
//          18d 
//
//  Clocks: 6  (-1 for fetching opcode) 
//
//  OpLength: 4 (-1 for fetching opcode)
//
//  Flags:  None
//
////////////////////////////////////////////////

void HC11::op_12h(HC11 *hc11)
{
	signed char offset;
	Word mem;

	mem.low(hc11->memory[hc11->PC]);	//direccionamiento directo
	mem.high(0);
	hc11->PC++;
	hc11->_clock();	

	if( (~(hc11->memory[mem]) & hc11->memory[hc11->PC])==0 )	//~ es la inversa o complemento a 1	
	{
		hc11->PC++;
		offset = (signed)hc11->memory[hc11->PC];
		hc11->PC++;
	    hc11->PC = hc11->PC + offset;	//salto: cargo PC con valor k tenia + offset
    }
	else
	{
		hc11->PC++;		//el opcode ocupa 4 bytes: tengo k avanzar PC 4 veces, 1 en Fetch(), y 3 aqui
		hc11->PC++;
	}

	hc11->_clock();	
	hc11->_clock();	
	hc11->_clock();	
	hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  BRCLR - Branches if mask bits are clear, salta si index & mascara=0
//
//  Addr. Mode: Direct
//  
//  Opcode: 13h
//          19d 
//
//  Clocks: 6  (-1 for fetching opcode) 
//
//  OpLength: 4 (-1 for fetching opcode)
//
//  Flags:  None
//
////////////////////////////////////////////////

void HC11::op_13h(HC11 *hc11)
{
	signed char offset;
	Word mem;

	mem.low(hc11->memory[hc11->PC]);
	mem.high(0);
	hc11->PC++;
	hc11->_clock();	

	if( (hc11->memory[mem] & hc11->memory[hc11->PC])==0 )	
	{
		hc11->PC++;
		offset = (signed)hc11->memory[hc11->PC];
		hc11->PC++;
	    hc11->PC = hc11->PC + offset;	//salto: cargo PC con valor k tenia + offset
    }
	else
	{
		hc11->PC++;
		hc11->PC++;
	}

	hc11->_clock();	
	hc11->_clock();	
	hc11->_clock();	
	hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  BSET - Sets all the bits in the mask, pone a 1 los bits segun la mascara
//
//  Addr. Mode: Direct
//  
//  Opcode: 14h
//          20d 
//
//  Clocks: 6  (-1 for fetching opcode) 
//
//  OpLength: 3 (-1 for fetching opcode)
//
//  Flags:  N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_14h(HC11 *hc11)
{
    Word mem;
	Byte data;

	mem.low(hc11->memory[hc11->PC]);	//mem es un operando buscado con direccionamiento directo
	mem.high(0);
	hc11->PC++;
    hc11->_clock();
	
    hc11->_clock();	
	data = hc11->memory[mem];	
    data = data | hc11->memory[hc11->PC];	//or bit a bit: si en la mascara hay 1 pone 1 en data, si hay 0 deja data como estaba
    hc11->_clock();	
	hc11->PC++;	    

	if(data==0)			//flag zero
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);

	if(data>=128)		//flag negative
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);

	hc11->CCR.V(0);		//flag overflow

	hc11->memory.Write(mem, data);		//escribe data en la direccion mem
  
	
	hc11->_clock();		//he añadido un ciclo que faltaba
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  BCLR - Clears all the bits in the mask, pongo a 0 los bits k en la mascara estan a 1
//
//  Addr. Mode: Direct
//  
//  Opcode: 15h
//          21d 
//
//  Clocks: 6  (-1 for fetching opcode) 
//
//  OpLength: 3 (-1 for fetching opcode)
//
//  Flags:  N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_15h(HC11 *hc11)
{
    Word mem;
	Byte data;

	mem.low(hc11->memory[hc11->PC]);
	mem.high(0);
	hc11->PC++;
    hc11->_clock();
	
	data = hc11->memory[mem];
	data = data & ~(hc11->memory[hc11->PC]);
	hc11->PC++;
	
	if(data==0)			//flag zero
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);
	
	if(data>=128)		//flag negative
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);

	hc11->CCR.V(0);		//añadido


	hc11->memory.Write(mem, data);

    hc11->_clock();	
	hc11->_clock();		//faltaban 3 ciclos
	hc11->_clock();	
	hc11->_clock();	
}


//falta 16, 17, (la 18 no, está de las primeras, es entrada a instru ext)


/////////////////////////////////////////////////
//
//  DAA - Decimal Adjust ACCA, se usa justo despues de una suma
//  
//  Addr. Mode: Inherent
//  
//  Opcode: 19h
//          25d        
//
//  Clocks: 2  (-1 for fetching opcode)
//
//  OpLength: 1 (-1 for fetching opcode)
//
//  Flags:  N, Z, V=?, C
//
////////////////////////////////////////////////

void HC11::op_19h(HC11 *hc11)
{
	CCRReg flag=0x0D;	//flags afectados 0000 1101, para usar despues en la funcion addbyte()
	byte_t low;
	byte_t high;
	byte_t num;
	byte_t cstate;

	low = hc11->ACCA & 0x0F;	//cojo los 4 bits de la parte baja (antes estaba mal, hacia mascara con 0xE)
	high = hc11->ACCA >> 4;	//cojo 4 bits de la parte alta
	
	if(hc11->CCR.C()==0)	//C=0
	{
		if(high<=9 && hc11->CCR.H()==0 && low<=9)		//mirar tabla del manual de motorola para ver todas las condiciones
		{
			num=0x00;
			cstate=0;
		}
		else if(high<=8 && hc11->CCR.H()==0 && low>=10)
		{
			num=0x06;
			cstate=0;
		}
		else if(high<=9 && hc11->CCR.H()==1 && low<=3)
		{
			num=0x06;
			cstate=0;
		}
		else if(high>=10 && hc11->CCR.H()==0 && low<=9)
		{
			num=0x60;
			cstate=1;
		}
		else if(high>=9 && hc11->CCR.H()==0 && low>=10)
		{
			num=0x66;
			cstate=1;
		}
		else if(high>=10 && hc11->CCR.H()==1 && low<=3)		//estaba mal, corregido, ponia <=10
		{
			num=0x66;
			cstate=1;
		}
		else
			assert(0);	//comprueba k la condicion es falsa, k no se dá ninguna de las anteriores
	}
	else					//C=1
	{
		if(high<=2 && hc11->CCR.H()==0 && low<=9)
		{
			num=0x60;
			cstate=1;
		}
		else if(high<=2 && hc11->CCR.H()==0 && low>=10)
		{
			num=0x66;
			cstate=1;
		}
		else if(high<=3 && hc11->CCR.H()==1 && low<=3)
		{
			num=0x66;
			cstate=1;
		}
		else
			assert(0);	//comprueba k la condicion es falsa, k no se dá ninguna de las anteriores
	}

	hc11->ACCA = addbyte(hc11, hc11->ACCA, num, flag, 0);	//hace la suma y cambia los flags necesarios, pero el Carry lo deja mal
	hc11->CCR.C(cstate);					//arregla el flag de carry
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  ABA - Adds ACCB to ACCA and place result in ACCA
//
//  Addr. Mode: Inherent
//  
//  Opcode: 1Bh
//          27d 
//
//  Clocks: 2  (-1 for fetching opcode) 
//
//  OpLength: 1 (-1 for fetching opcode)
//
//  Flags:  H, N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_1Bh(HC11 *hc11)
{
	CCRReg flag=0x2F;
	
	hc11->ACCA = addbyte(hc11, hc11->ACCA, hc11->ACCB, flag, 0);     //A=A+B
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  BSET - Sets all the bits in the mask
//
//  Addr. Mode: Indirect X, mal es indexado en x
//  
//  Opcode: 1Ch
//          28d 
//
//  Clocks: 7  (-1 for fetching opcode) 
//
//  OpLength: 3 (-1 for fetching opcode)
//
//  Flags:  N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_1Ch(HC11 *hc11)
{
    Word mem;
	Byte data;

	mem = (hc11->IX + hc11->memory[hc11->PC]);	//direccionamiento indexado en IX
	hc11->PC++;
    hc11->_clock();

	data = hc11->memory[mem];
    hc11->_clock();

	data = data | hc11->memory[hc11->PC];	//hace la mascara
	hc11->PC++;
    hc11->_clock();
	
	if(data==0)		//flag zero
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);
    
	hc11->_clock();

	if(data>=128)	//flag negative
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);
    
	hc11->_clock();

	hc11->CCR.V(0);	//flag overflow

	hc11->memory.Write(mem, data);
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  BCLR - Clears all the bits in the mask
//
//  Addr. Mode: Indirect X, mal, es indexado en X
//  
//  Opcode: 1Dh
//          29d 
//
//  Clocks: 7  (-1 for fetching opcode) 
//
//  OpLength: 3 (-1 for fetching opcode)
//
//  Flags:  N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_1Dh(HC11 *hc11)
{
    Word mem;
	Byte data;

	mem=(hc11->IX + hc11->memory[hc11->PC]);	//direccionamiento indexado en IX
	hc11->PC++;
    hc11->_clock();

	data=hc11->memory[mem];
    hc11->_clock();

	data=data & ~(hc11->memory[hc11->PC]);		//hace & con el inverso de la mascara
	hc11->PC++;
    hc11->_clock();
	
	if(data==0)			//flag zero
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);
    
	hc11->_clock();

	if(data>=128)		//flag negative
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);

	hc11->CCR.V(0);		//añadido
    
	hc11->_clock();

	hc11->memory.Write(mem, data);
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  BRSET - Branches mask bits are set
//
//  Addr. Mode: indirect X, mal es indexado en IX
//  
//  Opcode: 1Eh
//          14d 
//
//  Clocks: 7  (-1 for fetching opcode) 
//
//  OpLength: 4 (-1 for fetching opcode)
//
//  Flags:  None
//
////////////////////////////////////////////////

void HC11::op_1Eh(HC11 *hc11)
{
	signed char offset;
	Word mem;

	mem = (hc11->IX + hc11->memory[hc11->PC]);		//igual que antes pero ahora con direccionamiento indexado en X
	hc11->PC++;
	hc11->_clock();	

	if( (~(hc11->memory[mem]) & hc11->memory[hc11->PC])==0 )	//~ es la inversa o complemento a 1
	{
		hc11->PC++;
		offset = (signed)hc11->memory[hc11->PC];
		hc11->PC++;
	    hc11->PC = hc11->PC + offset;
    }
	else
	{
		hc11->PC++;
		hc11->PC++;
	}

	hc11->_clock();	
	hc11->_clock();	
	hc11->_clock();	
	hc11->_clock();	
	hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  BRCLR - Branches if mask bits are clear
//
//  Addr. Mode: indirect X, indexado en x
//  
//  Opcode: 1Fh
//          15d 
//
//  Clocks: 7  (-1 for fetching opcode) 
//
//  OpLength: 4 (-1 for fetching opcode)
//
//  Flags:  None
//
////////////////////////////////////////////////

void HC11::op_1Fh(HC11 *hc11)
{
	signed char offset;
	Word mem;

	mem = (hc11->IX + hc11->memory[hc11->PC]);		//direccionamiento indexado en X
	hc11->PC++;
	hc11->_clock();	

	if((hc11->memory[mem] & hc11->memory[hc11->PC])==0)	
	{
		hc11->PC++;
		offset = (signed)hc11->memory[hc11->PC];
		hc11->PC++;
	    hc11->PC = hc11->PC + offset;
    }
	else
	{
		hc11->PC++;
		hc11->PC++;
	}

	hc11->_clock();	
	hc11->_clock();	
	hc11->_clock();	
	hc11->_clock();	
    hc11->_clock();	
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//falta BRA 20 (está en opcodes2.cpp), hasta la 2F revisadas


/////////////////////////////////////////////////
//
//  BRN - Branches never (2 cycle NOP)
//
//  Addr. Mode: Relative
//  
//  Opcode: 21h
//          33d 
//
//  Clocks: 3  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  None
//
////////////////////////////////////////////////

void HC11::op_21h(HC11 *hc11)
{
 	hc11->PC++;
	hc11->_clock();	

    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  BHI - Branches if higher
//
//  Addr. Mode: Relative
//  
//  Opcode: 22h
//          34d 
//
//  Clocks: 3  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  None
//
////////////////////////////////////////////////

void HC11::op_22h(HC11 *hc11)
{
    signed char offset;
 	
	if((hc11->CCR.Z() | hc11->CCR.C()) == 0)		//?C+Z=0
	{
		offset = (signed)hc11->memory[hc11->PC];
		hc11->PC++;
	    hc11->PC = hc11->PC + offset;
    }
	else
		hc11->PC++;

	hc11->_clock();	

    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  BLS - Branches if lower or same
//
//  Addr. Mode: Relative
//  
//  Opcode: 23h
//          35d 
//
//  Clocks: 3  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  None
//
////////////////////////////////////////////////

void HC11::op_23h(HC11 *hc11)
{
    signed char offset;
 	
	if((hc11->CCR.Z() | hc11->CCR.C()) == 1)	//?C+Z=1
	{
		offset = (signed)hc11->memory[hc11->PC];
		hc11->PC++;
	    hc11->PC = hc11->PC + offset;
    }
	else
		hc11->PC++;

	hc11->_clock();	

    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  BCC - Branches if the Carry flag is clear, mismo opcode que BHS
//
//  Addr. Mode: Relative
//  
//  Opcode: 24h
//          36d 
//
//  Clocks: 3  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  None
//
////////////////////////////////////////////////

void HC11::op_24h(HC11 *hc11)
{
    signed char offset;
 	
	if(hc11->CCR.C()==0)	//?C=0
	{
		offset = (signed)hc11->memory[hc11->PC];
		hc11->PC++;
	    hc11->PC = hc11->PC + offset;
    }
	else
		hc11->PC++;

	hc11->_clock();	

    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  BCS - Branches if the Carry flag is set, mismo opcode que BLO
//
//  Addr. Mode: Relative
//  
//  Opcode: 25h
//          37d 
//
//  Clocks: 3  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  None
//
////////////////////////////////////////////////

void HC11::op_25h(HC11 *hc11)
{
    signed char offset;
 	
	if(hc11->CCR.C()==1)	//?C=1
	{
		offset = (signed)hc11->memory[hc11->PC];
		hc11->PC++;
	    hc11->PC = hc11->PC + offset;
    }
	else
		hc11->PC++;
	
	hc11->_clock();	

    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  BNE - Branches if the Zero flag is clear, not equal
//
//  Addr. Mode: Relative
//  
//  Opcode: 26h
//          38d 
//
//  Clocks: 3  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  None
//
////////////////////////////////////////////////

void HC11::op_26h(HC11 *hc11)
{
    signed char offset;
 	
	if(hc11->CCR.Z()==0)	//?Z=0
	{
		offset = (signed)hc11->memory[hc11->PC];
		hc11->PC++;
	    hc11->PC = hc11->PC + offset;
    }
	else
		hc11->PC++;
	
	hc11->_clock();	

    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  BEQ - Branches if the Zero flag is set, branch if equal
//
//  Addr. Mode: Relative
//  
//  Opcode: 27h
//          39d 
//
//  Clocks: 3  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  None
//
////////////////////////////////////////////////

void HC11::op_27h(HC11 *hc11)
{
    signed char offset;
 	
	if(hc11->CCR.Z()==1)	//?Z=1
	{
		offset = (signed)hc11->memory[hc11->PC];
		hc11->PC++;
	    hc11->PC = hc11->PC + offset;
    }
	else
		hc11->PC++;

	hc11->_clock();	

    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  BVC - Branches if overflow clear
//
//  Addr. Mode: Relative
//  
//  Opcode: 28h
//          40d 
//
//  Clocks: 3  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  None
//
////////////////////////////////////////////////

void HC11::op_28h(HC11 *hc11)
{
    signed char offset;
 	
	if(hc11->CCR.V()==0)	//?V=0
	{
		offset = (signed)hc11->memory[hc11->PC];
		hc11->PC++;
	    hc11->PC = hc11->PC + offset;
    }
	else
		hc11->PC++;

	hc11->_clock();	

    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  BVS - Branches if the overflow flag is set
//
//  Addr. Mode: Relative
//  
//  Opcode: 29h
//          41d 
//
//  Clocks: 3  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  None
//
////////////////////////////////////////////////

void HC11::op_29h(HC11 *hc11)
{
    signed char offset;
 	
	if(hc11->CCR.V()==1)	//?V=1
	{
		offset = (signed)hc11->memory[hc11->PC];
		hc11->PC++;
	    hc11->PC = hc11->PC + offset;
    }
	else
		hc11->PC++;

	hc11->_clock();	

    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  BPL - Branches if plus
//
//  Addr. Mode: Relative
//  
//  Opcode: 2Ah
//          42d 
//
//  Clocks: 3  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  None
//
////////////////////////////////////////////////

void HC11::op_2Ah(HC11 *hc11)
{
    signed char offset;
 	
	if(hc11->CCR.N()==0)	//?N=0
	{
		offset = (signed)hc11->memory[hc11->PC];
		hc11->PC++;
	    hc11->PC = hc11->PC + offset;
    }
	else
		hc11->PC++;
	
	hc11->_clock();	

    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  BMI - Branches if minus
//
//  Addr. Mode: Relative
//  
//  Opcode: 2Bh
//          43d 
//
//  Clocks: 3  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  None
//
////////////////////////////////////////////////

void HC11::op_2Bh(HC11 *hc11)
{
    signed char offset;
 	
	if(hc11->CCR.N()==1)	//?N=1
	{
		offset = (signed)hc11->memory[hc11->PC];
		hc11->PC++;
	    hc11->PC = hc11->PC + offset;
    }
	else
		hc11->PC++;

	hc11->_clock();	

    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  BGE - Branches if greater than or equal to zero
//
//  Addr. Mode: Relative
//  
//  Opcode: 2Ch
//          44d 
//
//  Clocks: 3  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  None
//
////////////////////////////////////////////////

void HC11::op_2Ch(HC11 *hc11)
{
    signed char offset;
 	
	if((hc11->CCR.N() ^ hc11->CCR.V()) == 0)	//?N exor V=0
	{
		offset = (signed)hc11->memory[hc11->PC];
		hc11->PC++;
	    hc11->PC = hc11->PC + offset;
    }
	else
		hc11->PC++;

	hc11->_clock();	

    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  BLT - Branches if less than zero
//
//  Addr. Mode: Relative
//  
//  Opcode: 2Dh
//          45d 
//
//  Clocks: 3  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  None
//
////////////////////////////////////////////////

void HC11::op_2Dh(HC11 *hc11)
{
    signed char offset;
 	
	if((hc11->CCR.N() ^ hc11->CCR.V()) == 1)	//?N exor V=1, estaba mal: antes ponía | en vez de ^
	{
		offset = (signed)hc11->memory[hc11->PC];
		hc11->PC++;
	    hc11->PC = hc11->PC + offset;
    }
	else
		hc11->PC++;

	hc11->_clock();	

    hc11->_clock();	
}

	
/////////////////////////////////////////////////
//
//  BGT - Branches if greater than zero
//
//  Addr. Mode: Relative
//  
//  Opcode: 2Eh
//          46d 
//
//  Clocks: 3  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  None
//
////////////////////////////////////////////////

void HC11::op_2Eh(HC11 *hc11)
{
    signed char offset;
 	
	if((hc11->CCR.Z() | (hc11->CCR.N() ^ hc11->CCR.V())) == 0)	//?Z+(N exor V)=0
	{
		offset = (signed)hc11->memory[hc11->PC];
		hc11->PC++;
	    hc11->PC = hc11->PC + offset;
    }
	else
		hc11->PC++;
	hc11->_clock();	

    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  BLE - Branches if less than or equal to zero
//
//  Addr. Mode: Relative
//  
//  Opcode: 2Fh
//          47d 
//
//  Clocks: 3  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  None
//
////////////////////////////////////////////////

void HC11::op_2Fh(HC11 *hc11)
{
    signed char offset;
 	
	if((hc11->CCR.Z() | (hc11->CCR.N() ^ hc11->CCR.V())) == 1)	//?Z+(N exor V)=1
	{
		offset = (signed)hc11->memory[hc11->PC];
		hc11->PC++;
	    hc11->PC = hc11->PC + offset;
    }
	else
		hc11->PC++;
	hc11->_clock();	

    hc11->_clock();	
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
//
//  INS - Increment the stack pointer
//  
//  Addr. Mode: Inherent
//  
//  Opcode: 31h
//          49d        
//
//  Clocks: 3  (-1 for fetching opcode)
//
//  OpLength: 1 (-1 for fetching opcode)
//
//  Flags:  None
//
////////////////////////////////////////////////

void HC11::op_31h(HC11 *hc11)
{
	//CCRReg flag=0x00;
	
	// INS does not affect any flags!
	//hc11->SP=addword(hc11,hc11->SP,1,flag,0);
	
	hc11->SP++;		
	
	hc11->_clock();
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  DES - Decrement the stack pointer
//  
//  Addr. Mode: Inherent
//  
//  Opcode: 34h
//          52d        
//
//  Clocks: 3  (-1 for fetching opcode)
//
//  OpLength: 1 (-1 for fetching opcode)
//
//  Flags:  None
//
////////////////////////////////////////////////

void HC11::op_34h(HC11 *hc11)		//decrementar es igual k restar 1
{
	CCRReg flag=0x00;	//ningun flag es afectado
	
	hc11->SP = subword(hc11, hc11->SP, 1, flag);	//subword() es una funcion de opbasic.h, tambien valdria hc11->SP--
	
	hc11->_clock();
	hc11->_clock();

}


/////////////////////////////////////////////////
//
//  ABX - Adds ACCB to IX and place result in IX
//  
//  Addr. Mode: Inherent
//  
//  Opcode: 3Ah
//          58d 
//
//  Clocks: 3  (-1 for fetching opcode)
//
//  OpLength: 1 (-1 for fetching opcode)
//
//  Flags:  None
//
////////////////////////////////////////////////

void HC11::op_3Ah(HC11 *hc11)
{
	CCRReg flag=0x00;
	Word wd_accb;   //so can add two words
	
	wd_accb.low(hc11->ACCB);	//meto B en un registro de 16 bits (parte alta 0x00), y sumo con IX
	wd_accb.high(0);
	hc11->_clock();

	hc11->IX = addword(hc11, hc11->IX, wd_accb, flag, 0);
	hc11->_clock();

}


/////////////////////////////////////////////////
//
//  MUL - Multiply both accumulators
//
//  Addr. Mode: Inherent
//  
//  Opcode: 3Dh
//          61d        
//
//  Clocks: 10  (-1 for fetching opcode)
//
//  OpLength: 1 (-1 for fetching opcode)
//
//  Flags:  C
//
////////////////////////////////////////////////

void HC11::op_3Dh(HC11 *hc11)
{
	Word result;
	int i;

	result = hc11->ACCA * hc11->ACCB;	//D=A * B
	hc11->ACCB=result.low();
	hc11->ACCA=result.high();

	if(0x80 & hc11->ACCB)	//flag carry: poner a 1 si el bit 7 de ACCB es 1
		hc11->CCR.C(1);
	else
		hc11->CCR.C(0);

	for(i=1; i<=10; i++)	//consume 10 ciclos de reloj, 9 de aqui + 1 del fetch
		hc11->_clock();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
//
//  COMA - Complement
//
//  Addr. Mode: Inherent
//  
//  Opcode: 43h
//          67d        
//
//  Clocks: 2  (-1 for fetching opcode)
//
//  OpLength: 1 (-1 for fetching opcode)
//
//  Flags:  N, Z, V=0, C=1
//
////////////////////////////////////////////////

void HC11::op_43h(HC11 *hc11)
{
	CCRReg flag=0x0C;	//0000 1100, flags afectados

	hc11->ACCA = subbyte(hc11, 0xFF, hc11->ACCA, flag);	//A=$FF-A
	
	hc11->CCR.V(0);
	hc11->CCR.C(1);

	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  LSRA - Logical shift right
//
//  Addr. Mode: Inherent
//  
//  Opcode: 44h
//          68d        
//
//  Clocks: 2  (-1 for fetching opcode)
//
//  OpLength: 1 (-1 for fetching opcode)
//
//  Flags:  N=0, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_44h(HC11 *hc11)
{
	hc11->CCR.C(0x01 & hc11->ACCA);		//si hay 1 en LSB tendré carry

	hc11->ACCA = (hc11->ACCA) >> 1;		//desplaza 1 a la derecha
	
	if(hc11->ACCA==0)	//flag zero
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);

	hc11->CCR.N(0);		//añadido 
	hc11->CCR.V(hc11->CCR.C());	//flag overflow, tiene el mismo valor k carry (V= N exor C, pero como N=0, V=C)
	
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  ASRA - Shifts all bits to the right.  The LSB shifts	desplaza pero mantiene valor del bit 7
//         into the carry flag, and the MSB is shifted
//         to itself.
//  
//  Addr. Mode: Inherent
//  
//  Opcode: 47h
//          71d        
//
//  Clocks: 2  (-1 for fetching opcode)
//
//  OpLength: 1 (-1 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_47h(HC11 *hc11)
{
	if(hc11->ACCA & 0x01)	//si el LSB es 1, habrá carry
		hc11->CCR.C(1);
	else
		hc11->CCR.C(0);

	hc11->ACCA = (hc11->ACCA) >> 1;	//desplaza a la derecha

	if(hc11->ACCA==0)		//flag Zero
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);
	
	if(hc11->ACCA & 0x40)	//si despues de desplazar, el bit 6 es 1, significa k antes el bit 7 era 1 y hay que mantenerlo
	{
		hc11->ACCA = hc11->ACCA | 0x80;	//pone el bit 7 a 1
		hc11->CCR.N(1);
	}
	else
		hc11->CCR.N(0);
	
	if((hc11->CCR.N()==0 && hc11->CCR.C()==1) || (hc11->CCR.N()==1 && hc11->CCR.C()==0))	//overflow, N exor C = N*/C + /N*C
		hc11->CCR.V(1);																		//valores de despues del desplazamiento
	else
		hc11->CCR.V(0);

	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  ASLA - Shifts all bits to the left.  The MSB shifts			mismo opcode que LSLA
//         into the carry flag, and a zero shifts into LSB.
//  
//  Addr. Mode: Inherent
//  
//  Opcode: 48h
//          72d        
//
//  Clocks: 2  (-1 for fetching opcode)
//
//  OpLength: 1 (-1 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_48h(HC11 *hc11)
{
	if(hc11->ACCA>=128)	//flag carry: si el bit 7 es 1, activar carry (se mira antes del desplazamiento)
		hc11->CCR.C(1);
	else
		hc11->CCR.C(0);

/*
	if((hc11->ACCA.bit6()==1 && hc11->ACCA.bit7()==0) || (hc11->ACCA.bit6()==0 && hc11->ACCA.bit7()==1))	//Overflow, exor
		hc11->CCR.V(1);											//lo he cambiado de sitio, tiene que ser despues del desplazamiento
	else
		hc11->CCR.V(0);

*/
	
	hc11->ACCA = (hc11->ACCA) << 1;		//desplaza a la izquierda

	if(hc11->ACCA==0)	//flag zero
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);
	
	if(hc11->ACCA>=128)	//flag negative (la diferencia con el carry es k se mira despues del desplazamiento)
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);


	if((hc11->CCR.N()==1 && hc11->CCR.C()==0) || (hc11->CCR.N()==0 && hc11->CCR.C()==1))	//Overflow, exor
		hc11->CCR.V(1);											//lo he cambiado de sitio, tiene que ser despues del desplazamiento
	else
		hc11->CCR.V(0);



	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  DECA - Decrement the accumulator
//  
//  Addr. Mode: Inherent
//  
//  Opcode: 4Ah
//          74d        
//
//  Clocks: 2  (-1 for fetching opcode)
//
//  OpLength: 1 (-1 for fetching opcode)
//
//  Flags:  N, Z, V
//
////////////////////////////////////////////////

void HC11::op_4Ah(HC11 *hc11)
{
	CCRReg flag=0x0E;
	
	hc11->ACCA = subbyte(hc11, hc11->ACCA, 1, flag);	//A=A-1
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  INCA - Increment the accumulator
//  
//  Addr. Mode: Inherent
//  
//  Opcode: 4Ch
//          76d        
//
//  Clocks: 2  (-1 for fetching opcode)
//
//  OpLength: 1 (-1 for fetching opcode)
//
//  Flags:  N, Z, V
//
////////////////////////////////////////////////

void HC11::op_4Ch(HC11 *hc11)
{
	CCRReg flag=0x0E;
	
	hc11->ACCA = addbyte(hc11, hc11->ACCA, 1, flag, 0);		//A=A+1
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  CLRA - Clears the accumulator to zero
//
//  Addr. Mode: Inherent
//  
//  Opcode: 4Fh
//          79d 
//
//  Clocks: 2  (-1 for fetching opcode) 
//
//  OpLength: 1 (-1 for fetching opcode)
//
//  Flags:  N=0, Z=1, V=0, C=0
//
////////////////////////////////////////////////

void HC11::op_4Fh(HC11 *hc11)
{
	hc11->ACCA = 0;			

	hc11->CCR.N(0);
	hc11->CCR.Z(1);
	hc11->CCR.V(0);
	hc11->CCR.C(0);			

    hc11->_clock();	
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
//
//  COMB - Complement
//
//  Addr. Mode: Inherent
//  
//  Opcode: 53h
//          83d        
//
//  Clocks: 2  (-1 for fetching opcode)
//
//  OpLength: 1 (-1 for fetching opcode)
//
//  Flags:  N, Z, V=0, C=1
//
////////////////////////////////////////////////

void HC11::op_53h(HC11 *hc11)
{
	CCRReg flag=0x0C;	//0000 1100

	hc11->ACCB = subbyte(hc11, 0xFF, hc11->ACCB, flag);	//B=$FF-B
	
	hc11->CCR.V(0);
	hc11->CCR.C(1);

	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  LSRB - Logical shift right B
//
//  Addr. Mode: Inherent
//  
//  Opcode: 54h
//          84d        
//
//  Clocks: 2  (-1 for fetching opcode)
//
//  OpLength: 1 (-1 for fetching opcode)
//
//  Flags:  N=0, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_54h(HC11 *hc11)
{
	hc11->CCR.C(0x01 & hc11->ACCB);	//flag carry, antes de desplazar
	
    hc11->ACCB = hc11->ACCB >> 1;	//desplaza

	if(hc11->ACCB==0)	//flag zero
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);

	hc11->CCR.N(0);		//añadido 
	hc11->CCR.V(hc11->CCR.C());	//flag overflow: N exor C, pero como N=0, V=C

	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  ASRB - Shifts all bits to the right.  The LSB shifts	mantiene el valor del bit 7
//         into the carry flag, and the MSB is shifted
//         to itself.
//  
//  Addr. Mode: Inherent
//  
//  Opcode: 57h
//          87d        
//
//  Clocks: 2  (-1 for fetching opcode)
//
//  OpLength: 1 (-1 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_57h(HC11 *hc11)
{
	if(hc11->ACCB & 0x01)	//flag carry, lo comprueba antes de desplazar
		hc11->CCR.C(1);
	else
		hc11->CCR.C(0);

	hc11->ACCB = (hc11->ACCB) >> 1;		//desplaza una posicion a la derecha

	if(hc11->ACCB==0)	//flag zero
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);
	
	if(hc11->ACCB & 0x40)	//si despues de desplazar, el bit 6 es 1, significa k antes el bit 7 era 1 y hay que mantenerlo
	{
		hc11->ACCB = hc11->ACCB | 0x80;	//pone el bit 7 a 1
		hc11->CCR.N(1);
	}
	else
		hc11->CCR.N(0);
	
	if((hc11->CCR.N()==0 && hc11->CCR.C()==1) || (hc11->CCR.N()==1 && hc11->CCR.C()==0))	//Overflow, N exor C
		hc11->CCR.V(1);
	else
		hc11->CCR.V(0);

	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  ASLB - Shifts all bits to the left.  The MSB shifts			mismo opcode que LSLB
//         into the carry flag, and a zero shifts into LSB.
//  
//  Addr. Mode: Inherent
//  
//  Opcode: 58h
//          88d        
//
//  Clocks: 2  (-1 for fetching opcode)
//
//  OpLength: 1 (-1 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_58h(HC11 *hc11)
{
	if(hc11->ACCB>=128)		//flag carry, mirar antes de desplazar
		hc11->CCR.C(1);
	else
		hc11->CCR.C(0);
/*

	if((hc11->ACCB.bit6()==1 && hc11->ACCB.bit7()==0) || (hc11->ACCB.bit6()==0 && hc11->ACCB.bit7()==1))	//Overflow
		hc11->CCR.V(1);
	else
		hc11->CCR.V(0);
*/

	hc11->ACCB = (hc11->ACCB) << 1;		//desplaza

	if(hc11->ACCB==0)		//flag zero
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);
	
	if(hc11->ACCB>=128)		//flag negative
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);



	if((hc11->CCR.N()==1 && hc11->CCR.C()==0) || (hc11->CCR.N()==0 && hc11->CCR.C()==1))	//Overflow, exor
		hc11->CCR.V(1);											//lo he cambiado de sitio, tiene que ser despues del desplazamiento
	else
		hc11->CCR.V(0);


	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  DECB - Decrement the accumulator
//  
//  Addr. Mode: Inherent
//  
//  Opcode: 5Ah
//          90d        
//
//  Clocks: 2  (-1 for fetching opcode)
//
//  OpLength: 1 (-1 for fetching opcode)
//
//  Flags:  N, Z, V
//
////////////////////////////////////////////////

void HC11::op_5Ah(HC11 *hc11)
{
	CCRReg flag=0x0E;
	
	hc11->ACCB = subbyte(hc11, hc11->ACCB, 1, flag);	//B=B-1
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  INCB - Increment the accumulator
//  
//  Addr. Mode: Inherent
//  
//  Opcode: 5Ch
//          92d        
//
//  Clocks: 2  (-1 for fetching opcode)
//
//  OpLength: 1 (-1 for fetching opcode)
//
//  Flags:  N, Z, V
//
////////////////////////////////////////////////

void HC11::op_5Ch(HC11 *hc11)
{
	CCRReg flag=0x0E;
	
	hc11->ACCB = addbyte(hc11, hc11->ACCB, 1, flag, 0);	//B=B+1
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  CLRB - Clears the accumulator to zero
//
//  Addr. Mode: Inherent
//  
//  Opcode: 5Fh
//          95d 
//
//  Clocks: 2  (-1 for fetching opcode) 
//
//  OpLength: 1 (-1 for fetching opcode)
//
//  Flags:  N=0, Z=1, V=0, C=0
//
////////////////////////////////////////////////

void HC11::op_5Fh(HC11 *hc11)
{
	hc11->ACCB = 0;
	
	hc11->CCR.N(0);
	hc11->CCR.Z(1);
	hc11->CCR.V(0);
	hc11->CCR.C(0);
			
    hc11->_clock();	
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
//
//  COM - Complement
//
//  Addr. Mode: Indirect X, indexado en X
//  
//  Opcode: 63h
//          94d        
//
//  Clocks: 6  (-1 for fetching opcode)
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V=0, C=1
//
////////////////////////////////////////////////

void HC11::op_63h(HC11 *hc11)
{
	CCRReg flag=0x0C;
	Word mem;
	Byte result;

	mem=(hc11->IX + hc11->memory[hc11->PC]);	//indexado en X
	hc11->PC++;
	hc11->_clock();

	result = subbyte(hc11, 0xFF, hc11->memory[mem], flag);	//M=$FF-M
	hc11->_clock();
	
	hc11->memory.Write(mem, result);
	hc11->_clock();

	hc11->_clock();

	hc11->CCR.V(0);
	hc11->CCR.C(1);	
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  LSR - Logical shift right
//
//  Addr. Mode: Indirect X, indexado en X
//  
//  Opcode: 64h
//          95d        
//
//  Clocks: 6  (-1 for fetching opcode)
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N=0, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_64h(HC11 *hc11)
{
	Word mem;
	Byte num;

	mem=(hc11->IX + hc11->memory[hc11->PC]);	//indexado en X
	hc11->PC++;
	hc11->_clock();

	num=hc11->memory[mem];
	hc11->_clock();

	hc11->CCR.C(0x01 & num);	//flag carry, mirar antes del desplazamiento

	num = num >> 1;		//desplaza
	hc11->_clock();

	if(num==0)		//flag zero
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);
	
	hc11->CCR.N(0);	//añadido
	hc11->CCR.V(hc11->CCR.C());	//flag overflow, V= N exor C, pero al ser N=0, V=C
	hc11->_clock();

	hc11->memory.Write(mem, num);
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  ASR - Shifts all bits to the right.  The LSB shifts			mantiene el valor del bit 7
//         into the carry flag, and the MSB goes to itself.
//  
//  Addr. Mode: Indirect X, indexado en X
//  
//  Opcode: 67h
//          103d        
//
//  Clocks: 6  (-1 for fetching opcode)
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_67h(HC11 *hc11)
{
	Byte num=(hc11->memory[(hc11->IX + hc11->memory[hc11->PC])]);	//lee de memoria numero indexado en X
	hc11->_clock();

	hc11->_clock();

	if(num & 0x01)		//flag carry, mirar antes de desplazar
		hc11->CCR.C(1);
	else
		hc11->CCR.C(0);

	hc11->_clock();

	num = num >> 1;	//desplaza
	hc11->_clock();

	if(num==0)		//flag zero
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);
	
	if(num & 0x40)	//si el bit 6 es 1, antes era el bit 7 1, asi que debo mantenerlo y activar flag negative
	{
		num = num | 0x80;
		hc11->CCR.N(1);
	}
	else
		hc11->CCR.N(0);
	

	if((hc11->CCR.N()==0 && hc11->CCR.C()==1) || (hc11->CCR.N()==1 && hc11->CCR.C()==0))	//Overflow, N exor C
		hc11->CCR.V(1);
	else
		hc11->CCR.V(0);

	hc11->memory.Write((hc11->IX + hc11->memory[hc11->PC]), num);	//escribe en memoria
	hc11->PC++;
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  ASL - Shifts all bits to the left.  The MSB shifts			mismo opcode que LSL
//         into the carry flag, and a zero shifts into LSB.
//  
//  Addr. Mode: Indirect X, indexado en X
//  
//  Opcode: 68h
//          104d        
//
//  Clocks: 6  (-1 for fetching opcode)
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_68h(HC11 *hc11)
{
	Byte num=(hc11->memory[(hc11->IX + hc11->memory[hc11->PC])]);	//lee de memoria
	hc11->_clock();

	hc11->_clock();

	if(num>=128)	//flag carry, mirar antes de desplazar
		hc11->CCR.C(1);
	else
		hc11->CCR.C(0);

/*
	if((num.bit6()==1 && num.bit7()==0) || (num.bit6()==0 && num.bit7()==1))	//Overflow
		hc11->CCR.V(1);
	else
		hc11->CCR.V(0);
*/

	hc11->_clock();

	num = num<<1;	//desplaza
	hc11->_clock();

	if(num==0)		//flag zero
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);
	
	if(num>=128)	//flag negative
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);


	if((hc11->CCR.N()==1 && hc11->CCR.C()==0) || (hc11->CCR.N()==0 && hc11->CCR.C()==1))	//Overflow, exor
		hc11->CCR.V(1);											//lo he cambiado de sitio, tiene que ser despues del desplazamiento
	else
		hc11->CCR.V(0);


	hc11->memory.Write((hc11->IX + hc11->memory[hc11->PC]), num);	//escribe en memoria
	hc11->PC++;
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  DEC - Decrement the memory location
//  
//  Addr. Mode: Indirect X, indexado X
//  
//  Opcode: 6Ah
//          106d        
//
//  Clocks: 6  (-1 for fetching opcode)
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V
//
////////////////////////////////////////////////

void HC11::op_6Ah(HC11 *hc11)
{
	Word mem;
	Byte data;
	CCRReg flag=0x0E;
	
	mem=(hc11->IX + hc11->memory[hc11->PC]);	//indexado en X
	hc11->PC++;
	hc11->_clock();

	hc11->_clock();
	
	data=hc11->memory[mem];	//lee de memoria
	hc11->_clock();

	data = subbyte(hc11, data, 1, flag);	//data=data-1
	hc11->_clock();
	
	hc11->memory.Write(mem, data);	//escribe en memoria
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  INC - Increment the memory location
//  
//  Addr. Mode: Indirect X, indexado en X
//  
//  Opcode: 6Ch
//          108d        
//
//  Clocks: 6  (-1 for fetching opcode)
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V
//
////////////////////////////////////////////////

void HC11::op_6Ch(HC11 *hc11)
{
	Word mem;
	Byte data;
	CCRReg flag=0x0E;
	
	mem=(hc11->IX + hc11->memory[hc11->PC]);
	hc11->PC++;
	hc11->_clock();

	hc11->_clock();
	
	data=hc11->memory[mem];	//lee de memoria
	hc11->_clock();

	data = addbyte(hc11, data, 1, flag, 0);	//data=data+1
	hc11->_clock();
	
	hc11->memory.Write(mem, data);	//escribe en memoria
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  CLR - Clears the memory location
//
//  Addr. Mode: Indexed X
//  
//  Opcode: 6Fh
//
//  Clocks: 6  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N=0, Z=1, V=0, C=0
//
////////////////////////////////////////////////

void HC11::op_6Fh(HC11 *hc11)
{
	Word mem;

	mem=(hc11->IX + hc11->memory[hc11->PC]);
	hc11->PC++;
    hc11->_clock();	

    hc11->_clock();	

	hc11->memory.Write(mem, 0);
    hc11->_clock();	

	hc11->CCR.N(0);
	hc11->CCR.Z(1);
	hc11->CCR.V(0);
	hc11->CCR.C(0);
    hc11->_clock();	

			
    hc11->_clock();	
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
//
//  COM - Complement
//
//  Addr. Mode: Extended
//  
//  Opcode: 73h
//          110d        
//
//  Clocks: 6  (-1 for fetching opcode)
//
//  OpLength: 3 (-1 for fetching opcode)
//
//  Flags:  N, Z, V=0, C=1
//
////////////////////////////////////////////////

void HC11::op_73h(HC11 *hc11)
{
	CCRReg flag=0x0C;
	Word mem;
	Byte result;

	mem.high(hc11->memory[hc11->PC]);	//lee de memoria parte alta de la dir
	hc11->PC++;
	hc11->_clock();

	mem.low(hc11->memory[hc11->PC]);	//lee de memoria parte baja de la dir
	hc11->PC++;
	hc11->_clock();

	result = subbyte(hc11, 0xFF, hc11->memory[mem], flag);	//mem = $FF-mem (es lo mismo k invertir todos los bits)
	hc11->_clock();
	
	hc11->memory.Write(mem, result);	//escribe en memoria
	hc11->_clock();

	hc11->CCR.V(0);
	hc11->CCR.C(1);	
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  LSR - Logical shift right
//
//  Addr. Mode: Extended
//  
//  Opcode: 74h
//          111d        
//
//  Clocks: 6  (-1 for fetching opcode)
//
//  OpLength: 3 (-1 for fetching opcode)
//
//  Flags:  N=0, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_74h(HC11 *hc11)
{
	Word mem;
	Byte num;

	mem.high(hc11->memory[hc11->PC]);	//lee de memoria la parte alta de la dir
	hc11->PC++;
	hc11->_clock();
	
	mem.low(hc11->memory[hc11->PC]);	//lee de memoria la parte baja de la dir
	hc11->PC++;
	hc11->_clock();

	num=hc11->memory[mem];		//lee el dato de la dir

	hc11->CCR.C(0x01 & num);	//flag carry, antes de desplazar

	num = num >> 1;				//desplaza
	hc11->_clock();

	if(num==0)				//flag zero
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);


	hc11->CCR.N(0);		//añadido


	hc11->CCR.V(hc11->CCR.C());	//flag overflow, V= N exor C, pero como N=0, V=C
	hc11->_clock();

	hc11->memory.Write(mem, num);
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  ASR - Shifts all bits to the right.  The LSB shifts			mantiene el valor del bit7
//         into the carry flag, and the LSB goes to itself.
//  
//  Addr. Mode: Extended
//  
//  Opcode: 77h
//          119d        
//
//  Clocks: 6  (-1 for fetching opcode)
//
//  OpLength: 3 (-1 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_77h(HC11 *hc11)
{
	Word mem;
    Byte num;

	mem.high(hc11->memory[hc11->PC]);	//lee la parte alta de la dir
	hc11->PC++;
	hc11->_clock();

	mem.low(hc11->memory[hc11->PC]);	//lee la parte baja de la dir
	hc11->PC++;
	hc11->_clock();

    num = hc11->memory[mem];		//lee el dato segun la dir

	if(num & 0x1)		//flag carry, antes de desplazar
		hc11->CCR.C(1);
	else
		hc11->CCR.C(0);

	hc11->_clock();

	num = (num)>>1;		//desplaza
	hc11->_clock();

	if(num==0)			//flag zero
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);
	
	if(num & 0x40)	//si el bit6 es 1, antes lo era el bit7, y como hay k mantenerlo lo pongo a 1, y activo flag negative
	{
		num = num | 0x80;
		hc11->CCR.N(1);
	}
	else
		hc11->CCR.N(0);

	
	if((hc11->CCR.N()==0 && hc11->CCR.C()==1) || (hc11->CCR.N()==1 && hc11->CCR.C()==0))	//Overflow, N exor C
		hc11->CCR.V(1);
	else
		hc11->CCR.V(0);

	// Write back to memory
	hc11->memory.Write(mem, num);

	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  ASL - Shifts all bits to the left.  The MSB shifts				mismo opcode que LSL
//         into the carry flag, and a zero shifts into LSB.
//  
//  Addr. Mode: Extended
//  
//  Opcode: 78h
//          120d        
//
//  Clocks: 6  (-1 for fetching opcode)
//
//  OpLength: 3 (-1 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_78h(HC11 *hc11)
{
	Word mem;
    Byte num;

	mem.high(hc11->memory[hc11->PC]);	//lee de memoria la parte alta de la dir
	hc11->PC++;
	hc11->_clock();

	mem.low(hc11->memory[hc11->PC]);	//lee de memoria la parte baja de la dir
	hc11->PC++;
	hc11->_clock();

    num = hc11->memory[mem];		//lee dato segun dir

	if(num>=128)		//flag carry, antes de desplazar
		hc11->CCR.C(1);
	else
		hc11->CCR.C(0);

/*
	if((num.bit6()==1 && num.bit7()==0) || (num.bit6()==0 && num.bit7()==1))	//Overflow
		hc11->CCR.V(1);
	else
		hc11->CCR.V(0);

*/

	hc11->_clock();

	num = (num)<<1;		//desplaza
	hc11->_clock();

	if(num==0)			//flag zero
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);
	
	if(num>=128)		//flag negative
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);


	if((hc11->CCR.N()==1 && hc11->CCR.C()==0) || (hc11->CCR.N()==0 && hc11->CCR.C()==1))	//Overflow, exor
		hc11->CCR.V(1);											//lo he cambiado de sitio, tiene que ser despues del desplazamiento
	else
		hc11->CCR.V(0);


	// Write the value back to memory
	hc11->memory.Write(mem, num);

	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  DEC - Decrement mem
//  
//  Addr. Mode: Extended
//  
//  Opcode: 7Ah
//          122d        
//
//  Clocks: 6  (-1 for fetching opcode)
//
//  OpLength: 3 (-1 for fetching opcode)
//
//  Flags:  N, Z, V
//
////////////////////////////////////////////////

void HC11::op_7Ah(HC11 *hc11)
{
	Word mem;
	Byte data;
	CCRReg flag=0x0E;
	
	mem.high(hc11->memory[hc11->PC]);
	hc11->PC++;
	hc11->_clock();

	mem.low(hc11->memory[hc11->PC]);
	hc11->PC++;
	hc11->_clock();
	
	data = hc11->memory[mem];
	hc11->_clock();


// hc11->CCR.V(data == 0x80);	//el overflow solo ocurre si el valor del dato es $80 antes de la operacion
// data = data - 1;
// hc11->CCR.N(data.bit7());
// hc11->CCR.Z(data == 0);


	data = subbyte(hc11, data, 1, flag);	//si funciona, el valor 0x80 sale de simplificar el calculo en subbyte
	hc11->_clock();    

	hc11->memory.Write(mem, data);
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  INC - Increment memoria
//  
//  Addr. Mode: Extended
//  
//  Opcode: 7Ch
//          124d        
//
//  Clocks: 6  (-1 for fetching opcode)
//
//  OpLength: 3 (-1 for fetching opcode)
//
//  Flags:  N, Z, V
//
////////////////////////////////////////////////

void HC11::op_7Ch(HC11 *hc11)
{
	Word mem;
	Byte data;
	CCRReg flag=0x0E;
	
	mem.high(hc11->memory[hc11->PC]);	//lee parte alta de dir
	hc11->PC++;
	hc11->_clock();

	mem.low(hc11->memory[hc11->PC]);	//lee parte baja de dir
	hc11->PC++;
	hc11->_clock();
	
	data=hc11->memory[mem];		//lee dato segun dir
	hc11->_clock();

	data = addbyte(hc11, data, 1, flag, 0);		//M=M+1, overflow solo pasa si el dato = $7F antes de la operacion
	hc11->_clock();

	hc11->memory.Write(mem, data);	//escribe en memoria
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  CLR - Clears the memory location
//
//  Addr. Mode: Extended
//  
//  Opcode: 7Fh
//          127d 
//
//  Clocks: 6  (-1 for fetching opcode) 
//
//  OpLength: 3 (-1 for fetching opcode)
//
//  Flags:  N=0, Z=1, V=0, C=0
//
////////////////////////////////////////////////

void HC11::op_7Fh(HC11 *hc11)
{
	Word mem;

	mem.high(hc11->memory[hc11->PC]);
	hc11->PC++;
    hc11->_clock();	

	mem.low(hc11->memory[hc11->PC]);
	hc11->PC++;
    hc11->_clock();	

	hc11->memory.Write(mem, 0);
    hc11->_clock();	

	hc11->CCR.N(0);
	hc11->CCR.Z(1);
	hc11->CCR.V(0);
	hc11->CCR.C(0);
    hc11->_clock();	
			
    hc11->_clock();	
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
//
//  SUBA - Subtracts the the passed Byte from ACCA
//         and places the result in ACCA
//  
//  Addr. Mode: Immediate
//  
//  Opcode: 80h
//          128d        
//
//  Clocks: 2  (-1 for fetching opcode)
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_80h(HC11 *hc11)
{
	CCRReg flag=0x0F;
	
	hc11->ACCA = subbyte(hc11, hc11->ACCA, hc11->memory[hc11->PC], flag);	//A=A-M
	hc11->PC++;
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  CMPA - Compare
//
//  Addr. Mode: Immediate
//  
//  Opcode: 81h
//          129d 
//
//  Clocks: 2  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z,  v c 
//
////////////////////////////////////////////////

void HC11::op_81h(HC11 *hc11)
{
	Byte num;
	CCRReg flag=0x0F;

	num=hc11->memory[hc11->PC];
	hc11->PC++;
	
	subbyte(hc11, hc11->ACCA, num, flag);	//A-M
	
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  SBCA - Subtract with carry
//
//  Addr. Mode: Immediate
//  
//  Opcode: 82h
//          130d 
//
//  Clocks: 2  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_82h(HC11 *hc11)
{
	Byte num;
	CCRReg flag=0x0F;

	num=hc11->memory[hc11->PC];
	hc11->PC++;
	num = num - hc11->CCR.C();	//resta el carry

	hc11->ACCA = subbyte(hc11, hc11->ACCA, num, flag);	//A=A-M-C
	
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  SUBD - Subtracts the the passed Word and ACCD
//         and places the result in ACCD
//  
//  Addr. Mode: Immediate
//  
//  Opcode: 83h
//          131d        
//
//  Clocks: 4  (-1 for fetching opcode)
//
//  OpLength: 3 (-1 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_83h(HC11 *hc11)
{
	CCRReg flag=0x0F;
	Word num;
	Word ACCD;
	
	num.high(hc11->memory[hc11->PC]);	//el dato es de 16 inmediato, tengo k hacer 2 lecturas en memoria incrementando PC
	hc11->PC++;
	hc11->_clock();

	num.low(hc11->memory[hc11->PC]);
	hc11->PC++;
	hc11->_clock();
	
	ACCD.low(hc11->ACCB);	//asocia la variable local D con los registros A y B
	ACCD.high(hc11->ACCA);

	ACCD = subword(hc11, ACCD, num, flag);	//D= D-dato
	
	hc11->ACCA=ACCD.high();	//asocia los registros A y B con la variable local D
	hc11->ACCB=ACCD.low();
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  ANDA - Ands ACCA with the passed parameter
//  
//  Addr. Mode: Immediate
//  
//  Opcode: 84h
//          132d        
//
//  Clocks: 2  (-1 for fetching opcode)
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_84h(HC11 *hc11)
{
	hc11->ACCA = hc11->ACCA & hc11->memory[hc11->PC];	//A= A & M
	hc11->PC++;

	if(hc11->ACCA>=128)		//flag negative
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);

	if(hc11->ACCA==0)		//flag zero
		hc11->CCR.Z(1);
	else 
		hc11->CCR.Z(0);

	hc11->CCR.V(0);			//flag overflow
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  BITA - performs an AND on the accumulator and memory location
//
//  Addr. Mode: Immediate
//  
//  Opcode: 85h
//          133d 
//
//  Clocks: 2  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_85h(HC11 *hc11)
{
	Byte data;

	data = hc11->ACCA & hc11->memory[hc11->PC];		//A & M, solo afecta a los flags (no se guarda el resultado)
	hc11->PC++;

	if(data==0)			//flag zero
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);

	if(data>=128)		//flag negative
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);

	hc11->CCR.V(0);		//flag overflow
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  EORA - XORs all accumulator
//
//  Addr. Mode: Immediate
//  
//  Opcode: 88h
//          136d 
//
//  Clocks: 2  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_88h(HC11 *hc11)
{
	hc11->ACCA = hc11->ACCA ^ hc11->memory[hc11->PC];		//A= A exor M
	hc11->PC++;				

	hc11->CCR.V(0);		//flag overflow
	
	if(hc11->ACCA>=128)	//flag negative
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);	//habia fallo

	if(hc11->ACCA==0)	//flag zero
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);
				
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  ADCA - Adds ACCA, the passed Byte, and the carry
//         flag and places the result in ACCA
//  
//  Addr. Mode: Immediate
//  
//  Opcode: 89h
//          137d        
//
//  Clocks: 2  (-1 for fetching opcode)
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  H, N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_89h(HC11 *hc11)
{
	CCRReg flag=0x2F;
	
	hc11->ACCA = addbyte(hc11, hc11->ACCA, hc11->memory[hc11->PC], flag, hc11->CCR.C());	//A = A+M+C
	hc11->PC++;     
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  ADDA - Adds ACCA and the the passed Byte
//         and places the result in ACCA
//  
//  Addr. Mode: Immediate
//  
//  Opcode: 8Bh
//          139d        
//
//  Clocks: 2  (-1 for fetching opcode)
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  H, N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_8Bh(HC11 *hc11)	//en esta instruccion no se tiene en cuanta el bit de carry, por eso le paso 0 a addbyte()
{
	CCRReg flag=0x2F;	//mascara para los indicadores 0010 1111
	
	hc11->ACCA = addbyte(hc11, hc11->ACCA, hc11->memory[hc11->PC], flag, 0);	//addbyte() es una funcion de opbasic.cpp
	hc11->PC++;
	
	hc11->_clock();		//consume 2 ciclos, éste mas el de fetch
}


/////////////////////////////////////////////////
//
//  BSR - Branch to subroutine
//
//  Addr. Mode: Relative
//  
//  Opcode: 8Dh
//          141d 
//
//  Clocks: 6  (-1 for fetching opcode) 
//
//  OpLength: 3 (-1 for fetching opcode)
//
//  Flags:  None
//
////////////////////////////////////////////////

void HC11::op_8Dh(HC11 *hc11)
{
    signed char offset;

    hc11->_clock();
    offset = (signed)hc11->memory[hc11->PC];	//el contenido de PC es el offset
    hc11->PC++;

    // these do clocking internally
    hc11->Push(hc11->PC.low());		//meto parte baja PC en la pila (la funcion push() ya decrementa SP y consume su _clock())
    hc11->Push(hc11->PC.high());	//necesito guardar PC para poder volver (al ser un salto ahora sobreescribo PC)

    hc11->_clock();
    hc11->_clock();
    hc11->PC = hc11->PC + offset;    //cargo PC con la nueva dir (PC + relativa)
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
//
//  SUBA - Subtracts a memory location and ACCA
//         and places the result in ACCA
//  
//  Addr. Mode: Direct
//  
//  Opcode: 90h
//          144d        
//
//  Clocks: 3  (-1 for fetching opcode)
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_90h(HC11 *hc11)
{
	CCRReg flag=0x0F;
	Word mem_location;

	mem_location.high(0);						//direccionamiento directo, la parte alta pongo 0
	mem_location.low(hc11->memory[hc11->PC]);        
	hc11->PC++;
	hc11->_clock();

	hc11->ACCA = subbyte(hc11, hc11->ACCA, hc11->memory[mem_location], flag);	//A=A-M
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  CMPA - Compare
//
//  Addr. Mode: Direct
//  
//  Opcode: 91h
//          145d 
//
//  Clocks: 3  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z,  v, c
//
////////////////////////////////////////////////

void HC11::op_91h(HC11 *hc11)
{
	Word mem;
	CCRReg flag=0x0F;

	mem.low(hc11->memory[hc11->PC]);
	mem.high(0);						//direccionamiento directo, parte alta 0
	hc11->PC++;
    hc11->_clock();	
	
	subbyte(hc11, hc11->ACCA, hc11->memory[mem], flag);		//A-M
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  SBCA - Subtract with carry
//
//  Addr. Mode: Direct
//  
//  Opcode: 92h
//          146d 
//
//  Clocks: 3  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_92h(HC11 *hc11)
{
	Word mem;
	CCRReg flag=0x0F;

	mem.low(hc11->memory[hc11->PC]);
	mem.high(0);
	hc11->PC++;

	hc11->ACCA = hc11->ACCA - hc11->CCR.C();	//resto carry

	hc11->ACCA = subbyte(hc11, hc11->ACCA, hc11->memory[hc11->PC], flag);	//A=A-M-C
	
	hc11->_clock();
	hc11->_clock();		//añadido
}


/////////////////////////////////////////////////
//
//  SUBD - Subtracts a memory location from ACCD
//         and places the result in ACCD
//  
//  Addr. Mode: Direct
//  
//  Opcode: 93h
//          147d        
//
//  Clocks: 5  (-1 for fetching opcode)
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_93h(HC11 *hc11)
{
	CCRReg flag=0x0F;
	Word mem;
	Word ACCD;
	Word num;
	
	mem.low(hc11->memory[hc11->PC]);	//direccionamiento directo, leo de memoria parte baja, parte alta 0; ya tengo dir donde buscar
	hc11->PC++;
	hc11->_clock();

	mem.high(0);
	hc11->_clock();

/*	
	num = hc11->memory[mem];		//estaba mal, probado ejecutando y debugeando la memoria 93 02 01 00 y D=7777, debe dar 7677
	num = hc11->memory[mem+1];
*/
		
	num.high(hc11->memory[mem]);
	num.low(hc11->memory[mem+1]);
	hc11->_clock();

	ACCD.low(hc11->ACCB);	//asocio D con regitros A y B
	ACCD.high(hc11->ACCA);
	
	ACCD = subword(hc11, ACCD, num, flag);	//D= D-M
	hc11->ACCA=ACCD.high();	//asocio registros A y B con D
	hc11->ACCB=ACCD.low();
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  ANDA - Ands ACCA with the memory location
//  
//  Addr. Mode: Direct
//  
//  Opcode: 94h
//          148d        
//
//  Clocks: 3  (-1 for fetching opcode)
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_94h(HC11 *hc11)
{
	Word mem;

	mem.low(hc11->memory[hc11->PC]);	//direccionamiento directo
	mem.high(0);
	hc11->PC++;

	hc11->ACCA = hc11->ACCA & hc11->memory[mem];	//A = A & M
	hc11->_clock();

	if(hc11->ACCA>=128)	//flag negative
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);

	if(hc11->ACCA==0)	//flag zero
		hc11->CCR.Z(1);
	else 
		hc11->CCR.Z(0);

	hc11->CCR.V(0);		//flag overflow
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  BITA - performs an AND on the accumulator and memory location
//
//  Addr. Mode: Direct
//  
//  Opcode: 95h
//          149d 
//
//  Clocks: 3  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_95h(HC11 *hc11)
{
	Byte data;
	Word mem;

	mem.low(hc11->memory[hc11->PC]);	//direccionamiento directo
	mem.high(0);
	hc11->PC++;
    hc11->_clock();	

	data = hc11->ACCA & hc11->memory[mem];	//A & M

	if(data==0)			//flag zero
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);

	if(data>=128)		//flag negative
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);

	hc11->CCR.V(0);		//flag overflow
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  EORA - XORs the accumulator
//
//  Addr. Mode: Direct
//  
//  Opcode: 98h
//          152d 
//
//  Clocks: 3  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_98h(HC11 *hc11)
{
	Word mem;
	
	mem.low(hc11->memory[hc11->PC]);
	mem.high(0);
	hc11->PC++;
    hc11->_clock();	


	hc11->ACCA = hc11->ACCA ^ hc11->memory[mem];	//A = A exor M

	hc11->CCR.V(0);			//flag overflow

	if(hc11->ACCA>=128)		//flag negative
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);		//habia fallo

	if(hc11->ACCA==0)		//flag zero
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);
				
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  ADCA - Adds ACCA, a memory location, and the carry
//         flag and places the result in ACCA
//  
//  Addr. Mode: Direct
//  
//  Opcode: 99h
//          153d        
//
//  Clocks: 3  (-1 for fetching opcode)
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  H, N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_99h(HC11 *hc11)
{
	CCRReg flag=0x2F;
	Word mem_location;
	
	mem_location.high(0);
	mem_location.low(hc11->memory[hc11->PC]);       
	hc11->PC++;
	hc11->_clock();

	hc11->ACCA = addbyte(hc11, hc11->ACCA, hc11->memory[mem_location], flag, hc11->CCR.C());	//A = A+M+C
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  ADDA - Adds ACCA and a memory location
//         and places the result in ACCA
//  
//  Addr. Mode: Direct
//  
//  Opcode: 9Bh
//          155d        
//
//  Clocks: 3  (-1 for fetching opcode)
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  H, N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_9Bh(HC11 *hc11)
{
	CCRReg flag=0x2F;
	Word mem_location;

	mem_location.high(0);
	mem_location.low(hc11->memory[hc11->PC]);        
	hc11->PC++;
	hc11->_clock();

	hc11->ACCA = addbyte(hc11, hc11->ACCA, hc11->memory[mem_location], flag, 0);	//A = A+M
	hc11->_clock();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
//
//  SUBA - Subtracts a memory location from ACCA
//         and places the result in ACCA
//  
//  Addr. Mode: Indirect X
//  
//  Opcode: A0h
//          160d        
//
//  Clocks: 4  (-1 for fetching opcode)
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_A0h(HC11 *hc11)
{
	CCRReg flag=0x0F;

	hc11->_clock();

	hc11->_clock();

	hc11->ACCA = subbyte(hc11, hc11->ACCA, hc11->memory[(hc11->IX + hc11->memory[hc11->PC])], flag);	//A = A-M
	hc11->PC++;
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  CMPA - Compare
//
//  Addr. Mode: Indirect X
//  
//  Opcode: A1h
//          161d 
//
//  Clocks: 4  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z,  v, c
//
////////////////////////////////////////////////

void HC11::op_A1h(HC11 *hc11)
{
	Word mem;
	CCRReg flag=0x0F;

	mem=(hc11->IX + hc11->memory[hc11->PC]);
	hc11->PC++;
    hc11->_clock();	

    hc11->_clock();	

	subbyte(hc11, hc11->ACCA, hc11->memory[mem], flag);		//A-M
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  SBCA - Subtract with carry
//
//  Addr. Mode: Indexed X
//  
//  Opcode: A2h
//          162d 
//
//  Clocks: 4  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_A2h(HC11 *hc11)
{
	Word mem;
	CCRReg flag=0x0F;

	mem=(hc11->IX + hc11->memory[hc11->PC]);
	hc11->PC++;
    hc11->_clock();	

	hc11->ACCA = hc11->ACCA - hc11->CCR.C();	//resto el carry
    hc11->_clock();	

	hc11->ACCA = subbyte(hc11, hc11->ACCA, hc11->memory[mem], flag);	//A = A-M-C
	
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  SUBD - Subtracts a memory location from ACCD
//         and places the result in ACCD
//  
//  Addr. Mode: Indirect X
//  
//  Opcode: A3h
//          163d        
//
//  Clocks: 6  (-1 for fetching opcode)
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_A3h(HC11 *hc11)
{
	CCRReg flag=0x0F;
	Word mem;
	Word num;
	Word ACCD;
	
	mem=(hc11->IX + hc11->memory[hc11->PC]);
	hc11->PC++;
	hc11->_clock();

	hc11->_clock();
	
	ACCD.low(hc11->ACCB);	//asocio la variable local D con los registros A y B
	ACCD.high(hc11->ACCA);
	hc11->_clock();
	
	num.high(hc11->memory[mem]);
	mem++;
	num.low(hc11->memory[mem]);
	hc11->_clock();

	ACCD = subword(hc11, ACCD, num, flag);	//D = D-M 
	hc11->ACCA=ACCD.high();		//asocio los registros A y B con la variable local D
	hc11->ACCB=ACCD.low();
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  ANDA - Ands ACCA with the memory location
//  
//  Addr. Mode: Indirect X
//  
//  Opcode: A4h
//          164d        
//
//  Clocks: 4  (-1 for fetching opcode)
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_A4h(HC11 *hc11)
{
	hc11->ACCA = hc11->ACCA & hc11->memory[(hc11->IX + hc11->memory[hc11->PC])];	//A = A & M
	hc11->PC++;
	hc11->_clock();

	hc11->_clock();

	if(hc11->ACCA>=128)	//negative
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);

	if(hc11->ACCA==0)	//zero
		hc11->CCR.Z(1);
	else 
		hc11->CCR.Z(0);

	hc11->CCR.V(0);		//overflow
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  BITA - performs an AND on the accumulator and memory location
//
//  Addr. Mode: Indirect X
//  
//  Opcode: A5h
//          165d 
//
//  Clocks: 4  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_A5h(HC11 *hc11)
{
	Byte data;
	Word mem;

	mem=(hc11->IX + hc11->memory[hc11->PC]);
	hc11->PC++;
    hc11->_clock();	

	data = hc11->ACCA & hc11->memory[mem];	//A & M
    hc11->_clock();	


	if(data>=128)		//negative
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);

	if(data==0)			//zero
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);


	hc11->CCR.V(0);		//overflow
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  EORA - XORs the accumulator
//
//  Addr. Mode: Indexed X
//  
//  Opcode: A8h
//          168d 
//
//  Clocks: 4  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_A8h(HC11 *hc11)
{
	Word mem;
	
	mem=(hc11->IX + hc11->memory[hc11->PC]);
	hc11->PC++;	
    hc11->_clock();	

	hc11->ACCA = hc11->ACCA ^ hc11->memory[mem];	//A = A exor M
    hc11->_clock();	
	
	if(hc11->ACCA>=128)	//negative
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);	//habia un fallo, CCR.V(0)

	if(hc11->ACCA==0)	//zero
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);

	hc11->CCR.V(0);		//overflow			
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  ADCA - Adds ACCA, a memory location, and the carry
//         flag and places the result in ACCA
//  
//  Addr. Mode: Indirect X
//  
//  Opcode: A9h
//          169d        
//
//  Clocks: 4  (-1 for fetching opcode)
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  H, N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_A9h(HC11 *hc11)
{
	CCRReg flag=0x2F;
	
	hc11->_clock();
	
	hc11->_clock();

	hc11->ACCA = addbyte(hc11, hc11->ACCA, hc11->memory[(hc11->IX + hc11->memory[hc11->PC])], flag, hc11->CCR.C());	//A=A+M+C
	
	hc11->PC++;
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  ADDA - Adds ACCA and a memory location
//         and places the result in ACCA
//  
//  Addr. Mode: Indirect X
//  
//  Opcode: ABh
//          171d        
//
//  Clocks: 4  (-1 for fetching opcode)
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  H, N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_ABh(HC11 *hc11)
{
	CCRReg flag=0x2F;

	hc11->_clock();

	hc11->_clock();

	hc11->ACCA = addbyte(hc11, hc11->ACCA, hc11->memory[(hc11->IX + hc11->memory[hc11->PC])], flag, 0);		//A=A+M
	hc11->PC++;
	hc11->_clock();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
//
//  SUBA - Subtracts  a memory location and ACCA
//         and places the result in ACCA
//  
//  Addr. Mode: Extended
//  
//  Opcode: B0h
//          176d        
//
//  Clocks: 4  (-1 for fetching opcode)
//
//  OpLength: 3 (-1 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_B0h(HC11 *hc11)
{
	CCRReg flag=0x0F;
	Word mem;
	
	mem.high(hc11->memory[hc11->PC]);	//busca la parte alta de la dir (direccionamiento extendido)
	hc11->PC++;
	hc11->_clock();

	mem.low(hc11->memory[hc11->PC]);	//busca la parte baja de la dir
	hc11->PC++;
	hc11->_clock();

	hc11->ACCA = subbyte(hc11, hc11->ACCA, hc11->memory[mem], flag);	//A=A-M
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  CMPA - Compare
//
//  Addr. Mode: Immediate
//  
//  Opcode: B1h
//          177d 
//
//  Clocks: 4  (-1 for fetching opcode) 
//
//  OpLength: 3 (-1 for fetching opcode)
//
//  Flags:  N, Z   v,c
//
////////////////////////////////////////////////

void HC11::op_B1h(HC11 *hc11)
{
	Word mem;
	CCRReg flag=0x0F;

	mem.high(hc11->memory[hc11->PC]);
	hc11->PC++;
    hc11->_clock();	

	mem.low(hc11->memory[hc11->PC]);
	hc11->PC++;
    hc11->_clock();	

	subbyte(hc11, hc11->ACCA, hc11->memory[mem], flag);		//A-M
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  SBCA - Subtract with carry
//
//  Addr. Mode: Extended
//  
//  Opcode: B2h
//          178d 
//
//  Clocks: 4  (-1 for fetching opcode) 
//
//  OpLength: 3 (-1 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_B2h(HC11 *hc11)
{
	Word mem;
	CCRReg flag=0x0F;

	mem.high(hc11->memory[hc11->PC]);
	hc11->PC++;
    hc11->_clock();	

	mem.low(hc11->memory[hc11->PC]);
	hc11->PC++;
    hc11->_clock();	
	
	hc11->ACCA = hc11->ACCA - hc11->CCR.C();	//resta el carry

	hc11->ACCA = subbyte(hc11, hc11->ACCA, hc11->memory[mem], flag);	//A=A+M+C
	
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  SUBD - Subtracts a memory location from ACCD
//         and places the result in ACCD
//  
//  Addr. Mode: Extended
//  
//  Opcode: B3h
//          179d        
//
//  Clocks: 6  (-1 for fetching opcode)
//
//  OpLength: 3 (-1 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_B3h(HC11 *hc11)
{
	CCRReg flag=0x0F;
	Word mem;
	Word num;
	Word ACCD;
	
	mem.high(hc11->memory[hc11->PC]);
	hc11->PC++;
	hc11->_clock();

	mem.low(hc11->memory[hc11->PC]);
	hc11->PC++;
	hc11->_clock();
	
	ACCD.low(hc11->ACCB);	//asocia A y B a la variable local D
	ACCD.high(hc11->ACCA);
	hc11->_clock();

	num.high(hc11->memory[mem]);
	mem++;
	num.low(hc11->memory[mem]);
	hc11->_clock();

	ACCD = subword(hc11, ACCD, num, flag);	//D=D-M
	hc11->ACCA=ACCD.high();		//asocia la variable local D con los registros A y B
	hc11->ACCB=ACCD.low();
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  ANDA - Ands ACCA with the memory location
//  
//  Addr. Mode: Extended
//  
//  Opcode: B4h
//          180d        
//
//  Clocks: 4  (-1 for fetching opcode)
//
//  OpLength: 3 (-1 for fetching opcode)
//
//  Flags:  N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_B4h(HC11 *hc11)
{
	Word mem;

	mem.high(hc11->memory[hc11->PC]);
	hc11->PC++;
	mem.low(hc11->memory[hc11->PC]);
	hc11->PC++;
	hc11->_clock();

	hc11->ACCA = hc11->ACCA & hc11->memory[mem];	//A= A & M
	hc11->_clock();

	if(hc11->ACCA>=128)	//negative
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);

	if(hc11->ACCA==0)	//zero
		hc11->CCR.Z(1);
	else 
		hc11->CCR.Z(0);

	hc11->CCR.V(0);		//overflow
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  BITA - performs an AND on the accumulator and memory location
//
//  Addr. Mode: Extended
//  
//  Opcode: B5h
//          181d 
//
//  Clocks: 4  (-1 for fetching opcode) 
//
//  OpLength: 3 (-1 for fetching opcode)
//
//  Flags:  N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_B5h(HC11 *hc11)
{
	Byte data;
	Word mem;

	mem.high(hc11->memory[hc11->PC]);
	hc11->PC++;
    hc11->_clock();	

	mem.low(hc11->memory[hc11->PC]);
	hc11->PC++;
    hc11->_clock();	

	data = hc11->ACCA & hc11->memory[mem];	//A & M


	if(data>=128)		//negative
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);

	if(data==0)			//zero
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);

	hc11->CCR.V(0);		//overflow
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  EORA - XORs the accumulator
//
//  Addr. Mode: Extended
//  
//  Opcode: B8h
//          184d 
//
//  Clocks: 4  (-1 for fetching opcode) 
//
//  OpLength: 3 (-1 for fetching opcode)
//
//  Flags:  N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_B8h(HC11 *hc11)
{
	Word mem;
	
	mem.high(hc11->memory[hc11->PC]);
	hc11->PC++;
	mem.low(hc11->memory[hc11->PC]);
	hc11->PC++;	
    hc11->_clock();	

	hc11->ACCA = hc11->ACCA ^ hc11->memory[mem];	//A= A exor M
    hc11->_clock();	
	
	if(hc11->ACCA>=128)		//negative
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);		//tenia fallo

	if(hc11->ACCA==0)		//zero
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);
	
	hc11->CCR.V(0);			//overflow
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  ADCA - Adds ACCA, a memory location, and the carry
//         flag and places the result in ACCA
//  
//  Addr. Mode: Extended
//  
//  Opcode: B9h
//          185d        
//
//  Clocks: 4  (-1 for fetching opcode)
//
//  OpLength: 3 (-1 for fetching opcode)
//
//  Flags:  H, N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_B9h(HC11 *hc11)
{
	CCRReg flag=0x2F;
	Word mem;

	mem.high(hc11->memory[hc11->PC]);
	hc11->PC++;
	hc11->_clock();

	mem.low(hc11->memory[hc11->PC]);        
	hc11->PC++;
	hc11->_clock();

	hc11->ACCA = addbyte(hc11, hc11->ACCA, hc11->memory[mem], flag, hc11->CCR.C());		//A = A+M+C
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  ADDA - Adds ACCA and a memory location
//         and places the result in ACCA
//  
//  Addr. Mode: Extended
//  
//  Opcode: BBh
//          187d        
//
//  Clocks: 4  (-1 for fetching opcode)
//
//  OpLength: 3 (-1 for fetching opcode)
//
//  Flags:  H, N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_BBh(HC11 *hc11)
{
	CCRReg flag=0x2F;
	Word mem;
	
	mem.high(hc11->memory[hc11->PC]);
	hc11->PC++;
	hc11->_clock();

	mem.low(hc11->memory[hc11->PC]);        
	hc11->PC++;
	hc11->_clock();

	hc11->ACCA = addbyte(hc11, hc11->ACCA, hc11->memory[mem], flag, 0);		//A= A+M
	hc11->_clock();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
//
//  SUBB - Subtracts the the passed Byte from ACCB
//         and places the result in ACCB
//  
//  Addr. Mode: Immediate
//  
//  Opcode: C0h
//          208d        
//
//  Clocks: 2  (-1 for fetching opcode)
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_C0h(HC11 *hc11)
{
	CCRReg flag=0x0F;
	
	hc11->ACCB = subbyte(hc11, hc11->ACCB, hc11->memory[hc11->PC], flag);	//B=B-M
	hc11->PC++;
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  CMPB - Compare
//
//  Addr. Mode: Immediate
//  
//  Opcode: C1h
//          209d 
//
//  Clocks: 2  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z  v,c
//
////////////////////////////////////////////////

void HC11::op_C1h(HC11 *hc11)
{
	Byte num;
	CCRReg flag=0x0F;

	num=hc11->memory[hc11->PC];
	hc11->PC++;
	
	subbyte(hc11, hc11->ACCB, num, flag);	//B-M
	
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  SBCB - Subtract with carry
//
//  Addr. Mode: Immediate
//  
//  Opcode: C2h
//          210d 
//
//  Clocks: 2  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_C2h(HC11 *hc11)
{
	Byte num;
	CCRReg flag=0x0F;

	num=hc11->memory[hc11->PC];
	hc11->PC++;
	num = num - hc11->CCR.C();	//resta el carry

	hc11->ACCB = subbyte(hc11, hc11->ACCB, num, flag);	//B=B-M-C
	
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  ADDD - Adds ACCD and the the passed Word
//         and places the result in ACCD
//  
//  Addr. Mode: Immediate
//  
//  Opcode: C3h
//          195d        
//
//  Clocks: 4  (-1 for fetching opcode)
//
//  OpLength: 3 (-1 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_C3h(HC11 *hc11)
{
	CCRReg flag=0x0F;
	Word num;
	
	num.high(hc11->memory[hc11->PC]);
	hc11->PC++;
	hc11->_clock();

	num.low(hc11->memory[hc11->PC]);
	hc11->PC++;
	hc11->_clock();
	
	hc11->ACCB = addbyte(hc11, hc11->ACCB, num.low(), flag, 0);
	hc11->ACCA = addbyte(hc11, hc11->ACCA, num.high(), flag, hc11->CCR.C());	//D=D+M
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  ANDB - Ands ACCB with the passed parameter
//  
//  Addr. Mode: Immediate
//  
//  Opcode: C4h
//          196d        
//
//  Clocks: 2  (-1 for fetching opcode)
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_C4h(HC11 *hc11)
{
	hc11->ACCB = hc11->ACCB & hc11->memory[hc11->PC];	//B= B & M
	hc11->PC++;

	if(hc11->ACCB>=128)		//negative
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);

	if(hc11->ACCB==0)		//zero
		hc11->CCR.Z(1);
	else 
		hc11->CCR.Z(0);

	hc11->CCR.V(0);		//overflow
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  BITB - performs an AND on the accumulator and memory location
//
//  Addr. Mode: Immediate
//  
//  Opcode: C5h
//          197d 
//
//  Clocks: 2  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_C5h(HC11 *hc11)
{
	Byte data;

	data = hc11->ACCB & hc11->memory[hc11->PC];	//B & M
	hc11->PC++;

	if(data>=128)		//negative
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);

	if(data==0)			//zero
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);


	hc11->CCR.V(0);		//overflow
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  EORB - XORs all accumulator
//
//  Addr. Mode: Immediate
//  
//  Opcode: C8h
//          200d 
//
//  Clocks: 2  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_C8h(HC11 *hc11)
{
	hc11->ACCB = hc11->ACCB ^ hc11->memory[hc11->PC];	//B = B exor M
	hc11->PC++;
	


	if(hc11->ACCB>=128)		//negative
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);		//tenia fallo

	if(hc11->ACCB==0)		//zero
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);
	
	hc11->CCR.V(0);		//overflow
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  ADCB - Adds ACCB, the passed Byte, and the carry
//         flag and places the result in ACCB
//  
//  Addr. Mode: Immediate
//  
//  Opcode: C9h
//          201d        
//
//  Clocks: 2  (-1 for fetching opcode)
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  H, N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_C9h(HC11 *hc11)
{
	CCRReg flag=0x2F;
	
	hc11->ACCB = addbyte(hc11, hc11->ACCB, hc11->memory[hc11->PC], flag, hc11->CCR.C());	//B=B+M+C
	hc11->PC++;
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  ADDB - Adds ACCB and the passed Word
//         and places the result in ACCB
//  
//  Addr. Mode: Immediate
//  
//  Opcode: CBh
//          203d        
//
//  Clocks: 2  (-1 for fetching opcode)
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  H, N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_CBh(HC11 *hc11)
{
	CCRReg flag=0x2F;

	hc11->ACCB = addbyte(hc11, hc11->ACCB, hc11->memory[hc11->PC], flag, 0);	//B= B+M
	hc11->PC++;
	hc11->_clock();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
//
//  SUBB - Subtracts a memory locationn from ACCB
//         and places the result in ACCB
//  
//  Addr. Mode: Direct
//  
//  Opcode: D0h
//          208d        
//
//  Clocks: 3  (-1 for fetching opcode)
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_D0h(HC11 *hc11)
{
	CCRReg flag=0x0F;
	Word mem;

	mem.high(0);						//direccionamiento directo, parte alta de la dir es 0
	mem.low(hc11->memory[hc11->PC]);
	hc11->PC++;
	hc11->_clock();

	hc11->ACCB = subbyte(hc11, hc11->ACCB, hc11->memory[mem], flag);	//B= B-M
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  CMPB - Compare
//
//  Addr. Mode: Direct
//  
//  Opcode: D1h
//          209d 
//
//  Clocks: 3  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z,  v,c
//
////////////////////////////////////////////////

void HC11::op_D1h(HC11 *hc11)
{
	Word mem;
	CCRReg flag=0x0F;

	mem.low(hc11->memory[hc11->PC]);
	mem.high(0);
	hc11->PC++;
    hc11->_clock();	
	
	subbyte(hc11, hc11->ACCB, hc11->memory[mem], flag);		//B-M
	
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  SBCB - Subtract with carry
//
//  Addr. Mode: Direct
//  
//  Opcode: D2h
//          210d 
//
//  Clocks: 3  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_D2h(HC11 *hc11)
{
	Word mem;
	CCRReg flag=0x0F;

	mem.low(hc11->memory[hc11->PC]);
	mem.high(0);
	hc11->PC++;

	hc11->ACCB = hc11->ACCB - hc11->CCR.C();	//resta el carry

	hc11->ACCB = subbyte(hc11, hc11->ACCB, hc11->memory[hc11->PC], flag);	//B=B-M-C
	
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  ADDD - Adds ACCD and a memory location
//         and places the result in ACCD
//  
//  Addr. Mode: Direct
//  
//  Opcode: D3h
//          211d        
//
//  Clocks: 5  (-1 for fetching opcode)
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_D3h(HC11 *hc11)
{
	CCRReg flag=0x0F;
	Word mem;

	mem.low(hc11->memory[hc11->PC]);	//mem es la direccion donde está el dato
	mem.high(0);
	hc11->PC++;
	hc11->_clock();
	
	hc11->_clock();
	
	hc11->_clock();

	mem++;		//para acceder a la parte baja
	hc11->ACCB = addbyte(hc11, hc11->ACCB, hc11->memory[mem], flag, 0);
	
	mem--;		//para acceder a la parte alta
	hc11->ACCA = addbyte(hc11, hc11->ACCA, hc11->memory[mem], flag, hc11->CCR.C());		//D=D+M
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  ANDB - Ands ACCB with the memory location
//  
//  Addr. Mode: Direct
//  
//  Opcode: D4h
//          212d        
//
//  Clocks: 3  (-1 for fetching opcode)
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_D4h(HC11 *hc11)
{
	Word mem;

	mem.low(hc11->memory[hc11->PC]);
	mem.high(0);
	hc11->PC++;

	hc11->ACCB = hc11->ACCB & hc11->memory[mem];	//B = B & M
	hc11->_clock();

	if(hc11->ACCB>=128)		//negative
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);

	if(hc11->ACCB==0)		//zero
		hc11->CCR.Z(1);
	else 
		hc11->CCR.Z(0);

	hc11->CCR.V(0);			//overflow
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  BITB - performs an AND on the accumulator and memory location
//
//  Addr. Mode: Direct
//  
//  Opcode: D5h
//          213d 
//
//  Clocks: 3  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_D5h(HC11 *hc11)
{
	Byte data;
	Word mem;

	mem.low(hc11->memory[hc11->PC]);
	mem.high(0);
	hc11->PC++;
    hc11->_clock();	

	data = hc11->ACCB & hc11->memory[mem];	//B & M

	if(data>=128)		//negative
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);

	if(data==0)			//zero
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);

	hc11->CCR.V(0);		//overflow
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  EORB - XORs the accumulator
//
//  Addr. Mode: Direct
//  
//  Opcode: D8h
//          216d 
//
//  Clocks: 3  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_D8h(HC11 *hc11)
{
	Word mem;
	
	mem.low(hc11->memory[hc11->PC]);
	mem.high(0);
	hc11->PC++;
    hc11->_clock();	


	hc11->ACCB = hc11->ACCB ^ hc11->memory[mem];	//B = B exor M

	
	if(hc11->ACCB>=128)		//negative
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);		//estaba mal

	if(hc11->ACCB==0)		//zero
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);
	
	hc11->CCR.V(0);			//overflow
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  ADCB - Adds ACCB, a memory location, and the carry
//         flag and places the result in ACCB
//  
//  Addr. Mode: Direct
//  
//  Opcode: D9h
//          217d        
//
//  Clocks: 3  (-1 for fetching opcode)
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  H, N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_D9h(HC11 *hc11)
{
	CCRReg flag=0x2F;
	Word mem;

	mem.high(0);
	mem.low(hc11->memory[hc11->PC]);        
	hc11->PC++;
	hc11->_clock();

	hc11->ACCB = addbyte(hc11, hc11->ACCB, hc11->memory[mem], flag, hc11->CCR.C());		//B = B+M+C
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  ADDB - Adds ACCB and a memory location
//         and places the result in ACCB
//  
//  Addr. Mode: Direct
//  
//  Opcode: DBh
//          219d        
//
//  Clocks: 3  (-1 for fetching opcode)
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  H, N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_DBh(HC11 *hc11)
{
	CCRReg flag=0x2F;
	Word mem;

	mem.high(0);
	mem.low(hc11->memory[hc11->PC]);
	hc11->PC++;
	hc11->_clock();

	hc11->ACCB = addbyte(hc11, hc11->ACCB, hc11->memory[mem], flag, 0);		//B = B+M
	hc11->_clock();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
//
//  SUBB - Subtract a memory location from ACCB
//         and places the result in ACCB
//  
//  Addr. Mode: Indirect X  indexado X
//  
//  Opcode: E0h
//          224d        
//
//  Clocks: 4  (-1 for fetching opcode)
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_E0h(HC11 *hc11)
{
	CCRReg flag=0x0F;

	hc11->_clock();

	hc11->_clock();

	hc11->ACCB = subbyte(hc11, hc11->ACCB, hc11->memory[(hc11->IX + hc11->memory[hc11->PC])], flag);	//B=B-M
	hc11->PC++;
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  CMPB - Compare
//
//  Addr. Mode: Indirect X  indexado X
//  
//  Opcode: E1h
//          225d 
//
//  Clocks: 4  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z  v,c
//
////////////////////////////////////////////////

void HC11::op_E1h(HC11 *hc11)
{
	Word mem;
	CCRReg flag=0x0F;

	mem=(hc11->IX + hc11->memory[hc11->PC]);
	hc11->PC++;
    hc11->_clock();	

    hc11->_clock();	

	subbyte(hc11, hc11->ACCB, hc11->memory[mem], flag);		//B-M
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  SBCB - Subtract with carry
//
//  Addr. Mode: Indexed X
//  
//  Opcode: E2h
//          226d 
//
//  Clocks: 4  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_E2h(HC11 *hc11)
{
	Word mem;
	CCRReg flag=0x0F;

	mem=(hc11->IX + hc11->memory[hc11->PC]);
	hc11->PC++;
    hc11->_clock();	

	hc11->ACCB = hc11->ACCB - hc11->CCR.C();	//resto el carry
    hc11->_clock();	

	hc11->ACCB = subbyte(hc11, hc11->ACCB, hc11->memory[mem], flag);	//B=B-M-C
	
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  ADDD - Adds ACCD and a memory location
//         and places the result in ACCD
//  
//  Addr. Mode: Indirect X   indexed X   
//  
//  Opcode: E3h
//          227d        
//
//  Clocks: 6  (-1 for fetching opcode)
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_E3h(HC11 *hc11)
{
	CCRReg flag=0x0F;
	Word mem;

	mem=hc11->IX + hc11->memory[hc11->PC];
	hc11->PC++;
	hc11->_clock();

	hc11->_clock();
	
	hc11->_clock();

	hc11->_clock();

	mem++;		//accede a la parte baja de la dir
	hc11->ACCB = addbyte(hc11, hc11->ACCB, hc11->memory[mem], flag, 0);
	
	mem--;		//accede a la parte alta de la dir
	hc11->ACCA = addbyte(hc11, hc11->ACCA, hc11->memory[mem], flag, hc11->CCR.C());		//D=D+M
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  ANDB - Ands ACCB with the memory location
//  
//  Addr. Mode: Indirect X   indexed X
//  
//  Opcode: E4h
//          228d        
//
//  Clocks: 4  (-1 for fetching opcode)
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_E4h(HC11 *hc11)
{

	hc11->ACCB = hc11->ACCB & hc11->memory[(hc11->IX + hc11->memory[hc11->PC])];	//B = B & M
	hc11->PC++;
	hc11->_clock();

	hc11->_clock();

	if(hc11->ACCB>=128)		//negative
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);

	if(hc11->ACCB==0)		//zero
		hc11->CCR.Z(1);
	else 
		hc11->CCR.Z(0);

	hc11->CCR.V(0);			//overflow
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  BITB - performs an AND on the accumulator and memory location
//
//  Addr. Mode: Indirect X   indexed X
//  
//  Opcode: E5h
//          229d 
//
//  Clocks: 4  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_E5h(HC11 *hc11)
{
	Byte data;
	Word mem;

	mem=(hc11->IX + hc11->memory[hc11->PC]);
	hc11->PC++;
    hc11->_clock();	

	data = hc11->ACCB & hc11->memory[mem];		//B & M
    hc11->_clock();	

	
	if(data>=128)		//negative
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);

	if(data==0)			//zero
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);

	hc11->CCR.V(0);		//overflow
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  EORB - XORs the accumulator
//
//  Addr. Mode: Indexed X
//  
//  Opcode: E8h
//          232d 
//
//  Clocks: 4  (-1 for fetching opcode) 
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_E8h(HC11 *hc11)
{
	Word mem;
	
	mem=(hc11->IX + hc11->memory[hc11->PC]);
	hc11->PC++;	
    hc11->_clock();	


	hc11->ACCB = hc11->ACCB ^ hc11->memory[mem];	//B = B exor M
    hc11->_clock();	

	
	if(hc11->ACCB>=128)		//negative
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);		//tenia fallo

	if(hc11->ACCB==0)		//zero
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);
	
	hc11->CCR.V(0);			//overflow
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  ADCB - Adds ACCB, a memory location, and the carry
//         flag and places the result in ACCB
//  
//  Addr. Mode: Indirect X   indexado X
//  
//  Opcode: E9h
//          233d        
//
//  Clocks: 4  (-1 for fetching opcode)
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  H, N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_E9h(HC11 *hc11)
{
	CCRReg flag=0x2F;

	hc11->_clock();
	
	hc11->_clock();

	hc11->ACCB = addbyte(hc11, hc11->ACCB, hc11->memory[(hc11->IX + hc11->memory[hc11->PC])], flag, hc11->CCR.C());	//B=B+M+C
	
	hc11->PC++;
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  ADDB - Adds ACCB and a memory location
//         and places the result in ACCB
//  
//  Addr. Mode: Indirect X   indexado X
//  
//  Opcode: EBh
//          235d        
//
//  Clocks: 4  (-1 for fetching opcode)
//
//  OpLength: 2 (-1 for fetching opcode)
//
//  Flags:  H, N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_EBh(HC11 *hc11)
{
	CCRReg flag=0x2F;

	hc11->_clock();

	hc11->_clock();

	hc11->ACCB = addbyte(hc11, hc11->ACCB, hc11->memory[(hc11->IX + hc11->memory[hc11->PC])], flag, 0);		//B=B+M
	hc11->PC++;
	hc11->_clock();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
//
//  SUBB - Subtracts a memory location and ACCB
//         and places the result in ACCB
//  
//  Addr. Mode: Extended
//  
//  Opcode: F0h
//          240d        
//
//  Clocks: 4  (-1 for fetching opcode)
//
//  OpLength: 3 (-1 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_F0h(HC11 *hc11)
{
	CCRReg flag=0x0F;
	Word mem;

	mem.high(hc11->memory[hc11->PC]);
	hc11->PC++;
	hc11->_clock();

	mem.low(hc11->memory[hc11->PC]);        
	hc11->PC++;
	hc11->_clock();

	hc11->ACCB = subbyte(hc11, hc11->ACCB, hc11->memory[mem], flag);	//B=B-M
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  CMPB - Compare
//
//  Addr. Mode: Immediate no,  Extendido
//  
//  Opcode: F1h
//          241d 
//
//  Clocks: 4  (-1 for fetching opcode) 
//
//  OpLength: 3 (-1 for fetching opcode)
//
//  Flags:  N, Z,  v,c
//
////////////////////////////////////////////////

void HC11::op_F1h(HC11 *hc11)
{
	Word mem;
	CCRReg flag=0x0F;

	mem.high(hc11->memory[hc11->PC]);
	hc11->PC++;
    hc11->_clock();	

	mem.low(hc11->memory[hc11->PC]);
	hc11->PC++;
    hc11->_clock();	

	subbyte(hc11, hc11->ACCB, hc11->memory[mem], flag);		//B-M
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  SBCB - Subtract with carry
//
//  Addr. Mode: Extended
//  
//  Opcode: F2h
//          242d 
//
//  Clocks: 4  (-1 for fetching opcode) 
//
//  OpLength: 3 (-1 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_F2h(HC11 *hc11)
{
	Word mem;
	CCRReg flag=0x0F;

	mem.high(hc11->memory[hc11->PC]);
	hc11->PC++;
    hc11->_clock();	

	mem.low(hc11->memory[hc11->PC]);
	hc11->PC++;
    hc11->_clock();	
	
	hc11->ACCB = hc11->ACCB - hc11->CCR.C();	//resta carry

	hc11->ACCB = subbyte(hc11, hc11->ACCB, hc11->memory[mem], flag);	//B=B-M  -C
	
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  ADDD - Adds ACCD and a memory location
//         and places the result in ACCD
//  
//  Addr. Mode: Extended
//  
//  Opcode: F3h
//          243d        
//
//  Clocks: 6  (-1 for fetching opcode)
//
//  OpLength: 3 (-1 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_F3h(HC11 *hc11)
{
	CCRReg flag=0x0F;
	Word mem;

	mem.high(hc11->memory[hc11->PC]);		//direccionamiento extendido
	hc11->PC++;
	hc11->_clock();

	mem.low(hc11->memory[hc11->PC]);
	hc11->PC++;
	hc11->_clock();
	
	hc11->_clock();

	hc11->_clock();

	mem++;		//accede a la parte baja de la dir
	hc11->ACCB = addbyte(hc11, hc11->ACCB, hc11->memory[mem], flag, 0);
	
	mem--;		//accede a la parte alta de la dir
	hc11->ACCA = addbyte(hc11, hc11->ACCA, hc11->memory[mem], flag, hc11->CCR.C());		//D=D+M
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  ANDB - Ands ACCB with the memory location
//  
//  Addr. Mode: Extended
//  
//  Opcode: F4h
//          244d        
//
//  Clocks: 4  (-1 for fetching opcode)
//
//  OpLength: 3 (-1 for fetching opcode)
//
//  Flags:  N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_F4h(HC11 *hc11)
{
	Word mem;

	mem.high(hc11->memory[hc11->PC]);
	hc11->PC++;
	
	mem.low(hc11->memory[hc11->PC]);
	hc11->PC++;
	hc11->_clock();

	hc11->ACCB = hc11->ACCB & hc11->memory[mem];	//B = B & M
	hc11->_clock();

	if(hc11->ACCB>=128)		//negative
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);

	if(hc11->ACCB==0)		//zero
		hc11->CCR.Z(1);
	else 
		hc11->CCR.Z(0);

	hc11->CCR.V(0);		//overflow
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  BITB - performs an AND on the accumulator and memory location
//
//  Addr. Mode: Extended
//  
//  Opcode: F5h
//          245d 
//
//  Clocks: 4  (-1 for fetching opcode) 
//
//  OpLength: 3 (-1 for fetching opcode)
//
//  Flags:  N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_F5h(HC11 *hc11)
{
	Byte data;
	Word mem;

	mem.high(hc11->memory[hc11->PC]);
	hc11->PC++;
    hc11->_clock();	

	mem.low(hc11->memory[hc11->PC]);
	hc11->PC++;
    hc11->_clock();	

	data = hc11->ACCB & hc11->memory[mem];	//B & M

	if(data>=128)		//negative
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);

	if(data==0)			//zero
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);

	hc11->CCR.V(0);		//overflow
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  EORB - XORs the accumulator
//
//  Addr. Mode: Extended
//  
//  Opcode: F8h
//          248d 
//
//  Clocks: 4  (-1 for fetching opcode) 
//
//  OpLength: 3 (-1 for fetching opcode)
//
//  Flags:  N, Z, V=0
//
////////////////////////////////////////////////

void HC11::op_F8h(HC11 *hc11)
{
	Word mem;
	
	mem.high(hc11->memory[hc11->PC]);
	hc11->PC++;

	mem.low(hc11->memory[hc11->PC]);
	hc11->PC++;	
    hc11->_clock();	

	hc11->ACCB = hc11->ACCB ^ hc11->memory[mem];	//B = B exor M
    hc11->_clock();	
		
	if(hc11->ACCB>=128)		//negative
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);		//estaba mal

	if(hc11->ACCB==0)		//zero
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);
	
	hc11->CCR.V(0);		//overflow

    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  ADCB - Adds ACCB, a memory location, and the carry
//         flag and places the result in ACCB
//  
//  Addr. Mode: Extended
//  
//  Opcode: F9h
//          249d        
//
//  Clocks: 4  (-1 for fetching opcode)
//
//  OpLength: 3 (-1 for fetching opcode)
//
//  Flags:  H, N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_F9h(HC11 *hc11)
{
	CCRReg flag=0x2F;
	Word mem;

	mem.high(hc11->memory[hc11->PC]);
	hc11->PC++;
	hc11->_clock();

	mem.low(hc11->memory[hc11->PC]);        
	hc11->PC++;
	hc11->_clock();

	hc11->ACCB = addbyte(hc11, hc11->ACCB, hc11->memory[mem], flag, hc11->CCR.C());		//B=B+M+C
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  ADDB - Adds ACCB and a memory location
//         and places the result in ACCB
//  
//  Addr. Mode: Extended
//  
//  Opcode: FBh
//          251d        
//
//  Clocks: 4  (-1 for fetching opcode)
//
//  OpLength: 3 (-1 for fetching opcode)
//
//  Flags:  H, N, Z, V, C
//
////////////////////////////////////////////////

void HC11::op_FBh(HC11 *hc11)
{
	CCRReg flag=0x2F;
	Word mem;

	mem.high(hc11->memory[hc11->PC]);
	hc11->PC++;
	hc11->_clock();

	mem.low(hc11->memory[hc11->PC]);        
	hc11->PC++;
	hc11->_clock();

	hc11->ACCB = addbyte(hc11, hc11->ACCB, hc11->memory[mem], flag, 0);		//B=B+M
	hc11->_clock();
}



















//funciones del segundo array de 256, para las 64 instrucciones extendidas de 0x18, aqui aparecen 32


/////////////////////////////////////////////////
//
//  INY - Increment the Y register
//  
//  Addr. Mode: Inherent
//  
//  Opcode: 18h 08h
//          
//
//  Clocks: 4  (-2 for fetching opcode)	
//
//  OpLength: 2 (-2 for fetching opcode)
//
//  Flags:  Z
//
////////////////////////////////////////////////

void HC11::opIY_8h(HC11 *hc11)
{
	//CCRReg flag=0x04;
	
	//hc11->IY=addword(hc11,hc11->IY,1,flag,0);
	
	hc11->IY++;

	if (hc11->IY == 0)
		hc11->CCR.Z(1);
	else 
		hc11->CCR.Z(0);

	hc11->_clock();

	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  DEY - Decrement the Y register
//  
//  Addr. Mode: Inherent
//  
//  Opcode: 18h 09h
//               
//
//  Clocks: 4  (-2 for fetching opcode)
//
//  OpLength: 2 (-2 for fetching opcode)
//
//  Flags:  Z
//
////////////////////////////////////////////////

void HC11::opIY_9h(HC11 *hc11)
{
	CCRReg flag=0x04;
	
	hc11->IY = subword(hc11, hc11->IY, 1, flag);
	
	hc11->_clock();

	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  BSET - Sets all the bits in the mask
//
//  Addr. Mode: Indexado Y
//  
//  Opcode: 18h 1Ch
//          
//
//  Clocks: 8  (-2 for fetching opcode) 
//
//  OpLength: 4 (-2 for fetching opcode)
//
//  Flags:  N, Z, V=0
//
////////////////////////////////////////////////

void HC11::opIY_1Ch(HC11 *hc11)
{
    Word mem;
	Byte data;

	mem=(hc11->IY + hc11->memory[hc11->PC]);
	hc11->PC++;
    hc11->_clock();

	data = hc11->memory[mem];
    hc11->_clock();

	data = data | hc11->memory[hc11->PC];	//pone a 1 segun la mascara
	hc11->PC++;
    hc11->_clock();
	
	if(data==0)
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);
    hc11->_clock();

	if(data>=128)
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);
    hc11->_clock();

	hc11->CCR.V(0);

	hc11->memory.Write(mem,data);
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  BCLR - Clears all the bits in the mask
//
//  Addr. Mode: Indexado Y
//  
//  Opcode: 18h 1Dh
//          
//
//  Clocks: 8  (-2 for fetching opcode) 
//
//  OpLength: 4 (-2 for fetching opcode)
//
//  Flags:  N, Z, V=0
//
////////////////////////////////////////////////

void HC11::opIY_1Dh(HC11 *hc11)
{
    Word mem;
	Byte data;

	mem=(hc11->IY + hc11->memory[hc11->PC]);
	hc11->PC++;
    hc11->_clock();

	data=hc11->memory[mem];
    hc11->_clock();

	data = data & ~(hc11->memory[hc11->PC]);	//pone a 0 segun la mascara
	hc11->PC++;
    hc11->_clock();
	
	if(data==0)
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);
    hc11->_clock();

	if(data>=128)
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);
    hc11->_clock();

	hc11->CCR.V(0);
	hc11->memory.Write(mem, data);
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  BRSET - Branches mask bits are set
//
//  Addr. Mode: indexado Y
//  
//  Opcode: 18h 1Eh
//          
//
//  Clocks: 8  (-2 for fetching opcode) 
//
//  OpLength: 5 (-2 for fetching opcode)
//
//  Flags:  None
//
////////////////////////////////////////////////

void HC11::opIY_1Eh(HC11 *hc11)
{
	signed char offset;
	Word mem;

	mem=(hc11->IY + hc11->memory[hc11->PC]);
	hc11->PC++;
	hc11->_clock();	

	if((~(hc11->memory[mem]) & hc11->memory[hc11->PC])==0)	
	{
		hc11->PC++;
		offset = (signed)hc11->memory[hc11->PC];
		hc11->PC++;
	    hc11->PC = hc11->PC + offset;
    }
	else
	{
		hc11->PC++;
		hc11->PC++;
	}

	hc11->_clock();	
	hc11->_clock();	
	hc11->_clock();	
	hc11->_clock();	
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  BRCLR - Branches mask bits are clear
//
//  Addr. Mode: indexado Y
//  
//  Opcode: 18h 1Fh
//          
//
//  Clocks: 8  (-2 for fetching opcode) 
//
//  OpLength: 5 (-2 for fetching opcode)
//
//  Flags:  None
//
////////////////////////////////////////////////

void HC11::opIY_1Fh(HC11 *hc11)
{
	signed char offset;
	Word mem;

	mem=(hc11->IY + hc11->memory[hc11->PC]);
	hc11->PC++;
	hc11->_clock();	

	if((hc11->memory[mem] & hc11->memory[hc11->PC])==0)	
	{
		hc11->PC++;
		offset = (signed)hc11->memory[hc11->PC];
		hc11->PC++;
	    hc11->PC = hc11->PC + offset;
    }
	else
		{
		hc11->PC++;
		hc11->PC++;
		}

	hc11->_clock();	
	hc11->_clock();	
	hc11->_clock();	
	hc11->_clock();	
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  ABY - Adds ACCB to IY and place result in IY
//  
//  Addr. Mode: Inherent
//
//  Opcode: 18h 3Ah
//            
//
//  Clocks: 4  (-2 for fetching both opcodes)
//
//  OpLength: 2 (-2 for fetching opcode)
//
//  Flags:  None
//
////////////////////////////////////////////////

void HC11::opIY_3Ah(HC11 *hc11)
{
	CCRReg flag=0x00;
	
	Word wd_accb;   //so can add two words
	
	wd_accb.low(hc11->ACCB);
	wd_accb.high(0);
	hc11->_clock();
	
	hc11->IY = addword(hc11, hc11->IY, wd_accb, flag, 0);	// Y = Y+B
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  COM - Complement
//
//  Addr. Mode: Indexado Y
//  
//  Opcode: 18h 63h
//                 
//
//  Clocks: 7  (-2 for fetching opcode)
//
//  OpLength: 3 (-2 for fetching opcode)
//
//  Flags:  N, Z, V=0, C=1
//
////////////////////////////////////////////////

void HC11::opIY_63h(HC11 *hc11)
{
	CCRReg flag=0x0C;	// 0000 1100
	Word mem;
	Byte result;

	mem=(hc11->IY + hc11->memory[hc11->PC]);
	hc11->PC++;
	hc11->_clock();

	result = subbyte(hc11, 0xFF, hc11->memory[mem], flag);		// M = $FF - M
	hc11->_clock();
	
	hc11->memory.Write(mem, result);
	hc11->_clock();

	hc11->_clock();

	hc11->CCR.V(0);
	hc11->CCR.C(1);	
	hc11->_clock();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
//
//  LSR - Logical shift right
//
//  Addr. Mode: Indexado Y
//  
//  Opcode: 18h 64h
//              
//
//  Clocks: 7  (-2 for fetching opcode)
//
//  OpLength: 3 (-2 for fetching opcode)
//
//  Flags:  N=0, Z, V, C
//
////////////////////////////////////////////////

void HC11::opIY_64h(HC11 *hc11)
{
	Word mem;
	Byte num;

	mem=(hc11->IY + hc11->memory[hc11->PC]);
	hc11->PC++;
	hc11->_clock();

	num=hc11->memory[mem];
	hc11->_clock();

	hc11->CCR.C(0x01 & num);	//el bit0 pasa a ser el carry

	num = num >> 1;		//desplaza a la derecha 
	hc11->_clock();

	if(num==0)
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);

	hc11->CCR.V(hc11->CCR.C());		//seria N exor C, pero al ser N=0, solo miro C
	hc11->_clock();

	hc11->memory.Write(mem, num);
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  ASR - Shifts all bits to the right.  The LSB shifts
//         into the carry flag, and the MSB goes to itself.
//  
//  Addr. Mode: Indexado Y
//  
//  Opcode: 18h 67h
//              
//
//  Clocks: 7  (-2 for fetching opcode)
//
//  OpLength: 3 (-2 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::opIY_67h(HC11 *hc11)
{
	Byte num = (hc11->memory[(hc11->IY + hc11->memory[hc11->PC])]);
	hc11->_clock();

	hc11->_clock();

	if(num & 0x1)
		hc11->CCR.C(1);
	else
		hc11->CCR.C(0);

	hc11->_clock();

	num = num>>1;	//desplaza a la derecha
	hc11->_clock();

	if(num==0)
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);
	
	if(num & 0x40)		//como el bit7 se tiene k realimentar, miro si el bit6 es 1 despues de desplazar
	{
		num = num | 0x80;	
		hc11->CCR.N(1);
	}
	else
		hc11->CCR.N(0);
	
	if((hc11->CCR.N()==0 && hc11->CCR.C()==1) || (hc11->CCR.N()==1 && hc11->CCR.C()==0))	//overflow: N exor C	
		hc11->CCR.V(1);
	else
		hc11->CCR.V(0);

	hc11->memory.Write((hc11->IY + hc11->memory[hc11->PC]), num);	//estaba mal, ponia IX
	hc11->PC++;
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  ASL - Shifts all bits to the left.  The MSB shifts
//         into the carry flag, and a zero shifts into LSB.
//  
//  Addr. Mode: Indexado Y
//  
//  Opcode: 18h 68h
//           
//
//  Clocks: 7  (-2 for fetching opcode)
//
//  OpLength: 3 (-2 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::opIY_68h(HC11 *hc11)
{
	Byte num = (hc11->memory[(hc11->IY + hc11->memory[hc11->PC])]);
	hc11->_clock();

	hc11->_clock();

	if(num>=128)
		hc11->CCR.C(1);
	else
		hc11->CCR.C(0);

	if((num.bit6()==1 && num.bit7()==0) || (num.bit6()==0 && num.bit7()==1))	//overflow: N exor C
		hc11->CCR.V(1);
	else
		hc11->CCR.V(0);

	hc11->_clock();

	num = num<<1;	//desplaza a la izquierda
	hc11->_clock();

	if(num==0)
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);
	
	if(num>=128)
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);

	hc11->memory.Write((hc11->IY + hc11->memory[hc11->PC]), num);
	hc11->PC++;
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  DEC - Decrement the memory location
//  
//  Addr. Mode: Ind Y
//  
//  Opcode: 18h 6Ah
//                  
//
//  Clocks: 7  (-2 for fetching opcode)
//
//  OpLength: 3 (-2 for fetching opcode)
//
//  Flags:  N, Z, V
//
////////////////////////////////////////////////

void HC11::opIY_6Ah(HC11 *hc11)
{
	Word mem;
	Byte data;
	CCRReg flag=0x0E;	// 0000 1110
	
	mem=(hc11->IY + hc11->memory[hc11->PC]);
	hc11->PC++;
	hc11->_clock();

	hc11->_clock();
	
	data = hc11->memory[mem];
	hc11->_clock();

	data = subbyte(hc11, data, 1, flag);	// M = M-1
	hc11->_clock();
	
	hc11->memory.Write(mem, data);
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  INC - Increment the memory location
//  
//  Addr. Mode: Ind Y
//  
//  Opcode: 18h 6Ch
//                 
//
//  Clocks: 7  (-2 for fetching opcode)
//
//  OpLength: 3 (-2 for fetching opcode)
//
//  Flags:  N, Z, V
//
////////////////////////////////////////////////

void HC11::opIY_6Ch(HC11 *hc11)
{
	Word mem;
	Byte data;
	CCRReg flag=0x0E;
	
	mem=(hc11->IY + hc11->memory[hc11->PC]);
	hc11->PC++;
	hc11->_clock();

	hc11->_clock();
	
	data = hc11->memory[mem];
	hc11->_clock();

	data = addbyte(hc11, data, 1, flag, 0);		//M = M+1
	hc11->_clock();
	
	hc11->memory.Write(mem, data);
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  CLR - Clears the memory location
//
//  Addr. Mode: Indexed Y
//  
//  Opcode: 18h 6Fh
//          
//
//  Clocks: 7  (-2 for fetching opcode) 
//
//  OpLength: 3 (-2 for fetching opcode)
//
//  Flags:  N=0, Z=1, V=0, C=0
//
////////////////////////////////////////////////

void HC11::opIY_6Fh(HC11 *hc11)
{
	Word mem;

	mem = (hc11->IY + hc11->memory[hc11->PC]);
	hc11->PC++;
    hc11->_clock();	

    hc11->_clock();	

	hc11->memory.Write(mem, 0);		// M=0
    hc11->_clock();	

	hc11->CCR.N(0);
	hc11->CCR.Z(1);
	hc11->CCR.V(0);
	hc11->CCR.C(0);
    hc11->_clock();	
			
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  SUBA - Subtracts a memory location from ACCA
//         and places the result in ACCA
//  
//  Addr. Mode: Ind Y
//  
//  Opcode: 18h A0h
//            
//
//  Clocks: 5  (-2 for fetching both opcodes)
//
//  OpLength: 3 (-2 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::opIY_A0h(HC11 *hc11)
{
	CCRReg flag=0x0F;	//0000 1111

	hc11->_clock();

	hc11->_clock();
	
	hc11->ACCA = subbyte(hc11, hc11->ACCA, hc11->memory[(hc11->IY + hc11->memory[hc11->PC])], flag);	//A=A-M
	hc11->PC++;
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  CMPA - Compare
//
//  Addr. Mode: Ind Y
//  
//  Opcode: 18h A1h
//          
//
//  Clocks: 5  (-2 for fetching opcode) 
//
//  OpLength: 3 (-2 for fetching opcode)
//
//  Flags:  N, Z
//
////////////////////////////////////////////////

void HC11::opIY_A1h(HC11 *hc11)
{
	Word mem;
	CCRReg flag=0x0F;

	mem = (hc11->IY + hc11->memory[hc11->PC]);
	hc11->PC++;
    hc11->_clock();	

    hc11->_clock();	

	subbyte(hc11, hc11->ACCA, hc11->memory[mem], flag);		// A-M
    hc11->_clock();	
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
//
//  SBCA - Subtract with carry
//
//  Addr. Mode: Indexed Y
//  
//  Opcode: 18h A2h
//          
//
//  Clocks: 5  (-2 for fetching opcode) 
//
//  OpLength: 3 (-2 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::opIY_A2h(HC11 *hc11)
{
	Word mem;
	CCRReg flag=0x0F;

	mem = (hc11->IY + hc11->memory[hc11->PC]);
	hc11->PC++;
    hc11->_clock();	

	hc11->ACCA = hc11->ACCA - hc11->CCR.C();
    hc11->_clock();	

	hc11->ACCA = subbyte(hc11, hc11->ACCA, hc11->memory[mem], flag);	//A = A-M-C
	
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  SUBD - Subtracts a memory location from ACCD
//         and places the result in ACCD
//  
//  Addr. Mode: Ind Y
//  
//  Opcode: 18h A3h
//                  
//
//  Clocks: 7  (-2 for fetching opcode)
//
//  OpLength: 3 (-2 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::opIY_A3h(HC11 *hc11)
{
	CCRReg flag=0x0F;
	Word mem;
	Word num;
	Word ACCD;
	
	mem = (hc11->IY + hc11->memory[hc11->PC]);
	hc11->PC++;
	hc11->_clock();

	hc11->_clock();
	
	ACCD.low(hc11->ACCB);		//prepara el registro D
	ACCD.high(hc11->ACCA);
	hc11->_clock();
	
	num.high(hc11->memory[mem]);	//prepara la memoria
	mem++;
	num.low(hc11->memory[mem]);
	hc11->_clock();

	ACCD = subword(hc11, ACCD, num, flag);		//D=D-M
	hc11->ACCA=ACCD.high();
	hc11->ACCB=ACCD.low();
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  ANDA - Ands ACCA with the memory location
//  
//  Addr. Mode: Ind Y
//  
//  Opcode: 18h A4h
//             
//
//  Clocks: 5  (-2 for fetching opcode)
//
//  OpLength: 2 (-2 for fetching opcode)
//
//  Flags:  N, Z, V=0
//
////////////////////////////////////////////////

void HC11::opIY_A4h(HC11 *hc11)
{
	hc11->ACCA = hc11->ACCA & hc11->memory[(hc11->IY + hc11->memory[hc11->PC])];	//A = A & M
	hc11->PC++;
	hc11->_clock();

	hc11->_clock();

	if(hc11->ACCA>=128)
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);

	if(hc11->ACCA==0)
		hc11->CCR.Z(1);
	else 
		hc11->CCR.Z(0);

	hc11->CCR.V(0);
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  BITA - performs an AND on the accumulator and memory location
//
//  Addr. Mode: Ind Y
//  
//  Opcode: 18h A5h
//          
//
//  Clocks: 5  (-2 for fetching opcode) 
//
//  OpLength: 3 (-2 for fetching opcode)
//
//  Flags:  N, Z, V=0
//
////////////////////////////////////////////////

void HC11::opIY_A5h(HC11 *hc11)
{
	Byte data;
	Word mem;

	mem = (hc11->IY + hc11->memory[hc11->PC]);
	hc11->PC++;
    hc11->_clock();	

	data = hc11->ACCA & hc11->memory[mem];		// A & M
    hc11->_clock();	

	if(data>=128)
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);

	if(data==0)
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);

	hc11->CCR.V(0);
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  EORA - XORs the accumulator
//
//  Addr. Mode: Indexed Y
//  
//  Opcode: 18h A8h
//          
//
//  Clocks: 5  (-2 for fetching opcode) 
//
//  OpLength: 3 (-2 for fetching opcode)
//
//  Flags:  N, Z, V=0
//
////////////////////////////////////////////////

void HC11::opIY_A8h(HC11 *hc11)
{
	Word mem;
	
	mem=(hc11->IY + hc11->memory[hc11->PC]);
	hc11->PC++;	
    hc11->_clock();	


	hc11->ACCA = hc11->ACCA ^ hc11->memory[mem];	// A = A exor M
    hc11->_clock();	


	if(hc11->ACCA>=128)
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);		//estaba mal

	if(hc11->ACCA==0)
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);

	hc11->CCR.V(0);
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  ADCA - Adds ACCA, a memory location, and the carry
//         flag and places the result in ACCA
//  
//  Addr. Mode: Ind Y
//  
//  Opcode: 18h A9h
//             
//
//  Clocks: 5  (-2 for fetching both opcodes)
//
//  OpLength: 3 (-2 for fetching opcode)
//
//  Flags:  H, N, Z, V, C
//
////////////////////////////////////////////////

void HC11::opIY_A9h(HC11 *hc11)
{
	CCRReg flag=0x2F;	//0010 1111

	hc11->_clock();

	hc11->_clock();

	hc11->ACCA = addbyte(hc11, hc11->ACCA, hc11->memory[(hc11->IY + hc11->memory[hc11->PC])], flag, hc11->CCR.C());		//A=A+M+C
	
	hc11->PC++;
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  ADDA - Adds ACCA and a memory location
//         and places the result in ACCA
//  
//  Addr. Mode: Ind Y
//  
//  Opcode: 18h ABh
//             
//
//  Clocks: 5  (-2 for fetching both opcodes)
//
//  OpLength: 3 (-2 for fetching opcode)
//
//  Flags:  H, N, Z, V, C
//
////////////////////////////////////////////////

void HC11::opIY_ABh(HC11 *hc11)
{
	CCRReg flag=0x2F;	//0010 1111

	hc11->_clock();

	hc11->_clock();
	
	hc11->ACCA = addbyte(hc11, hc11->ACCA, hc11->memory[(hc11->IY + hc11->memory[hc11->PC])], flag, 0);		//A=A+M
	hc11->PC++;
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  SUBB - Subtracts a memory location and ACCB
//         and places the result in ACCB
//  
//  Addr. Mode: Ind Y
//  
//  Opcode: 18h E0h
//            
//
//  Clocks: 5  (-2 for fetching both opcodes)
//
//  OpLength: 3 (-2 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::opIY_E0h(HC11 *hc11)
{
	CCRReg flag=0x0F;

	hc11->_clock();

	hc11->_clock();

	hc11->ACCB = subbyte(hc11, hc11->ACCB, hc11->memory[(hc11->IY + hc11->memory[hc11->PC])], flag);	//B=B-M
	hc11->PC++;
	hc11->_clock();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
//
//  CMPB - Compare
//
//  Addr. Mode: Ind Y
//  
//  Opcode: 18h E1h
//          
//
//  Clocks: 5  (-2 for fetching opcode) 
//
//  OpLength: 3 (-2 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::opIY_E1h(HC11 *hc11)
{
	Word mem;
	CCRReg flag=0x0F;

	mem = (hc11->IY + hc11->memory[hc11->PC]);
	hc11->PC++;
    hc11->_clock();	

    hc11->_clock();	

	subbyte(hc11, hc11->ACCB, hc11->memory[mem], flag);		//B-M
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  SBCB - Subtract with carry
//
//  Addr. Mode: Indexed Y
//  
//  Opcode: 18h E2h
//          
//
//  Clocks: 5  (-2 for fetching opcode) 
//
//  OpLength: 3 (-2 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::opIY_E2h(HC11 *hc11)
{
	Word mem;
	CCRReg flag=0x0F;

	mem=(hc11->IY + hc11->memory[hc11->PC]);
	hc11->PC++;
    hc11->_clock();	

	hc11->ACCB = hc11->ACCB - hc11->CCR.C();	//B = B - carry
    hc11->_clock();	

	hc11->ACCB = subbyte(hc11, hc11->ACCB, hc11->memory[mem], flag);	//B = B-M
	
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  ADDD - Adds ACCD and a memory location
//         and places the result in ACCD
//  
//  Addr. Mode: Ind Y
//  
//  Opcode: 18h E3h
//                
//
//  Clocks: 7  (-2 for fetching both opcode)
//
//  OpLength: 3 (-2 for fetching opcode)
//
//  Flags:  N, Z, V, C
//
////////////////////////////////////////////////

void HC11::opIY_E3h(HC11 *hc11)
{
	CCRReg flag=0x0F;
	Word mem;

	mem = hc11->IY + hc11->memory[hc11->PC];
	hc11->PC++;
	hc11->_clock();

	hc11->_clock();
	
	hc11->_clock();

	hc11->_clock();

	mem++;
	hc11->ACCB = addbyte(hc11, hc11->ACCB, hc11->memory[mem], flag, 0);			// D = D+M+C
	mem--;
	hc11->ACCA = addbyte(hc11, hc11->ACCA, hc11->memory[mem], flag, hc11->CCR.C());		//la k me llevo es el carry anterior
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  ANDB - Ands ACCB with the memory location
//  
//  Addr. Mode: Ind Y
//  
//  Opcode: 18h E4h
//                 
//
//  Clocks: 5  (-2 for fetching opcode)
//
//  OpLength: 2 (-2 for fetching opcode)
//
//  Flags:  N, Z, V=0
//
////////////////////////////////////////////////

void HC11::opIY_E4h(HC11 *hc11)
{
	hc11->ACCB = hc11->ACCB & hc11->memory[(hc11->IY + hc11->memory[hc11->PC])];		//B = B & M
	hc11->PC++;
	hc11->_clock();

	hc11->_clock();

	if(hc11->ACCB>=128)
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);

	if(hc11->ACCB==0)
		hc11->CCR.Z(1);
	else 
		hc11->CCR.Z(0);

	hc11->CCR.V(0);
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  BITB - performs an AND on the accumulator and memory location
//
//  Addr. Mode: Ind Y
//  
//  Opcode: 18h E5h
//           
//
//  Clocks: 5  (-2 for fetching opcode) 
//
//  OpLength: 3 (-2 for fetching opcode)
//
//  Flags:  N, Z, V=0
//
////////////////////////////////////////////////

void HC11::opIY_E5h(HC11 *hc11)
{
	Byte data;
	Word mem;

	mem=(hc11->IY + hc11->memory[hc11->PC]);
	hc11->PC++;
    hc11->_clock();	

	data = hc11->ACCB & hc11->memory[mem];		// B & M
    hc11->_clock();	


	if(data>=128)
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);

	if(data==0)
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);

	hc11->CCR.V(0);
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  EORB - XORs the accumulator
//
//  Addr. Mode: Indexed Y
//  
//  Opcode: 18h E8h
//          
//
//  Clocks: 5  (-2 for fetching opcode) 
//
//  OpLength: 3 (-2 for fetching opcode)
//
//  Flags:  N, Z, V=0
//
////////////////////////////////////////////////

void HC11::opIY_E8h(HC11 *hc11)
{
	Word mem;
	
	mem=(hc11->IY + hc11->memory[hc11->PC]);
	hc11->PC++;	
    hc11->_clock();	


	hc11->ACCB = hc11->ACCB ^ hc11->memory[mem];	//B = B exor M
    hc11->_clock();	


	if(hc11->ACCB>=128)
		hc11->CCR.N(1);
	else
		hc11->CCR.N(0);

	if(hc11->ACCB==0)
		hc11->CCR.Z(1);
	else
		hc11->CCR.Z(0);

	hc11->CCR.V(0);				
    hc11->_clock();	
}


/////////////////////////////////////////////////
//
//  ADCB - Adds ACCB, a memory location, and the carry
//         flag and places the result in ACCB
//  
//  Addr. Mode: Ind Y
//  
//  Opcode: 18h E9h
//              
//
//  Clocks: 5  (-2 for fetching both opcodes)
//
//  OpLength: 3 (-2 for fetching opcode)
//
//  Flags:  H, N, Z, V, C
//
////////////////////////////////////////////////

void HC11::opIY_E9h(HC11 *hc11)
{
	CCRReg flag=0x2F;	//0010 1111

	hc11->_clock();

	hc11->_clock();

	hc11->ACCB = addbyte(hc11, hc11->ACCB, hc11->memory[(hc11->IY + hc11->memory[hc11->PC])], flag, hc11->CCR.C());	// B = B+M+C
	
	hc11->PC++;
	hc11->_clock();
}


/////////////////////////////////////////////////
//
//  ADDB - Adds ACCB and a memory location
//         and places the result in ACCB
//  
//  Addr. Mode: Ind Y
//  
//  Opcode: 18h EBh
//           
//
//  Clocks: 5  (-2 for fetching both opcodes)
//
//  OpLength: 3 (-2 for fetching opcode)
//
//  Flags:  H, N, Z, V, C
//
////////////////////////////////////////////////

void HC11::opIY_EBh(HC11 *hc11)
{
	CCRReg flag=0x2F;

	hc11->_clock();

	hc11->_clock();

	hc11->ACCB = addbyte(hc11, hc11->ACCB, hc11->memory[(hc11->IY + hc11->memory[hc11->PC])], flag, 0);		//B=B+M
	hc11->PC++;
	hc11->_clock();
}