/*
 * loraApp.h
 *
 *  Created on: Jan 8, 2024
 *      Author: jgonz
 */

#include <string>
#include <stdint.h>

#ifndef INC_LORAAPP_H_
#define INC_LORAAPP_H_


////////////////////
// COMMAND STRUCT //
////////////////////

struct rn2903Command{
	uint8_t size;			// Tamaño de comando
	uint8_t *command;		// Puntero del comando
	uint8_t responseSize;	// Largo de lposibles respuestas
};

extern uint8_t sysReset_RN2903[];		// Reinicia sistema

extern uint8_t radioSetMod_RN2903[];	// Fija modulacion
extern uint8_t radioSetSf_RN2903[];		// Fija Spreading factor
extern uint8_t radioSetBw_RN2903[];		// Fija ancho de banda
extern uint8_t radioSetPwr_RN2903[];	// Fija potencia

extern uint8_t macSetDevEui_RN2903[];	// Fija devEui
extern uint8_t macSetDevAddr_RN2903[];	// Fija DevAddr
extern uint8_t macSetnwkskey_RN2903[];	// Fija network key
extern uint8_t macSetAppskey_RN2903[];	// Fija App key
extern uint8_t macJoinOtaa_RN2903[];	// Fija autenticacion OTAA
extern uint8_t macJoinAbp_RN2903[];		// Fija autenticacion ABP
extern uint8_t macSetSync_RN2903[];		// Fija sincronizacion
extern uint8_t macSave_RN2903[];		// Guarda configuracion

extern uint8_t macTxData_RN2903[];		// Transmite datos
extern uint8_t macTxGps_RN2903[];		// Transmite gps
extern uint8_t macTxSys_RN2903[];		// Transmite sistema

extern uint8_t macSetCh8_RN2903[];		// Deshabilita channel 8
extern uint8_t macSetCh9_RN2903[];		// Deshabilita channel 9
extern uint8_t macSetCh10_RN2903[];		// Deshabilita channel 10

extern rn2903Command sysReset;			// Reinicia sistema

extern rn2903Command radioSetMod;		// Fija modulacion
extern rn2903Command radioSetSf;		// Fija spreading factor
extern rn2903Command radioSetBw;		// Fija ancho de banda
extern rn2903Command radioSetPwr;		// Fija potencia

extern rn2903Command macSetDeveui;		// Fija devEui
extern rn2903Command macSetDevAddr;		// Fija devAddr
extern rn2903Command macSetNwkskey;		// Fija network key
extern rn2903Command macSetAppskey;		// Fija app key
extern rn2903Command macJoinOtaa;		// Fija transmision OTAA
extern rn2903Command macJoinAbp;		// Fija transmision ABP
extern rn2903Command macSetSync;		// Fija sincronizacion
extern rn2903Command macSave;			// Guarda configuracion

extern rn2903Command macTxData;			// Transmite datos sensores
extern rn2903Command macTxGps;			// Transmite datos Gps
extern rn2903Command macTxSys;			// Transmite datos sistema

extern rn2903Command macSetCh8;			// Deshabilita channel 8
extern rn2903Command macSetCh9;			// Deshabilita channel 9
extern rn2903Command macSetCh10;		// Deshabilita channel 10

extern uint8_t rxCommand_0[];			// Respuesta 0
extern uint8_t rxCommand_1[];			// Respuesta 1
extern uint8_t rxCommand_2[];			// Respuesta 2
extern uint8_t rxCommand_3[];			// Respuesta 3
extern uint8_t rxCommand_4[];			// Respuesta 4
extern uint8_t rxCommand_5[];			// Respuesta 5
extern uint8_t rxCommand_6[];			// Respuesta 6
extern uint8_t rxCommand_7[];			// Respuesta 7
extern uint8_t rxCommand_8[];			// Respuesta 8
extern uint8_t rxCommand_9[];			// Respuesta 9
extern uint8_t rxCommand_10[];			// Respuesta 10
extern uint8_t rxCommand_11[];			// Respuesta 11
extern uint8_t rxCommand_12[];			// Respuesta 12
extern uint8_t rxCommand_13[];			// Respuesta 13
extern uint8_t rxCommand_14[];			// Respuesta 14
extern uint8_t rxCommand_15[];			// Respuesta 15
extern uint8_t rxCommand_16[];			// Respuesta 16
extern uint8_t rxCommand_17[];			// Respuesta 17

