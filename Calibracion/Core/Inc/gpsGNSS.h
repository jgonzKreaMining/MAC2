/*
 * gps_Ublox_6M.h
 *
 *  Created on: Oct 24, 2023
 *      Author: jgonz
 */
#include <cstdint>
#include <string>

#ifndef INC_GPSGNSS_H_
#define INC_GPSGNSS_H_

///////////////
// GPS INPUT //
///////////////

/*
 * En esta clase se realiza el input de las variables obtenidas por hardware
 * Entrega el contenido deglosado del GPS y su comportamiento general
 * Esta es parte de un sistema de GPS.
 *
 */

class gpsInput{

	////////////////////////
	// VARIABLES PRIVADAS //
	////////////////////////

	private:
		float height;		// Altura el flotante
		uint8_t mode;		// Modo del GPS

		int lapsBetween;	// Vueltas entre ultimos dos mensajes
		int lapsCounter;	// Contador de vueltas
		char flagValid;		// Indica si el mensaje es valido
		bool flagLaps;		// Indica timeout entre mensajes
		bool flagNew;		// Indica si hay un nuevo mensaje

		//////////////////////////
		// VARIABLES DE METODOS //
		//////////////////////////


		uint8_t limitMsgModes;	// Limite de mensajes para modos
		char time[10];			// Hora del mensaje GPRMC

		char longDirect;		// Direccion de longitud GPRMC

		char latDirect;			// Direccion de latitud GPRMC
		char velocity[10];		// Velocidad GPRMC
		char angle[10];			// Angulo GPRMC
		char date[6];			// Fecha GPRMC
		uint8_t countGprmc;		// Contador para acumular simbolos en insertValue()

		bool flagSource;		// Indica valor del pin de HW

		void setMode(uint8_t mode);			// Fija modo
	////////////////////////
	// VARIABLES PUBLICAS //
	////////////////////////

	public:
		uint8_t countModes[6];		// Contadores de modos
		char latitud[10];		// Latitud del mensaje GPRMC
		char longitud[10];		// Longitud del mensaje GPRCM
		uint8_t stateInsert;	// Estado de inserción de datos

		gpsInput();					// Constructor del GPS

		int getLatitud();			// Obtiene latitud
		int getLatitud2();			// Obtiene latitud
		char getLatDirect();		// Obtiene direccion latitud

		int getLongitud();			// Obtiene longitud
		int getLongitud2();			// Obtiene longitud
		char getLongDirect();		// Obtener direccion longitud

		float getHeight();			// Obtiene altura
		uint8_t getMode();			// Obtiene modo de operacion del GPS
		int getBetween();			// Obtiene milisegundos entre mensajes
		float getTime();			// Obtiene hora GMT
		long int getDate();			// Obtiene fecha GMT
		bool getValid();			// Obtiene flagValid
		bool getTimeout();			// Obtiene flagBetween
		bool getNewMessage();		// Obtiene flagNew

		////////////////////////////////////
		// METODOS ORIENTADOS AL MENSAJE  //
		////////////////////////////////////

		void insertValue(uint8_t symbol);	// Inserta valor en cadena
		void setValid(char valid);			// Fija valor valido
		uint8_t state_Insert();

		////////////////////////////////////
		// METODOS ORIENTADOS AL HARDWARE //
		////////////////////////////////////

		void setSource(bool src);	// Fija valor de pin en hardware
		bool getSource();			// Obtiene valor de pin en Hardware
};

#endif /* INC_GPS_UBLOX_6M_H_ */
