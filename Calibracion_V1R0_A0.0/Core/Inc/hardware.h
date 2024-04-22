/*
 * hardware.h
 *
 *  Created on: Jan 11, 2024
 *      Author: jgonz
 */

#ifndef INC_HARDWARE_H_
#define INC_HARDWARE_H_

#include <7segments.h>
#include "EEPROM.h"

/***** INPUT *****/

void hwInput();		// Adquisición de datos de entrada
void hwGpsIn();		// Input UART GPS	TESEO VIC3D
void hwLoraIn();	// Input UART LoRa	RN2903
void hwBoton();		// Input Boton
void hwAnalog();	// Input ADC
void hwSht31();		// Input I2C
void hwAds1115();	// Input ADC externo
void hwEEPROM();	// Input EEPROM I2C


/***** OUTPUT *****/

void hwOutput();	// Salida de señales por Hw
void hwDisplay();	// Output Display
void hwAlarms();	// Output alarmas	- Relays
void hwGpsOut();	// Output UART GPS
void hwLoraOut();	// Output LORAWAN

#endif /* INC_HARDWARE_H_ */
