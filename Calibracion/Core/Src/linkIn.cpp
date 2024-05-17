/*
 * link.cpp
 *
 *  Created on: Jan 11, 2024
 *      Author: jgonz
 */

#include "link.h"
#include "stm32l4xx_hal.h"


///////////////
// SUPERLOOP //
///////////////

extern uint8_t superloop;

/////////
// GPS //
/////////

extern gpsInput gpsInput;					// Instancia de gpsInput

extern float latitud;						// Valor de latitud entero
extern uint16_t latitud2;					// Valor de latitud fraccion
extern float longitud;						// Valor de longitud entero
extern uint16_t longitud2;					// Valor de longitud fraccion
extern bool gpsValid;						// Valor mensaje valido

extern bool startGps;						// Indicadir de encender GPS
extern bool stopGps;						// Indicador de apagar GPS
extern bool savedGps;						// Indicador de coordenadas guardadas

uint8_t stateGpsLink;						// Estado de la maquina

bool gpsNewMessage;							// Indicador nuevo mensaje

long int countGpsLink;						// Contador de limite esperando mensajes validos
long int limitGpsLink	= 240000/superloop;	// Limite de tiempo esperando mensajes validos

uint8_t countValidMsgGps;					// Contador de mensajes validos
uint8_t countInvalidMsgGps;					// Contador de mensajes invalidos

extern bool flagUartGps;					// Indica que recibio mensaje de Gps
uint8_t countSavedGps;						// Cuenta las respuesta guardadas correctamente

//////////
// LORA //
//////////

extern loraInput loraInput;		// Objeto que contiene el comando entrante
extern loraCheck loraCheck;		// Objeto que compara respuesta de llegada con respuesta ideal
extern wdTimeout loraTimeWd;	// Objeto que actua como watchdog de los mensajes lora

uint8_t* responseLora;			// Puntero a la respuesta
uint8_t responseLoraSize;		// Tamaño de la respuesta

uint8_t qttyMessagesInputs;		// Cantidad de mensajes entrantes
uint8_t qttyMessagesInputs2;

///////////////////
// LORA WATCHDOG //
///////////////////

uint8_t stateWdLora;			// Estado de ME

extern bool flagWdLora;			// Indica si se debe reiniciar el chip
extern bool flagEnableHwLora;	// Indica si esta habilitado

///////////
// BOTON //
///////////

extern bool flagBoton;							//	Indicador boton presionado
int countEnableDisplay;							//	Contador de display habilitado
int limitEnableDisplay		= 3000/superloop;	//	Limite de display habilitado
uint8_t stateEnableDisplay;						//	Estado de ME

extern displayPhysical displayPhy;				//	Display

////////////
// ANALOG //
////////////

extern uint16_t battery;					// Valor bateria
extern uint16_t alphaAnalog_A;				// Valor A1 adc interno
extern uint16_t alphaAnalog_B;				// Valor B1 adc interno

extern uint16_t alphaA;						// Valor A1 adc externo
extern uint16_t alphaB;						// Valor B1 adc externo

extern uint16_t tempIntern;					// Valor temperatura interno
extern uint16_t tempExtern;					// Valor temperatura externa
extern uint16_t humIntern;					// Valor humedad interno
extern uint16_t humExtern;					// Valor humedad externo

uint8_t	alpha_A1_PPM;						// Valor en PPM de HCL A1
uint8_t alpha_B1_PPM;						// Valor en PPM de HCL B1

uint16_t tempApp;							// Valor final temperatura
uint16_t humApp;								// Valor final humedad

extern bool warningHardware[];				// warnings de hardware
extern bool errorHardware[];				// errores de hardware

uint8_t stateCalibration;					// Estado de calibracion
long countCalibration;						// Contador para esperar calibracion
long limitCalibration	= 900000/superloop;	// Limite de calibracion
bool enableSensors;							// Habilita sensores

bool flagAlarm_PPM;							// Indica alarma PPM
bool flagAlarm_Temp;						// Indica alarma temperatura
bool flagAlarm_Hum;							// Indica alarma Humedad

/////////////
// BATTERY //
/////////////

uint8_t stateBattery;					// Estado de máquina para bateria
int batScaled;							// Valor bateria escalado
int thresholdBat	= 110;				// Valor maximo de bateria
uint16_t limitBat	= 5000/superloop;	// Tiempo que la bateria se considera en estado fijo
uint16_t countBat;						// Contador bateria

