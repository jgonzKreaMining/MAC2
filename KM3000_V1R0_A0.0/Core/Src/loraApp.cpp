/*
 * loraApp.cpp
 *
 *  Created on: Jan 8, 2024
 *      Author: jgonz
 */

#include "loraApp.h"

//////////////////////
// POSSIBLE ANSWERS //
//////////////////////

uint8_t rxCommand_0[] = "ok\r";
uint8_t rxCommand_1[] = "bussy\r";
uint8_t rxCommand_2[] = "invalid_param\r";
uint8_t rxCommand_3[] = "invalid_data_len\r";
uint8_t rxCommand_4[] = "invalid_class\r";
uint8_t rxCommand_5[] = "keys_not_init\r";
uint8_t rxCommand_6[] = "mac_paused\r";
uint8_t rxCommand_7[] = "fram_counter_err_rejoin_needed\r";
uint8_t rxCommand_8[] = "multicast_keys_not_set\r";
uint8_t rxCommand_9[] = "no_free_ch\r";
uint8_t rxCommand_10[] = "not_joined\r";
uint8_t rxCommand_11[] = "silent\r";
uint8_t rxCommand_12[] = "err\r";

uint8_t rxCommand_13[] 	= "no_free_ch\r";
uint8_t rxCommand_14[]	= "frame_counter_err_rejoin_needed\r";
uint8_t rxCommand_15[]	= "mac_paused\r";
uint8_t rxCommand_16[]	= "mac_tx_ok\r";
uint8_t rxCommand_17[]	= "mac_err\r";

uint8_t rxCommand_18[] = "mac_tx_ok\r";

uint8_t *rxCommand[14] = {
		rxCommand_0,
		rxCommand_1,
		rxCommand_2,
		rxCommand_3,
		rxCommand_4,
		rxCommand_5,
		rxCommand_6,
		rxCommand_7,
		rxCommand_8,
		rxCommand_9,
		rxCommand_10,
		rxCommand_11,
		rxCommand_18,
		rxCommand_12
};

uint8_t *rxCommand_Loop[12] = {
		rxCommand_0,			// 0 - ok
		rxCommand_18,
		rxCommand_1,			// 1 - bussy
		rxCommand_2,			// 2 - invalid_param
		rxCommand_3,			// 3 - invalid_data_len
		rxCommand_10,			// 4 - not_joined
		rxCommand_13,			// 5 - no_free_ch
		rxCommand_12,			// 6 - silent
		rxCommand_14,			// 7 - frame_counter_err_rejoin_needed
		rxCommand_15,			// 8 - mac_paused
		rxCommand_16,			// 9 - mac_tx_ok
		rxCommand_17			// 10 - mac_err
};

/////////////////////
// COMMANDS RN2903 //
/////////////////////

uint8_t sysReset_RN2903[] 		= "sys reset\r\n";

uint8_t radioSetMod_RN2903[]	= "radio set mod lora\r\n";
uint8_t radioSetSf_RN2903[]		= "radio set sf sf10\r\n";
uint8_t radioSetBw_RN2903[]		= "radio set bw 125\r\n";
uint8_t radioSetPwr_RN2903[]	= "radio set pwr 20\r\n";

/*****************
 ***** MAC 1 *****
 *****************
 */

uint8_t macSetDevEui_RN2903[]	= "mac set deveui 0000004B000004B\r\n";
uint8_t macSetDevAddr_RN2903[]	= "mac set devaddr 0000004B\r\n";
uint8_t macSetNwkskey_RN2903[]	= "mac set nwkskey 237FCD5AE5126C04E2F0809004DCC7EC\r\n";
uint8_t macSetAppskey_RN2903[]	= "mac set appskey 237FCD5AE5126C04E2F0809004DCC7EC\r\n";

/*****************
 ***** MAC 2 *****
 *****************
 */

/*
uint8_t macSetDevEui_RN2903[]	= "mac set deveui 0000005200000052\r\n";
uint8_t macSetDevAddr_RN2903[]	= "mac set devaddr 00000052\r\n";
uint8_t macSetNwkskey_RN2903[]	= "mac set nwkskey 237FCD5AE5126C04E2F0809004DCC700\r\n",
uint8_t macSetAppskey_RN2903[]	= "mac set appskey 237FCD5AE5126C04E2F0809004DCC700\r\n";
*/

/*****************
 ***** MAC 3 *****
 *****************
 */

/*
uint8_t macSetDevEui_RN2903[]	= "mac set deveui 0000006500000065\r\n";
uint8_t macSetDevAddr_RN2903[]	= "mac set devaddr 00000065\r\n";
uint8_t macSetNwkskey_RN2903[]	= "mac set nwkskey 237FCD5AE5126C04E2F0809004DCC711\r\n",
uint8_t macSetAppskey_RN2903[]	= "mac set appskey 237FCD5AE5126C04E2F0809004DCC711\r\n";
*/

