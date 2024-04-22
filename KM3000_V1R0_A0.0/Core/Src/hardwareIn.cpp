/*
 * hw.c
 *
 *  Created on: Jan 11, 2024
 *      Author: jgonz
 */

#include <main.h>
#include <fifoUart.h>
#include <gpsGNSS.h>
#include <hardware.h>
#include <loraLink.h>

#include "stm32l4xx_hal.h"

///////////////
// SUPERLOOP //
///////////////

extern uint8_t superloop;			// milisegundos de superloop

//////////////
// I2C MAIN //
//////////////

extern I2C_HandleTypeDef hi2c1;		// Instancia de I2C_1
extern ADC_HandleTypeDef hadc1;		// Instancia de ADC_1
extern UART_HandleTypeDef huart4;	// Instancia de UART 4

///////////
// BOTON //
///////////

bool boton;							// Valor de hw del boton
int countBoton;						// Contador de boton
int limitBoton	= 400/superloop;	// 400 milisegundos en 100 microsegundos
uint8_t stateBoton;					// Estado del boton
extern bool flagBoton;				// Boton presionado por 400 ms

///////////
// SHT31 //
///////////

uint16_t command_Sht31	= 0x2721;				//0x240B;	// SINGE SHOT
												// NON STRETCH
												// MEDIUM REPEABILITY

uint8_t stateSht31;								// Estado SHT31
int countSht31;									// Contador sht31
int limitSht31_measure	= 2000/superloop;		// Limite de espera entre operaciones
int limitSht31_i2c		= 50/superloop;			// Limite

uint8_t buffer_SHT31_I2C_OUT[2]	= {0x24, 0x0B};	// BUFFER QUE ENVIA COMANDO
uint8_t buffer_SHT31_I2C_IN[6];					// Buffer que recibe respuesta

bool selectSht31;								// 0: Interno | 1: Externo
uint8_t addressSht31	= 0x44;					// Address de I2C

uint16_t tempIntern;							// Temperatura interna del sensor
uint16_t humIntern;								// Humedad interna del sensor

uint16_t tempExtern;							// Temperatura externa del sensor
uint16_t humExtern;								// Humedad externa del sensor

#define SHT31_ADDRESS_A 0x44 					// Direccion i2c sensor externo
#define SHT31_ADDRESS_B 0x45 					// Direccion i2c sensor interno


/////////////
// ADS1115 //
/////////////

#define ADS1115_ADDRESS 0x48					// I2D address of ads1115

uint8_t stateAdc;								// Estado de la ME

int countAdc;
const int sampleAdc	= 100/superloop;			// Tiempo de medicion

bool enableI2C	= 1;							// Indicador de I2C habilitado para transmitir/recibir
const int limitAds_i2c = 20/superloop-1;		// Tiempo que espera respuesta

uint8_t AdsCommand_0[3]	= {0x01, 0xD1, 0x83};	// Comando para solicitar valor Canal 0
uint8_t AdsCommand_1[1]	= {0x00};				// Comando para solicitar request
uint8_t AdsCommand_2[3] = {0x01, 0xC1, 0x83};	// Comando para solicitar valor Canal 1

uint8_t buffer_ADS_I2C_IN[2];					// Buffer de I2C

uint16_t alphaA;								// Valor de Alpha A
uint16_t alphaB;								// Valor de Alpha B

bool selectChannelAds;							// Seleccion de canal ADS1115
extern bool flagI2C_DMA;						// Indica que I2C esta habilitado

////////////
// ANALOG //
////////////

uint16_t battery;					// Medicion de bateria
uint16_t alphaAnalog_A;				// Medicion de A1 en ADC del micro
uint16_t alphaAnalog_B;				// Medicion de B1 en ADC del micro

extern bool flagAnalog;				// Indica que se lleno el bus
extern uint32_t analog[3];			// Buffer con datos de analog

int countAnalog;					// Contador para muestreo
int sampleAnalog	= 50/superloop;	// Frecuencia de muestreo

uint8_t stateAnalog;				// Estado de la máquina

////////////
// EEPROM //
////////////

/** Valores fijos **/

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

