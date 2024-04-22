/*
 * gpsInput.h
 *
 *  Created on: Oct 24, 2023
 *      Author: jgonz
 */
#include <cstdint>

#ifndef INC_GPSINPUT_H_
#define INC_GPSINPUT_H_
#endif /* INC_GPSINPUT_H_ */


///////////////
// GPS INPUT //
///////////////

/*
 * En esta clase se realiza el input de las variables obtenidas por hardware
 * Entrega el contenido deglosado del GPS y su comportamiento general
 * Esta es parte de un sistema de GPS.
 */

class gpsInput{

	////////////////////////
	// VARIABLES PRIVADAS //
	////////////////////////

	private:
		float latitud;		// Latitud en flotante
		float longitud;		// Longitud en flotante
		float height;		// Altura el flotante
		uint8_t mode;		// Modo del GPS
		int lapsBetween;	// Vueltas entre ultimos dos mensajes
		bool flagValid;		// Indica si el mensaje es valido
		bool flagLaps;		// Indica timeout entre mensajes
		bool flagNew;		// Indica si hay un nuevo mensaje

		//////////////////////////
		// VARIABLES DE METODOS //
		//////////////////////////

		uint8_t stateInsert;	// Estado de inserción de datos

	////////////////////////
	// VARIABLES PUBLICAS //
	////////////////////////

	public:
		gpsInput();				// Constructor del GPS
		float getLatitud();		// Obtiene latitud
		float getLongitud();	// Obtiene longitud
		float getHeight();		// Obtiene altura
		int getMode();			// Obtiene modo de operacion del GPS
		int getBetween();		// Obtiene milisegundos entre mensajes
		bool getValid();		// Obtiene flagValid
		bool getTimeout();		// Obtiene flagBetween
		bool getNewMessage();	// Obtiene flagNew

		////////////////////////////////////
		// METODOS ORIENTADOS AL MENSAJE  //
		////////////////////////////////////

		void insertValue(uint8_t symbol);	// Inserta valor en cadena
		void setLatitud(float lat);			// Fija latitud
		void setLongitud(float longit);		// Fija longitud
		void setHeight(float alt);			// Fija altura
		void setMode(uint8_t mode);			// Fija modo
		void setValid(bool valid);			// Fija valor valido

		//////////////////////////////////
		// METODOS ORIENTADOS AL TIEMPO //
		//////////////////////////////////
};
