/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo <simone@lorengo.org> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Simone Lorengo <simone@lorengo.org>
 */

#include "RelayOutput.h"

#include "GlobalDefines.h"

#ifdef ONBOARD

#include <Arduino.h>

OutputType RelayOutput::getType() {
	return type;
}

void RelayOutput::performOutput(long value) {
	if (value >= 1) {
		digitalWrite(relayPin, HIGH);
		lastOutputValue = 1;
	}
	else {
		digitalWrite(relayPin, LOW);
		lastOutputValue = 0;
	}
}

long RelayOutput::getLastOutputValue() {
	return lastOutputValue;
}

// Verifica ed eventualmente corregge lo stato del relay, utile nel caso in cui lo stato del pin digitale fosse stato erroneamente variato fuori dalla classe
// Restituisce TRUE se il valore teorico corrisponde a quello reale, restituisce FALSE se il valore teorico è diverso da quello reale e lo stato viene corretto.
bool RelayOutput::check() {
	if (lastOutputValue == digitalRead(relayPin)){
		return true;
	}
	else {
		performOutput(lastOutputValue);
		return false;
	}
}

RelayOutput::RelayOutput(int relayDigitalPin) {
	type = RELAY;

	relayPin = relayDigitalPin;
	pinMode(relayPin, OUTPUT);
}

#endif //ONBOARD
