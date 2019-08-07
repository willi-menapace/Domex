/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "XBeePacket.h"

#include "GlobalDefines.h"

const int XBeePacket::PAYLOAD_INDEX = 3;

unsigned char XBeePacket::getPayloadSumLSB() {
    long payloadSum = 0;

    for(int i = PAYLOAD_INDEX; i < getLength() + PAYLOAD_INDEX; ++i) {
        payloadSum += data[i];
    }

    return payloadSum & 0xFF; //Ritorna l'LSB
}

unsigned char XBeePacket::generateChecksum() {
    unsigned char payloadSum = getPayloadSumLSB();

    return 0xFF - payloadSum; //La somma di tutti i campi del payload piu' la checksum deve dasre 0xFF
}

unsigned char *XBeePacket::getData() {
    return data;
}

unsigned char XBeePacket::getLength() {
    //LSB lunghezza, si suppone che MSB sia a 0
    return data[PAYLOAD_INDEX - 1];
}

XBeePacket::XBeePacket(XBeePacket *packet) {
    unsigned char *buffer = getData();

    unsigned char *otherBuffer = packet->getData();

    //La lunghezza totale utilizzata del buffer e' la lunghezza del payload + i byte prima del payload + il byte di checksum
    unsigned char totalBufferLength = packet->getLength() + PAYLOAD_INDEX + 1;

    //Per costruire il pacchetto effettua una copia dei dati
    for(int i = 0; i < totalBufferLength; ++i) {
        buffer[i] = otherBuffer[i];
    }
}

XBeePacketType XBeePacket::getType() {
    //Il byte del tipo e' il primo del payload
    unsigned char typeByte = data[PAYLOAD_INDEX];

    //Effettua la traduzione del valore intero
    switch(typeByte) {
    case AT_COMMAND:
        return AT_COMMAND;
        break;
    case AT_RESPONSE:
        return AT_RESPONSE;
        break;
    case TRANSMIT_REQUEST:
        return TRANSMIT_REQUEST;
        break;
    case TRANSMIT_STATUS:
        return TRANSMIT_STATUS;
        break;
    case RECEIVE_PACKET:
        return RECEIVE_PACKET;
        break;
    case IO_DATA_SAMPLE:
        return IO_DATA_SAMPLE;
        break;
    case REMOTE_AT_COMMAND_REQUEST:
        return REMOTE_AT_COMMAND_REQUEST;
        break;
    case REMOTE_AT_COMMAND_RESPONSE:
        return REMOTE_AT_COMMAND_RESPONSE;
        break;
    default:
        return UNKNOWN_PACKET;
    }
}

bool XBeePacket::validateChecksum() {

    //Alla somma dei campi del payload aggiunge la checksum ricevuta
    unsigned int totalSum = getPayloadSumLSB() + data[getLength() + PAYLOAD_INDEX];

    //Se l'ultimo byte e' uguale a 0xFF allora la checksum e' corretta
    return  (totalSum & 0xFF) == 0xFF;
}

void XBeePacket::send(XBeeSerialManager *manager) {
    unsigned char payloadLength = getLength();

    //Il pacchetto e' completamente costruito salvo la checksum quindi si invia fino al penultimo byte
    for(int i = 0; i < PAYLOAD_INDEX + payloadLength; ++i) {
        manager->writeByte(data[i]);
    }

    //Invia la checksum del pacchetto
    manager->writeByte(generateChecksum());
}

XBeePacket::XBeePacket() {}