/** Valores desde EEPROM **/

uint8_t nameSensor_0[20];	// NOMBRE DEL SENSOR
uint8_t idSensor_0[4];		// ID DEL SENSOR
uint8_t rangeSensor_0[4];	// RANGO DE MEDICION DEL SENSOR
uint8_t amplifier_10[4];	// VALOR DE AMPLIFICACION 1
uint8_t amplifier_20[4];	// VALOR DE AMPLIFICACION 2
uint8_t groundSensor_0[4];	// N DE LA PENDIENTE
uint8_t curveSensor_0[4];	// M DE LA PENDIENTE
uint8_t dateCalib_0[10];	// FECHA DE CALIBRACION

uint8_t nameSensor_1[20];	// NOMBRE DEL SENSOR 1
uint8_t idSensor_1[4];		// ID DEL SENSOR
uint8_t rangeSensor_1[4];	// RANGO DEL SENSOR
uint8_t amplifier_11[4];	// VALOR DE AMPLIFICACION 1
uint8_t amplifier_21[4];	// VALOR DE AMPLIFICACION 2
uint8_t groundSensor_1[4];	// N DE LA PENDIENTE
uint8_t curveSensor_1[4];	// M DE LA PENDIENTE
uint8_t dateCalib_1[10];	// FECHA DE CALIBRACION

uint8_t stateEEPROM;		// Estado de maquina EEPROM

uint8_t nameAlphaA[]	= "HCL A1 ALPHASENSE";	// Valor de verificación específico MAC
uint8_t nameAlphaB[]	= "HCL B1 ALPHASENSE";	// Valor de verificación específico MAC

/////////
// GPS //
/////////

extern fifoUart gpsIn;		// Fifo de entrada
extern gpsInput gpsInput;	// Detecta GPRMC y separa datos
uint8_t gpsBus;				// Buffer de traspaso

//////////
// LORA //
//////////

uint8_t  loraBus;			// Buffer para traspasar info entre Fifo e Input
extern fifoUart loraIn;		// Fifo con elementos desde DMA
extern loraInput loraInput;	// Detecta mensaje valido en caractares entrantes

/////////////
// GENERAL //
/////////////

bool warningHardware[8];	/* SI HAY UN WARNING, EL SISTEMA PUEDE OPERAR
							*	Solo que de manera no optima
							*
							*	0: NO HAY SENSOR DE TEMPERATURA INTERNO
							*	1: NO HAY SENSOR DE TEMPERATURA EXTERNO
							*	2: NO HAY ADS1115
							*/

bool errorHardware[8];		/*	SI HAY UN ERROR, EL SISTEMA NO PUEDE OPERAR
							*	ESTA ENTREGANDO VALORES ERRONEOS
							*
							*
							*	0: NO HAY SHT INTERNO
							*	1: NO HAY SHT EXTERNO
							*	2: NO HAY SENSOR ALPHASENSE A
							*	3: NO HAY SENSOR ALPHASENSE B
							*	4: NO HAY EEPROM
							*	5: NO HAY ADS1115
							*	6: BATERIA BAJA
							*	7: FUTURO USO
							*/

/***** INPUT *****/

void hwInput(){
	hwBoton();
	hwSht31();
	hwAds1115();
	hwAnalog();
	hwEEPROM();
	hwGpsIn();
	hwLoraIn();
}

/***********************
 *****	LORA INPUT *****
 ***********************
 *
 * Traspaso de datos desde DMA a FIFO
 * Traspaso de FIFO a bus loraBus
 * Traspaso de de loraBus a loraInput
 *
 * loraInput detecta respuesta dentro de bytes entrantes
 */

void hwLoraIn(){
	if ( loraIn.available() ){					// Si hay elementos disponibles en FIFO desde DMA
		loraBus	= loraIn.readElement();			// Guarda el valor
		loraInput.insertElement( loraBus );		// Inserta en Lora Input
	}
}

/*******************
 ***** ADS1115 *****
 *******************
 *
 *	warningHardware[2]	: Indica si hay o no ADC
 *
 *	INPUT	:	I2C
 *	OUTPUT	:	alphaA, alphaB
 *				warningHardware[2]
 */

