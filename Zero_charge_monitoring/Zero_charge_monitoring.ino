#include <EEPROM.h>
#include <mcp_can.h>
#include <SPI.h>
#include <ZERO.h>
#include <SCV2.h>
#include "Network.h"
#include "unions.h"

#define VERSION 3

#define MIN_VOLTS 70000 //70.000v
#define DEFAULT_VOLTS 116400 //116.400v
#define DEFAULT_AMPS 130 // 13.0
#define DEFAULT_WATTS 1500

#define TargetWattsAddress 0
#define EndVoltsAddress 16

const long MESSAGE_THRESHOLD=20000;
const long SC_STALE_THRESHOLD=30000;
const long BIKE_STALE_THRESHOLD=1000;
const long COUNT_SCV2_THRESHOLD=22000;

long messageCount=0;
long scStaleCount=0;
long bikeStaleCount=0;
int chargerWaitCount=0;

byte connectedChargers=0;

const byte SPI_CS_PIN_CHARGER = 10;
const byte SPI_CS_PIN_BIKE = 9;

MCP_CAN Charger(SPI_CS_PIN_CHARGER);
MCP_CAN Bike(SPI_CS_PIN_BIKE);

Zero zero;
SCV2 sc;

long endVolts=DEFAULT_VOLTS; //116.400
ByteToShort throttle;

ByteToLong bikeVoltage;
ByteToShort bikeAmps;
ByteToShort bikeThrottle;

ByteToLong chargerVoltage;
ByteToLong chargerVoltageHolder;

ByteToShort chargerAmps;
short chargerAmpsList[16];

ByteToLong voltageDiff;

ByteToShort desiredOutputAmps;
ByteToShort desiredOutputVolts;

short targetAmps=DEFAULT_AMPS;
long targetWatts=DEFAULT_WATTS;

byte chargerMessage[8];
byte bikeMessage[8];

void setup(){
  zero.logInit();
   
  while (CAN_OK != Charger.begin(CAN_250KBPS)){
    if(Serial){
      Serial.println("bad Charger can");
    }
    delay(100);
  }

  while (CAN_OK != Bike.begin(CAN_500KBPS)){
    if(Serial){
      Serial.println("bad Bike can");
    }
    delay(100);
  }

  if(Serial){
    Serial.println("OK");
  }

  EEPROM.get(TargetWattsAddress,targetWatts);
  EEPROM.get(EndVoltsAddress,endVolts);

  targetAmps=DEFAULT_AMPS;
  setOutputVolts(endVolts);
  bikeVoltage.value=0;
  bikeAmps.value=0;
}

void loop(){
  chargerWaitCount++;
  
  if(chargerWaitCount>COUNT_SCV2_THRESHOLD){
    logDetails();
    if(isReadyToCharge()){
      sendChargeCommand();
    }
    connectedChargers=0;
    chargerWaitCount=0;
  }

  if(Serial){
    while(Serial.available()){
      String endVoltsCommand = Serial.readStringUntil(',');
      String wattsCommand = Serial.readStringUntil('\n');
  
      endVolts = endVoltsCommand.toInt()*100;
      targetWatts = wattsCommand.toInt();
  
      if(endVolts<MIN_VOLTS){
        endVolts=MIN_VOLTS;
      }
  
      if(targetWatts<0){
        targetWatts=DEFAULT_WATTS;
      }
      
      EEPROM.put(TargetWattsAddress, targetWatts);
      EEPROM.put(EndVoltsAddress, endVolts);
    }
  }
  
  if(CAN_MSGAVAIL == Charger.checkReceive()){
    byte len=0;
    byte buf[sc.messageLength];
    scStaleCount=0;
    connectedChargers++;
    Charger.readMsgBuf(&len, buf);
    
    short canId = Charger.getCanId();
    short volts=sc.voltage(buf);
    
    chargerVoltageHolder.bytes[0]=lowByte(volts);
    chargerVoltageHolder.bytes[1]=highByte(volts);
    chargerVoltageHolder.bytes[2]=0;
    chargerVoltageHolder.bytes[3]=0;
    chargerVoltageHolder.value=chargerVoltageHolder.value*100;
    
    chargerAmpsList[connectedChargers-1]=sc.amps(buf);
    chargerAmps.value=0;
    for(byte i=0; i<connectedChargers; i++){
      chargerAmps.value=chargerAmps.value+chargerAmpsList[i];
    }
    
    setVoltageDiff();
  }
    
  if(CAN_MSGAVAIL == Bike.checkReceive()){
    byte len=0;
    byte buf[zero.messageLength];
    
    bikeStaleCount=0;
    Bike.readMsgBuf(&len, buf);
    
    short canId = Bike.getCanId();
    if(zero.hasMonolythVoltage(canId)){
      bikeVoltage.value=zero.voltage(len,buf,canId);
      setVoltageDiff();
    }

    if(zero.hasMonolythAmps(canId)){
      bikeAmps.value=zero.amps(len,buf,canId);
    }

    if(zero.hasThrottle(canId)){
      throttle.value= zero.throttle(len,buf,canId);
    }

    if(throttle.value<300){
      if(canId==DASH_STATUS3.id){
        ByteToShort volts;
        volts.value=bikeVoltage.value*KM_TO_MI/10;
        buf[2]=volts.bytes[0];
        buf[3]=volts.bytes[1];
        Bike.sendMsgBuf(DASH_STATUS3.id, 0, len, buf);
      }

      if(canId==DASH_STATUS2.id){
        ByteToShort amps;
        amps.value=abs(bikeAmps.value)*MI_TO_KM*100;
        buf[4]=amps.bytes[0];
        buf[5]=amps.bytes[1];
        Bike.sendMsgBuf(DASH_STATUS2.id, 0, len, buf);
      }
    }
  }

  bikeStaleCount++;
  scStaleCount++;
  messageCount++;

  if(scStaleCount>SC_STALE_THRESHOLD){
    chargerVoltage.value=0;
    chargerAmps.value=0;
    chargerVoltageHolder.value=0;
    resetCharging();
  }

  if(bikeStaleCount>BIKE_STALE_THRESHOLD){
    bikeVoltage.value=0;
    bikeAmps.value=0;
    resetCharging();
  }
  
  if(messageCount>MESSAGE_THRESHOLD){
    messageCount=0;
    //logDetails();
  }
}

