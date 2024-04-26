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

/////////////
// DISPLAY //
/////////////

extern displayLink displayLink;		// Enlace de display
extern displayPhysical displayPhy;	// Capa fisica de display
uint8_t digit;						// Contador de digito


/***** OUTPUT *****/

void linkOutput(){
	//linkOutDisplay();
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
