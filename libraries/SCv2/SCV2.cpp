#include "SCV2.h"
#include "unions.h"

SCV2::SCV2(){

}

SCV2::~SCV2(){

}

byte SCV2::messageLength=SCV2_MESSAGE_LENGTH;
long SCV2::id=0x1806E5F4;

short SCV2::amps(byte buf[SCV2_MESSAGE_LENGTH]){
  ByteToShort amps;

  amps.bytes[0]=buf[3];
  amps.bytes[1]=buf[2];

  //Serial.println(amps.value);

  return amps.value;
}

short SCV2::voltage(byte buf[SCV2_MESSAGE_LENGTH]){
  ByteToShort voltage;

  voltage.bytes[0]=buf[1];
  voltage.bytes[1]=buf[0];

  //Serial.println(voltage.value);

  return voltage.value;
}
