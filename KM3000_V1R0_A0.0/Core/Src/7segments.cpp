/*
 * 7segments.cpp
 *
 *  Created on: Jan 9, 2024
 *      Author: jgonz
 */

#include "7segments.h"


/******************
 ***	LINK	***
 ******************
 *
 *	Ingreso de numero en display, separa el numero en unidad, decena, centena y mil
 *	Guarda posicion de Dot
 *
 *	Guia de usuario:
 *		1.-	inputDisplay:	Ingreso de numero en display y posicion de punto
 *		2.- dotPosition:	Retorna posicion del boton
 *		3.- numberInDigit:	Retorna numero en digito especifico
 */

//////////////////
// DOT POSITION //
//////////////////

uint8_t displayLink::dotPosition(){
	return this->dot;
}


///////////////////
// INPUT DISPLAY //
///////////////////

void displayLink::inputDisplay( int number, uint8_t dot ){
	this->dot	= dot;																		// Copia posicion del punto

	this->milesima	= number/1000;															// Obtiene milesima
	this->centena	= (number - this->milesima*1000)/100;									// Obtiene centena
	this->decena	= (number - this->milesima*1000 - this->centena*100)/10;				// Obtiene decena
	this->unidad	= (number - this->milesima*1000 - this->centena*100 - this->decena*10);	// Obtiene unidad
}

/////////////////////
// NUMBER IN DIGIT //
/////////////////////

uint8_t displayLink::numberInDigit( uint8_t digit ){
	uint8_t number;					// Crea variable

	switch(digit){					// Segun el numero del digito que entra
	case 1:							// Caso 1
		number	= this->milesima;	// Retorna milesima
		break;

	case 2:							// Caso 2
		number	= this->centena;	// Retorna centena
		break;

	case 3:							// Caso 3
		number	= this->decena;		// Retorna decena
		break;

	case 4:							// Caso 4
		number	= this->unidad;		// Retorna unidad
		break;

	default:						// En otro caso
		number	= 0;				// Retorna 0
		break;
	}
	return number;					// Retorna variable
}

/**********************
 ***	PHYSICAL	***
 **********************
 *
 *	Asigna valores de pines en hardware segun numero en display
 *
 *	Guia de usuario:
 *		1.-	enable:		Habilita display 1: Habilita 0: Deshabilita
 *		2.-	setDot:		Indica posicion del punto
 *		3.- setNumber:	Fija valor en digito
 *		4.-	next:		Indica digito que sigue en el despliegue
 *						Se actualiza cada vez que se usa setNumber
 *
 */

////////////
// ENABLE //
////////////

void displayPhysical::enable( bool onOff ){
	if ( !onOff ){					//	Si esta apagado
		this->flagEnable	= 0;	//	Deshabilita
	}
	else{							//	Si esta prendido
		this->flagEnable	= 1;	//	Habilita
	}
}

/////////////
// SET DOT //
/////////////

void displayPhysical::setDot( uint8_t dot ){
	this->dot	= dot;
}

//////////
// NEXT //
//////////

uint8_t displayPhysical::next(){
	return this->counter;
}

////////////////
// SET NUMBER //
////////////////

