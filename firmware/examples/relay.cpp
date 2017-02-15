// This #include statement was automatically added by the Particle IDE.
#include "MCP23008/MCP23008.h"
MCP23008 relayController;

void setup() {
    relayController.setOutputs(4);
    relayController.setAddress(0,0,0);
    Particle.function("controlR4", triggerRelay);
}

void loop() {

}

int triggerRelay(String command){
    if(command.equalsIgnoreCase("turnonallrelays")){
        relayController.turnOnAllRelays();
        return 1;
    }
    if(command.equalsIgnoreCase("turnoffallrelays")){
        relayController.turnOffAllRelays();
        return 1;
    }
    if(command.startsWith("setBankStatus:")){
        int status = command.substring(14).toInt();
        if(status < 0 || status > 255){
            return 0;
        }
        Serial.print("Setting bank status to: ");
        Serial.println(status);
        relayController.setBankStatus(status);
        Serial.println("done");
        return 1;
    }
    //Relay Specific Command
    int relayNumber = command.substring(0,1).toInt();
    Serial.print("relayNumber: ");
    Serial.println(relayNumber);
    String relayCommand = command.substring(1);
    Serial.print("relayCommand:");
    Serial.print(relayCommand);
    Serial.println(".");
    if(relayCommand.equalsIgnoreCase("on")){
        Serial.println("Turning on relay");
        relayController.turnOnRelay(relayNumber);
        Serial.println("returning");
        return 1;
    }
    if(relayCommand.equalsIgnoreCase("off")){
        relayController.turnOffRelay(relayNumber);
        return 1;
    }
    if(relayCommand.equalsIgnoreCase("toggle")){
        relayController.toggleRelay(relayNumber);
        return 1;
    }
    if(relayCommand.equalsIgnoreCase("momentary")){
        relayController.turnOnRelay(relayNumber);
        delay(300);
        relayController.turnOffRelay(relayNumber);
        return 1;
    }
    return 0;
}
