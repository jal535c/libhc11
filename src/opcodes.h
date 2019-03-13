//solo se puede incluir dentro de la declaracion de la clase HC11
//el proposito de este fichero es k la declaracion de los 256 opcodes no tenga k estar localizada dentro del fichero hc11.h

//no hace falta poner HC11:: en los prototipos, ya k son llamados atraves del #include que esta dentro de la clase, en hc11.h

//direccionamiento inmediato, relativo, e inherente son simples y no necesitan implementarse como funcion externa

	inline static byte_t DirAddr(HC11 *hc11)	//direccionamiento directo
	{
		byte_t address;

		hc11->_clock();
		address = hc11->memory[hc11->PC];
		hc11->PC++;
		return(address);
	}

	inline static word_t ExtAddr(HC11 *hc11)	//direccionamiento extendido: lee 2 veces en memoria para formar la dir
	{
		Word address;    

		hc11->_clock();
		address.high(hc11->memory[hc11->PC]);
		hc11->PC++;    

		hc11->_clock();
		address.low(hc11->memory[hc11->PC]);
		hc11->PC++;
		return(address);
	}

	inline static word_t IndXAddr(HC11 *hc11)	//direccionamiento indexado en x
	{
		word_t address;
		byte_t offset;    

		hc11->_clock();
		offset = hc11->memory[hc11->PC];	//lectura en memoria del offset
		hc11->PC++;    

		hc11->_clock();					//suma el offset con el contenido de IX
		address = hc11->IX + offset;    
		return(address);
	}

	inline static word_t IndYAddr(HC11 *hc11)	//direccionamiento indexado en y
	{
		word_t address;
		byte_t offset;    

		hc11->_clock();
		offset = hc11->memory[hc11->PC];
		hc11->PC++;    

		hc11->_clock();
		address = hc11->IY + offset;    
		return(address);
	}


//funcion Push, (pertenece a la clase HC11, no es necesario poner HC11::) mete un byte de datos en la pila

	void Push(byte_t data)
	{
		_clock();
		memory.Write(SP, data);        
		SP--;
	};

//funcion Pull, pertenece a la clase HC11, saca un byte de la pila
	
	byte_t Pull(void)
	{
		_clock();
		SP++;
		return(memory[SP]);        
	};

	void StackRegisters(void)		//mete los registros en la pila (justo antes de atender una int), usa la funcion Push()
	{
		Push(PC.low());
		Push(PC.high());
		Push(IY.low());
		Push(IY.high());
		Push(IX.low());
		Push(IX.high());
		Push(ACCA);
		Push(ACCB);    
		Push(CCR);
	};

	void UnStackRegisters(void)	//saca los registros de la pila con la funcion Pull(), llamada desde la instruccion RTI
	{
		CCR = Pull();
		ACCB = Pull();    
		ACCA = Pull();
		IX.high(Pull());
		IX.low(Pull());
		IY.high(Pull());
		IY.low(Pull());
		PC.high(Pull());
		PC.low(Pull());
	};


