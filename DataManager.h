/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef DATA_MANAGER_INCLUDED
#define DATA_MANAGER_INCLUDED

#include "GlobalDefines.h"

//File in cui vengono salvati i valori in caso non sia disponibile la connessione al DB
#define STORED_DATA_FILENAME "robots.bin"
//File in cui viene memorizzato l'ultimo byte di valori salvati in memoria
//che e' stato ricevuto sicuramente dal DB
#define BYTE_TO_DUMP_FILENAME "dump.bin"

#ifdef OFFBOARD
#include "FakeClient.h"
#endif // OFFBOARD

#include "DateTime.h"
#include "Settings.h"
#include "NetworkReader.h"
#include "NetworkWriter.h"
#include "Connector.h"
#include "SecureFileWriter.h"

/**
* Gestore della memorizzazione dei dati
* raccolti dai sensori
*/
class DataManager {
private:

    enum DbProtocolConstants {
        SINGLE_VALUE_PACKET = 1, //Invio di un singolo valore

        MULTI_VALUE_PACKET_START = 2, //Inizio dell'invio di valori multipli
        MULTI_VALUE_PACKET_END = 3,   //Fine dell'invio di valori multipli
        ALL_VALUES_RECEIVED = 0,      //Tutti i valori sono stati ricevuti
    };

    static const unsigned long CONNECTION_RETRY_PERIOD; //Tempo in millisecondi dopo cui ritentare la connessione

    static const unsigned long STORED_VALUE_SIZE;       //Dimensione in byte della entry di un valore
    static const unsigned long DUMP_SAVEPOINT_DISTANCE; //Numero di record dopo il quale l'avvenuta ricezione viene salvata su file
    static const unsigned long ALL_VALUES_RECEIVED_CONFIRM_TIMEOUT;   //Tempo in millisecondi di durata massima di attesa corretta ricezione di tutti i valori

    static const unsigned long COMMIT_TIMEOUT;  //Tempo in millisecondi dopo il quale effettuare forzatamente il commit dei valori salvati in memoria

    Connector dbConnector; //Gestore della connessione con il DB

    unsigned long lastConnectionAttempt; //Millisecondi in cui e' avvenuto l'ultimo tentativo di connessione al database

    NetworkReader *dbReader; //Reader per il dialogo con la base di dati
    NetworkWriter *dbWriter; //Writer per il dialogo con la base di dati

    SecureFileWriter memoryWriter; //Writer usato per la scrittura in memoria
    unsigned long lastCommit;      //Millisecondi in cui si e' effettuato l'ultimo commit dei valori salvati in memoria

    /**
    * Effettua la chiusura di tutte le risorse di rete
    */
    void closeConnection();

    /**
    * Effettua un tentativo di connessione con la base di dati
    */
    void tryDbConnection();
    /**
    * Verifica lo stato della connessione con il DB
    *
    * @return true se e' diponibile la connessione con il DB
    */
    bool isConnected();

    /**
    * Invia il valore letto sullo stream fornito. La dimensione in byte della entry e' STORED_VALUE_SIZE
    *
    * @param writer stream su cui inviare il valore
    * @param value il valore da memorizzare
    * @param deviceAddress indirizzo del device che ha fornito il valore
    * @param sensorId id del sensore sul device che ha fornito il valore
    * @param receptionTime secondi trascorsi dal 1 gennaio 2000 alla ricezione del valore
    */
    void store(StreamWriter *writer, long value, unsigned char *deviceAddress, unsigned char sensorId, unsigned long receptionTime);
    /**
    * Memorizza il valore in memoria
    *
    * @param value il valore da memorizzare
    * @param deviceAddress indirizzo del device che ha fornito il valore
    * @param sensorId id del sensore sul device che ha fornito il valore
    * @param receptionTime secondi trascorsi dal 1 gennaio 2000 alla ricezione del valore
    */
    void storeInMemory(long value, unsigned char *deviceAddress, unsigned char sensorId, unsigned long receptionTime);
    /**
    * Invia il valore al DB. Deve essere disponibile la connessione con il DB
    *
    * @param value il valore da memorizzare
    * @param deviceAddress indirizzo del device che ha fornito il valore
    * @param sensorId id del sensore sul device che ha fornito il valore
    * @param receptionTime secondi trascorsi dal 1 gennaio 2000 alla ricezione del valore
    */
    void storeInDb(long value, unsigned char *deviceAddress, unsigned char sensorId, unsigned long receptionTime);
    /**
    * Invia tutti i valori presenti in memoria al DB
    * e li cancella in caso di successo.
    * Deve essere disponibile la connessione con il DB
    */
    void dumpMemoryToDb();

public:

    /**
    * Memorizza il dato raccolto
    *
    * @param value il valore da memorizzare
    * @param deviceAddress indirizzo del device che ha fornito il valore
    * @param sensorId id del sensore sul device che ha fornito il vlaore
    * @param receptionTime data e ora di ricezione del valore
    */
    void store(long value, unsigned char *deviceAddress, unsigned char sensorId, DateTime *receptionTime);

    /**
    * Costruisce un gestore della memorizzazione che effettua la memorizzazione
    * con i parametri forniti
    *
    * @param _settings impostazioni da utilizzare per la memorizzazione
    */
    DataManager(Settings *_settings);
};

#endif // DATA_MANAGER_INCLUDED
