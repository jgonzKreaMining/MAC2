/*
 * fifoUart.h
 *
 *  Created on: Oct 19, 2023
 *      Author: jgonz
 */

#include "stdint.h"

#ifndef INC_FIFOUART_H_
#define INC_FIFOUART_H_

////////////////////////
// FIFO COMMAND CLASS //
////////////////////////


class fifoCommands{
private:
	uint8_t size;				// Largo del comando de entrada
	uint8_t *rxCommand[10] ={	// Lista de comandos interna
		rxCommand_0,
		rxCommand_1,
		rxCommand_2,
		rxCommand_3,
		rxCommand_4,
		rxCommand_5,
		rxCommand_6,
		rxCommand_7,
		rxCommand_8,
		rxCommand_9
	};

	uint8_t index;				// Indice de comandos guardados
	bool full;					// Indicador de fifo llena
	bool bussy;					// Indicador de fifo ocupada

public:
	uint8_t rxCommand_0[10];	// Memoria para primer comando
	uint8_t rxCommand_1[10];	// Memoria para segundo comando
	uint8_t rxCommand_2[10];	// Memoria para tercer comando
	uint8_t rxCommand_3[10];	// Memoria para cuarto comando
	uint8_t rxCommand_4[10];	// Memoria para quinto comando
	uint8_t rxCommand_5[10];	// Memoria para sexto comando
	uint8_t rxCommand_6[10];	// Memoria para septimo comando
	uint8_t rxCommand_7[10];	// Memoria para octavo comando
	uint8_t rxCommand_8[10];	// Memoria para noveno comando
	uint8_t rxCommand_9[10];	// Memoria para decimo comando
	bool available();							// Indica que hay elementos en fifo
	void add( uint8_t *command, uint8_t size );	// Añade comando a la fifo
	void read( uint8_t *buffer, uint8_t size );	// Lee comando
};

/////////////////////
// FIFO UART CLASS //
/////////////////////

class fifoUart{

/////////////
// PRIVATE //
/////////////

private:

	uint8_t *finalElement;			// Puntero al final de la cadena
	bool emptyFifo;					// Indica si la fifo esta vacia
	bool bussyFifo;					// Indica si la fifo esta llena
	int large;						// Largo de la fifo

////////////
// PUBLIC //
////////////

public:
	fifoUart();

	uint8_t elements[50];			// Cadena de elementos

	bool insertElement( uint8_t element );	// Inserta elemento a la Fifo
	uint8_t readElement();					// Lee un elemento de la Fifo
	uint8_t seeElement();					// Mirar el ultimo elemento
	int	largeFifo();						// Muestra el largo de la FIFO
	bool available();						// Indica si hay elementos en la Fifo
	bool bussy();							// Indica si la fifo está llena
};


#endif /* INC_FIFOUART_H_ */