/***** INPUT *****/

void linkInput(){
	//linkInGps();
	//linkInLora();
	linkBoton();
	linkAnalog();
	linkBattery();
}

/////////////
// BATTERY //
/////////////

/*
 * Indica errores en bateria y estabilidad de esta
 *
 *
 * Input:
 * 		battery
 *
 * Output:
 * 		errorHardware[6]
 *
 */

void linkBattery(){

	batScaled = battery *510/10000;		// Escalamiento de bateria

	switch ( stateBattery ){

	//////////////////////////
	// S0 -  WAIT STABILITY //
	//////////////////////////

	case 0:
		countBat++;						// Suma 1 al contador

		if ( countBat >= limitBat ){	// Si pasa limite
			countBat		= 0;		// Reinicia contador
			stateBattery	= 1;		// Pasa a S1
		}
		else{							// Si no
			stateBattery	= 0;		// Queda en S0
		}
		break;

	////////////////////////
	// S1 - CHECK BATTERY //
	////////////////////////

	case 1:
		errorHardware[6]	= 0;			// Sin error

		if ( batScaled < thresholdBat ){	// Si la bateria baja del limite
			stateBattery	= 2;			// Pasa a S2
		}
		else{								// Si no
			stateBattery	= 1;			// Queda en S1
		}
		break;

	//////////////////////////
	// S2 - CHECK STABILITY //
	//////////////////////////

	case 2:
		errorHardware[6]	= 0;			// Sin error
		countBat++;							// Suma 1 al contador

		if ( batScaled > thresholdBat ){	// Si la bateria esta Ok
			countBat		= 0;			// Reinicia contador
			stateBattery	= 1;			// Vuelve a S1
		}
		else if ( countBat >= limitBat ){	// Si el contador pasa limite
			countBat		= 0;			// Reinicia contador
			stateBattery	= 3;			// Pasa a S3
		}
		else{								// Si no
			stateBattery	= 2;			// Queda en S2
		}
		break;

	///////////////////////////////////
	// S3 - CHECK BATTERY WITH ERROR //
	///////////////////////////////////

	case 3:
		errorHardware[6]	= 1;			// Indica error

		if ( batScaled < thresholdBat ){	// Si la bateria esta baja
			stateBattery	= 3;			// Sigue en S3
		}
		else{								// Si no
			stateBattery	= 4;			// Pasa a S4
		}
		break;

	//////////////////////////
	// S4 - CHECK STABILITY //
	//////////////////////////

	case 4:
		errorHardware[6]	= 1;			// Indica error
		countBat++;							// Suma 1 al contador

		if ( batScaled < thresholdBat ){	// Si la bateria sigue baja
			countBat		= 0;			// Reinicia contador
			stateBattery	= 3;			// Vuelve a S3
		}
		else if ( countBat >= limitBat ){	// Si contador pasa limite
			countBat		= 0;			// Reinicia contador
			stateBattery	= 1;			// Pasa a S3
		}
		else{								// Si no
			stateBattery	= 4;			// Queda en S4
		}
		break;

	default:
		stateBattery	= 0;
		break;
	}
}

/************************
 *****	LORA INPUT	*****
 ************************
 *
 *	LORA LINK INPUT se divide en dos partes
 *
 *	SOFTWARE:	1. Consulta si se detectó una respuesta nueva
 *				2. Guarda la respuesta y tamaño
 *				3. Inserta en loraCheck la respuesta
 *				4. Indica que llegó una respuesta
 *
 *				INPUT	:	loraInput.newResponse, loraInput.response, loraInput.responseSize
 *
 *				OUTPUT	:	loraTimeWd.setInput
 *							loraCheck.newResponse()
 *
 *	WATCHDOG:	1. Suma 1 al contador de wd
 *				2. Consulta si hay timeOut
 *				3. Si hay timeOut indica que debe reiniciar
 *
 *				INPUT	:	flagEnableHwLora
 *
 *				OUTPUT	:	flagWdLora
 */

