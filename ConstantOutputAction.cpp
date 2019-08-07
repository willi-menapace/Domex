/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "ConstantOutputAction.h"

#include "GlobalDefines.h"

#include "ActionTypes.cpp"
#include "INetOutputRequest.h"
#include "XBeeTransmitRequest.h"

#include <assert.h>

#ifdef OFFBOARD
#include <iostream>
#endif // OFFBOARD

ActionTypes ConstantOutputAction::getType() {
    return CONSTANT_OUTPUT_ACTION;
}

void ConstantOutputAction::execute(NetworkResources *networkResources, XBee *xBee) {

    assert(networkResources != 0);

    //Per funzionalita' di test il modulo non viene passato
    #ifndef TEST
    assert(xBee != 0);
    #endif // TEST

    #ifdef OFFBOARD
    #ifdef TEST
    std::cout << "Constant output executed: " << this->outputValue << " on output ID " << this->outputId << "\n";
    #endif //TEST
    #endif //OFFBOARD

    #ifdef ONBOARD
    #ifdef DEBUG
    Serial.print("Constant output requested: ");
    Serial.print(this->outputValue);
    Serial.print(" on output ID ");
    Serial.print(this->outputId);
    Serial.print("\n");
    #endif //DEBUG
    #endif //ONBOARD

    //Se il device non e' presente sulla rete non e' necessario effettuare l'output
    NetworkDevice *device = networkResources->getDeviceByAddress(destinationAddress);
    if(device == 0) {
        #ifdef ONBOARD
        #ifdef DEBUG
        Serial.println("Constant output not executed: destination device unknown");
        #endif //DEBUG
        #endif //ONBOARD
        return;
    }

    //Calcola il messaggio da inviare
	unsigned char destinationNetAddress[NET_ADDRESS_BYTES] = {0xFF, 0xFE}; //Indirizzo di rete sconosciuto
    unsigned char outputRequestBuffer[15]; //Buffer in cui costruire il messaggio. Le dimensioni sono maggiori di quelle richieste da INetOupputRequest
    INetOutputRequest iNetOutputRequest(outputRequestBuffer, 1, &outputId, &outputValue);
	XBeeTransmitRequest xBeeTransmitRequest(iNetOutputRequest.getLength(), iNetOutputRequest.getData(), destinationAddress, destinationNetAddress);

    //Invia il messaggio. In caso di test potrebbe non essere possibile l'invio
    if(xBee != 0) {
		#ifdef ONBOARD
		#ifdef DEBUG
		Serial.println("Constant output executed");
		#endif //DEBUG
		#endif //ONBOARD
        xBee->send(&xBeeTransmitRequest);
    }

}

void ConstantOutputAction::initialize(unsigned char _destinationAddress[ADDRESS_BYTES], unsigned char _outputId, long _outputValue) {

    assert(_destinationAddress != 0);

    for(int i = 0; i < ADDRESS_BYTES; ++i) {
        destinationAddress[i] = _destinationAddress[i];
    }
    outputId = _outputId;
    outputValue = _outputValue;
}

void ConstantOutputAction::initialize(FileReader *fileReader) {
    assert(fileReader != 0);
    assert(fileReader->isOpen());

    for(int i = 0; i < ADDRESS_BYTES; ++i) {
        destinationAddress[i] = fileReader->readByte();
    }
    outputId = fileReader->readByte();
    outputValue = fileReader->readLong();
}

void ConstantOutputAction::writeToFile(FileWriter *fileWriter) {

    assert(fileWriter != 0);
    assert(fileWriter->isOpen());

    for(int i = 0; i < ADDRESS_BYTES; ++i) {
        fileWriter->writeByte(destinationAddress[i]);
    }
    fileWriter->writeByte(outputId);
    fileWriter->writeLong(outputValue);
}

ConstantOutputAction::~ConstantOutputAction() {
}
