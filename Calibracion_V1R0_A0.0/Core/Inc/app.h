/*
 * app.h
 *
 *  Created on: Mar 1, 2024
 *      Author: jgonz
 */


#ifndef INC_APP_H_
#define INC_APP_H_

#include <cstdint>

void app();								// Funcion general
void appContent();						// Funcion que procesa contenido del mensaje
void appLora();							// Funcion que envia el contenido del mensaje
uint8_t uint2hex_A( uint8_t number );	// Funcion que hashea primeros 4 bits a hex
uint8_t uint2hex_B( uint8_t number );	// Funcion que hashea segundos 4 bits a hex


#endif /* INC_LINK_H_ */
