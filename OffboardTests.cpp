/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

/*
Mettere gli assert nella libreria XBee. Includere i controlli che servono per verificare il tipo nei costruttori di copia
Verificare se viene calcolata in ricezione la checksum dei pacchetti XBee

Fare la documentazione delle forme serializzate

Finire di mettere i messaggi di debug
Forse la connessione TCP cade da sola o non sta in piedi senza traffico
Il DHCP cosi' non funziona se il cavo non e' connesso all'avvio
*/

/**
* Esegue tutti i test cases delle seguenti librerie disponibili in modalita' offboard:
* -Core
* -IO
* -XBee
* -INet
*
* Le librerie di sistema dove necessario vengono emulate
* Alcuni test delle librerie Core richiedono il controllo manuale dei risultati.
*/

#include "GlobalDefines.h"

#ifdef OFFBOARD
#ifdef TEST

#include "CoreLibraryTestCases.h"
#include "XBeeLibraryTestCases.h"
#include "INetLibraryTestCases.h"

int main() {

    executeAllCoreLibraryTests();
    std::cout << "\n--------------------------------------\n";
    std::cout << "Now testing XBeeATCommand\n";
    std::cout << "--------------------------------------\n";
    xBeeATCommandTest("test\\XBeeATCommandParameters.txt", "test\\XBeeATCommandVerification.txt");
    std::cout << "TEST SUCCESSFUL\n";
    std::cout << "\n--------------------------------------\n";
    std::cout << "Now testing XBeeTransmitRequest\n";
    std::cout << "--------------------------------------\n";
    xBeeTransmitRequestTest("test\\XBeeTransmitRequestParameters.txt", "test\\XBeeTransmitRequestVerification.txt");
    std::cout << "TEST SUCCESSFUL\n";
    std::cout << "\n--------------------------------------\n";
    std::cout << "Now testing XBeeRemoteATCommand\n";
    std::cout << "--------------------------------------\n";
    xBeeRemoteATCommandTest("test\\XBeeRemoteATCommandParameters.txt", "test\\XBeeRemoteATCommandVerification.txt");
    std::cout << "TEST SUCCESSFUL\n";
    executeAllINetLibraryTests();

    return 0;
}

#endif //TEST
#endif //OFFBOARD
