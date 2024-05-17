/*
 * linkOut.cpp
 *
 *  Created on: Feb 29, 2024
 *      Author: jgonz
 */

#include "link.h"

///////////////
// SUPERLOOP //
///////////////

extern uint8_t superloop;

/////////
// GPS //
/////////

extern bool startGps;		// Indica que se inicia gps
extern bool stopGps;		// Indica que se detiene gps
extern bool enableGps;		// Habilita Gps

/////////////
// DISPLAY //
/////////////

extern displayLink displayLink;		// Enlace de display
extern displayPhysical displayPhy;	// Capa fisica de display
uint8_t digit;						// Contador de digito

///////////////////
// LORA HARDWARE //
///////////////////

bool flagWdLora;		// Indica que se venció wd
bool flagResetHwLora;	// Indica que se debe reiniciar por sw

///////////////////
// LORA SOFTWARE //
///////////////////

extern wdTimeout loraTimeWd;	// Watchdog de Lorawan
extern bool flagTxLora;			// Indica que se envía un mensaje

/***** OUTPUT *****/

void linkOutput(){
	//linkOutLora();
	//linkOutGps();
	linkOutDisplay();
}

/************************
 *****	LORA OUTPUT *****
 ************************
 *
 */
void linkOutLora(){

	//////////////
	// SOFTWARE //
	//////////////

	if ( flagTxLora ){				// Si sale un comando
		loraTimeWd.setOutput();		// Se indica a wd
	}

	//////////////
	// HARDWARE //
	//////////////

	if ( flagWdLora ){			// Si vence wd
		flagResetHwLora	= 1;	// Reinicia chip
	}
}

/**********************
 ***** GPS OUTPUT *****
 **********************
 */
void linkOutGps(){

	if ( startGps ){		// Si se inicia Gps
		enableGps	= 1;	// Habilita gps
	}

	if ( stopGps ){			// Si se detiene gps
		stopGps		= 0;	// Reinicia Flag
		enableGps	= 0;	// Deshabilita gps
	}
}

/**************************
 ***	DISPLAY	OUT		***
 **************************
 *
 *	Interactua con physical layer a traves de displayPhy
 *	La instancia  displayLink contiene los valores en cada digito del display
 *	Incluye el punto
 *
 */
void linkOutDisplay(){

	digit	= displayPhy.next();					// Copia el valor del digito que se desplegara

	digit	= displayLink.numberInDigit( digit );	// Indica valor en Display
	displayPhy.setNumber(digit);					// Inserta numero en capa fisica

	digit	= displayLink.dotPosition();			// Guarda valor del digito con punto
	displayPhy.setDot( digit );						// Inserta punto en digito indicado

}
