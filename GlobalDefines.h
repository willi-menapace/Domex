/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

/**
* File di configurazione generale del software.
* Permette di impostare parametri che influenzano il comportamento del software
* Deve essere incluso prima di ogni altro file in ogni file
*
* - <ONBOARD | OFFBOARD>
* Deve essere definito per primo.
* ONBOARD indica che l'esecuzione avviene avendo a disposizione le librerie di sistema
* OFFBOARD indica che l'esecuzione avviene non avendo a disposizione le librerie di sistema
* Tipicamente il primo e' utilizzato in caso di esecuzione sulle schede mentre il secondo e'
* usato a fini di test per l'esecuzione su PC
*
* -ADDRESS_BYTES
* Deve essere definito.
* Indica la dimensione in bytes dell'indirizzo fisico dei moduli XBee
*
* -NET_ADDRESS_BYTES
* Deve essere definito.
* Indica la dimensione in bytes dell'indirizzo di rete dei moduli XBee
*
* -IP_ADDRESS_SIZE
* Deve essere definito nel caso di caricamento sull'unita' centrale.
* Indica la dimensione in bytes di un'indirizzo IP
*
* -XBEE_PACKET_BUFFER_SIZE
* Deve essere definito.
* Indica la dimensione in byte del buffer di ricezione per i pacchetti XBee
*
* -SYSTEM_PATH_SEPARATOR
* Deve essere definito nel caso di caricamento sull'unita' centrale.
* Indica il carattere utilizzato dal sistema come deparatore nei percorsi del filesystem
*
* -SETTINGS_FILENAME
* Deve essere definito nel caso di caricamento sull'unita' centrale.
* Indica il percorso e il nome del file di impostazioni.
*
* -DEVICES_FILENAME
* Deve essere definito nel caso di caricamento sull'unita' centrale.
* Indica il percorso e il nome del file contenente i device raggiungibili conosciuti
*
* -FAKE_CLIENT_INPUT_FILENAME
* Deve essere definito se in modalita' OFFBOARD.
* Indica il nome del file di input da utilizzare per la simulazione
* della classe della libreria di sistema Client
*
* -FAKE_CLIENT_OUTPUT_FILENAME
* Deve essere definito se in modalita' OFFBOARD.
* Indica il nome del file di output da utilizzare per la simulazione
* della classe della libreria di sistema Client
*
* -TEST
* Facoltativo.
* Indica che l'esecuzione avviene a fini di test
*
* -DEBUG
* Facoltativo.
* Attiva messaggi di debug
*
* -SETTINGS_EDITOR
* Attivabile facoltativamente se in modalita' OFFBAORD
* Avvia l'editor per la generazione di file di impostazioni
*/

#ifndef GLOBAL_DEFINES_INCLUDED
#define GLOBAL_DEFINES_INCLUDED

#define OFFBOARD

#define ADDRESS_BYTES 8
#define NET_ADDRESS_BYTES 2
#define IP_ADDRESS_SIZE 4
#define XBEE_PACKET_BUFFER_SIZE 80

#ifdef OFFBOARD
    #define SYSTEM_PATH_SEPARATOR '\\'
#endif // OFFBOARD
#ifdef ONBOARD
    #define SYSTEM_PATH_SEPARATOR '/'
#endif // ONBOARD

#define DEBUG
#define SETTINGS_EDITOR

//Nome del file di impostazioni di sistema
#define SETTINGS_FILENAME "config.bin"
//Nome del file contenente le risorse di rete
#define DEVICES_FILENAME "devices.bin"

//Nomi file utilizzati per la costruzione di client di test
#ifdef OFFBOARD
    #define FAKE_CLIENT_INPUT_FILENAME "cinput.bin"
    #define FAKE_CLIENT_OUTPUT_FILENAME "coutput.bin"
#endif // OFFBOARD

#endif // GLOBAL_DEFINES_INCLUDED
