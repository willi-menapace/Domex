/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "XBeeRemoteATCommand.h"

#include "GlobalDefines.h"

const int XBeeRemoteATCommand::ADDRESS_INDEX = 5;
const int XBeeRemoteATCommand::NET_ADDRESS_INDEX = 13;
const int XBeeRemoteATCommand::OPTIONS_INDEX = 15;
const int XBeeRemoteATCommand::AT_COMMAND_INDEX = 16;
const int XBeeRemoteATCommand::PARAMETERS_INDEX = 18;

XBeeRemoteATCommand::XBeeRemoteATCommand(unsigned char *_atCommand, unsigned char _parametersSize, unsigned char *_parameters,
                        unsigned char *_destinationAddress, unsigned char *_destinationNetAddress) {

    frameId = 0;
    options = 0x02; //Applica il comando immediatamente

    unsigned char *buffer = getData();
    buffer[0] = 0x7E;

    //MSB e LSB lunghezza
    buffer[1] = 0;
    buffer[2] = PARAMETERS_INDEX - PAYLOAD_INDEX + _parametersSize;

    //Frame type
    buffer[PAYLOAD_INDEX] = REMOTE_AT_COMMAND_REQUEST;
    buffer[PAYLOAD_INDEX + 1] = frameId;

    //Indirizzo destinatario, da MSB a LSB
    for(int i = ADDRESS_INDEX; i < ADDRESS_INDEX + ADDRESS_BYTES; ++i) {
        buffer[i] = _destinationAddress[i - ADDRESS_INDEX];
    }

    //Indirizzo di rete destinatario, da MSB a LSB
    buffer[NET_ADDRESS_INDEX] = _destinationNetAddress[0];
    buffer[NET_ADDRESS_INDEX + 1] = _destinationNetAddress[1];

    buffer[OPTIONS_INDEX] = options;

    //Il comando AT
    buffer[AT_COMMAND_INDEX] = _atCommand[0];
    buffer[AT_COMMAND_INDEX + 1] = _atCommand[1];

    //Inserisce i parametri
    for(int i = PARAMETERS_INDEX; i < PARAMETERS_INDEX + _parametersSize; ++i) {
        buffer[i] = _parameters[i - PARAMETERS_INDEX];
    }

}

