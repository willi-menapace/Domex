/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "XBeeTransmitRequest.h"

#include "GlobalDefines.h"

const int XBeeTransmitRequest::ADDRESS_INDEX = 5;
const int XBeeTransmitRequest::NET_ADDRESS_INDEX = 13;
const int XBeeTransmitRequest::BROADCAST_RADIUS_INDEX = 15;
const int XBeeTransmitRequest::OPTIONS_INDEX = 16;
const int XBeeTransmitRequest::DATA_INDEX = 17;

XBeeTransmitRequest::XBeeTransmitRequest(unsigned char _dataSize, unsigned char *_data,
                                         unsigned char *_destinationAddress, unsigned char *_destinationNetAddress) {

    frameId = 0;
    options = 0;
    broadcastRadius = 0;

    unsigned char *buffer = getData();
    buffer[0] = 0x7E;

    //MSB e LSB lunghezza
    buffer[1] = 0;
    buffer[2] = DATA_INDEX - PAYLOAD_INDEX + _dataSize;

    //Frame type
    buffer[PAYLOAD_INDEX] = TRANSMIT_REQUEST;
    buffer[PAYLOAD_INDEX + 1] = frameId;

    //Indirizzo destinatario, da MSB a LSB
    for(int i = ADDRESS_INDEX; i < ADDRESS_INDEX + ADDRESS_BYTES; ++i) {
        buffer[i] = _destinationAddress[i - ADDRESS_INDEX];
    }

    //Indirizzo di rete destinatario, da MSB a LSB
    buffer[NET_ADDRESS_INDEX] = _destinationNetAddress[0];
    buffer[NET_ADDRESS_INDEX + 1] = _destinationNetAddress[1];

    buffer[BROADCAST_RADIUS_INDEX] = broadcastRadius;
    buffer[OPTIONS_INDEX] = options;

    //I dati veri e propri dell'applicazione
    for(int i = DATA_INDEX; i < DATA_INDEX + _dataSize; ++i) {
        buffer[i] = _data[i - DATA_INDEX];
    }
}
