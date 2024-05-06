/*
 * hardwareOut.cpp
 *
 *  Created on: Jan 29, 2024
 *      Author: jgonz
 */

#include <main.h>
#include <hardware.h>
#include <string>
#include "stm32l4xx_hal.h"

extern uint8_t superloop;

/////////////
// DISPLAY //
/////////////

extern displayPhysical displayPhy;	//

/////////
// GPS //
/////////

extern UART_HandleTypeDef huart5;

extern bool enableGps;
extern bool startGps;
extern uint8_t rxGps[2];

bool availableGps;
uint16_t countGpsAvailable;
uint16_t limitGpsAvailable	= 5000/superloop;
uint8_t stateHwGpsEnable;

uint8_t commandUart[]	= "$PSTMCFGMSGL,0,1,1,0*87\r\n";
uint8_t commandUart2[]	= "$PSTMCFGMSGL,1,1,0,0*86\r\n";
uint8_t commandUart3[]	= "$PSTMCFGMSGL,2,1,2,0*85\r\n";
uint8_t commandUart4[]	= "$PSTMSETPAR,1210,0,0*35\r\n";
uint8_t commandUart5[]	= "$PSTMSETPAR,1229,0,0*3F\r\n";
uint8_t commandUart6[]	= "$PSTMSETPAR,1211,0,0*34\r\n";
uint8_t commandUart7[]	= "$PSTMSETPAR,1230,0,0*37\r\n";
uint8_t commandUart8[]	= "$PSTMSAVEPAR*58\r\n";
uint8_t commandUart9[]	= "$PSTMSRR*49\r\n";

uint8_t stateConfigGps;

bool flagUartGps;

/////////////
// ALARMAS //
/////////////

bool flagAlarm0;
bool flagAlarm1;

//////////
// LORA //
//////////

extern UART_HandleTypeDef huart4;	// Interfaz UART 4

bool flagTxLora;					// Señal de transmision por HW
extern uint8_t *loraTxCommand0;		// Puntero al comando RN2903
extern uint8_t sizeTxCommand0;		// Largo del comando RN2903
extern uint8_t rxLora[1];			// Buffer de recepcion

///////////////////
// LORA HARDWARE //
///////////////////

uint8_t stateResetHwLora;						// Estado de reinicio chip RN2903
extern bool flagResetHwLora;					// Señal para reinicio de chip
bool flagEnableHwLora;							// Señal de chip habilitado
uint16_t countResetHwLora;						// Contador de reinicio de chip
uint16_t resetHwLora		= 500/superloop;	// Tiempo que deshabilita chip

bool aux;						//
extern bool flagBoton;			//
uint8_t countResetHwLora2;		//

uint8_t stateUartGps;						//
uint16_t countUartGps;						//
uint16_t limitUartGps	= 20000/superloop;	//

/////////
// LED //
/////////

uint8_t stateLed;						//
uint16_t countLedOut;					//
uint16_t limitLed	= 500/superloop;	//

bool flagLedOn;							//
bool flagLedOff;						//
bool flagLedFreq;						//

////////////
// EEPROM //
////////////

bool flagSaveEeprom;
uint8_t stateEepromOut;

extern uint8_t nameSensor0[20];		// Nombre del dispositivo
extern uint8_t idSensor0[4];		// ID interno de Krea
extern uint8_t rangeSensor0[4];		// Valor maximo de medicion
extern uint8_t amplifier1_0[4];		// Valor primer amplificador en kOhm
extern uint8_t amplifier2_0[4];		// Valor segundo amplificador en Ganancia (multiplicador)

extern uint8_t groundSensor0[4];	// Piso de medicion (zero current)
extern uint8_t curveSensor0[4];		// PPM/pnto de cuantificación

extern uint8_t dateCalib0[10];		// Fecha de calibracion

extern uint8_t nameSensor1[20];		// Nombre del dispositivo
extern uint8_t idSensor1[4];		// ID interno de Krea
extern uint8_t rangeSensor1[4];		// Valor máximo de medición
extern uint8_t amplifier1_1[4];		// Valor primero amplificador en kOhm
extern uint8_t amplifier2_1[4];		// Valor segundo amplificador en Ganancia (multiplicador)

extern uint8_t groundSensor1[4];	// Piso de medición - zero current
extern uint8_t curveSensor1[4];		// PPM/pnto de cuantificación

extern uint8_t dateCalib1[10];		// Fecha de calibración