byte isReadyToCharge(){
  byte isReady=0;
  if(bikeVoltage.value>MIN_VOLTS 
    && chargerVoltageHolder.value>MIN_VOLTS
    && chargerVoltageHolder.value<endVolts
  ){
    isReady=1;
  }
  return isReady;
}

byte shouldCharge(){
  byte should=0;
  if(bikeVoltage.value>MIN_VOLTS 
    && bikeVoltage.value<endVolts
    && chargerVoltage.value<endVolts
  ){
    should=1;
  }
  return should;
}

void setVoltageDiff(){
  if(isReadyToCharge()){
    voltageDiff.value=bikeVoltage.value-chargerVoltage.value;
    chargerVoltage.value=(chargerVoltageHolder.value+chargerVoltage.value)/2;
    setOutputVolts(endVolts-voltageDiff.value);
    sendChargingCommand();
  }
}

void setOutputVolts(long volts){
  ByteToLong voltage;
  voltage.value=volts/100;
  desiredOutputVolts.bytes[0]=voltage.bytes[0];
  desiredOutputVolts.bytes[1]=voltage.bytes[1];
}

void sendChargeCommand(){
  if(shouldCharge()){
    desiredOutputAmps.value=(targetWatts*100)/(bikeVoltage.value/100)/connectedChargers;
    
    chargerMessage[0]=desiredOutputVolts.bytes[1];
    chargerMessage[1]=desiredOutputVolts.bytes[0];
    
    chargerMessage[2]=desiredOutputAmps.bytes[1];
    chargerMessage[3]=desiredOutputAmps.bytes[0];
    
    Charger.sendMsgBuf(sc.id, 1, sc.messageLength, chargerMessage);
  }
}

void sendChargingCommand(){
  if(shouldCharge()){
    byte chargingBuf[8];
  
    chargingBuf[0]=0x1;
  
    Bike.sendMsgBuf(CALEX_CHARGE_STATUS.id, 0, zero.messageLength, chargingBuf);
  }
}

void resetCharging(){
  desiredOutputVolts.value=0;
  desiredOutputAmps.value=0;
  voltageDiff.value=0;
}

void logDetails(){
//  Serial.println("------------------------");
//  Serial.print("SC Voltage:");
//  Serial.println(chargerVoltage.value);
//  Serial.print("bike Voltage:");
//  Serial.println(bikeVoltage.value);
//  Serial.print("Voltage Diff:");
//  Serial.println(voltageDiff.value);
//  Serial.print("Desired Output Voltage Adjusted :");
//  Serial.println(desiredOutputVolts.value);
//
//  Serial.println("");
//  Serial.print("SC Amps:");
//  Serial.println(chargerAmps.value);
//  Serial.print("bike Amps:");
//  Serial.println(bikeAmps.value);
//  Serial.print("Desired Output Amps Adjusted :");
//  Serial.println(desiredOutputAmps.value);
//
//  Serial.println("");
//  Serial.print("Chargers : ");
//  Serial.println(connectedChargers);

  if (Serial) {
    //battery volts
    Serial.print(bikeVoltage.value/100);
    Serial.print(",");

    //ending voltage
    Serial.print(endVolts/100);
    Serial.print(",");

    //total charging amps
    Serial.print(chargerAmps.value);
    Serial.print(",");

    //target watts
    Serial.print(targetWatts);
    Serial.print(",");

    //charger count
    Serial.print(connectedChargers);
    Serial.print(",");

    //target watts
    //Serial.print(watts);
    //Serial.print(",");

    //start extending with a comma
    //Serial.print(",");

    //print api version
    Serial.println(VERSION);
  }
}

