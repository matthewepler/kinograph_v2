// ClearPathMotor.h
#pragma once
#ifndef ClearPathMotors_h
#define ClearPathMotors_h

#include <Arduino.h>

// Base Class
class ClearPathMotor {
  private:
    int enablePin;
    int inputAPin;
    int inputBPin;
    int HLFBPin;
    String name;
    String mode;
    String dir;
     
    bool enabled;
    bool stopped;
    int error;

  public:
    ClearPathMotor(int _enable, int A, int B, int HLFB, String _name, String _mode, String _dirStr);
    int target;
    void init();
    void enable();
    void disable();
    void setDirection(String newDir);
    void update(int newTarget);
    void loop();
    void stop();
    void reset();
};

#endif
