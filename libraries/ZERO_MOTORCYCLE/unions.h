#ifndef ZERO_UNIONS_H
#define ZERO_UNIONS_H

#include <Arduino.h>

union ByteToDouble {
  byte bytes[8];
  double value;
};

union ByteToLong {
  byte bytes[4];
  long value;
};

union ByteToShort {
  byte bytes[2];
  short value;
};

union ByteToUnsignedShort {
  byte bytes[2];
  unsigned short value;
};

union ByteToUnsignedLong {
  byte bytes[4];
  unsigned long value;
};

#endif
