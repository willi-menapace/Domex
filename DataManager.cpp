/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "DataManager.h"

#include "GlobalDefines.h"

#include <assert.h>

#include "SecureFileReader.h"
#include "SecureFileWriter.h"

#include <assert.h>

const unsigned long DataManager::CONNECTION_RETRY_PERIOD = 120000;

const unsigned long DataManager::STORED_VALUE_SIZE = 17;
const unsigned long DataManager::DUMP_SAVEPOINT_DISTANCE = 255;
const unsigned long DataManager::ALL_VALUES_RECEIVED_CONFIRM_TIMEOUT = 15000;

const unsigned long DataManager::COMMIT_TIMEOUT = 10000;

void DataManager::closeConnection() {

    dbConnector.closeConnection();

    dbReader = 0;
    dbWriter = 0;
}

void DataManager::tryDbConnection() {

    dbConnector.tryConnection();
    if(dbConnector.isConnected()) {
        dbReader = dbConnector.getReader();
        dbWriter = dbConnector.getWriter();
    }

}

bool DataManager::isConnected() {
    return dbReader != 0 && dbWriter != 0 && dbReader->isConnected() && dbWriter->isConnected();
}

void DataManager::store(StreamWriter *writer, long value, unsigned char *deviceAddress, unsigned char sensorId, unsigned long receptionTime) {

    writer->writeLong((unsigned long) value);
    for(int i = 0; i < ADDRESS_BYTES; ++i) {
        writer->writeByte(deviceAddress[i]);
    }
    writer->writeByte(sensorId);
    writer->writeLong(receptionTime);

}

void DataManager::storeInMemory(long value, unsigned char *deviceAddress, unsigned char sensorId, unsigned long receptionTime) {

    #ifdef OFFBOARD
    assert(false); //Il metodo non funziona se richiamato al di fuori della piattaforma di esecuzione
    #endif // OFFBOARD

    //Se il non e' aperto a causa di un commit precedente lo apre
    if(!memoryWriter.isOpen()) {
        memoryWriter.open(STORED_DATA_FILENAME, true);
    }

    assert(memoryWriter.isOpen());

    //Invia il valore sul file
    store(&memoryWriter, value, deviceAddress, sensorId, receptionTime);

    unsigned long now = 0;
    #ifdef ONBOARD
    now = millis();
    #endif // ONBOARD
    //Se e' scaduto il timeout per il commit o se si e' resettato il contatore effettua il commit
    if(now - lastCommit > COMMIT_TIMEOUT || now <= lastCommit) {
        memoryWriter.close();
        lastCommit = now;
    }
}

void DataManager::storeInDb(long value, unsigned char *deviceAddress, unsigned char sensorId, unsigned long receptionTime) {
    //La connessione deve essere presente alla chiamata del metodo
    assert(isConnected());

    dbWriter->writeByte(SINGLE_VALUE_PACKET); //Si trasmette al DB un singolo valore

    store(dbWriter, value, deviceAddress, sensorId, receptionTime);

    dbWriter->flush();
}

