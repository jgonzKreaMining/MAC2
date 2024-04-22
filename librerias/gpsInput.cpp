/*
 * gpsInput.cpp
 *
 *  Created on: Oct 24, 2023
 *      Author: jgonz
 */


#include "gpsInput.h"

///////////////////
// CONTRSTRUCTOR //
///////////////////

gpsInput::gpsInput(){

}

/****************************************/
/******** METODOS DE ESCRITURA **********/
/******** VARIABLES PRIVADAS ************/
/****************************************/

//////////////////
// INSERT VALUE //
//////////////////

void gpsInput::insertValue(uint8_t symbol){

	uint8_t *state	= &this->stateInsert;	// El estado apunta al registro del objeto

	switch ( *state ){

		///////////////////////
		// S0 - FIRST SYMBOL //
		///////////////////////
		case 0:
			if ( symbol == '$' ){	// Si el simbolo es $
				*state	=	1;		// Pasa a S1
			}
			else{					// Si no
				*state	= 	0;		// Se queda en S0
			}
		break;

		////////////////////////
		// S1 - SECOND SYMBOL //
		////////////////////////

		case 1:
			if ( symbol ==  'G' ){	// Si el simbolo es G
				*state	= 2;		// Pasa a S2
			}
			else{					// Si no
				*state	= 0;		// Vuelve a S0
			}
			break;

		///////////////////////
		// S2 - THIRD SYMBOL //
		///////////////////////

		case 2:
			if ( symbol	== 'P'){	// Si el simbolo es P
				*state	= 3;		// Pasa a S3
			}
			else{					// Si no
				*state	= 0;		// Vuelve a s0
			}
			break;

		////////////////////////
		// S3 - FOURTH SYMBOL //
		////////////////////////

		case 3:
			if ( symbol == 'R'){	// Si el simbolo es R
				*state	= 4;		// Pasa a S4
			}
			else{					// Si no
				*state	= 0;		// Vuelve a S0
			}
			break;

		///////////////////////
		// S4 - FIFTH SYMBOL //
		///////////////////////

		case 4:
			if ( symbol == 'M'){	// Si el simbolo es M
				*state	= 5;		// Pasa a S5
			}
			else{					// Si no
				*state	= 0;		// Vieñve a S0
			}
			break;

		///////////////////////
		// S5 - SIXTH SYMBOL //
		///////////////////////

		case 5:
			if ( symbol == 'C'){	// Si el simbolo es M
				*state	= 6;		// Pasa a S6
			}
			else{					// Si no
				*state	= 0;		// Vieñve a S0
			}
			break;

		///////////////////////
		// S6 - GPRMC PACKET //
		///////////////////////

		case 6:
			if ( symbol == ','){	// Si el simbolo es , y confirma el formato del paquete
				*state	= 7;		// Pasa a S7
				this->setMode(1);	// Inserta el valor de mensaje tipo GPRMC
			}
			else{					// Si no
				*state	= 0;		// Vieñve a S0
			}
			break;

		/////////////
		// DEFAULT //
		/////////////

		default:
			*state	= 0;
			break;
	}
}

//////////////
// SET MODE //
//////////////

void gpsInput::setMode(uint8_t mode){
	this->mode	= mode;
}


void setLatitud(float lat);			// Fija latitud
void setLongitud(float longit);		// Fija longitud
void setHeight(float alt);			// Fija altura

///////////////
// SET VALID //
///////////////

void gpsInput::setValid(bool valid){
	this->flagValid	= valid;
}

/****************************************/
/********* METODOS DE LECTURA ***********/
/********* VARIABLES PTIVADAS ***********/
/****************************************/

/////////////////
// GET LATITUD //
/////////////////

float gpsInput::getLatitud(){
	return this->latitud;
}

//////////////////
// GET LONGITUD //
//////////////////

float gpsInput::getLongitud(){
	return this->longitud;
}

////////////////
// GET HEIGHT //
////////////////

float gpsInput::getHeight(){
	return this->height;
}

//////////////
// GET MODE //
//////////////

int gpsInput::getMode(){
	return this->mode;
}

/////////////////
// GET BETWEEN //
/////////////////

int gpsInput::getBetween(){
	return this->lapsBetween;
}

///////////////////////
// GET VALID MESSAGE //
///////////////////////

bool gpsInput::getValid(){
	return this->flagValid;
}

//////////////////////
// GET TIMEOUT FLAG //
//////////////////////

bool gpsInput::getTimeout(){
	return this->flagLaps;
}

//////////////////////////
// GET NEW MESSAGE FLAG //
//////////////////////////

bool gpsInput::getNewMessage(){
	bool flag =	this->flagNew;	// Guarda el valor de flagNew
	this->flagNew	= 0;		// Reinicia el flag
								// Ya que si se vuelve a consultar, no es un mensaje nuevo
	return flag;				// Entrega el valor original de flagNew

}
