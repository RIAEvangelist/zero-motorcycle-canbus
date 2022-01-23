#ifndef ZERO_H
#define ZERO_H

#include <Arduino.h>
#include "Network.h"
#include "unions.h"


class Zero {
  public:
    Zero();
    ~Zero();

    static byte messageLength;

    byte hasVoltage(short &canId);
    byte hasMonolythVoltage(short &canId);
    byte hasPowerTankVoltage(short &canId);
    byte hasSagAdjust(short &canId);

    byte hasAmps(short &canId);
    byte hasMonolythAmps(short &canId);
    byte hasPowerTankAmps(short &canId);

    byte hasThrottle(short &canId);

    long voltage(byte &len,byte buf[],short &canId);
    short sagAdjust(byte &len,byte buf[],short &canId);

    short amps(byte &len,byte buf[],short &canId);

    short throttle(byte &len,byte buf[],short &canId);

    void logInit();
    void logRaw(byte &len,byte buf[],short &canId);

};

#endif
