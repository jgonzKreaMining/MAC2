/*
 * loraLink.h
 *
 *  Created on: Jan 2, 2024
 *      Author: jgonz
 */

#include <string>
#include <stdint.h>


#ifndef INC_LORALINK_H_
#define INC_LORALINK_H_


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

class wdTimeout{
private:
	bool flagInput;			// Indica que llego una respuesta
	bool flagOutput;		// Indica que salio un comando
	bool flagTimeout;		// Indica que hay timeout
	long counter;			// Contador
	long limit;				// Limite de contador

public:
	void setInput();		// Indica que llego respuesta
	void setOutput();		// Indica que salio un comando
	void addOne();			// Agrega uno a contador

	bool newMessage();		// Indica que llegó una respuesta
	bool timeOut();			// Indica que hay timeout
	wdTimeout(long limit);	// Constructor, fija limite
};


/******************************
 ***	CHECK RESPONSE		***
 ******************************
 *
 *
 * Chequea la respuesta llegante entre las respuestas posibles
 *
 * Guia de uso:
 * 		1.-		setIdealResponses:	Se fijan las respuestas ideales
 * 		2.-		newResponse: 		Se inserta la respuesta del modulo
 *		3.-		check: 				Indica cuando se finaliza el procesamiento
 *		4.-		response: 			Entrega la posición de la respuesta
 *									99 si no coincidió con ninguna respuesta
 */

class loraCheck{
private:

	//uint8_t *realResponse;			//	Respuesta real
	uint8_t sizeResponse;			//	Largo de respuesta real
	uint8_t qtty;					//	Largo de respuesta inicial
	uint8_t position;				// 	Posición que realizó match

	bool flagCorrect;				//  Indica que hay respuesta correcta
	bool flagCheck;					//	Indica que hay nueva respuesta

public:
	uint8_t **idealResponses;		//  Puntero a la respuesta inicial
	uint8_t sizeIdealResponses;
	uint8_t *realResponse;

	void setResponsePointer( uint8_t *pointer[], uint8_t sizeIdealResponses );
	void setIdealResponses( uint8_t qtty );	// Inserta las respuestas ideales
	void newResponse( uint8_t *response, uint8_t sizeResponse );	// Inserta la respuesta del chip

	uint8_t response();												// Retorna respuesta
	bool check();													// Indica si se comparó la nueva respuesta
};

/******************************
 ***	DETECT RESPONSE 	***
 ******************************
 *
 *	Detecta un mensaje cuando encuentra un salto de linea
 *
 *	Manual de uso:
 *		1.-	insertElement:	Se insertan los elementos de a uno desdes la FIFO
 *		2.- newResponse:	Indica cuando se detectó un \n
 *		3.- response:		Entrega la respuesta como String
 */

class loraInput{
private:
	uint8_t count;
	uint8_t *buffer;
	uint8_t sizeBuffer;
	uint8_t bufferIntern[50];				// Acumula los elementos
	uint8_t sizeResponse;
	bool flagNewResponse;					// Indica que hay nueva respuesta
public:
	void insertElement(uint8_t element);	// Se insertan los elementos en bussElements
	bool newResponse();						// Indica que hay nueva respuesta
	uint8_t* response();					// Retorna la respuesta como string
	uint8_t responseSize();					// Largo de
	void insertBuffer(uint8_t *buffer, uint8_t sizeBuffer);
};

#endif /* INC_LORALINK_H_ */
