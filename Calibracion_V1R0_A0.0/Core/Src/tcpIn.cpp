/*
 * tcp.cpp
 *
 *  Created on: Jan 11, 2024
 *      Author: jgonz
 */

#include <tcp.h>
#include "loraTcp.h"

extern uint8_t superloop;

/////////
// GPS //
/////////

uint8_t stateGpsTcp;					// Estado de GPS TCP

extern bool startGps;					// Indica que se debe iniciar GPS
extern bool stopGps;					// Indica que se debe detener GPS
extern bool savedGps;					// Indica que se guardo GPS

bool flagGpsSaved;						// Indica que se guardó GPS

long int countGpsTcp;					// Contador para llegar al limite de GPS
long int limitGpsTcp = 120000/superloop;// Tiempo que esta apagado GPS

//////////
// LORA //
//////////

extern loraCheck loraCheck;			// Desde capa link, entrega el resultado de coincidencia
extern loraDecision loraDecision;	// Decide sobre la respuesta
extern loraSelect loraSelect;		// Contiene comando seleccionado y respuestas

extern bool flagWdLora;				// Indica si esta habilitado

uint8_t busTcpLora;					// Guarda codigo de respuesta

//////////
// DATA //
//////////

uint8_t stateTcpData;						// Estado TCP data
uint16_t countTcpData;						// Contador tcp data
uint16_t limitTcpData	= 120000/superloop;	// Limite para fijar datos
bool flagSetData;							// Indica que se debe enviar datos

////////////
// SYSTEM //
////////////

uint8_t stateTcpSys;						// Estado TCP system
uint16_t countTcpSys;						// Contador tcp system
uint16_t limitTcpSys	= 120000/superloop;	// Limite para fijar datos system
bool flagSetSys;							// Indica que se debe enviar system

/*****	INPUT	*****/

void tcpInput(){
	tcpData();
	tcpSystem();
	tcpGps();
	tcpInLora();
}

/******************
 ***** SYSTEM *****
 ******************
 *
 *	Reloj para enviar mensaje de variables de sistemas
 *
 *	OUTPUT	:	flagSetSys | Indica que se debe enviar un paquete con datos del sistema
 *
 */

void tcpSystem(){
	switch( stateTcpSys ){

	/////////////////////////
	// S0 - WAIT FOR LIMIT //
	/////////////////////////

	case 0:
		countTcpSys++;						// Suma 1 al contador

		if ( countTcpSys >= limitTcpSys ){	// Si pasa el limite
			stateTcpSys	= 1;				// Pasa a S1
		}
		else{								// Si no
			stateTcpSys	= 0;				// Espera en S0
		}
		break;

	///////////////////
	// S1 - SET DATA //
	///////////////////

	case 1:
		flagSetSys	= 1;	// Indica que se debe enviar datos
		countTcpSys	= 0;	// Reinicia contador
		stateTcpSys	= 0;	// Vuelve a S0
		break;

	default:
		stateTcpSys	= 0;
		break;
	}
}

/************************
 ***** DATA SENSORS *****
 ************************
 *
 *	Timer para enviar mensaje de datos de sensores
 *
 *	OUTPUT	:	flagSetData | Indica que se debe enviar
 *
 */

void tcpData(){
	switch( stateTcpData ){

	/////////////////////////
	// S0 - WAIT FOR LIMIT //
	/////////////////////////

	case 0:
		countTcpData++;							// Suma 1 al contador

		if ( countTcpData >= limitTcpData ){	// Si pasa el limite
			stateTcpData	= 1;				// Pasa a S1
		}
		else{									// Si no
			stateTcpData	= 0;				// Espera en S0
		}
		break;

	///////////////////
	// S1 - SET DATA //
	///////////////////

	case 1:
		flagSetData		= 1;	// Indica que se debe enviar datos
		countTcpData	= 0;	// Reinicia contador

		stateTcpData	= 0;	// Vuelve a S0
		break;

	default:
		stateTcpData	= 0;
		break;
	}
}

/************************
 *****	LORA INPUT	*****
 ************************
 *
 *	Guarda el codigo de conicidencia de la respuesta real vs respuesta ideal
 *	Deshabilita tcp si se esta reiniciando
 *
 *	INPUT	:	loraCheck.check
 *				loraCheck.response
 *				flagWdLora
 *
 *	OUTPUT	:	loraDecision.response()
 *				loraDecision.disable()
 */

void tcpInLora(){
	if ( loraCheck.check() ){					// Si hay respuesta nueva
		busTcpLora	= loraCheck.response();		// Copia el codigo de respuesta
		loraDecision.response( busTcpLora );	// Inserta en lora Decision
												// Lora Decision indica si hay que reintentar el comando
		/*
		if ( busTcpLora	== 98 ){
			busTcpLora	= 98;
		}
		else if (busTcpLora	== 6){
			busTcpLora	= 6;
		}*/

	}

	if ( flagWdLora ){			// Si hay WD
		loraDecision.disable();	// Deshabilita tcp y app
	}
}

/**************
 **** GPS *****
 **************
 *
 *	Espera que se guarde un dato, indica que se debe enviar un mensaje
 *	Espera un tiempo Fijo para iniciar gps nuevamente
 *
 *	INPUT	:	savedGps
 *
 *	OUTPUT	:	flagGpsSaved
 *				startGps
 */

void tcpGps(){
	switch ( stateGpsTcp ){

	//////////////////////////////////
	// S0 - WAIT FOR STOP MEASURING //
	//////////////////////////////////

	case 0:

		if ( savedGps ){		// Si se guardó un valor de GPS
			stateGpsTcp	= 1;	// Pasa a S1
		}
		else if( stopGps ){		// Si se paró la recepcion de datos
			stateGpsTcp	= 2;	// Pasa a S2
		}
		else{					// Si no pasa ninguna
			stateGpsTcp	= 0;	// Espera en S0
		}
		break;

	///////////////////
	// S1 - SET FLAG //
	///////////////////

	case 1:
		flagGpsSaved	= 1;	// Indica que se guardó un mensaje
		stateGpsTcp		= 2;	// Pasa a S3
		break;

	/////////////////////////////
	// S2 - WAIT FOR START GPS //
	/////////////////////////////

	case 2:
		countGpsTcp++;						// Suma 1 al contador

		if ( countGpsTcp >= limitGpsTcp ){	// Si pasa el limite
			startGps	= 1;				// Indica inicio de medicion gps
			countGpsTcp	= 0;				// Reinicia contador
			stateGpsTcp	= 0;				// Vuelve a S0
		}
		else{								// Si no
			stateGpsTcp	= 2;				// Se queda en S2
		}
		break;

	default:
		break;
	}
}




