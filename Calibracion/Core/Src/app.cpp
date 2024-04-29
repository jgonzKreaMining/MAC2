/*
 * app.cpp
 *
 *  Created on: Mar 1, 2024
 *      Author: jgonz
 */

#include <app.h>
#include <gpsGNSS.h>
#include <loraApp.h>
#include <loraTcp.h>
#include <loraLink.h>
#include <math.h>
#include "fifoUart.h"

///////////////
// SUPERLOOP //
///////////////

extern uint8_t superloop;

/////////////////
// APP CONTENT //
/////////////////

uint8_t stateContent;				// Estado de maquina de Contenido
uint8_t count_0_gps;				// Contador de la ME para gps
uint8_t count_1_data;				// Contador de la ME contenido

extern loraDecision loraDecision;	// Objeto de toma de decisiones Lora
extern fifoCommands fifoContent;	// Fifo de comandos

bool savedData;						// Indica que hay datos guardados en fifo
bool bussyRN2903;					// Indica que RN2903 está ocupado

extern bool flagGpsSaved;			// Indica que los datos de Gps se guardaron
extern bool flagSetData;			// Indica que reloj TCP de datos de sensores caducó
extern bool flagSetGps;				// Indica que reloj TCP de GPS caducó
extern bool flagSetSys;				// Indica que reloj TCP de datos de sistema caducó

uint8_t content[9];					// Bus para contenido de datos
uint8_t content2[9];				// Bus para contenido de datos 2

uint8_t b1_HCL	= 100;				// Valor de HCL sensor B1
uint8_t a1_HCL;						// Valor de HCL sensor A1

extern uint16_t tempApp;			// Temperatura a enviar
extern uint16_t humApp;				// Humedad a enviar

extern float latitud;				// Latitud captada
extern float longitud;				// Longitud captada

float latitudSend;										// Latitud a enviar
float longitudSend;										// Longitud a enviar

uint8_t *ptrFloat_Lat = (uint8_t *) &latitudSend;		// Puntero al primer byte del flotante latitud
uint8_t *ptrFloat_Lon = (uint8_t *) &longitudSend;		// Puntero al primer byte del flotante longitud


extern int batScaled;									// Valor bateria escalado
extern bool errorHardware[8];							// Errores de hardware
extern bool warningHardware[8];							// Warnings de hardware
uint8_t errorHw;										// Compresion de errores
uint8_t warnings;										// Compresion de warnings

extern uint8_t qttyMessagesInputs;
extern uint8_t qttyMessagesInputs2;

uint8_t qttyMsgData;
uint8_t qttyMsgGps;
uint8_t qttyMsgSys;

//////////////
// APP LORA //
//////////////

uint8_t stateLoraApp = 6;				//

extern loraSelect loraSelect;			//
extern loraCheck loraCheck;				//

rn2903Command *txCommandPointer;		//

extern loraRutine setupRN2903;			//
extern loraRutine chSetRN2903;			//
extern loraRutine loopRN2903;			//

int countLoraApp;						//
int limitLoraApp = 5000/superloop;		//

extern uint8_t macSetCh10_RN2903[];		//
uint8_t countChSet;						//

/////////////
// WARMING //
/////////////

uint8_t stateWarming;							//
uint32_t countWarming;							//
uint32_t limitWarming	= 5000;//1200000/superloop;	//

bool flagWarmingOk;								//
bool flagResetWarming;							//

/////////////
// PROCESS //
/////////////

uint8_t stateProcess;							//

uint32_t countProcess;							//
uint8_t countProcess2;
uint8_t countProcess3;

const uint32_t limitProcess	= 20000/superloop;	//

bool flagResetProcess;							//
bool flagStability;



extern uint16_t alphaA;
extern uint16_t alphaB;
extern uint16_t alphaAnalog_A;
extern uint16_t alphaAnalog_B;

uint16_t alphaA_LP;
int alphaA_BP;

extern uint8_t stateAdc;

uint16_t meanAlphaA;
uint16_t meanAlphaA_2;

extern bool flagBoton;

///////////////
// STABILITY //
///////////////

uint8_t stateStability;
uint32_t countStability;
uint8_t countStability2;
uint8_t countStability3;

const uint16_t limitGrowHcl		= 500;

const uint32_t limitStability	= 20000/superloop;
const uint8_t limitRetStab		= 6;

bool flagStartStability;
bool flagFinishStab;

uint8_t sizeBufStab	= 30;
uint16_t bufferStabA[ 30 ];

extern uint8_t groundSensor0[5];
extern uint8_t curveSensor0[5];

/////////////
// DISPLAY //
/////////////

uint8_t calibContent[4]	= {0, 0, 0, 0};

extern bool flagLedOn;
extern bool flagLedOff;
extern bool flagLedFreq;
extern bool flagSaveEeprom;

