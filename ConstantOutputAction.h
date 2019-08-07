/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef CONSTANT_OUTPUT_ACTION_INCLUDED
#define CONSTANT_OUTPUT_ACTION_INCLUDED

#include "GlobalDefines.h"

#include "Action.h"

/**
* Azione che quando eseguita invia un valore costante
* ad un determinato dispositivo di output situato su un device
*/
class ConstantOutputAction : public Action {
private:
    unsigned char destinationAddress[ADDRESS_BYTES];        //L'indirizzo del device a cui inviare il messaggio
    unsigned char outputId;                                 //Il dispositivo di output interno al destinatario da attivare
    long outputValue;                                       //Il valore da inviare al dispoditivo di output sul destinatario

public:
    ActionTypes getType();
    void execute(NetworkResources *networkResources, XBee *xBee); //Implementa il metodo della superclasse
    /**
    * Inizializza l'istanza
    *
    * @param _destinationAddress l'indirizzo del device a cui inviare il messaggio
    * @param _outputId il dispositivo di output interno al destinatario da attivare
    * @param _outputValue il valore da inviare al dispositivo di output sul destinatario
    */
    void initialize(unsigned char _destinationAddress[ADDRESS_BYTES], unsigned char _outputId, long _outputValue);
    /**
    * Inizializza l'istanza a partire da una scritta su file
    *
    * @param fileReader il file su cui e' scritta l'istanza
    */
    void initialize(FileReader *fileReader);
    void writeToFile(FileWriter *fileWriter);
    ~ConstantOutputAction();
};

#endif // CONSTANT_OUTPUT_ACTION_INCLUDED
