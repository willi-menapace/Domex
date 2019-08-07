/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "XBeeATCommand.h"

#include "GlobalDefines.h"

const int XBeeATCommand::AT_COMMAND_INDEX = 5;
const int XBeeATCommand::PARAMETERS_INDEX = 7;

XBeeATCommand::XBeeATCommand(unsigned char *_atCommand, unsigned char _parametersSize, unsigned char *_parameters) {

    frameId = 0;

    unsigned char *buffer = getData();

    buffer[0] = 0x7E;

    //MSB e LSB lunghezza
    buffer[1] = 0;
    buffer[2] = PARAMETERS_INDEX - PAYLOAD_INDEX + _parametersSize;

    buffer[PAYLOAD_INDEX] = AT_COMMAND;
    buffer[PAYLOAD_INDEX + 1] = frameId;

    //Il comando AT
    buffer[AT_COMMAND_INDEX] = _atCommand[0];
    buffer[AT_COMMAND_INDEX + 1] = _atCommand[1];

    //Inserisce i parametri
    for(int i = PARAMETERS_INDEX; i < PARAMETERS_INDEX + _parametersSize; ++i) {
        buffer[i] = _parameters[i - PARAMETERS_INDEX];
    }
}