void hwAds1115(){

	switch ( stateAdc ){

	////////////////////////////
	// S0 - WAIT FOR SAMPLING //
	////////////////////////////
	case 0:

		countAdc++;									// Suma 1 al contador

		if ( countAdc >= sampleAdc && enableI2C ){	// Si pasa limite de medición y el I2C esta habilitado
			enableI2C	= 0;						// Deshabilita I2C
			stateAdc	= 1;						// Pasa a S1
		}
		else{										// Si no
			stateAdc	= 0;						// Espera en S0
		}
		break;

	////////////////////////////
	// S1 - TRANSMIT COMMANDS //
	////////////////////////////

	case 1:
		countAdc	= 0;																// Reinicia contador

		if ( selectChannelAds ){														// Si esta seleccionado channel 0
			HAL_I2C_Master_Transmit_DMA(&hi2c1, ADS1115_ADDRESS<<1, AdsCommand_0, 3);	// Transmite comando
		}
		else{																			// Si esta seleccionado channel 1
			HAL_I2C_Master_Transmit_DMA(&hi2c1, ADS1115_ADDRESS<<1, AdsCommand_2, 3);	// Transmite comando
		}

		flagI2C_DMA	= 0;																// Reinicia flag DMA
		stateAdc	= 2;																// Pasa a S2

		break;

	/////////////////////////
	// S2 - CHECK TRANSMIT //
	/////////////////////////

	case 2:
		HAL_I2C_Master_Transmit_DMA(&hi2c1, ADS1115_ADDRESS<<1, AdsCommand_1, 1);	// Transmite comando
		errorHardware[5]	= !flagI2C_DMA;											// Registra si no hay ADS

		if ( flagI2C_DMA ){					// Si HAY ADC
			stateAdc	= 3;				// Pasa a S3
		}
		else{								// Si no
			stateAdc	= 5;				// Pasa  s5
		}
		break;

	////////////////////////////
	// S3 - WAIT FOR RESPONSE //
	////////////////////////////

	case 3:
		countAdc++;							// Suma 1 al contador

		if ( countAdc >= limitAds_i2c ){	// Si pasa el limite de espera de respuesta
			stateAdc	= 4;				// Pasa a S4
		}
		else{								// Si no
			stateAdc	= 3;				// Pasa a S3
		}
		break;

	//////////////////////
	// S4 - RECEIVE I2C //
	//////////////////////

	case 4:
		countAdc	= 0;																// Reinicia contador
		HAL_I2C_Master_Receive_DMA(&hi2c1, ADS1115_ADDRESS<<1, buffer_ADS_I2C_IN, 2);	// Solicita datos del buffer
		if ( selectChannelAds ){														// Si esta seleccionado channel 0
			alphaA = ( buffer_ADS_I2C_IN[0] << 8 | buffer_ADS_I2C_IN[1] );				// Guarda en Alpha0
		}
		else{																			// Si esta seleccionado channel 1
			alphaB = ( buffer_ADS_I2C_IN[0] << 8 | buffer_ADS_I2C_IN[1] );				// Guarda en Alpha1
		}
		stateAdc	= 5;																// Pasa a S5
		break;

	/////////////////////
	// S5 - CLOSE LOOP //
	/////////////////////

	case 5:
		selectChannelAds	= !selectChannelAds;	// Cambia canal
		enableI2C	= 1;							// Habilita uso de I2C
		stateAdc	= 0;							// Vuelve a S0
		break;

	default:
		break;
	}
}

/*****************
 ***** SHT31 *****
 *****************
 *
 *	Realiza la medición del SHT31 interno y externo
 *
 *	Primero configura, luego realiza medición
 *	Cambia de Address y reinicia el proceso
 *
 *
 *	warningHardware[0]: Indica si no hay sensor interno
 *	warningHardware[1]: Indica si no hay sensor externo
 *
 *
 *	El dispositivo puede seguir operando si falta algun sht31
 *	Por este motivo solo es un warning
 *
 *
 *	INPUT	: 	I2C
 *	OUTPUT	: 	tempIntern, tempExtern
 *				humIntern, humExtern
 *				warningHardware[0], warningHardware[1]
 */