////////////
// FILTER //
////////////

#define NCoef 2
#define NCoef2 4

#define DCgain 8
#define DCgain2 1

/////////
// APP //
/////////

void app(){
	warming();
	stability();
	process();
	displayCalib();
	//appContent();
	//appLora();
}

void displayCalib(){
	switch(stateProcess){
	case 0:
		flagLedOn	= 0;
		flagLedOff	= 1;
		flagLedFreq	= 0;
		break;

	case 1:
		flagLedOn	= 0;
		flagLedOff	= 0;
		flagLedFreq	= 1;

		calibContent[0]	= 'H';
		calibContent[1] = 'E';
		calibContent[2] = 'A';
		calibContent[3] = 'T';
		break;

	case 2:
		flagLedOn	= 0;
		flagLedOff	= 0;
		flagLedFreq	= 1;
		break;

	case 3:
		flagLedOn	= 0;
		flagLedOff	= 0;
		flagLedFreq	= 1;
		break;

	case 4:
		flagLedOn	= 1;
		flagLedOff	= 0;
		flagLedFreq	= 0;

		calibContent[0]	= 'E';
		calibContent[1] = 'R';
		calibContent[2] = 'R';
		calibContent[3] = ' ';
		break;

	case 5:
		flagLedOn	= 0;
		flagLedOff	= 1;
		flagLedFreq	= 0;

		calibContent[0]	= 'S';
		calibContent[1] = 'T';
		calibContent[2] = 'A';
		calibContent[3] = 'R';
		break;

	case 6:
		flagLedOn	= 0;
		flagLedOff	= 0;
		flagLedFreq	= 1;

		flagLedOn	= 0;
		flagLedOff	= 0;
		flagLedFreq	= 1;

		calibContent[0]	= 'C';
		calibContent[1] = 'A';
		calibContent[2] = 'L';
		calibContent[3] = 'I';
		break;

	case 7:
		flagLedOn	= 0;
		flagLedOff	= 0;
		flagLedFreq	= 1;

		calibContent[0]	= 'C';
		calibContent[1] = 'A';
		calibContent[2] = 'L';
		calibContent[3] = 'I';
		break;

	case 8:
		flagLedOn	= 1;
		flagLedOff	= 0;
		flagLedFreq	= 0;

		calibContent[0]	= 'O';
		calibContent[1] = 'K';
		calibContent[2] = ' ';
		calibContent[3] = ' ';
		break;

	default:
		break;
	}
}

///////////////
// STABILITY //
///////////////

void stability(){
	if ( stateAdc == 5 ){				// Si se toma una muestra
		alphaA_LP	= iir(alphaA);		// Inserta en filtro pasabajos
		alphaA_BP	= iir3(alphaA);		// Inserta en filtro pasabanda
	}

	switch( stateStability ){

	/////////////////////////
	// S0 - WAIT FOR START //
	/////////////////////////

	case 0:
		flagFinishStab	= 0;
		flagStability	= 0;
		if ( flagStartStability ){	// Si se indica inciar
			stateStability	= 1;	// Para a S1
		}
		else{						// Si no
			stateStability	= 0;	// Queda en S0
		}
		break;

	//////////////////////
	// S1 - WAIT A TIME //
	//////////////////////

	case 1:
		countStability++;						//

		if (countStability >= limitStability ){	//
			countStability	= 0;				//
			stateStability	= 2;				//
		}
		else{									//
			stateStability	= 1;				//
		}
		break;

	////////////////////
	// S2 - SAVE DATA //
	////////////////////
	case 2:
		if ( stateAdc == 5 ){							// Si llega un dato
			bufferStabA[ countStability2 ]	= alphaA;	// Guarda valor bruto
			countStability2++;							// Suma 1
		}

		if ( countStability2 >= sizeBufStab ){			// Si pasa limite del buffer
			countStability2	= 0;						// Reinicia contador
			stateStability	= 3;						// Pasa a S3
		}
		else{											// Si no
			stateStability	= 2;						// Queda en S2
		}

		break;

	case 3:
		for ( countStability2 = 0; countStability2 < sizeBufStab; countStability2++ ){	// Recorre el buffer
			if ( fabs(bufferStabA[ countStability2 ] - alphaA_LP) < limitStability){	// Si la diferencia entre la muestra y valor Mean es menor al limite de stabilidad
				flagStability	= 1;													// Marca que es estable
			}
			else{																		// Si no
				flagStability	= 0;													// Marcaque es inestable
				break;																	// Sale del ciclo for
			}
		}

		countStability3++;								// Suma 1 al contador

		if (flagStability){								// Si es estable
			countStability3	= 0;						// Reinicia contador
			stateStability	= 5;						// Pasa a S5
		}
		else if ( countStability3 >= limitRetStab ){	// Si lleva muchos reintentos
			countStability3	= 0;						// Reinicia contador
			stateStability	= 4;						// Pasa a S4
		}
		else{											// Si no
			stateStability	= 1;						// Vuelve a S1
		}
		break;

	case 4:
		flagFinishStab	= 1;
		flagStability	= 0;
		stateStability	= 0;
		break;

	case 5:
		flagFinishStab	= 1;
		flagStability	= 1;
		stateStability	= 0;
		break;

	default:
		break;
	}
}


