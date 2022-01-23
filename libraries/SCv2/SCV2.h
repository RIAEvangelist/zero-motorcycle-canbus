#ifndef SCV2_H
#define SCV2_H

#include <Arduino.h>

#define SCV2_MESSAGE_LENGTH 8


class SCV2 {
  public:
    SCV2();
    ~SCV2();

    static byte messageLength;
    static long id;

    short voltage(byte buf[]);
    short amps(byte buf[]);
};

#endif
