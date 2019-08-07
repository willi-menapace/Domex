/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "NetworkDeviceStatus.h"

#include "GlobalDefines.h"

#include <assert.h>

//Il numero di volte che i secondi di inattivita' devono superare i
//secondi massimi di silenzio dipici del device prima che questo venga
//considerato perso
#define LOST_TRESHOLD 3

void NetworkDeviceStatus::initialize(unsigned char address[ADDRESS_BYTES], unsigned char sensorsCount, SensorType sensorTypes[],
                                     unsigned char outputsCount, OutputType outputTypes[], unsigned long maxSilencePeriod, DateTime *_now) {
    networkDevice.initialize(address, sensorsCount, sensorTypes, outputsCount, outputTypes, maxSilencePeriod);
    lastContact.initialize(_now); //Inizializza l'ultimo contatto alla data di adesso
}

void NetworkDeviceStatus::initialize(FileReader *fileReader) {
    networkDevice.initialize(fileReader);
    lastContact.initialize(fileReader);
}

NetworkDevice *NetworkDeviceStatus::getNetworkDevice() {
    return &networkDevice;
}

ComputedDeviceStatus NetworkDeviceStatus::getComputedStatus(DateTime *now) {

    assert(now != 0);
    assert(now->getTimeOffset() >= lastContact.getTimeOffset());

    unsigned long inactivitySeconds = now->getTimeOffset() - lastContact.getTimeOffset();
    unsigned long maxSilencePeriod = networkDevice.getMaxSilencePeriod();

    //Calcola lo stato del device in base al periodo di silenzio
    if(inactivitySeconds <= maxSilencePeriod) {
        return UP_AND_RUNNING;
    } else if(inactivitySeconds <= maxSilencePeriod * LOST_TRESHOLD) {
        return TEMPORARILY_NOT_RESPONDING;
    } else {
        return LOST;
    }
}

void NetworkDeviceStatus::writeToFile(FileWriter *fileWriter) {
    networkDevice.writeToFile(fileWriter);
    lastContact.writeToFile(fileWriter);
}

bool NetworkDeviceStatus::equals(NetworkDeviceStatus *otherStatus) {
    if(otherStatus == 0) {
        return false;
    }

    //Un'istanza e' uguale a se stessa
    if(this == otherStatus) {
        return true;
    }

    //Controlla che l'ultimo contatto sia avvenuto nello stesso momento
    if(lastContact.equals(&(otherStatus->lastContact)) == false) {
       return false;
    }

    //Controlla che lo stato si riferisca agli stessi device
    if(networkDevice.equals(&(otherStatus->networkDevice)) == false) {
        return false;
    }

    return true;
}
