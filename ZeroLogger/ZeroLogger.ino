// demo: CAN-BUS Shield, receive data with check mode
// send data coming to fast, such as less than 10ms, you can use this way
// loovee, 2014-6-13



#include <SPI.h>
#include "mcp_can.h"
#include <ZERO.h>

Zero zero;

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int PROGMEM SPI_CS_PIN = 9;

MCP_CAN CAN(SPI_CS_PIN);

void setup(){
    zero.logInit();

    while (CAN_OK != CAN.begin(CAN_500KBPS)){
      if(Serial){
        Serial.println("CAN BUS Shield init fail");
        Serial.println(" Init CAN BUS Shield again");
      }
      delay(100);
    }
}

void loop(){
    byte len = 0;
    byte buf[zero.messageLength];

    while(CAN_MSGAVAIL == CAN.checkReceive()){
        CAN.readMsgBuf(&len, buf);

        short canId = CAN.getCanId();

        zero.logRaw(len,buf,canId);
    }
}