void hwSht31(){

	switch ( stateSht31 ){

	///////////////////////////
	// S0 - WAIT FOR MEASURE //
	///////////////////////////

	case 0:
		countSht31++;							// Suma 1 al contador

		if ( countSht31 >= limitSht31_measure && enableI2C ){// Si el contador pasa limite
			enableI2C	= 0;					// Deshabilita uso de I2C
			stateSht31	= 1;					// Pasa a S1
		}
		else{									// Si no
			stateSht31	= 0;					// Espera
		}
		break;

	///////////////////////////
	// S1 - TRANSMIT COMMAND //
	///////////////////////////

	case 1:
		countSht31	= 0;				// Reinicia contador
										// Transmite comando por I2C al SHT31 correspondiente
										// Solicita respuesta del proceso

		buffer_SHT31_I2C_OUT[0]	= command_Sht31 >> 8;				// Copia primer byte del comando
		buffer_SHT31_I2C_OUT[1]	= uint8_t ( command_Sht31 & 0xFF );	// Copia segundo byte del comando

		flagI2C_DMA	= 0;											// Reinicia flag de envio de datos
														 			// Transmite comando
		HAL_I2C_Master_Transmit_DMA(&hi2c1, addressSht31<<1, buffer_SHT31_I2C_OUT, 2);

		stateSht31	= 2;											// Pasa a S2
		break;

	////////////////////////
	// S2 - NO HAY SENSOR //
	////////////////////////

	case 2:
										// EL DISPOSITIVO PUEDE OPERAR SIN SENSORES DE TEMPERATURA
										// PERO CON FALTA DE INFORMACIÓN PARA EL SENSOR
										// flagI2C_DMA -> 0: No hay sensor | 1: Hay sensor
										// flagI2C_DMA se actualiza en HAL_I2C_MasterTxCpltCallback
										// HAL_I2C_MasterTxCpltCallback se activa cuando el DMA transmitió correctamente

		if ( !selectSht31 ){					// Si esta seleccionado SHT31 interno
			errorHardware[0]	= !flagI2C_DMA;	// Marca estado del sensor
		}
		else{									// Si esta seleccionado SHT31 externo
			errorHardware[1]	= !flagI2C_DMA;	// Marca estado del sensor
		}

		if ( flagI2C_DMA ){						// Si hay sensor
			stateSht31	= 3;					// Pasa a S3 para recibir datos
		}
		else{									// Si no hay sensor
			stateSht31	= 6;					// Pasa a S6 - cambio de sensor
		}
		break;

	///////////////////////////
	// S3 - ESPERA RESPUESTA //
	///////////////////////////

	case 3:
		countSht31++;							// Suma 1 al contador

		if ( countSht31 >= limitSht31_i2c ){	// Si pasa el limite de respuesta
			stateSht31	= 4;					// Pasa a S4
		}
		else{									// Sino
			stateSht31	= 3;					// Espera en S3
		}
		break;

	///////////////////////
	// S4 - RECIBE DATOS //
	///////////////////////
	case 4:
		countSht31	= 0;	// Reinicia contador
							// RECIBE LOS DATOS DEL BUS I2C
		HAL_I2C_Master_Receive_DMA(&hi2c1, addressSht31<<1, buffer_SHT31_I2C_IN, 6);
		stateSht31	= 5;	// Pasa a S5 para
		break;

	//////////////////////////////
	// S5 - ESCALA DE VARIABLES //
	//////////////////////////////

	case 5:

		if ( !selectSht31 ){														// Si esta seleccionado SHT INTERNO
			tempIntern	= (buffer_SHT31_I2C_IN[0] << 8) + buffer_SHT31_I2C_IN[1];	// Une valores del bus para temperatura
			tempIntern	= 1750*tempIntern/65535 - 450;								// Escala sugerida

			humIntern	= (buffer_SHT31_I2C_IN[3] << 8) + buffer_SHT31_I2C_IN[4];	// Une los valores del bus para humedad
			humIntern	= 1000*humIntern/65535;										// Escala sugerida
		}
		else{																		// Si esta seleccionado SHT EXTERNO
			tempExtern	= (buffer_SHT31_I2C_IN[0] << 8) + buffer_SHT31_I2C_IN[1];	// Une valores del bus paa temperatura externa
			tempExtern	= 1750*tempExtern/65535 - 450;								// Escala sugerida

			humExtern	= (buffer_SHT31_I2C_IN[3] << 8) + buffer_SHT31_I2C_IN[4];	// Une valores del bus para humedad
			humExtern	= 1000*humExtern/65535;										// Escala sugerida
		}
		stateSht31	= 6;
		break;

	///////////////////////////
	// S6 - CAMBIO DE SENSOR //
	///////////////////////////
	case 6:
		enableI2C	= 1;
		selectSht31	= !selectSht31;				//	Cambia sht31 seleccionado

		if ( selectSht31 ){						// Si selecciona SHT31 Externo
			addressSht31	= SHT31_ADDRESS_A;	// Dija address 0x4
		}
		else{									// Si selecciona SHT31 Interno
			addressSht31	= SHT31_ADDRESS_B;	// Fija address 0x45
		}

		stateSht31	= 0;						// Cierra el ciclo
		break;

	default:
		stateSht31	= 0;
		break;
	}
}