void linkInLora(){

	//////////////
	// SOFTWARE //
	//////////////

	if ( loraInput.newResponse() ){						// Si hay nueva respuesta desde el modulo lora
		responseLora	 = loraInput.response();		// Guarda la respuesta
		responseLoraSize = loraInput.responseSize();	// Guarda tamaño de la respuesta

		loraCheck.newResponse(responseLora, responseLoraSize);	// Inserta respuesta en modulo de checkeo de respuesta
		loraTimeWd.setInput();									// Indica al watchdog que llego mensaje
		qttyMessagesInputs++;									// Contador de respuestas recibidas
		flagUartGps	= 1;
	}

	//////////////
	// WATCHDOG //
	//////////////

	switch ( stateWdLora ){

	//////////////////////////
	// S0 - WAIT FOR ENABLE //
	//////////////////////////

	case 0:
		flagWdLora	= 0;			// Reinicia indicador de reset

		if ( flagEnableHwLora ){	// Si el chip esta habilitado
			loraTimeWd.setOutput();	// Indica que salio un mensaje
			stateWdLora = 1;		// Pasa a S1
		}
		else{						// Si no
			stateWdLora = 0;		// Espera en S0
		}
		break;

	////////////////////////////
	// S1 - WAIT FOR RESPONSE //
	////////////////////////////

	case 1:
		loraTimeWd.addOne();			// Suma 1 al wd

		if ( loraTimeWd.timeOut() ){	// Si se cumple timeout
			stateWdLora = 2;			// Pasa a S2
		}
		else if ( flagEnableHwLora ){	// Si sigue habilitado por hw
			stateWdLora = 1;			// Espera en S1
		}
		else{							// Si se deshabilita
			stateWdLora	= 0;			// Pasa a S0
		}

		break;

	///////////////////////////
	// S2 - SET RESTART CHIP //
	///////////////////////////

	case 2:
		flagWdLora	= 1;			// Reinicia chip por hw
		stateWdLora	= 0;			// Vuelve a S0
		break;

	default:
		break;
	}
}

/*********************
 ***** GPS INPUT *****
 *********************
 *
 *	Revisa mensajes hasta obtener coordenadas
 *
 *	INPUT	: 	gpsInput.getNewMessage(), 	gpsNewMessage	| Indicador nuevo mensaje
 *				gpsInput.getValid(),		gpsValid		| Indicador mensaje valido
 *
 *	OUTPUT	:	savedGps	|	Indicador coordenadas guardadas
 *				stopGps		|	Indicador detener GPS
 *				warning[]	|	Indica que no hay gps validos
 *
 */

