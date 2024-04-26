/*
 * loraTcp.h
 *
 *  Created on: Jan 8, 2024
 *      Author: jgonz
 */

#include <string>
#include <stdint.h>

#ifndef INC_LORATCP_H_
#define INC_LORATCP_H_


/***********************
 *** SELECT RESPONSE ***
 ***********************
 *
 *	Permite el paso de un comando a la capa de enlace
 *	Solo estanca el paso de un comando dado señales externas
 *
 *	Guia de usuario:
 *		1.-	setCommand:		Inserta comando, respuestas y cantidad de respuestas
 *							Activa señal de nuevo comando
 *		2.- enable:			Habilita o deshabilita paso de un comando
 *		3.- newCommand:		Indica que hay nuevo comando
 *		4.-	repeat:			Repite comando
 *		5.- getCommand:		Entrega puntero de comando
 *		6.- getResponses:	Entrega puntero de respuestas
 *		7.- qttyResponses:	Entrega cantidad de respuestas
 *
 */

class loraSelect{
private:
	uint8_t *command;										// Guarda puntero de comando
	uint8_t commandSize;										//	Guarda puntero de respuestas
	uint8_t qtty;												// Guarda cantidad de respuestas
	bool flagEnable;											// Indicador de siguiente comando habilitado
	bool flagNew;												// Indicador si hay nuevo comando
public:
	void setCommand( uint8_t *command, uint8_t sizeCommand, uint8_t qtty);	// Inserta comando, respuestas y cantidad de respuestas
	void enable(bool flag);										// Habilita/deshabilita comando
	bool newCommand();											// Indica que hay nuevo comando
	uint8_t *getCommand();									// Retorna puntero de comando
	uint8_t getCommandSize();								// Retorna puntero de respuestas
	uint8_t qttyResponses();									// Retorna cantidad de respuestas
};

/********************
 *** DECISION TCP ***
 ********************
 *
 * Indica si debe aprobar respuesta, indicar que se quemo el comando o repetir el comando
 *
 * Guia de usuario
 * 		1.- setRetries	: Cantidad de reintentos antes de quemar comando
 * 		2.- response	: Inserta una respuesta y decide si avanza, repite o quema comando
 * 		3.- correct		: Indica que es correcto
 * 		4.- burn		: Indica que esta quemado
 * 		5.- repeat:		: Indica que debe repetir
 */


class loraDecision{
private:
	uint8_t retries;					//	Cantidad máxima de reintentos
	uint8_t position;					//	Posicion que coincide respuesta
	uint8_t count;						//	Contador de reintentos

	bool flagCorrect;					//	Indica si la respuesta es correcta
	bool flagBurn;						//	Indica si el comando se quemo por reintentos
	bool flagRepeat;					//	Indica si se debe repetir comando
	bool flagEnable;

public:
	void response(uint8_t position);	//	Inserta posicion
	void reset();						//	Reinicia contador y flags
	void setRetries(uint8_t retries);	//	Fija la cantidad de reintentos
	void disable();

	bool enable();
	bool correct();						//	Indica si la posicion es correcta
	bool burn();						//	Indica si el comando se quemo
	bool repeat();						//	Indica si debe repetir comando
};

#endif /* INC_LORATCP_H_ */