void DataManager::dumpMemoryToDb() {

    #ifdef OFFBOARD
    assert(false); //Il metodo non funziona se richiamato al di fuori della piattaforma di esecuzione
    #endif // OFFBOARD

    //Alla chiamata la connessione deve essere presente
    assert(isConnected());

    assert(!dbReader->isAvailable()); //Si assicura che non ci siano residui di comunicazioni precedenti in arrivo

    if(memoryWriter.isOpen()) {
        memoryWriter.close(); //Si effettua il commit in modo da salvare i valori prima dell'invio
    }

    unsigned long dumpStartByte = 0; //Byte da cui iniziare ad inviare i dati al DB

    SecureFileReader startByteFile;
    startByteFile.open(BYTE_TO_DUMP_FILENAME);

    //Se non e' presenta il file con l'ultimo byte sincronizzato ne crea uno nuovo
    if(!startByteFile.isOpen()) {
        SecureFileWriter startByteCreator;
        startByteCreator.open(BYTE_TO_DUMP_FILENAME, false);
        startByteCreator.writeLong(0);
        startByteCreator.close();
    //Se e' presente legge il byte da cui cominciare il dump
    } else {
        dumpStartByte = startByteFile.readLong();
        startByteFile.close();
    }

    SecureFileReader storedData;
    storedData.open(STORED_DATA_FILENAME);

    assert(storedData.isOpen());

    unsigned long dumpFileSize = storedData.getSize();

    //Deve essere presente almeno un valore da inviare al DB
    assert(dumpFileSize > dumpStartByte);

    storedData.seek(dumpStartByte);

    //Segnala l'invio di valori multipli
    dbWriter->writeByte(MULTI_VALUE_PACKET_START);

    unsigned long bytesToSend = dumpFileSize - dumpStartByte;

    //Calcola e invia il numero di valori che verranno inviati
    assert(bytesToSend % STORED_VALUE_SIZE == 0);
    unsigned long valuesToSend = bytesToSend / STORED_VALUE_SIZE;
    dbWriter->writeLong(valuesToSend);

    unsigned long receivedRecords = 0; //Numero di record di cui e' avvenuta la conferma ricezione
    unsigned long lastPointSaved = 0;  //Numero di record la cui ricezione e' stata salvata su file

    long value;
    unsigned char deviceAddress[ADDRESS_BYTES];
    unsigned char sensorId;
    unsigned long receptionTime;

    //Finche' ci sono dati da sincronizzare
    while(storedData.isAvailable()) {

        //Se si e' persa la connessione
        if(!isConnected()) {
            closeConnection();

            storedData.close(); //Chiude il file con i valori
            return;
        }

        assert(storedData.isAvailable() >= STORED_VALUE_SIZE); //Deve essere disponibile il valore intero

        while(dbReader->isAvailable()) {
            unsigned char confirmedRecords = dbReader->readByte();
            assert(confirmedRecords != ALL_VALUES_RECEIVED); //E' impossibile che tutti i dati siano arrivati in questo momento
            receivedRecords += confirmedRecords;

            //Se non ci sono altre conferme in arrivo e se sono stati confermati molti record si salva l'avvenuta ricezione su file
            if(!dbReader->isAvailable() && receivedRecords - lastPointSaved > DUMP_SAVEPOINT_DISTANCE) {
                SecureFileWriter startByteFileWriter;
                startByteFileWriter.open(BYTE_TO_DUMP_FILENAME, false);
                startByteFileWriter.writeLong((receivedRecords * STORED_VALUE_SIZE) + dumpStartByte);
                startByteFileWriter.close();

                lastPointSaved = receivedRecords; //Abbiamo appena salvato la corretta ricezione
            }
        }

        value = (long) storedData.readLong();
        for(int i = 0; i < ADDRESS_BYTES; ++i) {
            deviceAddress[i] = storedData.readByte();
        }
        sensorId = storedData.readByte();
        receptionTime = storedData.readLong();

        store(dbWriter, value, deviceAddress, sensorId, receptionTime);
    }

    //Segnala la fine dell'invio
    dbWriter->writeByte(MULTI_VALUE_PACKET_END);

    //Si assicura che venga ricevuta la fine
    dbWriter->flush();

    storedData.close(); //Chiude il file con i valori memorizzati

    unsigned long requestTime = 0;
    unsigned long currentTime = 0;
    #ifdef ONBOARD
    requestTime = millis();
    #endif

    //Attende la conferma di ricezione di tutti i record
    while(true) {

        //Se si e' persa la connessione e non sono rimaste conferme da leggere
        if(!isConnected() && !dbReader->isAvailable()) {
            closeConnection();

            return;
        }

        #ifdef ONBOARD
        currentTime = millis();
        #endif // ONBOARD

        //Riceve conferme di ricezione pregresse e quella di ricezione completa
        while(dbReader->isAvailable()) {
            unsigned char confirmedRecords = dbReader->readByte();

            //Il DB conferma l'avvenuta ricezione di tutti i record
            if(confirmedRecords == ALL_VALUES_RECEIVED) {
                SecureFileWriter startByteFileWriter;
                SecureFileWriter storedDataWriter;

                //Elimina il vecchio file
                storedDataWriter.open(STORED_DATA_FILENAME, false);

                startByteFileWriter.open(BYTE_TO_DUMP_FILENAME, false);
                startByteFileWriter.writeLong(0);

                //Apporta le modifiche a entrambi i file
                startByteFileWriter.close();
                storedDataWriter.close();

                //Tutto e' stato completato
                return;

            //Si ricevono conferme parziali pregresse
            } else {
                receivedRecords += confirmedRecords;

                //Se non ci sono altre conferme in arrivo e se sono stati confermati molti record si salva l'avvenuta ricezione su file
                if(!dbReader->isAvailable() && receivedRecords - lastPointSaved > DUMP_SAVEPOINT_DISTANCE) {
                    SecureFileWriter startByteFileWriter;
                    startByteFileWriter.open(BYTE_TO_DUMP_FILENAME, false);
                    startByteFileWriter.writeLong((receivedRecords * STORED_VALUE_SIZE) + dumpStartByte);
                    startByteFileWriter.close();

                    lastPointSaved = receivedRecords; //Abbiamo appena salvato la corretta ricezione
                }
            }
        }

        //In caso sia superato il timeout o si sia resettato il contatore intero
        //il DB ha incontrato problemi quindi si termina la connessione
        if(currentTime - requestTime > ALL_VALUES_RECEIVED_CONFIRM_TIMEOUT || currentTime < requestTime) {

            closeConnection();
            break;
        }
    }
}