////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//prototipos de las funciones que implementaran cada instruccion

	//en opcodes.cpp se inicializan los arrays que las contiene: opcode[256], opIYcode[256]


	static void op_illegal(HC11 *hc11);

	static void op_0h(HC11 *hc11);
	static void op_1h(HC11 *hc11);
	static void op_2h(HC11 *hc11);
	static void op_3h(HC11 *hc11);
	static void op_4h(HC11 *hc11);
	static void op_5h(HC11 *hc11);
	static void op_6h(HC11 *hc11);
	static void op_7h(HC11 *hc11);
	static void op_8h(HC11 *hc11);
	static void op_9h(HC11 *hc11);
	static void op_Ah(HC11 *hc11);
	static void op_Bh(HC11 *hc11);
	static void op_Ch(HC11 *hc11);
	static void op_Dh(HC11 *hc11);
	static void op_Eh(HC11 *hc11);
	static void op_Fh(HC11 *hc11);

	static void op_10h(HC11 *hc11);
	static void op_11h(HC11 *hc11);
	static void op_12h(HC11 *hc11);
	static void op_13h(HC11 *hc11);
	static void op_14h(HC11 *hc11);
	static void op_15h(HC11 *hc11);
	static void op_16h(HC11 *hc11);
	static void op_17h(HC11 *hc11);
	static void op_18h(HC11 *hc11);
	static void op_19h(HC11 *hc11);
	static void op_1Ah(HC11 *hc11);
	static void op_1Bh(HC11 *hc11);
	static void op_1Ch(HC11 *hc11);
	static void op_1Dh(HC11 *hc11);
	static void op_1Eh(HC11 *hc11);
	static void op_1Fh(HC11 *hc11);

	static void op_20h(HC11 *hc11);
	static void op_21h(HC11 *hc11);
	static void op_22h(HC11 *hc11);
	static void op_23h(HC11 *hc11);
	static void op_24h(HC11 *hc11);
	static void op_25h(HC11 *hc11);
	static void op_26h(HC11 *hc11);
	static void op_27h(HC11 *hc11);
	static void op_28h(HC11 *hc11);
	static void op_29h(HC11 *hc11);
	static void op_2Ah(HC11 *hc11);
	static void op_2Bh(HC11 *hc11);
	static void op_2Ch(HC11 *hc11);
	static void op_2Dh(HC11 *hc11);
	static void op_2Eh(HC11 *hc11);
	static void op_2Fh(HC11 *hc11);

	static void op_30h(HC11 *hc11);
	static void op_31h(HC11 *hc11);
	static void op_32h(HC11 *hc11);
	static void op_33h(HC11 *hc11);
	static void op_34h(HC11 *hc11);
	static void op_35h(HC11 *hc11);
	static void op_36h(HC11 *hc11);
	static void op_37h(HC11 *hc11);
	static void op_38h(HC11 *hc11);
	static void op_39h(HC11 *hc11);
	static void op_3Ah(HC11 *hc11);
	static void op_3Bh(HC11 *hc11);
	static void op_3Ch(HC11 *hc11);
	static void op_3Dh(HC11 *hc11);
	static void op_3Eh(HC11 *hc11);
	static void op_3Fh(HC11 *hc11);

	static void op_40h(HC11 *hc11);
	static void op_41h(HC11 *hc11);
	static void op_42h(HC11 *hc11);
	static void op_43h(HC11 *hc11);
	static void op_44h(HC11 *hc11);
	static void op_45h(HC11 *hc11);
	static void op_46h(HC11 *hc11);
	static void op_47h(HC11 *hc11);
	static void op_48h(HC11 *hc11);
	static void op_49h(HC11 *hc11);
	static void op_4Ah(HC11 *hc11);
	static void op_4Bh(HC11 *hc11);
	static void op_4Ch(HC11 *hc11);
	static void op_4Dh(HC11 *hc11);
	static void op_4Eh(HC11 *hc11);
	static void op_4Fh(HC11 *hc11);

	static void op_50h(HC11 *hc11);
	static void op_51h(HC11 *hc11);
	static void op_52h(HC11 *hc11);
	static void op_53h(HC11 *hc11);
	static void op_54h(HC11 *hc11);
	static void op_55h(HC11 *hc11);
	static void op_56h(HC11 *hc11);
	static void op_57h(HC11 *hc11);
	static void op_58h(HC11 *hc11);
	static void op_59h(HC11 *hc11);
	static void op_5Ah(HC11 *hc11);
	static void op_5Bh(HC11 *hc11);
	static void op_5Ch(HC11 *hc11);
	static void op_5Dh(HC11 *hc11);
	static void op_5Eh(HC11 *hc11);
	static void op_5Fh(HC11 *hc11);

	static void op_60h(HC11 *hc11);
	static void op_61h(HC11 *hc11);
	static void op_62h(HC11 *hc11);
	static void op_63h(HC11 *hc11);
	static void op_64h(HC11 *hc11);
	static void op_65h(HC11 *hc11);
	static void op_66h(HC11 *hc11);
	static void op_67h(HC11 *hc11);
	static void op_68h(HC11 *hc11);
	static void op_69h(HC11 *hc11);
	static void op_6Ah(HC11 *hc11);
	static void op_6Bh(HC11 *hc11);
	static void op_6Ch(HC11 *hc11);
	static void op_6Dh(HC11 *hc11);
	static void op_6Eh(HC11 *hc11);
	static void op_6Fh(HC11 *hc11);

	static void op_70h(HC11 *hc11);
	static void op_71h(HC11 *hc11);
	static void op_72h(HC11 *hc11);
	static void op_73h(HC11 *hc11);
	static void op_74h(HC11 *hc11);
	static void op_75h(HC11 *hc11);
	static void op_76h(HC11 *hc11);
	static void op_77h(HC11 *hc11);
	static void op_78h(HC11 *hc11);
	static void op_79h(HC11 *hc11);
	static void op_7Ah(HC11 *hc11);
	static void op_7Bh(HC11 *hc11);
	static void op_7Ch(HC11 *hc11);
	static void op_7Dh(HC11 *hc11);
	static void op_7Eh(HC11 *hc11);
	static void op_7Fh(HC11 *hc11);

	static void op_80h(HC11 *hc11);
	static void op_81h(HC11 *hc11);
	static void op_82h(HC11 *hc11);
	static void op_83h(HC11 *hc11);
	static void op_84h(HC11 *hc11);
	static void op_85h(HC11 *hc11);
	static void op_86h(HC11 *hc11);
	static void op_87h(HC11 *hc11);
	static void op_88h(HC11 *hc11);
	static void op_89h(HC11 *hc11);
	static void op_8Ah(HC11 *hc11);
	static void op_8Bh(HC11 *hc11);
	static void op_8Ch(HC11 *hc11);
	static void op_8Dh(HC11 *hc11);
	static void op_8Eh(HC11 *hc11);
	static void op_8Fh(HC11 *hc11);

	static void op_90h(HC11 *hc11);
	static void op_91h(HC11 *hc11);
	static void op_92h(HC11 *hc11);
	static void op_93h(HC11 *hc11);
	static void op_94h(HC11 *hc11);
	static void op_95h(HC11 *hc11);
	static void op_96h(HC11 *hc11);
	static void op_97h(HC11 *hc11);
	static void op_98h(HC11 *hc11);
	static void op_99h(HC11 *hc11);
	static void op_9Ah(HC11 *hc11);
	static void op_9Bh(HC11 *hc11);
	static void op_9Ch(HC11 *hc11);
	static void op_9Dh(HC11 *hc11);
	static void op_9Eh(HC11 *hc11);
	static void op_9Fh(HC11 *hc11);

	static void op_A0h(HC11 *hc11);
	static void op_A1h(HC11 *hc11);
	static void op_A2h(HC11 *hc11);
	static void op_A3h(HC11 *hc11);
	static void op_A4h(HC11 *hc11);
	static void op_A5h(HC11 *hc11);
	static void op_A6h(HC11 *hc11);
	static void op_A7h(HC11 *hc11);
	static void op_A8h(HC11 *hc11);
	static void op_A9h(HC11 *hc11);
	static void op_AAh(HC11 *hc11);
	static void op_ABh(HC11 *hc11);
	static void op_ACh(HC11 *hc11);
	static void op_ADh(HC11 *hc11);
	static void op_AEh(HC11 *hc11);
	static void op_AFh(HC11 *hc11);

	static void op_B0h(HC11 *hc11);
	static void op_B1h(HC11 *hc11);
	static void op_B2h(HC11 *hc11);
	static void op_B3h(HC11 *hc11);
	static void op_B4h(HC11 *hc11);
	static void op_B5h(HC11 *hc11);
	static void op_B6h(HC11 *hc11);
	static void op_B7h(HC11 *hc11);
	static void op_B8h(HC11 *hc11);
	static void op_B9h(HC11 *hc11);
	static void op_BAh(HC11 *hc11);
	static void op_BBh(HC11 *hc11);
	static void op_BCh(HC11 *hc11);
	static void op_BDh(HC11 *hc11);
	static void op_BEh(HC11 *hc11);
	static void op_BFh(HC11 *hc11);

	static void op_C0h(HC11 *hc11);
	static void op_C1h(HC11 *hc11);
	static void op_C2h(HC11 *hc11);
	static void op_C3h(HC11 *hc11);
	static void op_C4h(HC11 *hc11);
	static void op_C5h(HC11 *hc11);
	static void op_C6h(HC11 *hc11);
	static void op_C7h(HC11 *hc11);
	static void op_C8h(HC11 *hc11);
	static void op_C9h(HC11 *hc11);
	static void op_CAh(HC11 *hc11);
	static void op_CBh(HC11 *hc11);
	static void op_CCh(HC11 *hc11);
	static void op_CDh(HC11 *hc11);
	static void op_CEh(HC11 *hc11);
	static void op_CFh(HC11 *hc11);

	static void op_D0h(HC11 *hc11);
	static void op_D1h(HC11 *hc11);
	static void op_D2h(HC11 *hc11);
	static void op_D3h(HC11 *hc11);
	static void op_D4h(HC11 *hc11);
	static void op_D5h(HC11 *hc11);
	static void op_D6h(HC11 *hc11);
	static void op_D7h(HC11 *hc11);
	static void op_D8h(HC11 *hc11);
	static void op_D9h(HC11 *hc11);
	static void op_DAh(HC11 *hc11);
	static void op_DBh(HC11 *hc11);
	static void op_DCh(HC11 *hc11);
	static void op_DDh(HC11 *hc11);
	static void op_DEh(HC11 *hc11);
	static void op_DFh(HC11 *hc11);

	static void op_E0h(HC11 *hc11);
	static void op_E1h(HC11 *hc11);
	static void op_E2h(HC11 *hc11);
	static void op_E3h(HC11 *hc11);
	static void op_E4h(HC11 *hc11);
	static void op_E5h(HC11 *hc11);
	static void op_E6h(HC11 *hc11);
	static void op_E7h(HC11 *hc11);
	static void op_E8h(HC11 *hc11);
	static void op_E9h(HC11 *hc11);
	static void op_EAh(HC11 *hc11);
	static void op_EBh(HC11 *hc11);
	static void op_ECh(HC11 *hc11);
	static void op_EDh(HC11 *hc11);
	static void op_EEh(HC11 *hc11);
	static void op_EFh(HC11 *hc11);

	static void op_F0h(HC11 *hc11);
	static void op_F1h(HC11 *hc11);
	static void op_F2h(HC11 *hc11);
	static void op_F3h(HC11 *hc11);
	static void op_F4h(HC11 *hc11);
	static void op_F5h(HC11 *hc11);
	static void op_F6h(HC11 *hc11);
	static void op_F7h(HC11 *hc11);
	static void op_F8h(HC11 *hc11);
	static void op_F9h(HC11 *hc11);
	static void op_FAh(HC11 *hc11);
	static void op_FBh(HC11 *hc11);
	static void op_FCh(HC11 *hc11);
	static void op_FDh(HC11 *hc11);
	static void op_FEh(HC11 *hc11);
	static void op_FFh(HC11 *hc11);


