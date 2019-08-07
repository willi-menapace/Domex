/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "FakeClient.h"

#include "GlobalDefines.h"

#include <assert.h>

FileReader *Client::getInputReader() {
    return &inFile;
}

FileWriter *Client::getOutputWriter() {
    return &outFile;
}

void Client::close() {
    inFile.close();
    outFile.close();
}

bool Client::connected() {
    return inFile.isOpen() && outFile.isOpen();
}

void Client::initialize(const char *inFileName, const char *outFileName) {
    inFile.open(inFileName);
    assert(inFile.isOpen());

    outFile.open(outFileName, false);
    assert(outFile.isOpen());
}