/*****************
 ***** MAC 4 *****
 *****************
 */

/*
uint8_t macSetDevEui_RN2903[]	= "mac set deveui 0000006100000061\r\n";
uint8_t macSetDevAddr_RN2903[]	= "mac set devaddr 00000061\r\n";
uint8_t macSetNwkskey_RN2903[]	= "mac set nwkskey 237FCD5AE5126C04E2F0809004DCC722\r\n",
uint8_t macSetAppskey_RN2903[]	= "mac set appskey 237FCD5AE5126C04E2F0809004DCC722\r\n";
*/

/*****************
 ***** MAC 5 *****
 *****************
 */

/*
uint8_t macSetDevEui_RN2903[]	= "mac set deveui 0000006D0000006D\r\n";
uint8_t macSetDevAddr_RN2903[]	= "mac set devaddr 0000006D\r\n";
uint8_t macSetNwkskey_RN2903[]	= "mac set nwkskey 237FCD5AE5126C04E2F0809004DCC733\r\n",
uint8_t macSetAppskey_RN2903[]	= "mac set appskey 237FCD5AE5126C04E2F0809004DCC733\r\n";
*/

uint8_t macJoinOtaa_RN2903[]	= "mac join otaa\r\n";
uint8_t macJoinAbp_RN2903[]		= "mac join abp\r\n";
uint8_t macSetSync_RN2903[]		= "mac set sync 34\r\n";
uint8_t macSave_RN2903[]		= "mac save\r\n";

uint8_t macTxData_RN2903[]		= "mac tx uncnf 1 AABBCCDD\r\n";
uint8_t macTxGps_RN2903[]		= "mac tx uncnf 2 AABBCCEE\r\n";
uint8_t macTxSys_RN2903[]		= "mac tx uncnf 3 AABB\r\n";

uint8_t macSetCh8_RN2903[]		= "mac set ch status 8 off\r\n";
uint8_t macSetCh9_RN2903[]		= "mac set ch status 9 off\r\n";
uint8_t macSetCh10_RN2903[]		= "mac set ch status 10 off\r\n";

///////////////////////
// COMMAND STRUCTURE //
///////////////////////

rn2903Command sysReset;

rn2903Command radioSetMod;
rn2903Command radioSetSf;
rn2903Command radioSetBw;
rn2903Command radioSetPwr;

rn2903Command macSetDeveui;
rn2903Command macSetDevAddr;
rn2903Command macSetNwkskey;
rn2903Command macSetAppskey;
rn2903Command macJoinOtaa;
rn2903Command macJoinAbp;
rn2903Command macSetSync;
rn2903Command macSave;

rn2903Command macTxData;
rn2903Command macTxGps;
rn2903Command macTxSys;

rn2903Command macSetCh8;
rn2903Command macSetCh9;
rn2903Command macSetCh10;

