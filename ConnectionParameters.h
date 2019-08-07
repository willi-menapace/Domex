/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef CONNECTION_PARAMETERS_INCLUDED
#define CONNECTION_PARAMETERS_INCLUDED

#include "GlobalDefines.h"

#define MAX_USERNAME_LENGTH 16
#define MAX_PASSWORD_LENGTH 16

#include "FileReader.h"
#include "FileWriter.h"
#include "Connector.h"

class Connector; //Evita errori dovuti a loop di inclusioni

/**
* Parametri per la connessione autenticata ad
* un server
*/
class ConnectionParameters {
    private:
        unsigned char ipAddress[IP_ADDRESS_SIZE];  //Indirizzo IP
        unsigned long port;                        //Porta
        char username[MAX_USERNAME_LENGTH + 1];    //Username dell'utente. +1 per il carattere di terminazione
        char password[MAX_PASSWORD_LENGTH + 1];    //Password dell'utente. +1 per il carattere di terminazione

        Connector *listener;  //Connettore da notificare in caso di inizializzazione della classe
                              //Permette la riconnessione con nuovi parametri nel momento del cambio
    public:
        /**
        * @return indirizzo IP di lunghezza IP_ADDRESS_SIZE a cui effettuare la connessione
        */
        unsigned char *getIpAddress();
        /**
        * @return porta a cui effettuare la connessione
        */
        unsigned long getPort();
        /**
        * @return username con cui effettuare l'autenticazione
        */
        char *getUsername();
        /**
        * @return password a cui effettuare l'autenticazione
        */
        char *getPassword();

        /**
        * Imposta l'indirizzo a cui connettersi
        *
        * @param _ipAddress l'indirizzo a cui connettersi
        */
        void setIpAddress(unsigned char *_ipAddress);
        /**
        * Imposta la porta a cui effettuare la connessione
        *
        * @param _port porta a cui effettuare la connessione
        */
        void setPort(unsigned long _port);
        /**
        * Imposta l'username per la connessione
        *
        * @param _username username con cui effettuare l'autenticazione
        */
        void setUsername(const char *_username);
        /**
        * Imposta la password per la connessione
        *
        * @param _password password con cui effettuare l'autenticazione
        */
        void setPassword(const char *_password);

        /**
        * Registra un listener da notificare nel momento di una
        * inizializzazione della classe
        *
        * @param _listener classe da notificare in caso di inizializzazione
        *                  Se impostato a 0 rimuove il listener preesistente
        */
        void setOnInitListener(Connector *_listener);

        /**
        * Inizializza l'istanza con i valori presenti su un file
        *
        * @param fileReader file da cui effettuare la lettura dei valori
        */
        void initialize(FileReader *fileReader);
        /**
        * Effettua il salvataggio dello stato dell'istanza su file
        *
        * @param fileWriter file su cui effettuare la scrittura
        */
        void writeToFile(FileWriter *fileWriter);

        /**
        * Effettua un test di uguaglianza con un'altra istanza
        *
        * @param other altra istanza con cui verificare l'uguaglianza
        *
        * @return true se l'istanza corrente e' logicamente equivalente a quella data
        */
        bool equals(ConnectionParameters *other);

        /**
        * Crea un'istanza vuota
        */
        ConnectionParameters();
};

#endif // CONNECTION_PARAMETERS_INCLUDED
