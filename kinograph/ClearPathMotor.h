// ClearPathMotor.h
#pragma once
#ifndef ClearPathMotor_h
#define ClearPathMotor_h

#include <Arduino.h>

class ClearPathMotor {
  private:
    int enablePin;
    int inputAPin;
    int inputBPin;
    int HLFBPin;
    String dir;
    int target;

  public:
    ClearPathMotor(int enablePin, int inputAPin, int inputBPin, int HLFBPin, String dir);
    void init();
    void enable();
    void disable();
    void setDirection(String dirString);
    void update(int target);
    void loop();
    void stop();
    void reset();
};

#endif
