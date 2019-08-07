/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef FAKE_CLIENT_INCLUDED
#define FAKE_CLIENT_INCLUDED

#include "GlobalDefines.h"

#ifdef OFFBOARD

#include "DirectFileReader.h"
#include "DirectFileWriter.h"

/**
* Classe che emula un client definito dalle librerie Arduino a scopi di test
*
* La lettura e la scrittura vengono effettuate su file che devono essere aperti
* e chiusi con i rispettivi metodi. Una volta chiusi possono essere letti e scritti a piacere.
*/
class Client {
private:
    DirectFileReader inFile;
    DirectFileWriter outFile;
public:
    /**
    * @return file da cui ottenere l'input
    */
    FileReader *getInputReader();
    /**
    * @return file su cui scrivere l'output
    */
    FileWriter *getOutputWriter();
    /**
    * Chiude le risorse associate
    */
    void close();

    /**
    * Verifica che i file di input e output siano aperti
    *
    * @return true se i file sono aperti
    */
    bool connected();

    /**
    * Effettua l'apertura dei file da cui ottenere input e output
    *
    * @param inFileName nome del file cui ottenere l'input. Deve esistere
    * @param outFileName nome del file su cui scrivere l'output
    */
    void initialize(const char *inFileName, const char *outFileName);

};

#endif // OFFBOARD

#endif // FAKE_CLIENT_INCLUDED
