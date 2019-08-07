/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

/**
* Permette la scrittura da codice di file di impostazioni.
*
* Modificare il codice secondo le esigenze
*/


//Luce ID 1
//Corrente ID 2

#include "GlobalDefines.h"

#ifdef OFFBOARD
#ifdef SETTINGS_EDITOR

#include "DirectFileWriter.h"
#include "ConnectionParameters.h"
#include "MemoryManager.h"
#include "ConstantOutputAction.h"
#include "Rule.h"

int main() {
    DirectFileWriter directFileWriter;

    unsigned char ipAddress[IP_ADDRESS_SIZE] = {192, 168, 1, 179};
    const unsigned long PORT = 25565;
    const char *username = "Username";
    const char *password = "Password";

    //Inizializza i parametri di connessione
    ConnectionParameters connectionParameters;
    connectionParameters.setIpAddress(ipAddress);
    connectionParameters.setPort(PORT);
    connectionParameters.setUsername(username);
    connectionParameters.setPassword(password);

    unsigned char meteoStation[ADDRESS_BYTES] = {0x00, 0x13, 0xA2, 0x00, 0x40, 0xBF, 0x57, 0x4F}; //Indirizzo dispositivo irrigazione
    unsigned char sockets[ADDRESS_BYTES] = {0x00, 0x13, 0xA2, 0x00, 0x40, 0xBE, 0x45, 0x0B}; //Indirizzo dispositivo irrigazione
    unsigned char irrigation[ADDRESS_BYTES] = {0x00, 0x13, 0xA2, 0x00, 0x40, 0xBF, 0x57, 0x49}; //Indirizzo dispositivo irrigazione

    MemoryManager memoryManager;

    Rule alwaysOn;
    alwaysOn.initialize(0); //Esegue le azioni sempre senza attendere timeout
    ConstantOutputAction *action = memoryManager.allocateConstantOutputAction();
    action->initialize(sockets, 0, 1);
    alwaysOn.addAction(action);

    Rule powerConsumption;
    powerConsumption.initialize(0);
    action = memoryManager.allocateConstantOutputAction();
    action->initialize(irrigation, 0, 1); //Accende
    powerConsumption.addAction(action);
    action = memoryManager.allocateConstantOutputAction();
    action->initialize(irrigation, 0, 0); //Spegne
    powerConsumption.addNotAction(action);
    ConstantCondition *condition = memoryManager.allocateConstantCondition();
    condition->initialize(sockets, 2, 330, HIGHER_THAN);
    powerConsumption.addCondition(condition);

    Rule luminosity;
    luminosity.initialize(0);
    action = memoryManager.allocateConstantOutputAction();
    action->initialize(sockets, 2, 1); //Accende
    luminosity.addAction(action);
    action = memoryManager.allocateConstantOutputAction();
    action->initialize(sockets, 2, 0); //Spegne
    luminosity.addNotAction(action);
    condition = memoryManager.allocateConstantCondition();
    condition->initialize(meteoStation, 1, 300, HIGHER_THAN);
    luminosity.addCondition(condition);


    //Inizializza un file di impostazini ad hoc
    directFileWriter.open("1CONFIG.bin", false);
    connectionParameters.writeToFile(&directFileWriter);
    connectionParameters.setPort(PORT + 1);
    connectionParameters.writeToFile(&directFileWriter);
    directFileWriter.writeByte(3);
    alwaysOn.writeToFile(&directFileWriter);
    powerConsumption.writeToFile(&directFileWriter);
    luminosity.writeToFile(&directFileWriter);


    directFileWriter.close();
    memoryManager.clear();

    return 0;
}

#endif //SETTINGS_EDITOR
#endif //OFFBAORD