extern uint8_t *rxCommand[];			// Puntero de punteros para respuestas
extern uint8_t *rxCommand_Loop[];		// Puntero de punteros

void setupLoraApp();

/////////////
// RUTINES //
/////////////
class loraRutine{
	rn2903Command* listCommands[100];	// LISTA DE PUNTEROS
	bool stopped;						// Indica que se lee el último
private:

public:
	uint8_t sizeList;							// Largo de lista
	uint8_t actual;								// Valor de lectura actual
	void addCommand (rn2903Command *command);	// Añade un comando
	rn2903Command *command();					// Entrega puntero de comando
	void reset();								// Reinicia lista
	bool finish();								// Indica si termino lista
};

/***********************
 *** ALGORITHM SETUP ***
 ***********************
 *
 * Contiene los parametros de setup del chip lora
 *
 * Guia de uso:
 * 		1.- start:			Inicia algoritmo
 * 		2.-	stop:			Forza termino algoritmo
 * 		3.-	next:			Avanza al siguiente comando
 * 		4.-	newCommand:		Indica si hay nuevo comando
 * 		5.-	finish:			Indica si terminó el algoritmo
 * 		6.-	outCommand:		Puntero al comando
 * 		7.-	outResponses:	Puntero a las respuestas
 *		7.-	sizeResponse:	Largo de respuestas
 */

class setup{
private:
	bool flagRun;					//	Indica que el algoritmo esta corriendo
	bool flagNew;					//	Indica que hay nuevo comando
	bool flagFinish;				//	Indica que el algoritmo termino
	uint8_t counter;				//	Contador de lista
	uint8_t qttyCommands	= 10;	//	Cantidad total de comandos
	uint8_t qttyResponses	= 4;	//	Cantidad total de respuestas

	std::string *actualCommand;		//	Puntero a comando
	std::string *actualResponse;	//	Puntero a respuesta

	uint8_t size;					//	Largo de respuestas

	uint8_t hashResponses[10]{		//	Respuestas asociadas a cada comando
		1,		//	0
		1,		//	1
		1,		//	2
		1,		//	3
		1,		//	4
		1,		//	5
		1,		//	6
		1,		//	7
		1,		//	8
		3,		//	9
	};

public:
	void start();					//	Inicia algoritmo
	void stop();					//	Forza el termino del algoritmo
	void next();					//	Indica que sigue el algoritmo
	bool newCommand();				//	Indica si hay nuevo comando
	bool finish();					//	Indica si el algoritmo finalizó

	std::string* outCommand();		//	Entrega puntero a comando actual
	std::string* outResponse();		//	Entrega puntero a pull de respuestas
	uint8_t sizeResponse();			//	Entrega el largo del pull de respuestas

	std::string commands[10]	=
			{"radio set bw 250",								//	Fija el ancho de banda
			"radio set sf sf10",								//	Fija el spreading factor
			"radio set pwr 20",									//	Fija la potencia de tx
			"radio set mod lora",								//	Fija la modulacion
			"mac set deveui 0000005200000052",					//	Fija DEVEUI
			"mac set devaddr 00000052",							//	Fija DEVADDR
			"mac set nwkskey 237FCD5AE5126C04E2F0809004DCC700",	//	Fija Network season key
			"mac set appskey 237FCD5AE5126C04E2F0809004DCC700",	//	Fija Application season key
			"mac set sync 34",									//	Fija el simbolo de sincronizacion
			"mac join abp"};									//	Se une a ABP

	std::string resp[4]	= {"ok",								//	Primera respuesta posible
			"invalid_param",									//	Segunda respuesta posible
			"accepted",											//	Tercera respuesta posible
			"denied"};											//	Cuarta respuesta posible
};


#endif /* INC_LORAAPP_H_ */