void DataManager::store(long value, unsigned char *deviceAddress, unsigned char sensorId, DateTime *receptionTime) {
    if(isConnected()) {

        #ifdef ONBOARD
        //Non devono essere presenti byte di comunicazioni precedenti non concluse correttamente
        assert(!dbReader->isAvailable());
        #endif //ONBOARD

        storeInDb(value, deviceAddress, sensorId, receptionTime->getTimeOffset());
    } else {

        //Il valore va comunque inserito in memoria indipendentemente dall'avvenuta riconnessione
        storeInMemory(value, deviceAddress, sensorId, receptionTime->getTimeOffset());

        unsigned long currentTime = 0;

        #ifdef ONBOARD
        currentTime = millis();
        #endif // ONBOARD

        //E' passato il tempo di guardia o il contatore si e' resettato
        if(currentTime - lastConnectionAttempt > CONNECTION_RETRY_PERIOD || currentTime < lastConnectionAttempt) {

            //Ritenta la connessione con la base di dati
            tryDbConnection();

            //Se la connessione ha successo
            if(isConnected()) {

                #ifdef ONBOARD
                //Non devono essere presenti valori di comunicazioni precedenti non concluse correttamente
                assert(!dbReader->isAvailable());
                #endif //ONBOARD

                //Memorizza il valore e spedisce tutto il contenuto della memoria alla base di dati
                dumpMemoryToDb();
            } else {

                #ifdef ONBOARD
                //Imposta la data dell'ultimo tentativo di connessione
                lastConnectionAttempt = millis();
                #endif // ONBOARD
            }
        }
    }
}

DataManager::DataManager(Settings *_settings) : dbConnector(_settings->getDbParameters()) {
    assert(_settings != 0);

    dbReader = 0;
    dbWriter = 0;

    lastCommit = 0;
    #ifdef ONBOARD
    lastCommit = millis(); //Non si effettua il commit immediatamente
    #endif // ONBOARD

    lastConnectionAttempt = 4294967294; //Forza il tentativo di connessione all'avvio
}
