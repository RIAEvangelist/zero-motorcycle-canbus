#include "ZERO.h"
#include "Network.h"
#include "Logging.h"

Zero::Zero(){

}

Zero::~Zero(){

}

void Zero::logInit(){
  if(Serial){
    Serial.begin(115200);
  }
}

void Zero::logRaw(byte len,byte buf[ZERO_MESSAGE_LENGTH],short canId){
  if(!Serial){
    return void;
  }
  byte i;
  byte knownMessage=0;
  byte show=0;

  for(i=0; i<TOTAL_MESSAGES; i++){
    if(canId==Zero_Messages[i].id){
      knownMessage=1;
      show=1;
      if(Serial){
        Serial.println("-----------------------------");
        Serial.println(Zero_Messages[i].name);
      }
      break;
    }
  }

  if(Serial){
    Serial.print(" ID: ");
    Serial.println(canId,HEX);
    Serial.println("-----------------------------");
  }

  for(int i = 0; i<len; i++){
      if(Serial){
        Serial.print(buf[i], HEX);
        Serial.print("\t");
      }
  }

  if(Serial){
    Serial.println();
  }
}
