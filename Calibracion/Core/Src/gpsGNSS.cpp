/*
 * gps_Ublox_6M.cpp
 *
 *  Created on: Oct 24, 2023
 *      Author: jgonz
 */


#include <gpsGNSS.h>
#include <math.h>



///////////////////////////
// CONSTRUCTOR GPS INPUT //
///////////////////////////

gpsInput::gpsInput(){
};

/****************************************/
/******** METODOS DE ESCRITURA **********/
/******** VARIABLES PRIVADAS   **********/
/****************************************/

//////////////////
// INSERT VALUE //
//////////////////

void gpsInput::insertValue(uint8_t symbol){

	uint8_t *state	= &this->stateInsert;	// El estado apunta al registro del objeto

	this->lapsCounter++;			// Aumenta una vuelta

	switch ( *state ){

		///////////////////////
		// S0 - FIRST SYMBOL //
		///////////////////////
		case 0:
			this->flagNew	= 0;

			if ( symbol == '$' ){	// Si el simbolo es $
				*state	=	1;		// Pasa a S1
			}
			else if( symbol == 'G' ){
				*state	= 2;
			}
			else{					// Si no
				*state	= 	0;		// Se queda en S0
			}
		break;

		////////////////////////
		// S1 - SECOND SYMBOL //
		////////////////////////

		case 1:

			if ( symbol ==  'G' ){		// Si el simbolo es G
				*state	= 2;			// Pasa a S2
			}
			else{						// Si no
				*state	= 0;			// Vuelve a S0
			}
			break;

		///////////////////////
		// S2 - THIRD SYMBOL //
		///////////////////////

		case 2:

			if ( symbol	== 'P'){		// Si el simbolo es P
				*state	= 3;			// Pasa a S3
			}
			else{						// Si no
				*state	= 0;			// Vuelve a s0
			}
			break;

		////////////////////////
		// S3 - FOURTH SYMBOL //
		////////////////////////

		case 3:
			if ( symbol == 'R'){		// Si el simbolo es R
				*state	= 4;			// Pasa a S4
			}
			else if ( symbol == 'V'){	// Si el simbolo es T
				*state	= 50;			// Pasa a S50 - DETECTA SI ES MODO GPTVG
			}
			else if ( symbol == 'G'){	// Si simboolo es G
				*state = 53;			// Pasa a S53 - DETECTA GPGSV
			}
			else{						// Si no
				*state	= 0;			// Vuelve a S0
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
			this->lapsBetween	= this->lapsCounter;	//Guarda valor de contador
			this->lapsCounter	= 0;					// Reinicia contador
			this->flagNew		= 1;					// Indica que llego mensaje

			this->setMode(1);							// Inserta el valor de mensaje tipo GPRMC
			this->countGprmc	= 0;					// Reinicia el contador que guarda elementos

			if ( symbol == ','){						// Si el simbolo es , y confirma el formato del paquete
				*state	= 7;							// Pasa a S7
			}
			else{										// Si no
				*state	= 0;							// Vieñve a S0
			}
			break;

		////////////////////
		// S7 - SAVE TIME //
		////////////////////

		case 7:
			this->flagNew		= 0;				// Indica que llego mensaje

			if ( symbol == ',' ){					// Si el simbolo es ,
				this->countGprmc	= 0;			// Reinicia contador
				*state	= 8;						// Pasa a S8
			}
			else if ( this->countGprmc >= 10 ){
				this->countGprmc	= 0;
				*state	= 0;
			}
			else{										// Si no
				this->time[this->countGprmc]  =	symbol;	// Guarda el simbolo en el buffer
				this->countGprmc++;						// Suma uno al indice
				*state	= 7;							// Se queda en S7
			}

			break;

		/////////////////////
		// S8 - SAVE VALID //
		/////////////////////

		case 8:
			/*
			 * El mensaje debería venir tipo
			 *
			 * 	,A,
			 *
			 * 	Fija VALID hasta que encuentre ,
			 */
			if( symbol == ',' ){			// Si es ,
				*state	= 9;				// Pasa a S9
			}
			else{							// Si no
				this->setValid(symbol);		// Fija simbolo de valido
				*state	= 8;				// Se queda en S8
			}
			break;

		////////////////////////
		// S9 - SAVE LONGITUD //
		////////////////////////

		case 9:
			if ( symbol == ',' ){							// Si es ,
				this->countGprmc	= 0;					// Reinicia contador
				*state 	= 10;								// Pasa a S10
			}
			else if ( this->countGprmc >= 10 ){
				this->countGprmc	= 0;
				*state	= 0;
			}
			else{											// Si no
				this->latitud[this->countGprmc]  =	symbol;	// Guarda el simbolo en el buffer
				this->countGprmc++;							// Suma uno al indice
				*state	= 9;								// Se queda en S9
			}
			break;

		///////////////////////////////////
		// S10 - SAVE LONGITUD DIRECTION //
		///////////////////////////////////

		case 10:
			if ( symbol == ',' ){			// Si es ,
				*state	= 11;				// Pasa a S11
			}
			else{							// Si no
				this->latDirect	= symbol;	// Guarda el simbolo
				*state	= 10;				// Pasa a S10
			}
			break;

		////////////////////////
		// S11 - SAVE LATITUD //
		////////////////////////

		case 11:
			if ( symbol == ',' ){							// Si el simbolo es ,
				this->countGprmc	= 0;					// Reinicia contador
				*state	= 12;								// Pasa a S12
			}
			else if ( this->countGprmc >= 10 ){
				this->countGprmc	= 0;
				*state	= 0;
			}
			else{											// Si no
				this->longitud[this->countGprmc]  =	symbol;	// Guarda el simbolo en el buffer
				this->countGprmc++;							// Suma uno al indice
				*state	= 11;								// Queda en S11
			}
			break;

		//////////////////////////////////
		// S12 - SAVE DIRECTION LATITUD //
		//////////////////////////////////

		case 12:
			if ( symbol == ',' ){
				*state = 13;
			}

			else{
				this->longDirect	= symbol;
				*state = 12;
			}
			break;

		////////////////////
		// S13 - VELOCITY //
		////////////////////

		case 13:
			if ( symbol == ',' ){							//
				this->countGprmc	= 0;					// Reinicia contador
				*state	= 14;								//
			}
			else if ( this->countGprmc >= 10 ){
				this->countGprmc	= 0;
				*state	= 0;
			}
			else{											//
				this->velocity[this->countGprmc]  =	symbol;	// Guarda el simbolo en el buffer
				this->countGprmc++;							// Suma uno al indice
				*state 	= 13;								//
			}
			break;

		/////////////////
		// S14 - ANGLE //
		/////////////////

		case 14:
			if ( symbol == ',' ){							//
				this->countGprmc	= 0;					// Reinicia contador
				*state	= 15;								//
			}
			else if ( this->countGprmc >= 10 ){
				this->countGprmc	= 0;
				*state	= 0;
			}
			else{											//
				this->angle[this->countGprmc]  =	symbol;	// Guarda el simbolo en el buffer
				this->countGprmc++;							// Suma uno al indice
				*state	= 14;								//
			}
			break;

		////////////////
		// S15 - DATE //
		////////////////

		case 15:
			if ( symbol == ','){						// Si es una ,
				*state	= 0;							// Cierra la maquina
			}
			else{										// Si no
				this->date[this->countGprmc]  =	symbol;	// Guarda el simbolo en el buffer
				this->countGprmc++;						// Suma uno al indice
				*state	= 15;
			}
			break;

		////////////////////
		// S50 - DETECT V //
		////////////////////

		case 50:
			if (symbol == 'T'){	// Si detecta V
				*state	= 51;	// Pasa a S51
			}
			else{				// Si no
				*state	= 0;	// Vuelve a S0
			}
			break;

		/////////////////////
		// S51	- DETECT G //
		/////////////////////

		case 51:
			if (symbol == 'G'){	// Si detecta G
				*state	= 52;	// Pasa a S51
			}
			else{				// Si no
				*state	= 0;	// Vuelve a S0
				break;
			}
			break;

		/////////////////////////////////
		// S52 - DETECT A GPTVG PACKET //
		/////////////////////////////////

		case 52:
			this->setMode(2);
			*state	= 0;
			break;

		//////////////////////
		// S53 - DETECT A S //
		//////////////////////

		case 53:

			if (symbol == 'G'){
				*state = 54;
			}
			else if (symbol == 'L'){
				*state	= 57;
			}
			else if (symbol == 'S'){
				*state	= 59;
			}
			else{
				*state = 0;
			}
			break;

		//////////////////////
		// S54 - DETECT A V //
		//////////////////////

		case 54:
			if (symbol == 'A'){
				*state	= 55;
			}
			else if (symbol == 'V'){
				*state = 56;
			}
			else{
				*state	= 0;
			}
			break;

		////////////////////////
		// S55 - DETECT GPGSA //
		////////////////////////

		case 55:
			this->setMode(32);
			*state	= 0;
			break;

		////////////////////////
		// S56 - DETECT GPGSV //
		////////////////////////

		case 56:
			this->setMode(4);
			*state	= 0;
			break;

		////////////////////
		// S57 - DETECT L //
		////////////////////

		case 57:
			if (symbol == 'L'){
				*state	= 58;
			}
			else{
				*state	= 0;
			}
			break;

		////////////////////////
		// S58 - DETECT GPGLL //
		////////////////////////

		case 58:
			this->setMode(8);
			*state	= 0;
			break;

		////////////////////
		// S59 - DETECT A //
		////////////////////

		case 59:
			if (symbol == 'V'){
				*state	= 60;
			}
			else{
				*state	= 0;
			}
			break;

		////////////////////////
		// S60 - DETECT GPGGA //
		////////////////////////

		case 60:
			this->setMode(16);
			*state = 0;
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
	this->limitMsgModes	= 15;
	uint8_t output	= 0;
	uint8_t pot2[6]	= {1, 2, 4, 8, 16, 32};

	switch (mode){

	///////////
	// GPRMC //
	///////////
	case 1:
		this->countModes[0]	= this->limitMsgModes;	// Asigna valor maximo
		break;

	///////////
	// GPTVG //
	///////////
	case 2:
		this->countModes[1]	= this->limitMsgModes;	// Asigna valor maximo
		break;

	///////////
	// GPGGV //
	///////////

	case 4:
		this->countModes[2]	= this->limitMsgModes;	// Asigna valor maximo
		break;

	///////////
	// GPGLL //
	///////////
	case 8:
		this->countModes[3]	= this->limitMsgModes;	// Asigna valor maximo
		break;

	///////////
	// GPGSV //
	///////////
	case 16:
		this->countModes[4]	= this->limitMsgModes;	// Asigna valor maximo
		break;

	///////////
	// GPGGA //
	///////////
	case 32:
		this->countModes[5]	= this->limitMsgModes;
		break;

	default:
		break;
	}

	for (int i = 0; i<6; i++){			// Recorre array de count
		if (this->countModes[i] > 0){	// Si el contador >0
			this->countModes[i]--;		// Resta 1
			output = output + pot2[i];		// Suma un exponente de 2
		}								// Genera numero unico
		else{							// Si no
			//this->countModes[i]	= 0;	// Asigna 0
		}
	}
	this->mode	= output;	// Asigna modo
}

///////////////
// SET VALID //
///////////////

void gpsInput::setValid(char valid){
	this->flagValid	= valid;
}

/****************************************/
/********* METODOS DE LECTURA ***********/
/********* VARIABLES PTIVADAS ***********/
/****************************************/


//////////////
// GET DATE //
//////////////

long int gpsInput::getDate(){
	long int date;

	date	= (this->date[0]	- '0')*100000;	// DIA
	date	+= (this->date[1]	- '0')*10000;	// DIA
	date	+= (this->date[2]	- '0')*1000;	// MES
	date	+= (this->date[3]	- '0')*100;		// MES
	date	+= (this->date[4]	- '0')*10;		// DIA
	date	+= (this->date[5]	- '0');			// DIA

	return date;
}

//////////////
// GET TIME //
//////////////

float gpsInput::getTime(){
	float time;

	time	= 	(this->time[0] - '0')*100000;	// HORA
	time	+=	(this->time[1] - '0')*10000;	// HORA
	time	+=	(this->time[2] - '0')*1000;		// Minuto
	time	+= 	(this->time[3] - '0')*100;		// Minuto
	time	+=	(this->time[4] - '0')*10;		// Segundo
	time	+=	(this->time[5] - '0')*1;		// Segundo

	time	+= 	(this->time[7] - '0')*0.1;		// Decima
	time	+=	(this->time[8] - '0')*0.01;		// Centecima
	time	+=	(this->time[9] - '0')*0.001;	// Milesima

	return time;
}
////////////////////////////
// GET LONGITUD DIRECTION //
////////////////////////////

char gpsInput::getLatDirect(){
	return this->latDirect;
}

/////////////////
// GET LATITUD //
/////////////////

int gpsInput::getLatitud(){
	int latitud = 0;

	latitud	= 	(this->latitud[0] - '0')*1000;
	latitud	+= 	(this->latitud[1] - '0')*100;
	latitud	+= 	(this->latitud[2] - '0')*10;
	latitud	+= 	(this->latitud[3] - '0')*1;

	return latitud;
}

int gpsInput::getLatitud2(){
	int latitud = 0;

	latitud	= 	(this->latitud[5] - '0')*1000;
	latitud	+= 	(this->latitud[6] - '0')*100;
	latitud	+= 	(this->latitud[7] - '0')*10;
	latitud	+= 	(this->latitud[8] - '0')*1;
	//latitud	+= 	(this->latitud[9] - '0')*1;

	return latitud;
}

////////////////////////////
// GET LONGITUD DIRECTION //
////////////////////////////

char gpsInput::getLongDirect(){
	return this->longDirect;
}

//////////////////
// GET LONGITUD //
//////////////////

int gpsInput::getLongitud(){
	int longtd 	= 0;

	//longtd 	= 	(this->longitud[0]	- '0')*10000;
	longtd 	+= 	(this->longitud[1]	- '0')*1000;
	longtd 	+= 	(this->longitud[2]	- '0')*100;
	longtd 	+= 	(this->longitud[3]	- '0')*10;
	longtd	+=	(this->longitud[4]	- '0')*1;

	return longtd;
}

int gpsInput::getLongitud2(){
	int longtd 	= 0;

	longtd 	= 	(this->longitud[6]	- '0')*1000;
	longtd 	+= 	(this->longitud[7]	- '0')*100;
	longtd 	+= 	(this->longitud[8]	- '0')*10;
	longtd 	+= 	(this->longitud[9]	- '0')*1;
	//longtd 	+= 	(this->longitud[9]	- '0')*1;

	return longtd;
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

uint8_t gpsInput::getMode(){
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
	bool flag	= 0;

	if (this->flagValid == 'A'){
		flag = 1;
	}
	else{
		flag = 0;
	}

	return flag;
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
	return	this->flagNew;	// Guarda el valor de flagNew

}

uint8_t gpsInput::state_Insert(){
	return this->stateInsert;
}

//////////////
// HARDWARE //
//////////////

bool gpsInput::getSource(){
	return this->flagSource;
}

void gpsInput::setSource(bool src){
	this->flagSource	= src;
}