/////////////
// PROCESS //
/////////////

/*
 * S0:	START PROCESS
 * S1: 	WAIT FOR WARMING / EVALUATE STABILITY
 * S2:	WAIT FOR STABILITY
 * S3:	ERROR FOR INESTABILITY
 * S4:	SET GROUND
 * S5:	WAIT FOR START EXPERIMENT
 * S6: 	CHECK GROW / CHECK STABILITY
 * S7:	SET FINAL POINT
 * S8:	SAVE IN EEPROM
 * S9:	WAIT FOR RESET PROCESS
 */

void process(){


	switch ( stateProcess ){

	////////////////////////
	// S0 - START PROCESS //
	////////////////////////

	case 0:
		flagResetWarming	= 1;	// Indica que empieza el calentaiento del sensor
		stateProcess		= 1;	// Pasa a S1
		break;

	///////////////////////////
	// S1 - WAIT FOR WARMING //
	///////////////////////////

	case 1:
		flagResetWarming	= 0;	// Reinicia indicador

		if ( flagWarmingOk ){		// Si indica que se calentó
			stateProcess	= 2;	// Pasa a S2
		}
		else{						// Si no
			stateProcess	= 1;	// Queda en S1
		}
		break;

	/////////////////////
	// S2 - SET GROUND //
	/////////////////////

	case 2:
		countProcess++;							// Suma 1 al contador

		if ( countProcess >= limitProcess){		// Si se llega al limite de tiempo
			countProcess		= 0;			// Reinicia contador
			meanAlphaA			= alphaA_LP;	// Guarda valor en filtro
			flagStartStability	= 1;			//
			stateProcess		= 3;			// Pasa a S3
		}
		else{									// Si no
			stateProcess	= 2;				// Espera en S2
		}
		break;

	//////////////////////////
	// S3 - CHECK STABILITY //
	//////////////////////////

	case 3:
		flagStartStability	= 0;						//

		if ( flagFinishStab && flagStability){			// Si es estable
			meanAlphaA		= alphaA_LP;				//
			stateProcess	= 5;						// Pasa a S5
		}
		else if ( flagFinishStab && !flagStability){	// Si es inestable
			stateProcess	= 4;						// Pasa  S4
		}
		else{											// Si no marca
			stateProcess	= 3;						// Espera en S3
		}
		break;

	////////////////////
	// S4 - SET ERROR //
	////////////////////

	case 4:
		if (flagBoton){				// Si se aprieta boton
			stateProcess	= 0;	// Reinicia la maquina
		}
		else{						// Si no
			stateProcess	= 4;	// Espera en S4
		}
		break;

	///////////////////////////
	// S5 - WAIT INSTRUCTION //
	///////////////////////////

	case 5:
		if (flagBoton){				// Si se aprieta el boton
			stateProcess	= 6;	// Inicia proceso calibracion
		}
		else{						// Si no
			stateProcess	= 5;	// Espera en S5
		}
		break;

	/////////////////////////
	// S6 - DETECT INCEASE //
	/////////////////////////

	case 6:
		countProcess++;										// Suma 1 al contador

		if (alphaA_LP - meanAlphaA >= limitGrowHcl ){ 	// Si se detecta increento
			flagStartStability	= 1;
			countProcess	= 0;							// Reinicia contador
			stateProcess	= 7;							// Pasa a S7
		}
		else if ( countProcess >= limitProcess*2 ){			// Si no se detecta crecimiento
			countProcess		= 0;						// Reinicia contador
			stateProcess		= 4;						// Pasa a S4 - ERROR
		}
		else{												// Si aun no se cumple el contador
			stateProcess	= 6;							// Queda en S4
		}
		break;

	/////////////////////////////
	// S7 - WAIT FOR STABILITY //
	/////////////////////////////

	case 7:
		flagStartStability	= 0;							// Reinicia flag
		countProcess++;										// Suma 1 al proceso

		if ( flagFinishStab && flagStability){				// Si es estable
			meanAlphaA_2	= (alphaA_LP - meanAlphaA)/50;	//

			groundSensor0[0]	= meanAlphaA & 0xFF;		//
			groundSensor0[1]	= meanAlphaA >> 8;			//
			groundSensor0[2]	= 0;						//
			groundSensor0[3]	= 0;						//

			curveSensor0[0]		=  meanAlphaA_2 & 0xFF;	//
			curveSensor0[1]		=  meanAlphaA_2 >> 8;	//
			curveSensor0[2]		= 0;					//
			curveSensor0[3]		= 0;					//

			flagSaveEeprom	= 1;						//
			countProcess	= 0;						//
			stateProcess	= 8;						// Pasa a S5
		}
		else if ( flagFinishStab && !flagStability){	// Si es inestable
			countProcess	= 0;						//
			stateProcess	= 4;						// Pasa  S4
		}
		else{											// Si no marca
			stateProcess	= 7;						// Espera en S3
		}
		break;

	////////////////////////
	// S8 - FINISH PROCESS//
	////////////////////////

	case 8:
		flagSaveEeprom		= 0;
		if (flagBoton){				// Si se aprieta el boton
			stateProcess	= 0;	// Inicia proceso calibracion
		}
		else{						// Si no
			stateProcess	= 8;	// Espera en S5
		}
		break;

	default:
		stateProcess	= 0;
		break;
	}

}