void linkInGps(){

	switch ( stateGpsLink ){

	/////////////////////////
	// S0 - WAIT FOR START //
	/////////////////////////

	case 0:
		if ( startGps ){			// Si se inicia GPS
			startGps		= 0;	// Reinicia indicador
			stateGpsLink	= 1;	// Pasa a S1
		}
		else{						// Si no
			stateGpsLink	= 0;	// Espera en S0
		}
		break;

	///////////////////////////
	// S1 - WAIT FOR MESSAGE //
	///////////////////////////

	case 1:
		countGpsLink++;								// Suma 1 al contador
		gpsNewMessage	= 0;						// Reinicia flag nuevo mensaje

		if ( gpsInput.getNewMessage() ){			// Si llega nuevo mensaje
			gpsNewMessage	= 1;					// Marca flag de nuevo mensaje
			gpsValid		= gpsInput.getValid();	// Copia mensaje valido
		}

		if ( countGpsLink >= limitGpsLink ){		// Si pasa limite de 5 min
			stateGpsLink	= 2;					// Cierra en S2
		}
		else if ( gpsNewMessage && gpsValid ){		// Si llega mensaje valido
			stateGpsLink	= 3;					// Pasa a S3
		}/*
		else if ( gpsNewMessage && !gpsValid ){		// Si llega mensaje no valido
			stateGpsLink	= 4;					// Pasa a S4
		}*/
		else{										// Si no
			stateGpsLink	= 1;					// Espera
		}
		break;

	///////////////////////
	// S2 - STOP MEASURE //
	///////////////////////

	case 2:
		savedGps			= 0;	//  Reinicia indicador coordenadas guardadas
		countValidMsgGps	= 0;	//	Reinicia contador mensajes validos
		countInvalidMsgGps	= 0;	//	Reinicia contador mensajes invalidos
		countGpsLink		= 0;	//	Reinicia contador de espera mensajes gps
		stopGps				= 1;	//	Fija apagado de GPS
		stateGpsLink		= 0;	//	Vuelve a S0
		break;

	////////////////////////////
	// S3 - NEW VALID MESSAGE //
	////////////////////////////

	case 3:
		countValidMsgGps++;				// Suma 1 a mensaje valids nuevos

		if ( countValidMsgGps >= 4 ){	// Si es mayor a 10 mensajes
			stateGpsLink	= 5;		// Pasa a S5
		}
		else{							// Si no
			stateGpsLink	= 1;		// Vuelve a S5
		}
		break;

	//////////////////////////////
	// S4 - NEW INVALID MESSAGE //
	//////////////////////////////

	case 4:
		countInvalidMsgGps++;				// Suma 1 a mensaje invalido nuevos

		if ( countInvalidMsgGps >= 30 ){	// Si es mayor a 10 mensajes
			stateGpsLink	= 2;			// Pasa a S2
		}
		else{								// Si no
			stateGpsLink	= 1;			// Pasa a S1
		}
		break;

	////////////////////////////////
	// S5 - SAVE LATITUD/LONGITUD //
	////////////////////////////////

	case 5:
		savedGps	= 1;										// Indica que se guardó valor correctamente

		latitud =	(gpsInput.latitud[0] - '0')*1000;			// Guarda unidad de mil latitud
		latitud +=	(gpsInput.latitud[1] - '0')*100;			// Guarda centena de latitud
		latitud +=	(gpsInput.latitud[2] - '0')*10;				// Guarda decena de latitud
		latitud +=	(gpsInput.latitud[3] - '0')*1;				// Guarda unidad de latitud
		latitud +=	( float(gpsInput.latitud[5]) - '0')/10;		// Guarda decima de latitud
		latitud +=	( float(gpsInput.latitud[6]) - '0')/100;	// Guarda centecina de latitud
		latitud +=	( float(gpsInput.latitud[7]) - '0')/1000;	// Guarda milecima de latitud
		latitud +=	( float(gpsInput.latitud[8]) - '0')/10000;	// Guarda decima de milecima de latitud

		longitud =	(gpsInput.longitud[0] - '0')*10000;			// Guarda decena de mil longitud
		longitud +=	(gpsInput.longitud[1] - '0')*1000;			// Guarda unidad de mil longitud
		longitud +=	(gpsInput.longitud[2] - '0')*100;			// Guarda centecima de longitud
		longitud +=	(gpsInput.longitud[3] - '0')*10;			// Guarda decena de longitud
		longitud +=	(gpsInput.longitud[4] - '0')*1;				// Guarda unidad de longitud
		longitud +=	( float(gpsInput.longitud[6]) - '0')/10;	// Guarda decima de longitud
		longitud +=	( float(gpsInput.longitud[7]) - '0')/100;	// Guarda centecima de longitud
		longitud +=	( float(gpsInput.longitud[8]) - '0')/1000;	// Guarda milesima de longitud
		longitud +=	( float(gpsInput.longitud[9]) - '0')/10000;	// Guarda decima de milesima de longitud

		countSavedGps++;										// Contador de veces que se guardó un valor correctamente
		stateGpsLink	= 2;									// Pasa a S2
		break;
	}

}

/***************************
 ***** BOTON - DISPLAY *****
 ***************************
 *
 *	Mantiene encendido el display cada
 *
 *	INPUT	:	flagBoton
 *	OUTPUT	:	displayPhy.enable
 */


void linkBoton(){

	switch ( stateEnableDisplay ){

	//////////////////////
	// S0 - WAIT BUTTON //
	//////////////////////

	case 0:
		displayPhy.enable(1);			//	Inhabilita display
		countEnableDisplay	= 0;		//	Reinicia contador

		if ( flagBoton ){				//	Si se presiona el boton
			stateEnableDisplay	= 1;	//	Pasa a S1
		}
		else{							//	Si no
			stateEnableDisplay	= 0;	//	Espera en S0
		}
		break;

	/////////////////
	// S1 - ENABLE //
	/////////////////

	case 1:
		displayPhy.enable(1);							// Habilita display

		countEnableDisplay++;							// Suma 1 al contador

		if ( countEnableDisplay > limitEnableDisplay ){	// Si llega al limite
			stateEnableDisplay	= 0;					// Vuelve a S0
		}
		else{											// Si no
			stateEnableDisplay	= 1;					// Espera en S1
		}
		break;

	default:
		stateEnableDisplay	= 0;
		break;

	}
}


/**********************
 ***** ADC TO PPM *****
 **********************
 *
 *	Escala valores de ADC a PPM
 *
 *	MODE 1	:	ADC externo A1
 *	MODE 2	:	ADC interno A1
 *	MODE 3	:	ADC externo B1
 *	MODE 4	:	ADC interno B1
 */

