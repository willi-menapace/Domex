/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef CONSTANT_CONDITION_INCLUDED
#define CONSTANT_CONDITION_INCLUDED

#include "GlobalDefines.h"

#include "ComparisonOperator.cpp"
#include "Condition.h"

/**
* Rappresenta una condizione che verifica un valore variabile ottenuto
* da un determinato sensore su un device con un valore costante di riferimento
*/
class ConstantCondition: public Condition {
private:
    unsigned char sensorAddress[ADDRESS_BYTES]; //Il device che fornisce i valori da usare per testare al regola
    unsigned char sensorId;                     //Identificativo del sensore interno al device che fornisce i valori per testare la condizione
    long testValue;                             //Il valore costante contro cui testare la condizione
    ComparisonOperator comparisonOperator;      //L'operatore di comparazione da utilizzare per confrontare il valore del sensore con la costante

public:
    ConditionTypes getType();
    /**
    * Testa la condizione ottenendo l'ultima lettura del sensore sul device specificato
    *
    * @param networkResources il contesto di rete
    * @param now data e ora correnti
    * @return true se il valore soddisfa la condizione, false se non la soddisfa, se il
    *         sensore non ha mai effettuato letture o se il sensore non e' presente nella rete
    */
    bool test(NetworkResources *networkResources, DateTime *now); //Implementa il metodo della superclasse
    /**
    * Inizializza la condizione
    *
    * @param _sensorAddress l'indirizzo del device da cui estrarre i valori con cui testare la condizione
    * @param _sensorId l'identificatore del sensore interno al device che fornisce i valori con cui testare la condizione
    * @param _testValue la costante contro cui testare la condizione
    * @param _comparisonOperator l'operatore di comparazione che definisce come testare valore del sensore e costante
    */
    void initialize(unsigned char _sensorAddress[ADDRESS_BYTES], unsigned char _sensorId, long _testValue, ComparisonOperator _comparisonOperator);
    /**
    * Inizializza l'istanza a partire da una scritta su file
    *
    * @param fileReader il file su cui e' scritta l'istanza
    */
    void initialize(FileReader *fileReader);
    void writeToFile(FileWriter *fileWriter);
    ~ConstantCondition();
};

#endif // CONSTANT_CONDITION