/////////////
// WARMING //
/////////////

void warming(){
	switch( stateWarming ){

	/////////////////////////
	// S0 - WAIT FOR RESET //
	/////////////////////////

	case 0:
		flagWarmingOk	= 0;		//
		if ( flagResetWarming ){	//
			stateWarming	= 1;	//
		}
		else{						//
			stateWarming	= 0;	//
		}
		break;

	/////////////////////
	// S1 - WAIT LIMIT //
	/////////////////////

	case 1:
		countWarming++;							//
		flagWarmingOk	= 0;					//

		if ( countWarming >= limitWarming ){	//
			countWarming	= 0;				//
			stateWarming	= 2;				//
		}
		else{									//
			stateWarming	= 1;				//
		}
		break;

	/////////////////////
	// S2 - WARMING OK //
	/////////////////////

	case 2:
		flagWarmingOk	= 1;	//
		stateWarming	= 0;	//
		break;
	}
}

/////////////////
// APP CONTENT //
/////////////////

/*	Maquina de estados encargada de
 * 			- Calcular valores a enviar
 * 			- Agregar comandos a Fifo
 * 			- Esperar relojes de TCP
 */

void appContent(){

	switch ( stateContent ){

	//////////////////////////
	// S0 - WAIT FOR ENABLE //
	//////////////////////////

	case 0:
		if ( loraDecision.enable() ){	// Si el chip esta habilitado
			stateContent	= 1;		// Pasa a S1
		}
		else{							// Si no
			stateContent	= 0;		// Espera en S0
		}
		break;

	////////////////////////
	// S1 - WAIT FOR FLAG //
	////////////////////////

	case 1:
		bussyRN2903	= 0;

		if ( !loraDecision.enable() ){			// Si no está habilitado
			stateContent	= 0;				// Vuelve a S0
		}
		else if ( fifoContent.available() ){	// Si hay
			stateContent	= 5;				//
		}
		else if ( flagSetData ){				// Si se guardó datos de GPS
			count_1_data++;
			stateContent	= 2;				// Pasa a S3
		}
		else if ( flagGpsSaved ){				// Si pide guardar datos
			count_0_gps++;
			stateContent	= 3;				// Pasa a S2
		}
		else if ( flagSetSys ){					// Si se guardan datos de sistema
			stateContent	= 4;				// Pasa a S4
		}
		else{									// Si no pasa nada
			stateContent	= 1;				// Espera en S1
		}
		break;

	////////////////////////////
	// S2 - SAVE SENSORS DATA //
	////////////////////////////

	case 2:
		flagSetData	= 0;					// Reinicia indicador de enviar datos

		tempApp		= tempApp/10;			// Reduce resolucion de temperatura
		humApp		= humApp/10;			// Reduce resolucion de humedad

		content[0]	= '1';					// Indica que los datos van sobre puerto 1

		content[1]	= uint2hex_A(b1_HCL);	// Guarda primeros 4 bits de B1 HCL
		content[2]	= uint2hex_B(b1_HCL);	// Guarda segundos 4 bits de B1 HCL

		content[3]	= uint2hex_A(a1_HCL);	// Guarda primeros 4 bits de A1 HCL
		content[4] 	= uint2hex_B(a1_HCL);	// Guarda segundos 4 bits de A1 HCL

		content[5]	= uint2hex_A(tempApp);	// Guarda primeros 4 bits de Temperatura
		content[6]  = uint2hex_B(tempApp);	// Guarda segundos 4 bits de Temperatura

		content[7] 	= uint2hex_A(humApp);	// Guarda primeros 4 bits de Humedad
		content[8]	= uint2hex_B(humApp);	// Guarda segundos 4 bits de Humedad

		qttyMsgData++;

		fifoContent.add(content, sizeof(content));	// Envia paquete a la FIFO
		stateContent 	= 5;						// Pasa a S5
		break;

	////////////////////////
	// S3 - SAVE LAT/LONG //
	////////////////////////

	case 3:
		flagGpsSaved	= 0;						// Reinicia indicado de enviar GPS

		latitudSend		= latitud;					// Copia valor de latitud
		longitudSend	= longitud;					// Copia valor de longitud

		content[0]	= '2';							// Indica que latitud va en puerto 2

		content[1]	= uint2hex_A(ptrFloat_Lat[0]);	// Guarda primeros 4 bits de latitud - 1byte
		content[2]	= uint2hex_B(ptrFloat_Lat[0]);	// Guarda segundos 4 bits de latitud - 1byte

		content[3]	= uint2hex_A(ptrFloat_Lat[1]);	// Guarda terceros 4 bits de latitud - 2byte
		content[4] 	= uint2hex_B(ptrFloat_Lat[1]);	// Guarda cuartos 4 bits de latitud  - 2byte

		content[5]	= uint2hex_A(ptrFloat_Lat[2]);	// Guarda quintos 4 bits de latitud  - 3byte
		content[6]  = uint2hex_B(ptrFloat_Lat[2]);	// Guarda sextos 4 bits de latitud   - 3byte

		content[7] 	= uint2hex_A(ptrFloat_Lat[3]);	// Guarda septimos 4 bits de latitud - 4byte
		content[8]	= uint2hex_B(ptrFloat_Lat[3]);	// Guarda octavos 4 bits de latitud  - 4byte

		fifoContent.add(content, sizeof(content));	// Añade contenido de latitud a FIFO
		fifoContent.add(content, sizeof(content));	// Repite contenido para enviar 2 veces

		content[0]	= '3';							// Indica que longitud va en puerto 3

		content[1]	= uint2hex_A(ptrFloat_Lon[3]);	// Guarda primeros 4 bits de longitud - 1byte
		content[2]	= uint2hex_B(ptrFloat_Lon[3]);	// Guarda segundos 4 bits de longitud - 1byte

		content[3]	= uint2hex_A(ptrFloat_Lon[2]);	// Guarda terceros 4 bits de longitud - 2byte
		content[4] 	= uint2hex_B(ptrFloat_Lon[2]);	// Guarda cuartos 4 bits de longitud  - 2byte

		content[5]	= uint2hex_A(ptrFloat_Lon[1]);	// Guarda quintos 4 bits de longitud  - 3byte
		content[6]  = uint2hex_B(ptrFloat_Lon[1]);	// Guarda sextos 4 bits de longitud   - 3byte

		content[7] 	= uint2hex_A(ptrFloat_Lon[0]);	// Guarda septimos 4 bits de longitud - 4byte
		content[8]	= uint2hex_B(ptrFloat_Lon[0]);	// Guarda octavos 4 bits de longitud  - 4byte

		fifoContent.add(content, sizeof(content));	// Añade contenido de longitud de la FIFO
		fifoContent.add(content, sizeof(content));	// Repite contenido para enviar 2 veces

		qttyMsgGps++;
		qttyMsgGps++;
		qttyMsgGps++;
		qttyMsgGps++;

		stateContent	= 5;						// Pasa a S5
		break;

	////////////////////////
	// S4 - SAVE SYS DATA //
	////////////////////////

	case 4:
		warnings	= warningHardware[0]	+ warningHardware[1]*2;			// Multiplexa warning 0 y 1
		warnings	+= warningHardware[2]*4	+ warningHardware[3]*8;			// Multiplexa warning 2 y 3
		warnings	+= warningHardware[4]*16	+ warningHardware[5]*32;	// Multiplexa warning 4 y 5
		warnings	+= warningHardware[6]*64	+ warningHardware[7]*128;	// Multiplexa warning 6 y 7

		errorHw	= errorHardware[0] 		+ errorHardware[1]*2;				// Multiplexa error 0 y 1
		errorHw	+= errorHardware[2]*4 	+ errorHardware[3]*8;				// Multiplexa error 2 y 3
		errorHw	+= errorHardware[4]*16 	+ errorHardware[5]*32;				// Multiplexa error 4 y 5
		errorHw	+= errorHardware[6]*64;										// Multiplexa error 6

		content[0]	= '4';						// Indica que longitud va en puerto 3

		content[1]	= uint2hex_A(warnings);		// Guarda primeros 4 bits de warnings
		content[2]	= uint2hex_A(warnings);		// Guarda segundos 4 bits de warnings

		content[3]	= uint2hex_A(errorHw);		// Guarda primeros 4 bits de errores
		content[4]	= uint2hex_B(errorHw);		// Guarda segundos 4 bits de errores

		content[5]	= uint2hex_A(batScaled);	// Guarda primeros 4 bits de bateria escalada
		content[6]	= uint2hex_A(batScaled);	// Guarda segundos 4 bits de bateria escalada

		content[7]	= uint2hex_A(0);			// Futuro uso
		content[8]	= uint2hex_A(0);			// Futuro iso

		fifoContent.add(content, sizeof(content));	// Agrega contenido a la fifo

		qttyMsgSys++;

		flagSetSys		= 0;						// Reinicia indicador de datos de sistema
		stateContent	= 5;						// Pasa a  S5
		break;

	///////////////////
	// S5 - SET DATA //
	///////////////////

	case 5:
		fifoContent.read(content2, sizeof(content2));	// Lee contenido disponible en FIFO

		macTxData_RN2903[13] = content2[0];			// Copia PUERTO
		macTxData_RN2903[15] = content2[1];			// 0 Byte
		macTxData_RN2903[16] = content2[2];			// 0 Byte
		macTxData_RN2903[17] = content2[3];			// 1 Byte
		macTxData_RN2903[18] = content2[4];			// 1 Byte
		macTxData_RN2903[19] = content2[5];			// 2 Byte
		macTxData_RN2903[20] = content2[6];			// 2 Byte
		macTxData_RN2903[21] = content2[7];			// 3 Byte
		macTxData_RN2903[22] = content2[8];			// 3 Byte

		savedData		= 1;						// Indica que se guardaron los datos		stateContent	= 6;						// Pasa a S
		stateContent	= 6;						// Pasa a S6
		break;

	//////////////////////////
	// S6 - WAIT FOR ENABLE //
	//////////////////////////

	case 6:
		if ( !loraDecision.enable() ){	// Si no está habilitado
			stateContent	= 0;		// Vuelve a S0
		}
		else if (bussyRN2903){			// Si se habilita
			stateContent	= 1;		// Vuelve a S1
		}
		else{							// Si no
			stateContent	= 6;		// Espera en S6
		}
		break;

	default:
		stateContent	= 0;
		break;
	}
}

