/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "XBee.h"

#include "GlobalDefines.h"

#include <assert.h>

#ifdef ONBOARD
#include <Arduino.h>
#endif // ONBOARD

unsigned char XBee::getNextFrameID() {
    unsigned char frameID = nextFrameID;

    if(nextFrameID < 255) {
        ++nextFrameID;
    } else {
        nextFrameID = 1; //0 e' riservato per messaggi senza richiesta di conferma ricezione
    }

    //0 e' riservato
    assert(frameID != 0);

    return frameID;
}

void XBee::send(XBeePacket *packet) {
    assert(packet != 0);

    packet->send(manager);
}

unsigned char XBee::sendWithAck(XBeeFrameablePacket *packet) {
    assert(packet != 0);

    unsigned char frameID = getNextFrameID();

    packet->setFrameId(frameID);
    packet->send(manager);

    return frameID;
}

bool XBee::packetAvailable() {
    //Se ci sono byte da leggere deve essere un nuovo pacchetto
    return manager->available();
}

bool XBee::receivePacket(XBeePacket *packet) {
    assert(packet != 0);

    unsigned char currentByte;
    //Scorre i byte in arrivo fino a trovare lo start byte
    while(true) {
        if(manager->available()) {
            currentByte = manager->readByte();
            //Se e' stato trovato lo start byte inizia la ricezione del pacchetto
            if(currentByte == 0x7E) {
                break;
            }
        }
    }

    unsigned char *data = packet->getData();

    data[0] = currentByte; //Inserisce lo start byte

    unsigned char counter = 1;
    unsigned char bytesLeft = 255;
    while(true) {
        if(manager->available()) {
            data[counter] = manager->readByte();
            if(counter == 2) {
                bytesLeft = data[counter] + 2; //I byte ancora da leggere + 1 di checksum + 1 che viene subito scalato
                //Evita l'overflow del buffer di ricezione
                if(data[counter] > XBEE_PACKET_BUFFER_SIZE - 3) {
                    return false;
                }
            }

            assert(counter < XBEE_PACKET_BUFFER_SIZE);

            ++counter;



            --bytesLeft; //Si possono comunque decrementare i byte, alla terza lettura verranno reimpostati
            if(bytesLeft == 0) {
                break;
            }
        }
    }

    return packet->validateChecksum();
}

XBee::XBee(XBeeSerialManager *_manager) {

    manager = _manager;
    nextFrameID = 1; //Gli ID iniziano da 1, 0 e' per i messaggi senza conferma di ricezione
}
