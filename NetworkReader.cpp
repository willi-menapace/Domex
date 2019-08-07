/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "NetworkReader.h"

#include "GlobalDefines.h"

#ifdef OFFBOARD
std::istream *NetworkReader::getStream() {
    //Se il socket non e' stato impostato
    if(client == 0) {
        return 0;
    }
    return client->getInputReader()->getStream();
}
#endif // ONFFBOARD
#ifdef ONBOARD
Stream *NetworkReader::getStream() {
    if(client == 0) {
        return 0;
    }
    return client;
}
#endif // ONFFBOARD

void NetworkReader::initialize(Client *_client) {
    client = _client;
}

bool NetworkReader::isConnected() {
    if(client == 0) {
        return false;
    }
    return client->connected();
}

NetworkReader::NetworkReader() {
    client = 0;
}
