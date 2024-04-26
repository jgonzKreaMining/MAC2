/*
 * loraTcp.cpp
 *
 *  Created on: Jan 8, 2024
 *      Author: jgonz
 */

#include "loraTcp.h"

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

/////////////////
// SET COMMAND //
/////////////////

void loraSelect::setCommand( uint8_t *command, uint8_t sizeCommand, uint8_t qtty ){
	this->command		= command;		//	Copia comando
	this->commandSize	= sizeCommand;	//	Copia lista de respuestas
	this->qtty			= qtty;			//	Copia cantidad de respeustas
	this->flagNew		= 1;			//	Indica que hay nuevo comando
}

////////////
// ENABLE //
////////////

void loraSelect::enable( bool flag ){
	this->flagEnable	= flag;		// Copia habilitacion externa
}

/////////////////
// NEW COMMAND //
/////////////////

bool loraSelect::newCommand(){
	bool flag = this->flagNew;								// Crea variable
	this->flagNew	= 0;

	return flag;							// Retorna salida
}


/////////////////
// GET COMMAND //
/////////////////

uint8_t	*loraSelect::getCommand(){
	return this->command;
}

///////////////////
// GET RESPONSES //
///////////////////

uint8_t loraSelect::getCommandSize(){
	return this->commandSize;
}

////////////////////////
// QUANTITY RESPONSES //
////////////////////////

uint8_t loraSelect::qttyResponses(){
	return this->qtty;
}


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


//////////////////
// SET RETRIES  //
//////////////////

void loraDecision::setRetries( uint8_t retries ){
	this->retries	= retries;					//
}

//////////////
// RESPONSE //
//////////////


void loraDecision::response ( uint8_t position ){
	this->position	= position;					//	Copia posicion

	if ( position == 0 || position == 12 ){						//	Si no hay error
		this->flagCorrect	= 1;				//	Indica que la respuesta es correcta
		this->flagEnable	= 1;
		this->flagBurn		= 0;				//	Reinicia flag quemado
		this->flagRepeat	= 0;				//	Reinicia flag repetir
		this->count			= 0;				//	Reinicia contador
	}
	else if ( position == 10 ){
		this->flagCorrect	= 0;				//	Indica que la respuesta es correcta
		this->flagEnable	= 1;
		this->flagBurn		= 1;				//	Reinicia flag quemado
		this->flagRepeat	= 0;				//	Reinicia flag repetir
		this->count			= 0;				//	Reinicia contador
	}
	else if ( position < 98 ){
		this->flagCorrect	= 1;				//	Indica que la respuesta es correcta
		this->flagEnable	= 1;
		this->flagBurn		= 0;				//	Reinicia flag quemado
		this->flagRepeat	= 0;				//	Reinicia flag repetir
		this->count			= 0;				//	Reinicia contador
	}
	else if ( position == 98 ){
		this->flagCorrect	= 0;
		this->flagBurn		= 0;
		this->flagRepeat	= 0;
		this->count			= 0;
		this->flagEnable	= 1;
	}
	else{										//	Si hay error
		this->flagCorrect	= 0;				//	Reinicia flag correcto
		this->count++;							//	Suma uno al contador

		if ( this->count >= this->retries ){	//	Si pasa limite de reintentos
			this->flagBurn		= 1;			//	Indica que esta quemado
			this->flagRepeat	= 0;			//	No hay que repetir
		}
		else{									//	Si no pasa  limite
			this->flagBurn		= 0;			//	No esta quemado
			this->flagRepeat	= 1;			//	Solicita repetir
		}
	}
}

///////////
// RESET //
///////////

void loraDecision::reset(){
	this->flagCorrect	= 0;	//	Reinicia indicador de msj correcto
	this->flagBurn		= 0;	//	Reinicia indicador de msj quemado
	this->flagRepeat	= 0;	//	Reinicia indicador de repetir mensaje
	this->count			= 0;	//	Reinicia contador
}

void loraDecision::disable(){
	this->flagEnable	= 0;
}

bool loraDecision::enable(){
	return this->flagEnable;
}

/////////////
// CORRECT //
/////////////

bool loraDecision::correct(){
	return this->flagCorrect;	//
}

//////////
// BURN //
//////////

bool loraDecision::burn(){
	return this->flagBurn;		//
}


////////////
// REPEAT //
////////////

bool loraDecision::repeat(){
	return this->flagRepeat;	//
}
