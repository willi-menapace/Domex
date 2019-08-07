/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo <simone@lorengo.org> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Simone Lorengo <simone@lorengo.org>
 */

#ifndef RELAY_OUTPUT_INCLUDED
#define RELAY_OUTPUT_INCLUDED

#include "GlobalDefines.h"

#ifdef ONBOARD

#include "Output.h"

/**
* Output corrispondente a un attuatore a relè
*/

class RelayOutput : public Output {

private:
	OutputType type;
	long lastOutputValue;
	unsigned char relayPin;

public:

	OutputType getType();

	/**
	* Imposta un valore di output sull'output corrente (spento 0, acceso 1)
	*/
	void performOutput(long value);

	/**
	* @return l'ultimo valore inviato in output sull'output corrente
	*/
	long getLastOutputValue();

	/**
	* Verifica ed eventualmente corregge il valore dell'output reale basandosi sull'ultimo impostato
	* @return valore corrente corretto. Se FALSE, esegue automaticamente la correzione del valor
	*/
	bool check();

	/**
	* Costruisce un output di tipo RELAY
	*
	* @param relayDigitalPin Pin corrispondente all'output
	*/
	RelayOutput(int relayDigitalPin);
};

#endif //ONBOARD

#endif // RELAY_OUTPUT_INCLUDED