void setupLoraApp(){
	///////////////
	// SYS RESET //
	///////////////

	sysReset.command		= sysReset_RN2903;
	sysReset.size			= sizeof( sysReset_RN2903 );
	sysReset.responseSize	= 1;

	/***** RADIO *****/


	///////////////////
	// RADIO SET MOD //
	///////////////////

	radioSetMod.command		= radioSetMod_RN2903;
	radioSetMod.size		= sizeof( radioSetMod_RN2903 ) - 1;
	radioSetMod.responseSize= 3;

	//////////////////
	// RADIO SET SF //
	//////////////////

	radioSetSf.command		= radioSetSf_RN2903;
	radioSetSf.size			= sizeof( radioSetSf_RN2903 ) - 1;
	radioSetSf.responseSize	= 3;

	//////////////////
	// RADIO SET BW //
	//////////////////

	radioSetBw.command		= radioSetBw_RN2903;
	radioSetBw.size			= sizeof( radioSetBw_RN2903 ) - 1;
	radioSetBw.responseSize = 3;

	///////////////////
	// RADIO SET PWR //
	///////////////////

	radioSetPwr.command		= radioSetPwr_RN2903;
	radioSetPwr.size		= sizeof( radioSetPwr_RN2903 ) - 1;
	radioSetPwr.responseSize= 3;


	/***** MAC *****/

	////////////////////
	// MAC SET DEVEUI //
	////////////////////

	macSetDeveui.command	= macSetDevEui_RN2903;
	macSetDeveui.size		= sizeof( macSetDevEui_RN2903 ) - 1;
	macSetDeveui.responseSize	= 3;

	/////////////////////////
	// MAC SET NETWORK KEY //
	/////////////////////////

	macSetNwkskey.command	= macSetNwkskey_RN2903;
	macSetNwkskey.size		= sizeof( macSetNwkskey_RN2903 ) - 1;
	macSetNwkskey.responseSize	= 3;

	//////////////////////
	// MAC SET APPS KEY //
	//////////////////////

	macSetAppskey.command	= macSetAppskey_RN2903;
	macSetAppskey.size		= sizeof( macSetAppskey_RN2903 ) - 1;
	macSetAppskey.responseSize	= 3;

	/////////////////////////
	// MAC SET DEV ADDRESS //
	/////////////////////////

	macSetDevAddr.command		= macSetDevAddr_RN2903;
	macSetDevAddr.size			= sizeof ( macSetDevAddr_RN2903 ) - 1;
	macSetDevAddr.responseSize	= 2;

	///////////////////
	// MAC JOIN OTAA //
	///////////////////

	macJoinOtaa.command			= macJoinOtaa_RN2903;
	macJoinOtaa.size			= sizeof ( macJoinOtaa_RN2903 ) - 1;
	macJoinOtaa.responseSize	= 7;

	//////////////////
	// MAC JOIN ABP //
	//////////////////

	macJoinAbp.command		= macJoinAbp_RN2903;
	macJoinAbp.size			= sizeof( macJoinAbp_RN2903 ) - 1;
	macJoinAbp.responseSize	= 2;

	//////////////////
	// MAC SET SYNC //
	//////////////////

	macSetSync.command		= macSetSync_RN2903;
	macSetSync.size			= sizeof( macSetSync_RN2903 ) - 1;
	macSetSync.responseSize	= 3;

	//////////////
	// MAC SAVE //
	//////////////

	macSave.command			= macSave_RN2903;
	macSave.size			= sizeof( macSave_RN2903 ) - 1;
	macSave.responseSize	= 3;

	/////////////////
	// MAC TX DATA //
	/////////////////

	 macTxData.command		= macTxData_RN2903;
	 macTxData.size			= sizeof( macTxData_RN2903 ) - 1;
	 macTxData.responseSize	= 14;

	 /////////////////
	 // MAC TX DATA //
	 /////////////////

	 macTxGps.command		= macTxGps_RN2903;
	 macTxGps.size			= sizeof( macTxGps_RN2903 ) - 1;
	 macTxGps.responseSize	= 14;


	 /////////////////
	 // MAC TX DATA //
	 /////////////////

	  macTxSys.command		= macTxSys_RN2903;
	  macTxSys.size			= sizeof( macTxSys_RN2903 ) - 1;
	  macTxSys.responseSize	= 14;

	  //////////////////
	  // MAC SET CH 8 //
	  //////////////////

	  macSetCh8.command		= macSetCh8_RN2903;
	  macSetCh8.size		= sizeof( macSetCh8_RN2903 ) - 1;
	  macSetCh8.responseSize	= 9;

	  //////////////////
	  // MAC SET CH 9 //
	  //////////////////

	  macSetCh9.command		= macSetCh9_RN2903;
	  macSetCh9.size		= sizeof( macSetCh9_RN2903 ) - 1;
	  macSetCh9.responseSize	= 9;

	  ///////////////////
	  // MAC SET CH 10 //
	  ///////////////////
	  macSetCh10.command		= macSetCh10_RN2903;
	  macSetCh10.size			= sizeof( macSetCh10_RN2903 ) - 1;
	  macSetCh10.responseSize	= 9;

}

/////////////////
// ADD COMMAND //
/////////////////

void loraRutine::addCommand ( rn2903Command *command ){
	this->listCommands[ this->sizeList ]	= command;	// AÑADE PUNTERO A LISTA
	this->sizeList++;									// Suma 1 al largo de lista
}


/////////////
// COMMAND //
/////////////

rn2903Command *loraRutine::command(){
	uint8_t count	= this->actual;			// Copia elemento actual de rutina

	this->actual++;							// Suma 1 al elemento actuañ

	if (this->actual >= this->sizeList){	// Si es el ultimo de la lista
		this->actual	= 0;				// Reinicia la lista
		this->stopped	= 1;				// Indica que es el ultimo elemento
	}
	return this->listCommands[count];		// Retorta puntero
}

///////////
// RESET //
///////////

void loraRutine::reset(){
	this->actual	= 0;	// Reinicia contador de lista
	this->stopped	= 0;	// No esta en el ultimo elemento
}

////////////
// FINISH //
////////////
bool loraRutine::finish(){
	return this->stopped;	// Indica si es el ultimo elemento

}