/***** OUTPUT	*****/

void hwOutput(){
	//hwDisplay();
	//hwAlarms();
	//hwLoraOut();
	//hwGpsOut();
	hwLedOut();
	hwEEPROMOut();
}

////////////
// EEPROM //
////////////

void hwEEPROMOut(){
	switch( stateEepromOut ){
	case 0:
		if (flagSaveEeprom){
			stateEepromOut	= 1;
		}
		else{
			stateEepromOut	= 0;
		}
		break;

	case 1:
		EEPROM_Write(0, 0, nameSensor0, sizeof(nameSensor0));
		EEPROM_Write(0, 20, idSensor0, sizeof(idSensor0));
		EEPROM_Write(0, 24, rangeSensor0, sizeof(rangeSensor0));
		EEPROM_Write(0, 28, amplifier1_0, sizeof(amplifier1_0));
		stateEepromOut	= 2;

		break;

	case 2:
		EEPROM_Write(0, 32, amplifier2_0, sizeof(amplifier2_0));
		EEPROM_Write(0, 36, groundSensor0, sizeof(groundSensor0));
		EEPROM_Write(0, 40, curveSensor0, sizeof(curveSensor0));
		EEPROM_Write(0, 44, dateCalib0, sizeof(dateCalib0));
		stateEepromOut	= 3;
		break;

	case 3:
		EEPROM_Write(1, 0, nameSensor1, sizeof(nameSensor0));
		EEPROM_Write(1, 20, idSensor1, sizeof(idSensor1));
		EEPROM_Write(1, 24, rangeSensor1, sizeof(rangeSensor0));
		EEPROM_Write(1, 28, amplifier1_1, sizeof(amplifier1_0));
		stateEepromOut	= 4;
		break;

	case 4:
		EEPROM_Write(1, 32, amplifier2_1, sizeof(amplifier2_0));
		EEPROM_Write(1, 36, groundSensor1, sizeof(groundSensor0));
		EEPROM_Write(1, 40, curveSensor1, sizeof(curveSensor0));
		EEPROM_Write(1, 44, dateCalib1, sizeof(dateCalib0));
		stateEepromOut	= 0;
		break;

	default:
		stateEepromOut	= 0;
		break;
	}
}

/////////
// LED //
/////////

void hwLedOut(){

	switch(stateLed){

	//////////////////
	// S0 - LED OFF //
	//////////////////

	case 0:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

		if ( flagLedOn ){			//
			stateLed	= 1;		//
		}
		else if ( flagLedFreq ){	//
			stateLed	= 2;		//
		}
		else{						//
			stateLed	= 0;		//
		}
		break;

	/////////////////
	// S1 - LED ON //
	/////////////////

	case 1:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

		if ( flagLedOff ){			//
			stateLed	= 0;		//
		}
		else if ( flagLedFreq ){	//
			stateLed	= 2;		//
		}
		else{						//
			stateLed	= 1;		//
		}
		break;

	///////////////////
	// S2 - LED FREQ //
	///////////////////

	case 2:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

		countLedOut++;							//

		if ( flagLedOn ){						//
			countLedOut	= 0;					//
			stateLed	= 1;					//
		}
		else if ( flagLedOff ){					//
			countLedOut	= 0;					//
			stateLed	= 0;					//
		}
		else if ( countLedOut >= limitLed ){	//
			countLedOut	= 0;					//
			stateLed	= 3;					//
		}
		else{									//
			stateLed	= 2;					//
		}
		break;

	///////////////////
	// S3 - LED FREQ //
	///////////////////

	case 3:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

		countLedOut++;							//

		if ( flagLedOn ){						//
			countLedOut	= 0;					//
			stateLed	= 1;					//
		}
		else if ( flagLedOff ){					//
			countLedOut	= 0;					//
			stateLed	= 0;					//
		}
		else if ( countLedOut >= limitLed ){	//
			countLedOut	= 0;					//
			stateLed	= 3;					//
		}
		else{									//
			stateLed	= 2;					//
		}
		break;

	default:
		stateLed	= 0;
		break;
	}
}

/////////
// GPS //
/////////