//////////////
// APP LORA //
//////////////


/* Maquina que recorre las tres rutinas del chip
 *
 * 	setupRN2903:	Fija comandos de configuracion del chip
 * 	chSetRN2903:	Fija configuracion de canales del chip
 * 	loopRN2903:		Fija comandos de transmision en el chip
 *
 */

void appLora(){

	switch ( stateLoraApp ){

	//////////////////////////
	// S0 - WAIT FOR ENABLE //
	//////////////////////////

	case 0:
		if ( loraDecision.enable() ){						// Si el chip esta habilitado por SW
			loraCheck.setResponsePointer( rxCommand, 14);	//
			stateLoraApp	= 7;							// Pasa a S1
		}
		else{												// Si no
			stateLoraApp	= 0;							// Espera en S0
		}
		break;

	////////////////////////////
	// S1 - SET SETUP COMMAND //
	////////////////////////////

	case 1:
		loraDecision.reset();
		txCommandPointer	= setupRN2903.command();		// Solicita comando
		loraSelect.setCommand( txCommandPointer->command,	// Inserta puntero al comando
			txCommandPointer->size,							// Inserta tamañp del comando
			txCommandPointer->responseSize);				// Inserta tamaño de respuesta
		stateLoraApp	= 2;								// Pasa a S2
		break;

	////////////////////////
	// S2 - WAIT FOR NEXT //
	////////////////////////

	case 2:

		if ( !loraDecision.enable() ){									// Si el chip esta habilitado
			stateLoraApp	= 6;										// Pasa a S6
		}
		else if ( loraDecision.correct() && setupRN2903.finish() ){		// Si llegó la respuesta correcta y es la ultima
			stateLoraApp	= 3;										// Pasa a  S3
		}
		else if ( loraDecision.correct() ){								// Si solo es correctao
			stateLoraApp	= 1;										// Vuelve a S1
		}
		else{															// Si no
			stateLoraApp	= 2;										// Se queda en S2
		}
		break;

	/////////////////////////
	// S3 - GET TX COMMAND //
	/////////////////////////

	case 3:

		if ( savedData ){										//
			qttyMessagesInputs2++;
			loraDecision.reset();								//
			loopRN2903.reset();									//
			txCommandPointer	= loopRN2903.command();			// Solicita comando

			loraSelect.setCommand( txCommandPointer->command,	// Inserta puntero al comando
				txCommandPointer->size,							// Inserta tamañp del comando
				txCommandPointer->responseSize);				// Inserta tamaño de respuesta
			stateLoraApp	= 4;								// Pasa a S4
		}
		else{													// Si no
			stateLoraApp	= 3;								// Espera en S3
		}
		break;

	////////////////////////////
	// S4 - WAIT FOR RESPONSE //
	////////////////////////////

	case 4:
		if ( !loraDecision.enable() ){			// Si el chip esta habilitado
			stateLoraApp	= 6;				// Pasa a S6
		}
		else if ( loraDecision.correct() ){		// Si es correcto
			stateLoraApp	= 5;				// Vuelve a S5
		}
		else if ( loraDecision.burn()){
			setupRN2903.reset();	// Reinicia rutina
			chSetRN2903.reset();	// Reinicia rutina
			loopRN2903.reset();		// Reinicia rutina
			stateLoraApp	= 1;
		}
		else{									// Si no
			stateLoraApp	= 4;				// Se queda en S4
		}
		break;

	////////////////////////////////
	// S5 - WAIT FOR NEXT MESSAGE //
	////////////////////////////////

	case 5:
		savedData = 0;								// Indica que se envia
		countLoraApp++;								// Suma 1 al contador

		if ( !loraDecision.enable() ){				// Si el chip esta habilitado
			stateLoraApp	= 6;					// Pasa a S6
		}
		else if ( countLoraApp >= limitLoraApp ){	// Si pasa el limite
			bussyRN2903		= 1;					// Habilita
			countLoraApp	= 0;					// Reinicia contador
			stateLoraApp	= 3;					// Pasa a S3
		}
		else{										// Si no
			stateLoraApp	= 5;					// Espera
		}

		break;

	////////////////////////
	// S6 - RESET RUTINES //
	////////////////////////

	case 6:
		setupRN2903.reset();	// Reinicia rutina
		chSetRN2903.reset();	// Reinicia rutina
		loopRN2903.reset();		// Reinicia rutina

		stateLoraApp	= 0;	// Vuelve a S0
		break;

	/////////////////////////
	// S7 - SET CH COMMAND //
	/////////////////////////

	case 7:
		loraDecision.reset();								// Reinicia decisiones
		txCommandPointer	= chSetRN2903.command();		// Solicita comando
		loraSelect.setCommand( txCommandPointer->command,	// Inserta puntero al comando
				txCommandPointer->size,						// Inserta tamañp del comando
				txCommandPointer->responseSize );			// Inserta tamaño de respuesta
		stateLoraApp	= 8;								// Pasa a S2
		break;

	/////////////////////////
	// S8 - CHECK RESPONSE //
	/////////////////////////

	case 8:
		if ( !loraDecision.enable() ){									// Si el chip esta habilitado
			stateLoraApp	= 6;										// Pasa a S6
		}
		else if ( loraDecision.correct() && chSetRN2903.finish() ){		// Si llegó la respuesta correcta y es la ultima
			countChSet		= 10;										// Inicia contador en 10
			stateLoraApp	= 9;										// Pasa a  S9
		}
		else if ( loraDecision.correct() ){								// Si solo es correctao
			stateLoraApp	= 7;										// Vuelve a S7
		}
		else{															// Si no
			stateLoraApp	= 8;										// Se queda en S8
		}
		break;

	///////////////////////////
	// S9 - GET LAST COMMAND //
	///////////////////////////

	case 9:
		loraDecision.reset();										// Reinicia decisiones
		macSetCh10_RN2903[18]	= uint8_t(countChSet/10) + '0';		// Inserta decena del contador
		macSetCh10_RN2903[19]	= countChSet%10 + '0';				// Inserta unidad de contador
		loraSelect.setCommand( txCommandPointer->command,			// Fija comando en la salida
				txCommandPointer->size,								// Inserta tamañp del comando
				txCommandPointer->responseSize);					// Inserta tamaño de respuesta
		stateLoraApp	= 10;										// Pasa a S10
		break;

	//////////////////////////
	// S10 - CHECK RESPONSE //
	//////////////////////////

	case 10:
		if ( !loraDecision.enable() ){									// Si el chip esta habilitado
			stateLoraApp	= 6;										// Pasa a S6
		}
		else if ( loraDecision.correct() && countChSet == 71 ){			// Si llegó la respuesta correcta y es la ultima
			macSetCh10_RN2903[18]	= '1';								// Fija comando en valor original '1'
			macSetCh10_RN2903[19]	= '0';								// Fija comando en valor original '0'
			countChSet		= 10;										// Reinicia contador en 10
			stateLoraApp	= 1;										// Pasa a  S3
		}
		else if ( loraDecision.correct() ){								// Si solo es correctao
			countChSet++;												// Suma 1 al contador
			stateLoraApp	= 9;										// Vuelve a S9
		}
		else{															// Si no
			stateLoraApp	= 10;										// Se queda en S10
		}
		break;

	default:
		stateLoraApp	= 0;
		break;
	}
}