/*****************
 ***** BOTON *****
 *****************
 *
 *	Algoritmo de entrada del boton.
 *	Para considerar al boton presionado debe mantenerse apretado cierto tiempo.
 *
 *	INPUT 	: GPIO_A, PIN_3
 *	OUTPUT	: flagBoton
 *
 */

void hwBoton(){
	///////////
	// BOTON //
	///////////

	boton	= ! HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3);	// Invierte la entrada

	switch (stateBoton){

	//////////////////////////
	// S0 - WAIT FOR SIGNAL //
	//////////////////////////

	case 0:
		countBoton	= 0;		//	Reinicia contador
		flagBoton	= 0;		//	Reinicia flag de boton apretado

		if ( boton ){			//	Si se presiona el boton
			stateBoton	= 1;	//	Pasa a S1
		}
		else{					// Si no
			stateBoton	= 0;	// Se queda en S0
		}

		break;

	/////////////////////////
	// S1 - WAIT FOR LIMIT //
	/////////////////////////

	case 1:
		countBoton++;								// Suma 1 al contador

		if ( boton && countBoton >= limitBoton ){	// Si el boton esta presionado y llega al limite del contador
			stateBoton	= 2;						// Pasa a S2
		}
		else if ( !boton ){							// Si el boton no estaba apretado
			stateBoton	= 0;						// Vuelve a S0
		}
		else{										// Si falta que contador llegue al limite
			stateBoton	= 1;						// Se queda en S1
		}

		break;

	////////////////////
	// S2 - SET VALUE //
	////////////////////

	case 2:
		flagBoton	= 1;		// Indica que se presiono el boton
		stateBoton	= 0;		// Vuelve a S0
		break;

	default:
		break;
	}
}

/**********************
 *****	GPS INPUT	***
 **********************
 *
 *	Revisa el estado de la FIFO sobre el puerto serial de GPS
 *	Inserta el valor en la clase gpsInput
 *
 *	INPUT	:	gpsIn.available()
 *	OUTPUT	:	gpsInput.inserValue()
 */

void hwGpsIn(){
	if ( gpsIn.available() ){			// Si hay un elemento en fifo
		gpsBus = gpsIn.readElement();	// Lee valor en fifo
		gpsInput.insertValue( gpsBus );	// Pasa el simbolo al otro metodo

		if ( gpsInput.stateInsert == 7){
			gpsBus = gpsBus;
		}
	}
}

/******************
 ***** ANALOG *****
 ******************
 *
 *	Realiza la medicion del ADC1, espera un tiempo de muestreo
 *
 *	INPUT	:	analog[0],	analog[1], flagAnalog
 *	OUTPUT	:	battery, alphaAnalog_A, alphaAnalog_B
 */

