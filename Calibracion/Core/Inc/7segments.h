/*
 * 7segments.h
 *
 *  Created on: Jan 9, 2024
 *      Author: jgonz
 */

#include <stdint.h>

#ifndef INC_7SEGMENTS_H_
#define INC_7SEGMENTS_H_

/******************
 ***	LINK	***
 ******************
 *
 *	Ingreso de numero en display, separa el numero en unidad, decena, centena y mil
 *	Guarda posicion de Dot
 *
 *	Guia de usuario:
 *		1.-	inputDisplay:	Ingreso de numero en display y posicion de punto
 *		2.- dotPosition:	Retorna posicion del boton
 *		3.- numberInDigit:	Retorna numero en digito especifico
 */

class displayLink{
private:


public:
	uint8_t numberInDigit( uint8_t digit );			//	Se inserta numero del digito y retorna valor en el digito
	void inputDisplay( int number, uint8_t dot );	//	Se inserta el valor completo que se muestra en display
	uint8_t dotPosition();							//	Retorna la posicion del punto

	uint8_t dot;		//	Digito en que se fija el punto
	uint8_t milesima;	//	Milesima del numero insertado
	uint8_t centena;	//	Centena del numero insertado
	uint8_t decena;		//	Decena del numero insertado
	uint8_t	unidad;		//	Unidad del numero insertado
};

/**********************
 ***	PHYSICAL	***
 **********************
 *
 *	Asigna valores de pines en hardware segun numero en display
 *
 *	Guia de usuario:
 *		1.-	enable:		Habilita display 1: Habilita 0: Deshabilita
 *		2.-	setDot:		Indica posicion del punto
 *		3.- setNumber:	Fija valor en digito
 *		4.-	next:		Indica digito que sigue en el despliegue
 *						Se actualiza cada vez que se usa setNumber
 *
 */

class displayPhysical{
private:
	bool flagEnable;	//	Indica si esta habilitado

	uint8_t dot;		//	Posicion del punto
	uint8_t counter;	//	Contador de digito

	bool oneSignal;		//	Señal de primero digito
	bool twoSignal;		//	Señal de segundo digito
	bool threeSignal;	//	Señal de tercer digito
	bool fourSignal;	//	Señal de cuarto digito

	bool aSignal;		//	Señal de A
	bool bSignal;		//	Señal de B
	bool cSignal;		//	Señal de C
	bool dSignal;		//	Señal de D
	bool eSignal;		//	Señal de E
	bool fSignal;		//	Señal de F
	bool gSignal;		//	Señal de G
	bool dpSignal;		//	Señal de DP

public:
	void enable( bool onOff );			//	Habilita o deshabilita display
	void setDot( uint8_t dot );			//	Inserta posicion del punto
	void setNumber( uint8_t number );	//	Inserta numero en digito

	uint8_t next();						//	Indica cual es el siguiente digito

	bool firstNumber();					//	Retorna señal de primer digito
	bool secondNumber();				//	Retorna señal de segundo digito
	bool thirdNumber();					//	Retorna señal de tercer digito
	bool fourthNumber();				//	Retorna señal de cuarto digito

	bool aLed();						//	Retorna señal de led A
	bool bLed();						//	Retorna señal de led B
	bool cLed();						//	Retorna señal de led C
	bool dLed();						//	Retorna señal de led D
	bool eLed();						//	Retorna señal de led E
	bool fLed();						//	Retorna señal de led F
	bool gLed();						//	Retorna señal de led G
	bool dpLed();						//	Retorna señal de led DP
};


#endif /* INC_7SEGMENTS_H_ */
