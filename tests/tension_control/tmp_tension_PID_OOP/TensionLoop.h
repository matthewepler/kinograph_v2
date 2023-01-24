// TensionLoop.h
#pragma once
#ifndef TensionLoop_h
#define TensionLoop_h

#include <Arduino.h>

class TensionLoop {
  private:
    int counter;
    int errorTotal;
    int interval;
    int inverted;
    int prevOutput;
    double prevTime;

  public:
    TensionLoop(int _id, int _sensorPin, int _outputPin, int _sensorTarget, bool _inverted);
    int id;
    int sensorPin;
    float sensorTarget;
    int outputPin;
    float gain;

    void adjustTarget(); // TODO
    void calibrate();
    void update();
};

#endif