uint8_t uint2hex_A( uint8_t number ){
	uint8_t value = (number >> 4 & 0x0F);

	switch ( value ){
	case 0:
		value = '0';
		break;

	case 1:
		value = '1';
		break;

	case 2:
		value = '2';
		break;

	case 3:
		value = '3';
		break;

	case 4:
		value = '4';
		break;

	case 5:
		value = '5';
		break;

	case 6:
		value = '6';
		break;

	case 7:
		value = '7';
		break;

	case 8:
		value = '8';
		break;

	case 9:
		value = '9';
		break;

	case 10:
		value = 'A';
		break;

	case 11:
		value = 'B';
		break;

	case 12:
		value = 'C';
		break;

	case 13:
		value = 'D';
		break;

	case 14:
		value = 'E';
		break;

	case 15:
		value = 'F';
		break;
	}
	return  value;
}

uint8_t uint2hex_B( uint8_t number ){
	uint8_t value = (number & 0x0F);

	switch ( value ){
	case 0:
		value = '0';
		break;

	case 1:
		value = '1';
		break;

	case 2:
		value = '2';
		break;

	case 3:
		value = '3';
		break;

	case 4:
		value = '4';
		break;

	case 5:
		value = '5';
		break;

	case 6:
		value = '6';
		break;

	case 7:
		value = '7';
		break;

	case 8:
		value = '8';
		break;

	case 9:
		value = '9';
		break;

	case 10:
		value = 'A';
		break;

	case 11:
		value = 'B';
		break;

	case 12:
		value = 'C';
		break;

	case 13:
		value = 'D';
		break;

	case 14:
		value = 'E';
		break;

	case 15:
		value = 'F';
		break;
	}
	return  value;
}


