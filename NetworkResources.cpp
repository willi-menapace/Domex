/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "NetworkResources.h"

#include "GlobalDefines.h"

#ifdef OFFBOARD
#include <assert.h>
#endif // OFFBOARD

NetworkDeviceStatus *NetworkResources::getDeviceStatusByAddress(unsigned char address[ADDRESS_BYTES]) {

    assert(address != 0);

    InstanceIterator<NetworkDeviceStatus> *devicesIterator = networkDevices.getInstanceIterator();
    NetworkDevice *currentDevice = 0;
    NetworkDeviceStatus *currentStatus = 0;
    unsigned char *currentAddress;
    while(devicesIterator->hasNext()) {
        currentStatus = devicesIterator->next();
        currentDevice = currentStatus->getNetworkDevice();
        currentAddress = currentDevice->getAddress();
        for(int i = 0; i < ADDRESS_BYTES; ++i) {
            if(currentAddress[i] != address[i]) {
                break;
            }
            //Se l'indirizzo non e' ancora stato scartato dopo
            //aver esaminato l'ultimo byte allora e' uguale a quello cercato
            if(i == ADDRESS_BYTES - 1) {
                return currentStatus;
            }
        }
    }
    //Se non e' ancora stato ritornato nulla allora lo status cercato non e' presente
    return 0;
}

void NetworkResources::addDevice(unsigned char address[ADDRESS_BYTES],
                                 unsigned char sensorsCount,
                                 SensorType sensorTypes[],
                                 unsigned char outputsCount,
                                 OutputType outputTypes[],
                                 unsigned long maxSilencePeriod,
                                 DateTime *now) {

    NetworkDeviceStatus *newDevice = networkDevices.allocate();
    newDevice->initialize(address, sensorsCount, sensorTypes, outputsCount, outputTypes, maxSilencePeriod, now);

}

unsigned char NetworkResources::removeDeadDevices(DateTime *now) {

    assert(now != 0);
    unsigned char removedDevices = 0;

    InstanceIterator<NetworkDeviceStatus> *iterator = networkDevices.getInstanceIterator();
    //Scorre tutti i device presenti nella rete
    while(iterator->hasNext()) {
        NetworkDeviceStatus *deviceStatus = iterator->next();
        ComputedDeviceStatus computedStatus = deviceStatus->getComputedStatus(now);

        //Se il device e' considerato perso viene rimosso
        if(computedStatus == LOST) {
            bool success = networkDevices.deallocate(deviceStatus);
            assert(success);
            ++removedDevices;
        }
    }

    return removedDevices;
}

NetworkDevice *NetworkResources::getDeviceByAddress(unsigned char address[ADDRESS_BYTES]) {

    NetworkDeviceStatus *deviceStatus = getDeviceStatusByAddress(address);

    if(deviceStatus == 0) {
        return 0;
    }

    return deviceStatus->getNetworkDevice();
}

NetworkResources::NetworkResources() : networkDevices(MAX_NETWORK_DEVICES) {
}

void NetworkResources::initialize(FileReader *fileReader) {

    assert(fileReader != 0);

    //Dealloca tutte le istanze preesistenti
    networkDevices.clear();

    unsigned char devicesCount = fileReader->readByte();

    //Alloca tutti i device descritti dal file di stato
    for(int i = 0; i < devicesCount; ++i) {
        NetworkDeviceStatus *currentDevice = networkDevices.allocate();
        currentDevice->initialize(fileReader);
    }
}

void NetworkResources::writeToFile(FileWriter *fileWriter) {
    //Scrive il numero di device
    unsigned char devicesCount = networkDevices.getInstanceCount();
    fileWriter->writeByte(devicesCount);

    //Scrive le informazioni di tutti i device
    InstanceIterator<NetworkDeviceStatus> *iterator = networkDevices.getInstanceIterator();

    #ifdef OFFBOARD
    unsigned char writtenDevices = 0;
    #endif // OFFBOARD

    while(iterator->hasNext()) {
        NetworkDeviceStatus *currentStatus = iterator->next();
        currentStatus->writeToFile(fileWriter);

        #ifdef OFFBOARD
        ++writtenDevices;
        #endif // OFFBOARD
    }

    //Si assicura che siano state scritte il
    //numero di device attesi
    #ifdef OFFBOARD
    assert(writtenDevices == devicesCount);
    #endif // OFFBOARD
}

/**
* @return true se le due istanze sono logicamente equivalenti
*/
bool NetworkResources::equals(NetworkResources *otherResources) {
    if(otherResources == 0) {
        return false;
    }

    //Un'istanza e' uguale a se stessa
    if(this == otherResources) {
        return true;
    }

    //Se ci sono numeri diversi di device allora lo stato logico rappresentato e' diverso
    //Serve anche a garantire la riflettivita' dell'uguaglianza
    if(networkDevices.getInstanceCount() != otherResources->networkDevices.getInstanceCount()) {
        return false;
    }

    //Verifica che nelle istanze siano contenuti gli stessi stati
    InstanceIterator<NetworkDeviceStatus> *iterator = networkDevices.getInstanceIterator();
    while(iterator->hasNext()) {
        NetworkDeviceStatus *currentDevice = iterator->next();
        NetworkDeviceStatus *otherDeviceStatus = otherResources->getDeviceStatusByAddress(currentDevice->getNetworkDevice()->getAddress());

        if(currentDevice->equals(otherDeviceStatus) == false) {
            return false;
        }
    }

    return true;
}
