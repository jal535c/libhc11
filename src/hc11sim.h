#ifndef __HC11SIM_H__
#define __HC11SIM_H__


#include "hc11.h"	
class RAM;
class ROM;
#include "word.h"		


typedef enum {None, BriefCase, RugWarriorBootstrap, RugWarriorExpanded} HC11SimConfig;

//None: la que tengo por defecto, 
//Briefcase: maletin, como el pic trainer con mas memoria y con display, 
//RugWarrior: un tipo de robot, configura el hc11 para servos...

//esta clase se usa para poner el hc11 en varias configuraciones, como briefcase o rugwarrior.
//y para cargar un fichero en memoria

class HC11Sim {
private:   
	HC11SimConfig cfg;		//cfg guarda el enum k me indica el tipo de simulador
    RAM* expanded_ram;
    ROM* briefcase_rom;

public:    
	HC11 hc11;		//Un objeto de esta clase es creado en el document de la app mfc, llamaria al constructor HC11Sim pero antes al de HC11
				
    HC11Sim();		
    ~HC11Sim();    
	void Config(HC11SimConfig ncfg);		//para añadir expansion de ram o rom para el robot, se llama en constructor Doc
    void SetupPseudoVectors(word_t vector_base_addr);
    void SetStartAddress(Word address);		//cambia la direccion en briefcase o rugwarrior, se llama en constructor Doc
  	bool LoadS19File(const char *fname);	//se llama desde el Doc metodo OnOpenDocument
};


#endif