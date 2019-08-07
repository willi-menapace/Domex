/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "GlobalDefines.h"

#ifdef OFFBOARD

#include <stdlib.h>
#include <iostream>

/**
* Effettua un test del funzionamento di XBeeATCommand
*
* @param parameters nome del file contenente i parametri di inizializzazione del test strutturato esattamente come il costruttore
* @param verification nome delfile contenente il pacchetto corretto che deve essere inviato al modulo XBee
*/
void xBeeATCommandTest(std::string parameters, std::string verification);

/**
* Effettua un test del funzionamento di XBeeTransmitRequest
*
* @param parameters nome del file contenente i parametri di inizializzazione del test strutturato esattamente come il costruttore
* @param verification nome delfile contenente il pacchetto corretto che deve essere inviato al modulo XBee
*/
void xBeeTransmitRequestTest(std::string parameters, std::string verification);

/**
* Effettua un test del funzionamento di XBeeRemoteATCommand
*
* @param parameters nome del file contenente i parametri di inizializzazione del test strutturato esattamente come il costruttore
* @param verification nome delfile contenente il pacchetto corretto che deve essere inviato al modulo XBee
*/
void xBeeRemoteATCommandTest(std::string parameters, std::string verification);

#endif //OFFBOARD