__int16_t iir(__int16_t NewSample) {
    __int16_t ACoef[NCoef+1] = {
         8294,
        16588,
         8294
    };

    __int16_t BCoef[NCoef+1] = {
        16384,
        -17567,
         5330
    };

    static __int32_t y[NCoef+1]; //output samples
    //Warning!!!!!! This variable should be signed (input sample width + Coefs width + 2 )-bit width to avoid saturation.

    static __int16_t x[NCoef+1]; //input samples
    int n;

    //shift the old samples
    for(n=NCoef; n>0; n--) {
       x[n] = x[n-1];
       y[n] = y[n-1];
    }

    //Calculate the new output
    x[0] = NewSample;
    y[0] = ACoef[0] * x[0];
    for(n=1; n<=NCoef; n++)
        y[0] += ACoef[n] * x[n] - BCoef[n] * y[n];

    y[0] /= BCoef[0];

    return y[0] / DCgain;
}

__int16_t iir2(__int16_t NewSample) {
    __int16_t ACoef2[NCoef+1] = {
        16207,
        -32415,
        16207
    };

    __int16_t BCoef2[NCoef+1] = {
        16384,
        -32412,
        16031
    };

    static __int32_t y2[NCoef+1]; //output samples
    //Warning!!!!!! This variable should be signed (input sample width + Coefs width + 2 )-bit width to avoid saturation.

    static __int16_t x2[NCoef+1]; //input samples
    int n;

    //shift the old samples
    for(n=NCoef; n>0; n--) {
       x2[n] = x2[n-1];
       y2[n] = y2[n-1];
    }

    //Calculate the new output
    x2[0] = NewSample;
    y2[0] = ACoef2[0] * x2[0];
    for(n=1; n<=NCoef; n++)
        y2[0] += ACoef2[n] * x2[n] - BCoef2[n] * y2[n];

    y2[0] /= BCoef2[0];

    return y2[0] / DCgain2;
}


__int16_t iir3(__int16_t NewSample) {
    __int16_t ACoef3[NCoef2+1] = {
        10238,
            0,
        -20476,
            0,
        10238
    };

    __int16_t BCoef3[NCoef2+1] = {
        16384,
        -12005,
        -12451,
         4472,
         4188
    };

    static __int32_t y3[NCoef2+1]; //output samples
    //Warning!!!!!! This variable should be signed (input sample width + Coefs width + 4 )-bit width to avoid saturation.

    static __int16_t x3[NCoef2+1]; //input samples
    int n;

    //shift the old samples
    for(n=NCoef2; n>0; n--) {
       x3[n] = x3[n-1];
       y3[n] = y3[n-1];
    }

    //Calculate the new output
    x3[0] = NewSample;
    y3[0] = ACoef3[0] * x3[0];
    for(n=1; n<=NCoef2; n++)
        y3[0] += ACoef3[n] * x3[n] - BCoef3[n] * y3[n];

    y3[0] /= BCoef3[0];

    return y3[0] / DCgain2;
}