//instrucciones extendidas k empiezan por 18, aunque son 64 aqui pongo las 256

	static void opIY_0h(HC11 *hc11);
	static void opIY_1h(HC11 *hc11);
	static void opIY_2h(HC11 *hc11);
	static void opIY_3h(HC11 *hc11);
	static void opIY_4h(HC11 *hc11);
	static void opIY_5h(HC11 *hc11);
	static void opIY_6h(HC11 *hc11);
	static void opIY_7h(HC11 *hc11);
	static void opIY_8h(HC11 *hc11);
	static void opIY_9h(HC11 *hc11);
	static void opIY_Ah(HC11 *hc11);
	static void opIY_Bh(HC11 *hc11);
	static void opIY_Ch(HC11 *hc11);
	static void opIY_Dh(HC11 *hc11);
	static void opIY_Eh(HC11 *hc11);
	static void opIY_Fh(HC11 *hc11);

	static void opIY_10h(HC11 *hc11);
	static void opIY_11h(HC11 *hc11);
	static void opIY_12h(HC11 *hc11);
	static void opIY_13h(HC11 *hc11);
	static void opIY_14h(HC11 *hc11);
	static void opIY_15h(HC11 *hc11);
	static void opIY_16h(HC11 *hc11);
	static void opIY_17h(HC11 *hc11);
	static void opIY_18h(HC11 *hc11);
	static void opIY_19h(HC11 *hc11);
	static void opIY_1Ah(HC11 *hc11);
	static void opIY_1Bh(HC11 *hc11);
	static void opIY_1Ch(HC11 *hc11);
	static void opIY_1Dh(HC11 *hc11);
	static void opIY_1Eh(HC11 *hc11);
	static void opIY_1Fh(HC11 *hc11);

	static void opIY_20h(HC11 *hc11);
	static void opIY_21h(HC11 *hc11);
	static void opIY_22h(HC11 *hc11);
	static void opIY_23h(HC11 *hc11);
	static void opIY_24h(HC11 *hc11);
	static void opIY_25h(HC11 *hc11);
	static void opIY_26h(HC11 *hc11);
	static void opIY_27h(HC11 *hc11);
	static void opIY_28h(HC11 *hc11);
	static void opIY_29h(HC11 *hc11);
	static void opIY_2Ah(HC11 *hc11);
	static void opIY_2Bh(HC11 *hc11);
	static void opIY_2Ch(HC11 *hc11);
	static void opIY_2Dh(HC11 *hc11);
	static void opIY_2Eh(HC11 *hc11);
	static void opIY_2Fh(HC11 *hc11);

	static void opIY_30h(HC11 *hc11);
	static void opIY_31h(HC11 *hc11);
	static void opIY_32h(HC11 *hc11);
	static void opIY_33h(HC11 *hc11);
	static void opIY_34h(HC11 *hc11);
	static void opIY_35h(HC11 *hc11);
	static void opIY_36h(HC11 *hc11);
	static void opIY_37h(HC11 *hc11);
	static void opIY_38h(HC11 *hc11);
	static void opIY_39h(HC11 *hc11);
	static void opIY_3Ah(HC11 *hc11);
	static void opIY_3Bh(HC11 *hc11);
	static void opIY_3Ch(HC11 *hc11);
	static void opIY_3Dh(HC11 *hc11);
	static void opIY_3Eh(HC11 *hc11);
	static void opIY_3Fh(HC11 *hc11);

	static void opIY_40h(HC11 *hc11);
	static void opIY_41h(HC11 *hc11);
	static void opIY_42h(HC11 *hc11);
	static void opIY_43h(HC11 *hc11);
	static void opIY_44h(HC11 *hc11);
	static void opIY_45h(HC11 *hc11);
	static void opIY_46h(HC11 *hc11);
	static void opIY_47h(HC11 *hc11);
	static void opIY_48h(HC11 *hc11);
	static void opIY_49h(HC11 *hc11);
	static void opIY_4Ah(HC11 *hc11);
	static void opIY_4Bh(HC11 *hc11);
	static void opIY_4Ch(HC11 *hc11);
	static void opIY_4Dh(HC11 *hc11);
	static void opIY_4Eh(HC11 *hc11);
	static void opIY_4Fh(HC11 *hc11);

	static void opIY_50h(HC11 *hc11);
	static void opIY_51h(HC11 *hc11);
	static void opIY_52h(HC11 *hc11);
	static void opIY_53h(HC11 *hc11);
	static void opIY_54h(HC11 *hc11);
	static void opIY_55h(HC11 *hc11);
	static void opIY_56h(HC11 *hc11);
	static void opIY_57h(HC11 *hc11);
	static void opIY_58h(HC11 *hc11);
	static void opIY_59h(HC11 *hc11);
	static void opIY_5Ah(HC11 *hc11);
	static void opIY_5Bh(HC11 *hc11);
	static void opIY_5Ch(HC11 *hc11);
	static void opIY_5Dh(HC11 *hc11);
	static void opIY_5Eh(HC11 *hc11);
	static void opIY_5Fh(HC11 *hc11);

	static void opIY_60h(HC11 *hc11);
	static void opIY_61h(HC11 *hc11);
	static void opIY_62h(HC11 *hc11);
	static void opIY_63h(HC11 *hc11);
	static void opIY_64h(HC11 *hc11);
	static void opIY_65h(HC11 *hc11);
	static void opIY_66h(HC11 *hc11);
	static void opIY_67h(HC11 *hc11);
	static void opIY_68h(HC11 *hc11);
	static void opIY_69h(HC11 *hc11);
	static void opIY_6Ah(HC11 *hc11);
	static void opIY_6Bh(HC11 *hc11);
	static void opIY_6Ch(HC11 *hc11);
	static void opIY_6Dh(HC11 *hc11);
	static void opIY_6Eh(HC11 *hc11);
	static void opIY_6Fh(HC11 *hc11);

	static void opIY_70h(HC11 *hc11);
	static void opIY_71h(HC11 *hc11);
	static void opIY_72h(HC11 *hc11);
	static void opIY_73h(HC11 *hc11);
	static void opIY_74h(HC11 *hc11);
	static void opIY_75h(HC11 *hc11);
	static void opIY_76h(HC11 *hc11);
	static void opIY_77h(HC11 *hc11);
	static void opIY_78h(HC11 *hc11);
	static void opIY_79h(HC11 *hc11);
	static void opIY_7Ah(HC11 *hc11);
	static void opIY_7Bh(HC11 *hc11);
	static void opIY_7Ch(HC11 *hc11);
	static void opIY_7Dh(HC11 *hc11);
	static void opIY_7Eh(HC11 *hc11);
	static void opIY_7Fh(HC11 *hc11);

	static void opIY_80h(HC11 *hc11);
	static void opIY_81h(HC11 *hc11);
	static void opIY_82h(HC11 *hc11);
	static void opIY_83h(HC11 *hc11);
	static void opIY_84h(HC11 *hc11);
	static void opIY_85h(HC11 *hc11);
	static void opIY_86h(HC11 *hc11);
	static void opIY_87h(HC11 *hc11);
	static void opIY_88h(HC11 *hc11);
	static void opIY_89h(HC11 *hc11);
	static void opIY_8Ah(HC11 *hc11);
	static void opIY_8Bh(HC11 *hc11);
	static void opIY_8Ch(HC11 *hc11);
	static void opIY_8Dh(HC11 *hc11);
	static void opIY_8Eh(HC11 *hc11);
	static void opIY_8Fh(HC11 *hc11);

	static void opIY_90h(HC11 *hc11);
	static void opIY_91h(HC11 *hc11);
	static void opIY_92h(HC11 *hc11);
	static void opIY_93h(HC11 *hc11);
	static void opIY_94h(HC11 *hc11);
	static void opIY_95h(HC11 *hc11);
	static void opIY_96h(HC11 *hc11);
	static void opIY_97h(HC11 *hc11);
	static void opIY_98h(HC11 *hc11);
	static void opIY_99h(HC11 *hc11);
	static void opIY_9Ah(HC11 *hc11);
	static void opIY_9Bh(HC11 *hc11);
	static void opIY_9Ch(HC11 *hc11);
	static void opIY_9Dh(HC11 *hc11);
	static void opIY_9Eh(HC11 *hc11);
	static void opIY_9Fh(HC11 *hc11);

	static void opIY_A0h(HC11 *hc11);
	static void opIY_A1h(HC11 *hc11);
	static void opIY_A2h(HC11 *hc11);
	static void opIY_A3h(HC11 *hc11);
	static void opIY_A4h(HC11 *hc11);
	static void opIY_A5h(HC11 *hc11);
	static void opIY_A6h(HC11 *hc11);
	static void opIY_A7h(HC11 *hc11);
	static void opIY_A8h(HC11 *hc11);
	static void opIY_A9h(HC11 *hc11);
	static void opIY_AAh(HC11 *hc11);
	static void opIY_ABh(HC11 *hc11);
	static void opIY_ACh(HC11 *hc11);
	static void opIY_ADh(HC11 *hc11);
	static void opIY_AEh(HC11 *hc11);
	static void opIY_AFh(HC11 *hc11);

	static void opIY_B0h(HC11 *hc11);
	static void opIY_B1h(HC11 *hc11);
	static void opIY_B2h(HC11 *hc11);
	static void opIY_B3h(HC11 *hc11);
	static void opIY_B4h(HC11 *hc11);
	static void opIY_B5h(HC11 *hc11);
	static void opIY_B6h(HC11 *hc11);
	static void opIY_B7h(HC11 *hc11);
	static void opIY_B8h(HC11 *hc11);
	static void opIY_B9h(HC11 *hc11);
	static void opIY_BAh(HC11 *hc11);
	static void opIY_BBh(HC11 *hc11);
	static void opIY_BCh(HC11 *hc11);
	static void opIY_BDh(HC11 *hc11);
	static void opIY_BEh(HC11 *hc11);
	static void opIY_BFh(HC11 *hc11);

	static void opIY_C0h(HC11 *hc11);
	static void opIY_C1h(HC11 *hc11);
	static void opIY_C2h(HC11 *hc11);
	static void opIY_C3h(HC11 *hc11);
	static void opIY_C4h(HC11 *hc11);
	static void opIY_C5h(HC11 *hc11);
	static void opIY_C6h(HC11 *hc11);
	static void opIY_C7h(HC11 *hc11);
	static void opIY_C8h(HC11 *hc11);
	static void opIY_C9h(HC11 *hc11);
	static void opIY_CAh(HC11 *hc11);
	static void opIY_CBh(HC11 *hc11);
	static void opIY_CCh(HC11 *hc11);
	static void opIY_CDh(HC11 *hc11);
	static void opIY_CEh(HC11 *hc11);
	static void opIY_CFh(HC11 *hc11);

	static void opIY_D0h(HC11 *hc11);
	static void opIY_D1h(HC11 *hc11);
	static void opIY_D2h(HC11 *hc11);
	static void opIY_D3h(HC11 *hc11);
	static void opIY_D4h(HC11 *hc11);
	static void opIY_D5h(HC11 *hc11);
	static void opIY_D6h(HC11 *hc11);
	static void opIY_D7h(HC11 *hc11);
	static void opIY_D8h(HC11 *hc11);
	static void opIY_D9h(HC11 *hc11);
	static void opIY_DAh(HC11 *hc11);
	static void opIY_DBh(HC11 *hc11);
	static void opIY_DCh(HC11 *hc11);
	static void opIY_DDh(HC11 *hc11);
	static void opIY_DEh(HC11 *hc11);
	static void opIY_DFh(HC11 *hc11);

	static void opIY_E0h(HC11 *hc11);
	static void opIY_E1h(HC11 *hc11);
	static void opIY_E2h(HC11 *hc11);
	static void opIY_E3h(HC11 *hc11);
	static void opIY_E4h(HC11 *hc11);
	static void opIY_E5h(HC11 *hc11);
	static void opIY_E6h(HC11 *hc11);
	static void opIY_E7h(HC11 *hc11);
	static void opIY_E8h(HC11 *hc11);
	static void opIY_E9h(HC11 *hc11);
	static void opIY_EAh(HC11 *hc11);
	static void opIY_EBh(HC11 *hc11);
	static void opIY_ECh(HC11 *hc11);
	static void opIY_EDh(HC11 *hc11);
	static void opIY_EEh(HC11 *hc11);
	static void opIY_EFh(HC11 *hc11);

	static void opIY_F0h(HC11 *hc11);
	static void opIY_F1h(HC11 *hc11);
	static void opIY_F2h(HC11 *hc11);
	static void opIY_F3h(HC11 *hc11);
	static void opIY_F4h(HC11 *hc11);
	static void opIY_F5h(HC11 *hc11);
	static void opIY_F6h(HC11 *hc11);
	static void opIY_F7h(HC11 *hc11);
	static void opIY_F8h(HC11 *hc11);
	static void opIY_F9h(HC11 *hc11);
	static void opIY_FAh(HC11 *hc11);
	static void opIY_FBh(HC11 *hc11);
	static void opIY_FCh(HC11 *hc11);
	static void opIY_FDh(HC11 *hc11);
	static void opIY_FEh(HC11 *hc11);
	static void opIY_FFh(HC11 *hc11);


//7 instrucciones extendidas k empiezan por 0x1A

	static void op1A_ACh(HC11 *hc11);
	static void op1A_EEh(HC11 *hc11);
	static void op1A_EFh(HC11 *hc11);
	static void op1A_83h(HC11 *hc11);
	static void op1A_93h(HC11 *hc11);
	static void op1A_B3h(HC11 *hc11);
	static void op1A_A3h(HC11 *hc11);

//4 instrucciones extendidas k empiezan por 0xCD

	static void opCD_ACh(HC11 *hc11);
	static void opCD_EEh(HC11 *hc11);
	static void opCD_EFh(HC11 *hc11);
	static void opCD_A3h(HC11 *hc11);