void hwAnalog(){
	switch ( stateAnalog ){

	///////////////////////////
	// S0 - WAIT SAMPLE TIME //
	///////////////////////////

	case 0:
		countAnalog++;						// Suma 1 al contador

		if ( countAnalog >= sampleAnalog ){	// Si cumple el tiempo de muestreo
			stateAnalog	= 1;				// Pasa a S1
		}
		else{								// Si no
			stateAnalog	= 0;				// Espera en S0
		}
		break;

	////////////////////////
	// S1 - START MEASURE //
	////////////////////////

	case 1:
		countAnalog	= 0;									// Reinicia contador
		HAL_ADC_Start_DMA(&hadc1, (uint32_t *) analog, 3);	// Inicia muestreo
		stateAnalog	= 2;									// Pasa a S2
		break;

	////////////////////
	// S2 - SAVE DATA //
	////////////////////

	case 2:
		if ( flagAnalog ){									// Si guardo correctamente
			battery			= (analog[0] & 0xFFFF);			// Guarda valor bateria
			alphaAnalog_A	= (analog[0] >> 16 & 0xFFFF);	// Guarda valor Alpha A
			alphaAnalog_B	= (analog[1] & 0xFFFF);			// Guarda valor Alpha B
			flagAnalog		= 0;							// Reinicia flag de medicion analogica
			stateAnalog		= 0;							// Vuelve a S0
		}
		else{												// Si no ha guardado
			stateAnalog		= 2;							// Espera en S2
		}
		break;
	}
}

/********************
 *****  EEPROM	*****
 ********************
 *
 * SOLO LECTURA DE EEPROM
 * En operacion normal, el sistema MAC no debe grabar en EEPROM
 *
 *	Pagina 0 : Sensor B1
 *	Pagina 1 : Sensor A1
 *
 *
 * nameSensor 	[0-19] : Nombre y fabricante del sensor
 * idSensor		[20-23]: Número de sensor calibrado por Krea Mining
 * rangeSensor	[24-27]: Rango de medicion del sensor en PPM
 * amplifier1	[28-31]: Valor del primer amplificador en kilo Ohms
 * amplifier2	[32-35]: Valor del segundo amplificador en factor multiplicativo
 * groundSensor	[36-39]: Valor del piso de ruido en puntos de ADC
 * cuveSensor	[40-43]: Valor de pendiente en PPM/puntos ADC
 * dateCalib	[44-53]: Fecha de calibracion
 *
 */

/* EN LA ACTUAL CALIBRACION
 *
 * 1. Esperamos 20 min
 * 2. Apretar boton --->> Obtenemos el Zero current
 * 3. Esperamos 30s y esperamos si el crecimiento es "relevante", cerca de 100-150 pnts de medicion
 * 4. Espera los siguientes 2min 30s midiendo el gas cada 10s.
 * 5. Toma el promedio de los ultimos 10 datos -->> Aqui se calcula la pendiente
 * 6. Prende led
 *
 *
 * CUANDO PRESENTABA PROBLEMAS
 *
 * 1. Hardware - cable
 * 2. No apretaban el boton
 *
 * SUGERENCIAS
 *
 * 1. Indicar cada uno de los pasos de calibracion
 * 2. Guardas datos de forma periodica en EEPROM
 * 3.
 */

