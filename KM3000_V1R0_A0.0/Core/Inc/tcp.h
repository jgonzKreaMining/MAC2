/*
 * tcp.h
 *
 *  Created on: Jan 11, 2024
 *      Author: jgonz
 */

#ifndef INC_TCP_H_
#define INC_TCP_H_

#include <loraLink.h>
#include <loraTcp.h>


///////////
// INPUT //
///////////

void tcpInput();
void tcpInLora();
void tcpGps();
void tcpData();
void tcpSystem();

////////////
// OUTPUT //
////////////

void tcpOutput();
void tcpOutLora();

#endif /* INC_TCP_H_ */
