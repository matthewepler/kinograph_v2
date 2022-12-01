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

  public:
    ClearPathMotor(int _enable, int A, int B, int HLFB, String _name, String _mode, String _dirStr);
    int target;
    int curr;
    void init();
    void enable();
    void disable();
    void start();
    void stop();
    void reset();
    void setDirection(String newDir);
    void update(int newTarget);
    void loop();
    void debug();
};

#endif