uint8_t adc2PPM( uint16_t signal, uint8_t mode){
	uint8_t result;

	result	= 1;

	switch ( mode ){

	////////////
	// MODE 1 //
	////////////

	case 1:
		break;

	////////////
	// MODE 2 //
	////////////

	case 2:
		break;

	////////////
	// MODE 3 //
	////////////

	case 3:
		break;

	////////////
	// MODE 4 //
	////////////

	case 4:
		break;
	}

	return result;
}

/******************
 ***** ANALOG *****
 ******************
 *
 *	Genera alarmas de datos adquiridos
 *
 *	INPUT	:	battery,
 *				alphaAnalog_A, alphaAnalog_B
 *				alphaA, alphaB
 *				tempIntern, tempExtern
 *				humIntern, humExtern
 *
 *	OUTPUT	:	tempApp, humApp,
 *				alpha_A1_PPM,
 *				alpha_B1_PPM,
 *				flagAlarm_PPM,
 *				flagAlarm_TEMP
 *				flagAlarm_HUM
 */

void linkAnalog(){

	/////////////////
	// CALIBRATION //
	/////////////////

	switch ( stateCalibration ){

	///////////////////////////////
	// S0 - WAIT FOR CALIBRATION //
	///////////////////////////////

	case 0:
		countCalibration++;								// Suma 1 al contador
		enableSensors	= 0;							// Deshabilita sensores

		if ( countCalibration >= limitCalibration ){	// Si se cumple el tiempo de calibracion
			enableSensors		= 1;					// Habilita sensores
			stateCalibration	= 1;					// Pasa a S1
		}
		else{											// Si no
			stateCalibration	= 0;					// Espera en S0
		}
		break;

	////////////////////
	// S1 - NO ACTION //
	////////////////////
	case 1:
		break;

	default:
		stateCalibration	= 0;
		break;
	}

	////////////
	// A1 HCL //
	////////////

	if ( !errorHardware[5] ){										// Si esta habilitado el ADC
		alpha_A1_PPM = adc2PPM(alphaA, 1) * enableSensors;			// Convierte en PPM
	}
	else{															// Si no esta habiltiado el ADC
		alpha_A1_PPM = adc2PPM(alphaAnalog_A, 2) * enableSensors;	// Convierte en PPM
	}

	if ( alpha_A1_PPM > 50 && enableSensors){		// Si pasa de 5 ppm
		flagAlarm_PPM	= 1;						// Indica alarma
	}
	else{											// Si no
		flagAlarm_PPM	= 0;						// No hay alarma
	}

	////////////
	// B1 HCL //
	////////////

	if ( !errorHardware[5] ){										// Si esta habitado el ADC
		alpha_A1_PPM = adc2PPM(alphaA, 3) * enableSensors;			// Convierte en PPM
	}
	else{															// Si no esta habilitado el ADC
		alpha_A1_PPM = adc2PPM(alphaAnalog_A, 4) * enableSensors;	// Convierte en PPM
	}

	if ( alpha_A1_PPM > 50 && enableSensors ){		// Si pasa de 5 ppm
		flagAlarm_PPM	= 1;						// Indica alarma
	}
	else{											// Si no
		flagAlarm_PPM	= 0;						// No hay alarma
	}

	//////////////
	// TEMP HUM //
	///////////////

	if ( !errorHardware[1] ){			// Si hay sensor externo
		tempApp	= tempExtern;			// Copia temperatura
		humApp	= humExtern;			// Copia humedad
	}
	else if ( !errorHardware[0] ){	// Si hay sensor interno
		tempApp	= tempIntern;			// Copia temperatura
		humApp	= humIntern;			// Copia humedad
	}
	else{								// Si no hay ninguno de ambos sensores
		tempApp	= 0;					// Copia 0
		humApp	= 0;					// Copia 0
	}

	////////////
	// ALARMS //
	////////////

	if ( tempApp > 60 || tempApp < 5 ){	// Si pasa limite de sensores
		warningHardware[0]	= 1;			// Indica alarma
	}
	else{								// Si no
		warningHardware[0]	= 0;			// No hay alarma
	}

	if ( humApp < 5 || humApp > 90){	// Si pasa limite de sensores
		warningHardware[1]	= 1;			// Indica alarma
	}
	else{								// Si no
		warningHardware[1]	= 0;			// No hay alarma
	}
}



