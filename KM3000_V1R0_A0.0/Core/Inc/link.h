/*
 * link.h
 *
 *  Created on: Jan 11, 2024
 *      Author: jgonz
 */

#ifndef INC_LINK_H_
#define INC_LINK_H_

#include <7segments.h>
#include <gpsGNSS.h>
#include <hardware.h>
#include <loraLink.h>

///////////
// INPUT //
///////////

void linkInput();
void linkInLora();
void linkInGps();
void linkBoton();
void linkAnalog();
void linkBattery();
uint16_t adc2PPM( uint16_t signal, uint8_t mode );

////////////
// OUTPUT //
////////////

void linkOutput();
void linkOutGps();
void linkOutDisplay();
void linkOutLora();

#endif /* INC_LINK_H_ */
