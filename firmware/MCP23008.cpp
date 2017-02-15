#include "MCP23008/MCP23008.h"
#include "spark_wiring_usbserial.h"

//Comment line below out to turn off Serial logging
//#define LOGGING

//Constructor
MCP23008::MCP23008(){
}
void MCP23008::setOutputs(int num){
    ioset = 256 - (1 << num);
    relayCount = num;
    setIoDir();
}
void MCP23008::setOutput(int num){
    ioset = ioset ^ (1<<(num-1));
    iosetCustom = true;
    setIoDir();
}
void MCP23008::setIoDir(){
    //Start I2C port
    Wire.begin();
    //Open connection to specified address
    sendCommand(0x00, ioset);
    sendCommand(0x06, ioset);
    readStatus();
}
void MCP23008::setAddress(int a0, int a1, int a2){
    address = 0x20;
    if(a0 == 1){
        address = address | 1;
    }
    if(a1 == 1){
        address = address | 2;
    }
    if(a2 == 1){
        address = address | 4;
    }
    setIoDir();
}

void MCP23008::turnOnRelay(int relay){
    if((ioset & (1<<(relay-1))) > 0){
        return;
    }
    byte bankValue = (bankOneStatus | (1 << (relay-1)));
    setBankStatus(bankValue);
}

void MCP23008::turnOffRelay(int relay){
    if((ioset & (1<<(relay-1))) > 0){
        return;
    }
    byte bankValue = bankOneStatus & ~(1 << (relay-1));
    setBankStatus(bankValue);
}

void MCP23008::turnOnAllRelays(){
    setBankStatus((255 ^ ioset));
}

void MCP23008::turnOffAllRelays(){
    setBankStatus(0);
}

void MCP23008::toggleRelay(int relay){
    if((ioset & (1<<(relay-1))) > 0){
        return;
    }
    byte bankValue =  bankOneStatus;
    bankValue = bankValue ^ (1 << (relay-1));
    setBankStatus(bankValue);
}

void MCP23008::setBankStatus(int status){
    sendCommand(0x0A, status);
    readStatus();
}

int MCP23008::readRelayStatus(int relay){
    int value = (1<<(relay-1));
    if((ioset & value) > 0){
        return 256;
    }
    byte bankStatus = readRelayBankStatus();
    if(bankStatus & value){
        return 1;
    }else{
        return 0;
    }
}

int MCP23008::readRelayBankStatus(){
    byte bankStatus = sendCommand(0x0A);
    #ifdef LOGGING
        if(bankStatus == 256){
            Serial.println("Read bank status command failed");
            return;
        }
    #endif
    Serial.print("Bank status: ");
    return bankStatus;
}

void MCP23008::readStatus(){
    byte status = readRelayBankStatus();
    #ifdef LOGGING
        if(status == 256){
            Serial.println("Read bank status command failed");
            return;
        }
    #endif
    bankOneStatus = status;
}

int MCP23008::readInputStatus(int input){
    if(!iosetCustom){
        input += relayCount;
    }
    int value = (1<<(input-1));
    if((ioset & value) == 0){
        return 256;
    }
    byte bankStatus = readAllInputs();
    #ifdef LOGGING
        if(bankStatus == 256){
            Serial.println("Read input status command failed");
            return 256;
        }
    #endif
    if(bankStatus & value){
        return 0;
    }else{
        return 1;
    }
}

int MCP23008::readAllInputs(){
    byte bankStatus = sendCommand(0x09);
    #ifdef LOGGING
        if(bankStatus == 256){
            Serial.println("Read all input status command failed");
            return 256;
        }
    #endif
    byte inverted = ~bankStatus;
    byte shifted;
    if(!iosetCustom){
        shifted = inverted >> relayCount;
    }else{
        shifted = inverted;
    }
    return shifted;
}
int MCP23008::sendCommand(int reg){
    return sendCommand(reg, 256);
}

int MCP23008::sendCommand(int reg, int cmd){
    int status = 256;
    while(retrys < 4 && status > 0){
        Wire.beginTransmission(address);
        Wire.write(reg);
        if(cmd < 256) Wire.write(cmd);
        status = Wire.endTransmission();
        retrys++;
    }
    initialized = false;
    int ret = 256;
    if(retrys < 4){
        initialized = true;
        Wire.requestFrom(address, 1);
        ret = Wire.read();
    }
    retrys = 0;
    return ret;
}
