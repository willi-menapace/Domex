#include "Moisture.h"
#include <Arduino.h>
#include <math.h>

// Costruttore
Moisture::Moisture(int sensorAnalogPin) {
    sensorPin = sensorAnalogPin;
	offset = 0;
	backup = false;
}

// OVERLOAD: Costruttore con impostazione di controllo sensore a doppia lettura
Moisture::Moisture(int sensorAnalogPin, int backupAnalogPin) {
    sensorPin = sensorAnalogPin;
	backupPin = backupAnalogPin;
	offset = 0;
	backup = true;
}

// Verifica che il sensore sia collegato correttamente
bool Moisture::check() {
	if(backup) {
		int sensorValue = analogRead(sensorPin);
		int backupValue = analogRead(backupPin);
		if(fabs(sensorValue - backupValue) < 2) return true;
		else return false;
	}
	else {
		const int READINGS = 10; //Numero di letture da eseguire
		int values[READINGS],    //Array di valori letti
			deltaValues = 0;     //Differenza tra un valore e un altro
		values[0] = analogRead(sensorPin);

		for(int i = 1; i < READINGS; i++) {
			values[i] = analogRead(sensorPin);
			deltaValues += values[i - 1] - values[i];
			delay(200);
		}

		if(deltaValues > (READINGS / 2)) return false;
		else return true;
	}
    
}

// Restituisce il valore analogico letto dal sensore
int Moisture::getRaw() {
    return analogRead(sensorPin);
}

// Restituisce lo stato di umidità
GroundHumidity Moisture::getValue() {
    int humidity = analogRead(sensorPin) + offset;
    const int MAX = 1023 + offset,
              MIN = 0    + offset;

    if     (humidity >= MIN         && humidity < DRY)         return COMPLETELY_DRY;
    else if(humidity >= DRY         && humidity < QUITE_HUMID) return DRY;
    else if(humidity >= QUITE_HUMID && humidity < HUMID)       return QUITE_HUMID;
    else if(humidity >= HUMID       && humidity < QUITE_WET)   return HUMID;
    else if(humidity >= QUITE_WET   && humidity < WET)         return QUITE_WET;
    else if(humidity >= WET         && humidity < MAX)         return WET;
    else return ERROR;
}

// OVERLOAD: Restituisce lo stato di umidità facendo una media di valori
GroundHumidity Moisture::getValue(int readings) {
    int humidity = 0;
    const int MAX = 1023 + offset,
              MIN = 0    + offset;

    for(int i = 0; i = readings; i++)
        humidity += analogRead(sensorPin);
    humidity = (humidity / readings) + offset;

    if     (humidity >= MIN         && humidity < DRY)         return COMPLETELY_DRY;
    else if(humidity >= DRY         && humidity < QUITE_HUMID) return DRY;
    else if(humidity >= QUITE_HUMID && humidity < HUMID)       return QUITE_HUMID;
    else if(humidity >= HUMID       && humidity < QUITE_WET)   return HUMID;
    else if(humidity >= QUITE_WET   && humidity < WET)         return QUITE_WET;
    else if(humidity >= WET         && humidity < MAX)         return WET;
    else return ERROR;
}
// Calibrazione sensore
void Moisture::setOffset(int calibOffset){
	offset = calibOffset;
}
