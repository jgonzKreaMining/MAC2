/*
 * loraLink.cpp
 *
 *  Created on: Jan 2, 2024
 *      Author: jgonz
 */

#include "loraLink.h"


/////////////////
// NEW COMMAND //
/////////////////

/**********************
 ***	TIMEOUT		***
 **********************
 *
 *	Cuenta el tiempo entre la salida de un comando y la llegada de la repsuesta
 *
 *	Guia de uso:
 *
 *		1.- wdTimeout:	Fijar el timeout
 *		2.- setOutput:	Indica que salió un comando
 *		3.- addOne:		Añade uno al contador y revisa timeout
 *		4.- setInput:	Indica cuando llega la respuesta
 *		5.- newMessage:	Indica si se recibió un mensaje
 *		6.- timeout:	Indica si hubo un timeout
 *
 */

wdTimeout::wdTimeout( long limit ){	// Constructor
	this->limit	= limit;			// Se debe fijar el limite de timeout
}

////////////////
// SET OUTPUT //
////////////////

void wdTimeout::setOutput(){	//
	this->flagOutput	= 1;	// Indica que salió un comando
	this->counter		= 0;	// Reinicia contador
}

/////////////
// ADD ONE //
/////////////

void wdTimeout::addOne(){
	if ( this->flagOutput ){			// Si salió un comando
		this->counter++;				// Suma uno al contador
	}

	if (this->counter >= this->limit ){	// Si pasa el límite
		this->flagTimeout	= 1;		// Se considera timeout
	}
	else{								// Si no
		this->flagTimeout	= 0;		// No indica timeout
	}
}

///////////////
// SET INPUT //
///////////////

void wdTimeout::setInput(){
	this->flagInput		= this->flagOutput;		// Copia indicador salida de comando )
												// Si hay multiples respuestas toma sentido
	this->flagOutput	= 0;					// Reinicia indicador de salida comando
	this->counter		= 0;					// Reinicia contador
}

/////////////////
// NEW MESSAGE //
/////////////////

bool wdTimeout::newMessage(){
	bool flag		= this->flagInput;	// Copia indicador llegada de mensaje
	this->flagInput	= 0;				// Reinicia indicador llegada de mensaje
	return flag;						// Retorna valor guardado
}

/////////////
// TIMEOUT //
/////////////

bool wdTimeout::timeOut(){
	bool flag			= this->flagTimeout;	// Copia indicador timeout
	this->flagTimeout	= 0;					// Reinicia indicador timeout
	return flag;								// Retorna valor guardado
}


/******************************
 ***	CHECK RESPONSE		***
 ******************************
 *
 *
 * Chequea la respuesta llegante entre las respuestas posibles
 *
 * Guia de uso:
 * 		1.-		setIdealResponses: Se fijan las respuestas ideales
 * 		2.-		newResponse: Se inserta la respuesta del modulo
 *		3.-		check: Indica cuando se finaliza el procesamiento
 *		4.-		response: Entrega la posición de la respuesta
 *							99 si no coincidió con ninguna respuesta
 */

//////////////////////////
// SET RESPONSE POINTER //
//////////////////////////

void loraCheck::setResponsePointer( uint8_t *pointer[], uint8_t sizeIdealResponses ){
	this->idealResponses		= &pointer[0];
	this->sizeIdealResponses	= sizeIdealResponses;
}

////////////////////////
// SET IDEAL RESPONSE //
////////////////////////

void loraCheck::setIdealResponses( uint8_t qtty ){
	this->qtty				= qtty;				// Copia el largo del arreglo
}

//////////////////
// NEW RESPONSE //
//////////////////

void loraCheck::newResponse( uint8_t *response, uint8_t sizeResponse ){
	this->realResponse	= response;			// Copia la respuesta entregada por el modulo
	this->sizeResponse	= sizeResponse;

	uint8_t i;	// Crea variable auxiliar
	uint8_t j;

	for( i = 0; i < this->qtty; i++ ){								// Recorre el arreglo con for
		for ( j = 0; j < this->sizeResponse; j++ ){
			if ( *(this->idealResponses[i] + j) == this->realResponse[j]){
				this->flagCorrect	= 1;
			}
			else{
				this->flagCorrect	= 0;
				break;
			}
		}
		if (this->flagCorrect){
			break;
		}
	}

	this->position	= i;											// Copia posicion final
	this->flagCheck	= 1;											// Indica que hay nuevo resultado
}

////////////////////
// CHECK RESPONSE //
////////////////////

bool loraCheck::check(){
	bool flag;							// Crea variable auxiliar
	flag			= this->flagCheck;	// Copia resultado de flag
	this->flagCheck	= 0;				// Reinicia el indicador de nuevo resultado

	return 	flag;						// Entrega resultado
}

//////////////
// RESPONSE //
//////////////

uint8_t loraCheck::response(){
	uint8_t	finalResult;				// Crea variable auxiliar
	uint8_t i;

	if (this->flagCorrect){				// Si hubo respuesta correcta
		finalResult	= this->position;	// Copia posición
	}
	else{								// Si no
		finalResult	= 99;				// Entrega 99
	}
	finalResult = this->position;

	if ( this->sizeResponse > 30 ){
		for (i = 0; i < this->sizeResponse-1; i++){
			if ( this->realResponse[i] == 'R' && this->realResponse[i+1] == 'N'){
				finalResult	= 98;
			}
		}
	}

	return finalResult;					// Retorna decisión
}

/******************************
 ***	DETECT RESPONSE 	***
 ******************************
 *
 *	Detecta un mensaje cuando encuentra un salto de linea
 *
 *	Manual de uso:
 *		1.-	insertElement:	Se insertan los elementos de a uno desdes la FIFO
 *		2.- newResponse:	Indica cuando se detectó un \n
 *		3.- response:		Entrega puntero de la respuesta
 *		4.- responseSize:	Entrega tamaño de la respuesta
 *		5.- insertBuffer:	Inserta puntero del buffer respuesta
 */

///////////////////
// INSERT BUFFER //
///////////////////

void loraInput::insertBuffer(uint8_t *buffer, uint8_t sizeBuffer){
	this->buffer		= buffer;		// Guarda puntero inicial del buffer
	this->sizeBuffer	= sizeBuffer;	// Guarda tamaño del buffer
}

////////////////////
// INSERT ELEMENT //
////////////////////

void loraInput::insertElement(uint8_t element){
	if (element == '\n'){							// Si llega un salto de linea
		this->sizeResponse		= this->count;		// Guarda tamaño de la respuesta
		this->count				= 0;				// Reinicia contador
		this->flagNewResponse	= 1;				// Indica que llego mensaje nuevo
	}
	else{											// Si no

		if (this->count >= this->sizeBuffer){		// Si el contador se pasa del buffero
			this->count	= 0;						// Reinicia buffer
		}
		this->buffer[this->count]	= element;	// Copia elemento en buffer
		this->count++;
	}
}

//////////////////
// NEW RESPONSE //
//////////////////

bool loraInput::newResponse(){						// Se reinicia cuando es consultado
	bool flag				= this->flagNewResponse;	// Copia flag de nuevo mensaje
	this->flagNewResponse	= 0;						// Reinicia flag
	return flag;										// Retorna respuesta guardada
}

//////////////
// RESPONSE //
//////////////

uint8_t* loraInput::response(){	//
	return this->buffer;		// Entrega la respuesta final
}

uint8_t loraInput::responseSize(){
	return this->sizeResponse;
}
