#include "ZERO.h"

Zero::Zero(){

}

Zero::~Zero(){

}

byte Zero::messageLength=ZERO_MESSAGE_LENGTH;

byte Zero::hasVoltage(short &canId){
  if(canId == CALEX_CHARGE_CONTROL.id
    || canId == BMS_CELL_VOLTAGE.id
    || canId == BMS1_CELL_VOLTAGE.id ) {

      return 1;

  }
    return 0;
}

byte Zero::hasMonolythVoltage(short &canId){
  if(canId == BMS_CELL_VOLTAGE.id) {

      return 1;

  }
    return 0;
}

byte Zero::hasPowerTankVoltage(short &canId){
  if(canId == BMS1_CELL_VOLTAGE.id ) {

      return 1;

  }
    return 0;
}

byte Zero::hasSagAdjust(short &canId){
  if(canId == BMS_PACK_CONFIG.id
    || canId == BMS1_PACK_CONFIG.id ) {

      return 1;

  }
    return 0;
}

byte Zero::hasAmps(short &canId){
  if(canId == BMS_PACK_ACTIVE_DATA.id
    || canId == BMS1_PACK_ACTIVE_DATA.id ) {

      return 1;

  }
    return 0;
}

byte Zero::hasMonolythAmps(short &canId){
  if(canId == BMS_PACK_ACTIVE_DATA.id) {

      return 1;

  }
    return 0;
}

byte Zero::hasPowerTankAmps(short &canId){
  if(canId == BMS1_PACK_ACTIVE_DATA.id) {

      return 1;

  }
    return 0;
}

byte Zero::hasThrottle(short &canId){
  if(canId == CONTROLLER_RPM_THROTTLE_MOT_TEMP.id) {

      return 1;

  }
    return 0;
}

short Zero::throttle(byte &len,byte buf[ZERO_MESSAGE_LENGTH],short &canId){
  ByteToShort throttleVoltage;

  throttleVoltage.bytes[0]=buf[4];
  throttleVoltage.bytes[1]=buf[5];

  //Serial.println(throttleVoltage.value);

  return throttleVoltage.value;
}

short Zero::amps(byte &len,byte buf[ZERO_MESSAGE_LENGTH],short &canId){
  ByteToShort amps;

  amps.bytes[0]=buf[3];
  amps.bytes[1]=buf[4];

  //Serial.println(amps.value);

  return amps.value;
}

long Zero::voltage(byte &len,byte buf[ZERO_MESSAGE_LENGTH],short &canId){
  ByteToLong voltage;

  voltage.bytes[0]=buf[3];
  voltage.bytes[1]=buf[4];
  voltage.bytes[2]=buf[5];
  voltage.bytes[3]=buf[6];

  //Serial.println(voltage.value);

  return voltage.value;
}

short Zero::sagAdjust(byte &len,byte buf[ZERO_MESSAGE_LENGTH],short &canId){
  ByteToShort sagAdjust;

  sagAdjust.bytes[0]=buf[0];
  sagAdjust.bytes[1]=buf[1];

  //Serial.println(sagAdjust.value);

  return sagAdjust.value;
}

void Zero::logInit(){
  if(Serial){
    Serial.begin(SERIAL_BAUD);
  }
}

void Zero::logRaw(byte &len,byte buf[ZERO_MESSAGE_LENGTH],short &canId){
  if(!Serial){
    return;
  }
  byte i;
  byte knownMessage=0;
  byte show=0;

  for(i=0; i<TOTAL_MESSAGES; i++){
    if(canId==Zero_Messages[i].id){
      knownMessage=1;
      show=1;
      Serial.println("-----------------------------");
      Serial.println(Zero_Messages[i].name);
      break;
    }
  }


  Serial.print(" ID: ");
  Serial.println(canId,HEX);
  Serial.println("-----------------------------");
  for(int i = 0; i<len; i++){
      Serial.print(buf[i], HEX);
      Serial.print("\t");
  }

  Serial.println();

}
