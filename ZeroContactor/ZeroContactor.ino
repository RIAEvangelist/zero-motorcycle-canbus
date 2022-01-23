// demo: CAN-BUS Shield, receive data with check mode
// send data coming to fast, such as less than 10ms, you can use this way
// loovee, 2014-6-13



#include <SPI.h>
#include "mcp_can.h"

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int PROGMEM SPI_CS_PIN = 9;

//open contactor.
unsigned char openContactor[8] = {0x0, 0x31, 0x4, 0xc3, 0x4D, 0x00, 0x0, 0x0};

MCP_CAN CAN(SPI_CS_PIN);                                   

void setup()
{
    Serial.begin(115200);

    while (CAN_OK != CAN.begin(CAN_500KBPS))           
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println(" Init CAN BUS Shield again");
        delay(100);
    }
    Serial.println("CAN BUS Shield init ok!");
  
}

byte sent=0;

void loop()
{
    if(!sent){
     // CAN.sendMsgBuf(DASH_STATUS3.id, 0, 8, stmp);
      CAN.sendMsgBuf(0x81, 0, 8, openContactor);
      
      sent++;
    }
}


