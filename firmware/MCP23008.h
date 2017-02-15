#include "spark_wiring_i2c.h"
#include "spark_wiring_usbserial.h"
#include "spark_wiring_constants.h"

class MCP23008{
public:
    //Constructor
    MCP23008(void);
    //Set Address.  Indicate status of jumpers on board.  Send 0 for not installed, send 1 for installed
    void setAddress(int a0, int a1, int a2);
    //Set number or outputs, for relay boards they wills start at 0, if you need outputs at specific locations
    //use the setAsOutput method for each
    void setOutputs(int num);
    void setOutput(int num);
    
    void setIoDir();
    //Turn on Relay
    void turnOnRelay(int relay);
    //Turn off Relay
    void turnOffRelay(int relay);
    //Turn On all Relays
    void turnOnAllRelays();
    //Turn Off All Relays
    void turnOffAllRelays();
    //Toggle Relay
    void toggleRelay(int relay);
    //Set status of all relays in bank
    void setBankStatus(int status);
    //Read status of relay. Valid return 0 for off 1 for on.  256 returned if there is an error
    int readRelayStatus(int relay);
    //Read status of all relays in bank.  0-255 valid.  256 returned on error
    int readRelayBankStatus();
    //Read status of input
    int readInputStatus(int input);
    //Read status of all inputs
    int readAllInputs();

    //User Accessible Variables
    //Whether or not the controller is ready to accept commands
    bool initialized;
    int sendCommand(int reg, int cmd);
    int sendCommand(int reg);
private:
    //internal use method for refreshing bank status variables
    void readStatus();
    //Status of relays in bank 1
    byte bankOneStatus;
    //Status of relays in bank 2
    byte bankTwoStatus;
    //Status of relays in bank 3
    byte bankThreeStatus;
    //Status of relays in bank 4
    byte bankFourStatus;

    byte ioset = 255;
    bool iosetCustom = false;
    int relayCount = 8;

    int address = 0x20;
    int address2 = 0x21;
    int retrys = 0;
    byte outputRegister = 0x0A;

};
