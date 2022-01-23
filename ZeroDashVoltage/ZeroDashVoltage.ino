
#include <SPI.h>
#include "mcp_can.h"
#include <ZERO.h>
#include "Network.h"
#include "unions.h"

Zero zero;
// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int PROGMEM SPI_CS_PIN = 9;

MCP_CAN CAN(SPI_CS_PIN);

void setup()
{
    zero.logInit();

    while (CAN_OK != CAN.begin(CAN_500KBPS))
    {
        if(Serial){
          Serial.println("CAN BUS Shield init fail");
          Serial.println(" Init CAN BUS Shield again");
        }
        delay(100);
    }
}

ByteToLong volts;
ByteToUnsignedShort amps;
ByteToShort throttle;

void loop(){
    while(CAN_MSGAVAIL == CAN.checkReceive()){
        byte len = 0;
        byte buf[zero.messageLength];

        CAN.readMsgBuf(&len, buf);

        short canId = CAN.getCanId();

        if(zero.hasVoltage(canId)){
          volts.value= zero.voltage(len,buf,canId)*KM_TO_MI/10;
        }

        if(zero.hasAmps(canId)){
          amps.value= abs(zero.amps(len,buf,canId))*MI_TO_KM*100;
          
        }

        if(zero.hasThrottle(canId)){
          throttle.value= zero.throttle(len,buf,canId);
          
        }

        if(throttle.value<300){
          if(canId==DASH_STATUS3.id){
            buf[2]=volts.bytes[0];
            buf[3]=volts.bytes[1];
            CAN.sendMsgBuf(DASH_STATUS3.id, 0, len, buf);
          }
  
          if(canId==DASH_STATUS2.id){
            buf[4]=amps.bytes[0];
            buf[5]=amps.bytes[1];
            CAN.sendMsgBuf(DASH_STATUS2.id, 0, len, buf);
          }
        }
    }
}