void hwEEPROM(){

	switch ( stateEEPROM ){

	//////////////////////
	// S0 - READ PAGE 0 //
	//////////////////////

	case 0:

		EEPROM_Read(0, 0,  nameSensor_0,	sizeof(nameSensor_0));		// Name of sensor
		EEPROM_Read(0, 20, idSensor_0,		sizeof(idSensor_0));		// ID of sensor
		EEPROM_Read(0, 24, rangeSensor_0,	sizeof(rangeSensor_0));		// Range of sensor
		EEPROM_Read(0, 28, amplifier_10,	sizeof(amplifier_10));		// Amplifier 1 of sensor
		EEPROM_Read(0, 32, amplifier_20,	sizeof(amplifier_20));		// Amplifier 2 of sensor
		EEPROM_Read(0, 36, groundSensor_0,	sizeof(groundSensor_0));	// N of curve
		EEPROM_Read(0, 40, curveSensor_0,	sizeof(curveSensor_0));		// M of curve
		EEPROM_Read(0, 44, dateCalib_0,		sizeof(dateCalib_0));		// Date of calibration
		stateEEPROM	= 1;												// S1 to read page 1
		break;

	//////////////////////
	// S1 - READ PAGE 1 //
	//////////////////////

	case 1:
		EEPROM_Read(1, 0,  nameSensor_1,	sizeof(nameSensor_1));		// Name of sensor
		EEPROM_Read(1, 20, idSensor_1,		sizeof(idSensor_1));		// ID of sensor
		EEPROM_Read(1, 24, rangeSensor_1,	sizeof(rangeSensor_1));		// Range of sensor
		EEPROM_Read(1, 28, amplifier_11,	sizeof(amplifier_11));		// Amplifier 1 of sensor
		EEPROM_Read(1, 32, amplifier_21,	sizeof(amplifier_21));		// Amplifier 2 of sensor
		EEPROM_Read(1, 36, groundSensor_1,	sizeof(groundSensor_1));	// N of curve
		EEPROM_Read(1, 40, curveSensor_1,	sizeof(curveSensor_1));		// M of curve
		EEPROM_Read(1, 44, dateCalib_1,		sizeof(dateCalib_1));		// Date of calibration
		stateEEPROM	= 2;												// S2 to stop of read
		break;

	///////////////////////
	// S2 - CHECK ERRORS //
	///////////////////////

	case 2:
		/*
		for (int i=0; i<512; i++){
			EEPROM_PageErase(i);
		}*/
		/*
		EEPROM_Write(0, 0, nameSensor0, sizeof(nameSensor0));
		EEPROM_Write(0, 20, idSensor0, sizeof(idSensor0));
		EEPROM_Write(0, 24, rangeSensor0, sizeof(rangeSensor0));
		EEPROM_Write(0, 28, amplifier1_0, sizeof(amplifier1_0));
		EEPROM_Write(0, 32, amplifier2_0, sizeof(amplifier2_0));
		EEPROM_Write(0, 36, groundSensor0, sizeof(groundSensor0));
		EEPROM_Write(0, 40, curveSensor0, sizeof(curveSensor0));
		EEPROM_Write(0, 44, dateCalib0, sizeof(dateCalib0));

		EEPROM_Write(1, 0, nameSensor1, sizeof(nameSensor0));
		EEPROM_Write(1, 20, idSensor1, sizeof(idSensor1));
		EEPROM_Write(1, 24, rangeSensor1, sizeof(rangeSensor0));
		EEPROM_Write(1, 28, amplifier1_1, sizeof(amplifier1_0));
		EEPROM_Write(1, 32, amplifier2_1, sizeof(amplifier2_0));
		EEPROM_Write(1, 36, groundSensor1, sizeof(groundSensor0));
		EEPROM_Write(1, 40, curveSensor1, sizeof(curveSensor0));
		EEPROM_Write(1, 44, dateCalib1, sizeof(dateCalib0));
		*/
		stateEEPROM	= 3;								// Pasa a S3
		break;

	///////////////////////
	// S3 - STOP READING //
	///////////////////////

	case 3:
		uint8_t i;										// Inicia contador
		for (i = 0; i<sizeof(nameAlphaB) ; i++){		// Recorre caracter a caracter
			if ( nameAlphaB[i]	== nameSensor_0[i] ){	// Compara caracter ideal y caracter leido
				errorHardware[4]	= 0;				// Si son iguales  no hay error
			}
			else{										// Si son distintos
				errorHardware[4]	= 1;				// Indica error
				break;									// Termina loop
			}
		}

		for (i = 0; i<sizeof(nameAlphaB) ; i++){		// Recorre caracter a caracter del valor ideal
			if ( nameAlphaB[i]	== nameSensor_0[i] ){	// Compara caracter ideal y caracter leido
				errorHardware[4]	= 0;				// Si son iguales no idnica error
			}
			else{										// Si son distintos
				errorHardware[4]	= 1;				// Indica error
				break;									// Termina loop
			}
		}
		stateEEPROM	= 4;
		break;

	case 4:
		break;
	default:
		stateEEPROM	= 0;
		break;
	}
}
