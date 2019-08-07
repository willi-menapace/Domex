/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "NetworkWriter.h"

#include "GlobalDefines.h"

#ifdef OFFBOARD
std::ostream *NetworkWriter::getStream() {
    //Se il socket non e' stato impostato
    if(client == 0) {
        return 0;
    }
    return client->getOutputWriter()->getStream();
}
#endif // ONFFBOARD
#ifdef ONBOARD
Stream *NetworkWriter::getStream() {
    if(client == 0) {
        return 0;
    }
    return client;
}
#endif // ONFFBOARD

void NetworkWriter::initialize(Client *_client) {
    client = _client;
}

bool NetworkWriter::isConnected() {
    if(client == 0) {
        return false;
    }
    return client->connected();
}

void NetworkWriter::flush() {
    //Lo svuotamento e' necessario solo con i client non di test
    #ifdef ONBOARD
    client->flush();
    #endif // ONBOARD
}

NetworkWriter::NetworkWriter() {
    client = 0;
}