void displayPhysical::setNumber( uint8_t number ){

	this->dpSignal	= 0;					// Reinicia la señal de Dot
	if (this->dot	== this->counter ){		// Si la posicion dot coincide con contador
		this->dpSignal	= 1;				// Se activa la señal de dot
	}

	switch ( this->counter ){				// Dependiendo del contador
	case 1:									//	Caso 1
		this->oneSignal		= 1;			//	Activa led 1
		this->twoSignal		= 0;			//	Desactiva led 2
		this->threeSignal	= 0;			//	Desactiva led 3
		this->fourSignal	= 0;			//	Desactiva led 4
		this->counter		= 2;			//	Cambia contador a 2
		break;

	case 2:									//	Caso 2
		this->oneSignal		= 0;			//	Desactiva led 1
		this->twoSignal		= 1;			//	Activa led 2
		this->threeSignal	= 0;			//	Desactiva led 3
		this->fourSignal	= 0;			//	Desactiva led 4
		this->counter		= 3;			//	Cambia contador a 3
		break;

	case 3:									//	Caso 3
		this->oneSignal		= 0;			//	Desactiva led 1
		this->twoSignal		= 0;			//	Desactiva led 2
		this->threeSignal	= 1;			//	Activa led 3
		this->fourSignal	= 0;			//	Desactiva led 4
		this->counter		= 4;			//	Cambia a contador 4
		break;

	case 4:									//	Caso 4
		this->oneSignal		= 0;			//	Desactiva led 1
		this->twoSignal		= 0;			//	Desactiva led 2
		this->threeSignal	= 0;			//	Desactiva led 3
		this->fourSignal	= 1;			//	Activa led 4
		this->counter		= 1;			//	Cambia a contador en 1
		break;

	default:
		this->counter	= 1;
		break;
	}

	if ( this->flagEnable ){		//	Si el display esta habilitado

		switch( number ){			//	Segun el numero que ingresa

		case 0:						//	Caso 0: asigna pines para marcar 0
			this->aSignal	= 1;
			this->bSignal	= 1;
			this->cSignal	= 1;
			this->dSignal	= 1;
			this->eSignal	= 1;
			this->fSignal	= 1;
			this->gSignal	= 0;
			break;

		case 1:						//	Caso 1: asigna pines para marcar 1
			this->aSignal	= 0;
			this->bSignal	= 1;
			this->cSignal	= 1;
			this->dSignal	= 0;
			this->eSignal	= 0;
			this->fSignal	= 0;
			this->gSignal	= 0;
			break;

		case 2:						//	Caso 2: asigna pines para marcar 2
			this->aSignal	= 1;
			this->bSignal	= 1;
			this->cSignal	= 0;
			this->dSignal	= 1;
			this->eSignal	= 1;
			this->fSignal	= 0;
			this->gSignal	= 1;
			break;

		case 3:						//	Caso 3: asigna pines para marcar 3
			this->aSignal	= 1;
			this->bSignal	= 1;
			this->cSignal	= 1;
			this->dSignal	= 1;
			this->eSignal	= 0;
			this->fSignal	= 0;
			this->gSignal	= 1;
			break;

		case 4:						//	Caso 4: asigna pines para marcar 4
			this->aSignal	= 0;
			this->bSignal	= 1;
			this->cSignal	= 1;
			this->dSignal	= 0;
			this->eSignal	= 0;
			this->fSignal	= 1;
			this->gSignal	= 1;
			break;

		case 5:						//	Caso 5: asigna pines para marcar 5
			this->aSignal	= 1;
			this->bSignal	= 0;
			this->cSignal	= 1;
			this->dSignal	= 1;
			this->eSignal	= 0;
			this->fSignal	= 1;
			this->gSignal	= 1;
			break;

		case 6:						//	Caso 6: asigna pines para marcar 6
			this->aSignal	= 1;
			this->bSignal	= 0;
			this->cSignal	= 1;
			this->dSignal	= 1;
			this->eSignal	= 1;
			this->fSignal	= 1;
			this->gSignal	= 1;
			break;

		case 7:						//	Caso 7: asigna pines para marcar 7
			this->aSignal	= 1;
			this->bSignal	= 1;
			this->cSignal	= 1;
			this->dSignal	= 0;
			this->eSignal	= 0;
			this->fSignal	= 0;
			this->gSignal	= 0;
			break;

		case 8:						//	Caso 8: asigna pines para marcar 8
			this->aSignal	= 1;
			this->bSignal	= 1;
			this->cSignal	= 1;
			this->dSignal	= 1;
			this->eSignal	= 1;
			this->fSignal	= 1;
			this->gSignal	= 1;
			break;

		case 9:						//	Caso 9: asigna pines para marcar 9
			this->aSignal	= 1;
			this->bSignal	= 1;
			this->cSignal	= 1;
			this->dSignal	= 1;
			this->eSignal	= 0;
			this->fSignal	= 1;
			this->gSignal	= 1;
			break;

		default:
			this->aSignal	= 0;
			this->bSignal	= 0;
			this->cSignal	= 0;
			this->dSignal	= 0;
			this->eSignal	= 0;
			this->fSignal	= 0;
			this->gSignal	= 0;
			break;
		}
	}
	else{						// Si esta deshabiliado no marca display
		this->aSignal	= 0;
		this->bSignal	= 0;
		this->cSignal	= 0;
		this->dSignal	= 0;
		this->eSignal	= 0;
		this->fSignal	= 0;
		this->gSignal	= 0;
		this->dpSignal	= 0;
		this->counter	= 0;
	}
}

//////////////////
// FIRST NUMBER //
//////////////////

bool displayPhysical::firstNumber(){
	return this->oneSignal;
}

///////////////////
// SECOND NUMBER //
///////////////////

bool displayPhysical::secondNumber(){
	return this->twoSignal;
}

//////////////////
// THIRD NUMBER //
//////////////////

bool displayPhysical::thirdNumber(){
	return this->threeSignal;
}

///////////////////
// FOURTH NUMBER //
///////////////////

bool displayPhysical::fourthNumber(){
	return this->fourSignal;
}

///////////
// A LED //
///////////

bool displayPhysical::aLed(){
	return this->aSignal;
}

///////////
// B LED //
///////////

bool displayPhysical::bLed(){
	return this->bSignal;
}

///////////
// C LED //
///////////

bool displayPhysical::cLed(){
	return this->cSignal;
}

///////////
// D LED //
///////////

bool displayPhysical::dLed(){
	return this->dSignal;
}

///////////
// E LED //
///////////

bool displayPhysical::eLed(){
	return this->eSignal;
}

///////////
// F LED //
///////////

bool displayPhysical::fLed(){
	return this->fSignal;
}

///////////
// G LED //
///////////

bool displayPhysical::gLed(){
	return this->gSignal;
}

////////////
// DP LED //
////////////

bool displayPhysical::dpLed(){
	return this->dpSignal;
}