void hwGpsOut(){

	////////////
	// ENABLE //
	////////////

	switch ( stateUartGps ){

	/////////////////////////
	// S0 - WAIT AVAILABLE //
	/////////////////////////

	case 0:
		if ( availableGps ){										// Si
			HAL_UART_Receive_DMA(&huart5,	rxGps, sizeof(rxGps));	//
			stateUartGps	= 1;									//
		}
		else{														//
			stateUartGps	= 0;									//
		}
		break;

	case 1:
		countUartGps++;

		if ( !availableGps ){
			countUartGps	= 0;
			stateUartGps	= 0;
		}
		else if ( (countUartGps >= limitUartGps || flagUartGps)){
			stateUartGps	= 2;
		}
		else{
			stateUartGps	= 1;
		}
		break;

	case 2:
		flagUartGps		= 0;
		countUartGps	= 0;
		HAL_UART_Receive_DMA(&huart5,	rxGps, sizeof(rxGps));
		stateUartGps	= 1;
		break;

	default:
		stateUartGps	= 0;
		break;
	}

	/////////////
	// MESSAGE //
	/////////////

	switch ( stateHwGpsEnable ){

		case 0:
			availableGps	= 0;

			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);
			//HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
			//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);
			if ( enableGps ){
				HAL_UART_Init(&huart5);
				stateHwGpsEnable	= 1;
			}
			else{
				stateHwGpsEnable	= 0;
			}
			break;

		case 1:
			availableGps	= 0;
			countGpsAvailable++;

			if ( countGpsAvailable >= limitGpsAvailable ) {
				countGpsAvailable	= 0;
				stateHwGpsEnable	= 2;
			}
			else{
				stateHwGpsEnable	= 1;
			}
			break;

		case 2:
			availableGps	= 1;
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET);
			countGpsAvailable++;

			if ( countGpsAvailable >= limitGpsAvailable ) {
				countGpsAvailable	= 0;
				stateHwGpsEnable	= 3;
			}
			else{
				stateHwGpsEnable	= 2;
			}
			break;

		case 3:
			if ( enableGps ){
				stateHwGpsEnable	= 3;
			}
			else{

				HAL_UART_DeInit(&huart5);
				stateHwGpsEnable	= 0;
			}
			break;
		default:
			stateHwGpsEnable	= 0;
			break;
		}

	switch( stateConfigGps ){
	case 0:
		if ( availableGps ){
			//HAL_UART_Receive_DMA(&huart5,	rxGps, sizeof(rxGps));
			stateConfigGps	= 1;
		}
		else{
			stateConfigGps	= 0;
		}
		break;

	case 1:
		HAL_UART_Transmit_DMA(&huart5, commandUart, sizeof(commandUart) - 1);
		stateConfigGps	= 2;
		break;

	case 2:

		stateConfigGps	= 3;
		break;

	case 3:
		HAL_UART_Transmit_DMA(&huart5, commandUart3, sizeof(commandUart2) - 1);
		stateConfigGps	= 4;
		break;

	case 4:
		//HAL_UART_Transmit_DMA(&huart5, commandUart4, sizeof(commandUart4) - 1);
		stateConfigGps	= 5;
		break;

	case 5:
		HAL_UART_Transmit_DMA(&huart5, commandUart2, sizeof(commandUart3) - 1);
		//HAL_UART_Transmit_DMA(&huart5, commandUart5, sizeof(commandUart5) - 1);
		stateConfigGps	= 6;
		break;

	case 6:
		//HAL_UART_Transmit_DMA(&huart5, commandUart6, sizeof(commandUart6) - 1);
		stateConfigGps	= 7;
		break;

	case 7:
		HAL_UART_Transmit_DMA(&huart5, commandUart8, sizeof(commandUart8) - 1);
		//HAL_UART_Transmit_DMA(&huart5, commandUart7, sizeof(commandUart7) - 1);
		stateConfigGps	= 8;
		break;

	case 8:

		stateConfigGps	= 9;
		break;

	case 9:
		HAL_UART_Transmit_DMA(&huart5, commandUart9, sizeof(commandUart9) - 1);
		stateConfigGps	= 10;
		break;
	case 10:
		if ( enableGps ){
			stateConfigGps	= 10;
		}
		else{
			stateConfigGps	= 0;
		}
		break;
	}
}

//////////
// LORA //
//////////

