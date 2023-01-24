// TensionLoop.cpp
#include "TensionLoop.h"
#include "Arduino.h"

TensionLoop::TensionLoop(
  int _id,
  int _sensorPin,
  int _outputPin,
  int _sensorTarget,
  bool _inverted
){
  id = _id;
  sensorPin = _sensorPin;
  outputPin = _outputPin;
  sensorTarget = _sensorTarget;
  inverted = _inverted;

  interval = 100; //ms
  errorTotal = 0; 
  gain = 0.0;
  prevOutput = 0;
  prevTime = 0.0;
  counter = 1;
}

void TensionLoop::update() {
  if (millis() - prevTime < interval) {
    // between intervals, accumulate readings so we can average them later.
    int sensorVal = analogRead(sensorPin);
    int error = sensorTarget - sensorVal;
    if (inverted) error *= -1;
    errorTotal += error;
    counter++;

  } else {
    // use readings average to calc output signal strength
    int errorAvg = floor(errorTotal / counter);
    int gain = map(errorAvg, 0.0, 17.0, 0.4, 1.0); 
    int correction = (int) errorAvg * gain;
    int output = prevOutput + constrain(correction, 0, 10);
    
    Serial.print(id);
    Serial.print(" : ");
    Serial.print(errorAvg);
    Serial.print(" : ");
    Serial.print(output);
    Serial.print("  ");
    
    analogWrite(outputPin, constrain(output, 0, 40));
    prevOutput = output;
    errorTotal = 0;
    counter = 1;
    prevTime = millis();
  }
  
  delay(10); // keep serial from clogging
}

void TensionLoop::adjustTarget() {
  // TODO. Use encoder (++ or --)
}

int minSensorVal = 1000;
int maxSensorVal = 0;
void TensionLoop::calibrate() {
  int sensorVal = analogRead(sensorPin);
  if (sensorVal < minSensorVal) minSensorVal = sensorVal;
  if (sensorVal > maxSensorVal) maxSensorVal = sensorVal;

  Serial.print(minSensorVal);
  Serial.print(" : ");
  Serial.println(maxSensorVal);
}

