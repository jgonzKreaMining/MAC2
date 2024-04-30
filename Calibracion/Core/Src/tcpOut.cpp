/*
 * tcpOut.cpp
 *
 *  Created on: Feb 29, 2024
 *      Author: jgonz
 */

#include <tcp.h>
#include "loraTcp.h"
#include "loraLink.h"

///////////////
// SUPERLOOP //
///////////////

extern uint8_t superloop;

//////////
// LORA //
//////////

extern loraSelect loraSelect;						// Objeto que contiene comando a enviar
extern loraDecision loraDecision;					// Objeto que contiene comparación de resultado esperado vs real
extern loraCheck loraCheck;							// Objeto que checkea la entrada

extern bool flagLoraReset;							// Indica si se reinicia lora
extern bool flagTxLora;								// Indica si se envía mensaje

uint8_t *loraTxCommand0;							// Puntero de comando
uint8_t sizeTxCommand0;								// Largo de comando
uint8_t stateTpcLoraOut;							// Estado de la ME
uint16_t countTcpLoraOut;							// Contador para esperar
const uint16_t limitTcpLoraOut	= 1000/superloop;	// Limite para enviar comando

/*****	OUTPUT	*****/

void tcpOutput(){
	//tcpOutLora();
}

void tcpOutLora(){
	switch( stateTpcLoraOut ){

	///////////////////////
	// S0 - WAIT COMMAND //
	///////////////////////

	case 0:
		if ( loraSelect.newCommand() ){	// Si hay nuevo comando
 			stateTpcLoraOut	= 1;		// Pasa a S1
		}
		else{							// Si no
			stateTpcLoraOut	= 0;		// Queda en S0
		}
		break;

	////////////////////////
	// S1 - WAIT FOR SEND //
	////////////////////////

	case 1:
		countTcpLoraOut++;							// Suma 1 al contador

		if ( countTcpLoraOut >= limitTcpLoraOut ){	// Si llega al limite
			stateTpcLoraOut	= 2;					// Pasa a S2
		}
		else{										// Si no
			stateTpcLoraOut	= 1;					// Queda en S1
		}
		break;

	/////////////////////////////
	// S2 - CONFIG BY TRANSMIT //
	/////////////////////////////

	case 2:
		countTcpLoraOut	= 0;										// Reinicia contador

		loraTxCommand0 	= loraSelect.getCommand();					// Guarda comando
		sizeTxCommand0	= loraSelect.getCommandSize();				// Guarda tamaño de comando

		loraCheck.setIdealResponses( loraSelect.qttyResponses() );	// Guarda largo de lista de respuestas
		loraDecision.reset();										// Reinicia TCP
		flagTxLora	= 1;											// Transmite
		stateTpcLoraOut	= 0;										// Vuelve a S0
		break;

	default:
		stateTpcLoraOut	= 0;
		break;
	}

}
