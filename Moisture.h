#ifndef MOISTURE_H
#define MOISTURE_H
#include "GroundHumidity.cpp"

/*
* Rappresenta un sensore di umidità del terreno
* E' stato implementato anche un controllo dei collegamenti del sensore, sia con pin di backup (affidabile) sia con calcolo (sconsigliato e lento)
*/

class Moisture {
    public:
		// Costruttore dell'oggetto
        Moisture(int sensorAnalogPin);
		// Overload del costruttore con pin di backup
        Moisture(int sensorAnalogPin, int backupAnalogPin);
		// Controllo dei collegamenti del sensore tramite confronto pins o calcolo
        bool check();
		// Ritorno del valore RAW del pin analogico
        int getRaw();
		// Ritorno del valore calibrato e interpretato
        GroundHumidity getValue();
		// Overload della funzione di ritorno del valore calibrato e interpretato; effettua una media su un numero definito di valori
        GroundHumidity getValue(int readings);
		// Imposta un valore di offset per la calibrazione del sensore
		void setOffset(int calibOffset);

    private:
        int sensorPin;
		int backupPin;
		bool backup;
        int offset;
};

#endif // MOISTURE_H
