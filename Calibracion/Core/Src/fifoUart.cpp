/*
 * fifoUart.cpp
 *
 *  Created on: Oct 19, 2023
 *      Author: jgonz
 */
#include "fifoUart.h"
#include <stdint.h>

/*
 * FIFO COMMANDS
 *
 * Estructura de Fifo para comandos a enviar en LORAWAN
 */

///////////////
// AVAILABLE //
///////////////

bool fifoCommands::available(){		// Indica si hay elementos en Fifo
	return this->bussy;				// Retorna flag
}

/////////////////
// ADD COMMAND //
/////////////////

void fifoCommands::add( uint8_t *command, uint8_t size ){
	uint8_t i;															// Crea variable

	if ( this->index < 10 ){											// Si el indice es menor a 10
		for( i = 0; i < size ; i++){									// Recorre el largo del comando
			*(this->rxCommand[this->index] + i)  = *(command + i);		// Copia valores del comando
		}
		this->index++;													// Suma 1 al indice
		this->full 	= 0;												// Indica que no está full
		this->bussy	= 1;												// Indica que hay elementos
	}
	else{																// Si no
		this->bussy	= 1;												// Indica que hay elementos
		this->full 	= 1;												// Indica que esta full
	}
}

//////////////////
// READ COMMAND //
//////////////////

/*
 * El output de esta operacion es la variable *buffer que a su vez es una entrada
 */
void fifoCommands::read( uint8_t *buffer, uint8_t size ){
	uint8_t i;																// Crea variable
	uint8_t j;																// Crea variable

	if ( this->index > 0 ){													// Si el indice es mayo a 0
		for (i = 0; i < size; i++){											// Recorre largo del comando
			buffer[i]	= this->rxCommand_0[i];								// Copia comando en buffer
		}

		for (i = 0; i < this->index; i++){									// Recorre Fifo ocupada
			for (j = 0; j < 10; j++){										// Recorre elementos de comando
				*(this->rxCommand[i] + j) = *(this->rxCommand[i + 1] + j);	// Realiza un shift de los comandos en fifo
			}
		}
		this->index--;														// Resta 1 en largo de fifo ocupada
	}

	if ( this->index == 0){													// Si el largo es 0
		this->bussy	= 0;													// Indica que no está ocupada
	}
}


/********************/

fifoUart::fifoUart(){
	this->finalElement = this->elements;	// Cuando se construye la fifo
											// El puntero de ultimo elemento
											// Apunta al primer elemento de la cadena
}

////////////////////
// INSERT ELEMENT //
////////////////////

bool fifoUart::insertElement( uint8_t element ){

	if (!(this->bussyFifo)){							// Si no está lleno
		*this->finalElement	= element;					// Inserta elemento
		this->finalElement = this->finalElement + 1;	// Suma 1 al puntero
		this->large++;
	}

	if ( this->finalElement == &this->elements[50] ){	// Si el puntero es el último del array
		this->bussyFifo	= 1;							// Indica que se lleno el buffer
	}
	else{												// Si no
		this->bussyFifo	= 0;							// Indica que no se ha llenado
	}

	return !(this->bussyFifo);							// 1: Se insertó correctamente
														// 0: No se inserto por Fifo llena
}

//////////////////
// READ ELEMENT //
//////////////////

uint8_t fifoUart::readElement(){

	uint8_t elementy	= this->elements[0];							// Copia primer valor en fifo

	for (uint8_t *j = this->elements; j < this->finalElement; j++){		// Recorre los elementos de fifo
		*j	= *(j+1);													// Copia el elemento siguiente
	}

	*this->finalElement = 0;											// Rellena el último valor con 0

	if (this->finalElement != this->elements){							// Si el último elemento no es el primero
		this->finalElement	= this->finalElement - 1;					//  Resta una posición al elemento final
		this->emptyFifo	= 0;											// Indica que fifo no esta vacia
		this->large--;													// Quita 1 al largo
	}
	else{																// Si no
		this->emptyFifo	= 1;											// Indica que fifo esta vacia
	}

	return elementy;													// Retorna elementos a leer
}

/////////////////
// SEE ELEMENT //
/////////////////

uint8_t fifoUart::seeElement(){		// No realiza un shift de la funcion
	return this->elements[0];		// Solo observa primer elemento
}

///////////////
// AVAILABLE //
///////////////

bool fifoUart::available(){
	if (this->finalElement == &this->elements[0]){		// Si el puntero está apuntando
														// al primer elemento de la fifo
		this->emptyFifo	= 1;							// Indica que la fifo esta vacia
	}
	else{												// Sino
		this->emptyFifo	= 0;							// Indica que la fifo contiene elementos
	}
	return !(this->emptyFifo);							// 1: Elementos disponibles
}														// 0: Sin elementos

///////////
// BUSSY //
///////////

bool fifoUart::bussy(){
	return (this->bussyFifo);
}

///////////
// LARGE //
///////////

int fifoUart::largeFifo(){
	return this->large;
}