void hwLoraOut(){

	/******************
	 ***** SERIAL *****
	 ******************
	 *
	 * Transmite un comando por hardware
	 *
	 */

	if ( flagTxLora ){													// Si hay una señal de transmision
		flagTxLora	= 0;												// Reinicia señal
		HAL_UART_Transmit_DMA(&huart4, loraTxCommand0, sizeTxCommand0);	// Transmite por DMA para no estancar procesamiento
		HAL_UART_Receive_DMA(&huart4, rxLora, sizeof(rxLora));			// Inicia recepcion por DMA
		flagUartGps	= 1;
	}

	/********************
	 ***** HARDWARE *****
	 ********************
	 *
	 * Maquina de dos estados para reiniciar por hw el chip RN2903
	 *
	 * S0: Espera 500ms, indica que esta deshabilitado
	 * S1: Indica que esta deshabiltiado, espera señal de RESET
	 *
	 */

	switch ( stateResetHwLora ){

	//////////////////
	// S0 - DISABLE //
	//////////////////

	case 0:

		countResetHwLora++;										// Suma 1 al contador
		flagEnableHwLora	= 0;								// Indica que el chip esta deshabilitado
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);	// Pin en RESET

		if ( countResetHwLora >= resetHwLora ){					// Si pasa el limite de reset
			countResetHwLora	= 0;							// Reinicia contador
			stateResetHwLora	= 1;							// Pasa a S1
		}
		else{													// Si no
			stateResetHwLora	= 0;							// Espera en S0
		}
		break;

	/////////////////
	// S1 - ENABLE //
	/////////////////

	case 1:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);	// Pin en SET
		flagEnableHwLora	= 1;							// Indica que el chip esta habilitado

		if ( flagResetHwLora ){								// Si se da la señal de reinicio
			flagResetHwLora		= 0;						// Apaga señal
			HAL_UART_Receive_DMA(&huart4, rxLora, sizeof(rxLora));
			countResetHwLora2++;
			stateResetHwLora	= 0;						// Pasa  S0
		}
		else{												// Si no
			stateResetHwLora	= 1;						// Espera en S1
		}
		break;

	default:
		stateResetHwLora	= 0;
		break;

	}
}



/******************
 ***** ALARMS *****
 ******************
 *
 *	Accion sobre hardware de alarmas
 *
 */

void hwAlarms(){

	/////////////
	// ALARM 0 //
	/////////////

	if ( flagAlarm0 ){											// Si hay flag de alarma
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);		// Enciende relay
	}
	else{														// Si no
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);	// Lo apaga
	}

	////////////
	// ALAM 1 //
	////////////

	if ( flagAlarm1 ){											// Si hay flag de alarma
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);		// Enciende relay
	}
	else{														// Si no
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);	// Lo apaga
	}
}

/************************
 *** DISPLAY HARDWARE ***
 ************************
 *
 *	Accion sobre leds
 *
 */

void hwDisplay(){
	/***** DIGITS 	*****/

	/////////////////
	// FIRST DIGIT //
	/////////////////

	if ( displayPhy.firstNumber() ){
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_SET);
	}
	else{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);
	}

	//////////////////
	// SECOND DIGIT //
	//////////////////

	if ( displayPhy.secondNumber() ){
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_SET);
	}
	else{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);
	}

	/////////////////
	// THIRD DIGIT //
	/////////////////

	if ( displayPhy.thirdNumber() ){
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_SET);
	}
	else{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_RESET);
	}

	//////////////////
	// FOURTH DIGIT //
	//////////////////

	if ( displayPhy.fourthNumber() ){
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);
	}
	else{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);
	}

	/***** 	LEDS	*****/

	///////////
	// A LED //
	///////////

	if ( displayPhy.aLed() ){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8,  GPIO_PIN_SET);
	}
	else{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8,  GPIO_PIN_RESET);
	}

	///////////
	// B LED //
	///////////

	if ( displayPhy.bLed() ){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9,  GPIO_PIN_SET);
	}
	else{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9,  GPIO_PIN_RESET);
	}

	///////////
	// C LED //
	///////////

	if ( displayPhy.cLed() ){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET);
	}
	else{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_RESET);
	}

	///////////
	// D LED //
	///////////

	if ( displayPhy.dLed() ){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);
	}
	else{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);
	}

	///////////
	// E LED //
	///////////

	if ( displayPhy.eLed() ){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
	}
	else{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
	}

	///////////
	// F LED //
	///////////

	if ( displayPhy.fLed() ){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
	}
	else{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
	}

	///////////
	// G LED //
	///////////

	if ( displayPhy.gLed() ){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
	}
	else{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
	}

	////////////
	// DP LED //
	////////////

	if ( displayPhy.dpLed() ){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
	}
	else{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
	}
}


