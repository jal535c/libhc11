#ifndef __LIST_H__
#define __LIST_H__


#include <stdlib.h>		//tiene definido el valor del puntero NULL


template <class DataType>	
class list_node {
public:			//lo pongo todo publico para no usar las clases amigas
	DataType data;					
	list_node<DataType>* next;		
	list_node<DataType>* previous;	
 
	list_node() 	
	{	
		next = NULL;
		previous = NULL;
	}	
	
	~list_node() {	
	
	}		
}; 


template <class DataType> 
class list_iter {			//uso un iterador para acceder al nodo
public:
	list_node<DataType>* node;		

	list_iter() 	
	{	
		node = NULL;
	} 
	
	~list_iter() {	
	
	} 

	bool is_valid() {
		return (node) ? true:false;
	}
	
	list_iter& operator++(int ) {	//sobrecarga del operador post incremento, el iterador avanza 
		node = node->next;
		return *this;				
	}
	
	list_iter& operator--(int ) {	//post decremento, el puntero de iteracion retrocede
		node = node->previous;
		return *this;
	}
}; 


template <class DataType> 
class list {
public:
	list_node<DataType>* head;	
    list_node<DataType>* tail;	
  	int length;

	list() 		
	{	
		head = new list_node<DataType>;		
 		head->next = NULL;		//no hay siguiente
 		tail = head;			//la cola es igual a la cabeza
		length = 0;				//la lista no tiene elementos todavia
	}

	~list() {	
		clear();		//funcion miembro de esta clase, mirar mas abajo
		delete head;	
	}
   
	list_iter<DataType> first() {  
		list_iter<DataType> index;
		
		index.node = head->next;
		return index;
	}
    
	list_iter<DataType> last() {
		list_iter<DataType> index;
		
		index.node = tail;
		return index;
	}

	int size() {
		return length;
	}
    
	DataType& operator[](const list_iter<DataType>& index) {	//sobrecarga del operador []: meto iter y devuelve dato al que apunta
 		return index.node->data;
	}

	list_iter<DataType> add(DataType d) {	//añade un dato a la lista, lo pone como ultimo
		list_node<DataType>* nodeTemp;
		list_iter<DataType> index;

		nodeTemp = tail;		      
		tail = new list_node<DataType>;     
	
		tail->previous = nodeTemp;	      
		nodeTemp->next = tail;	      
		
		tail->data = d;		      
 		
		index.node = tail;
		length++;
 		return index;	      
	}

	void remove(list_iter<DataType>& index) {	//elimina un nodo de la lista, segun el puntero de iteracion
		list_node<DataType>* nodeTemp;
		
		nodeTemp = index.node;
		index.node = index.node->next;		//muevo el indice al siguiente

 		if (nodeTemp==tail) {					//si el iter apuntaba a la cola
  			nodeTemp->previous->next = NULL;  
  			tail = nodeTemp->previous;
 		} else {
  			nodeTemp->previous->next = nodeTemp->next;
  			nodeTemp->next->previous = nodeTemp->previous;
 		}
 		
 		length--;
 		delete nodeTemp;
	}

	void clear() {			//borra la lista
		list_iter<DataType> index;

		while (head->next) {		//mientras haya siguiente
			index.node = head->next;
			remove(index);
		}
 		length=0;
	}
};


#endif



