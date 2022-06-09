// ClearPathMotor.cpp
#include "ClearPathMotor.h"
#include "Arduino.h"

const int delayTime = 20; // ms

ClearPathMotor::ClearPathMotor(int _enable, int A, int B, int HLFB, String _name, String _mode, String _dir){
  enablePin = _enable;
  inputAPin = A;
  inputBPin = B;
  HLFBPin = HLFB; // "Servo On" = enabled and no errors
  name = _name;   // used for debugging
  mode = _mode;
  dir = _dir;
  enabled = false; // enabled, will respond to commands
  stopped = true;  // enabled without move commands
  error = 0;
  target = 0;

  pinMode(enablePin, OUTPUT);
  pinMode(inputAPin, OUTPUT);
  pinMode(inputBPin, OUTPUT);
  // HLFB LOW = MOTOR DISABLED, HIGH = ENABLED -- NOT WORKING AS EXPECTED, IGNORED
  pinMode(HLFBPin, INPUT_PULLUP); 

 init();
}

void ClearPathMotor::init() {
  digitalWrite(enablePin, LOW);
  delay(delayTime);
  analogWrite(inputBPin, 0);
  setDirection(dir);
}

void ClearPathMotor::enable() {
  digitalWrite(enablePin, HIGH);
  delay(delayTime);
  enabled = true;
}

void ClearPathMotor::disable() {
  // will not respond to commands
  stop();
  digitalWrite(enablePin, LOW);
  delay(delayTime);
  enabled = false;
}

void ClearPathMotor::setDirection(String newDir) {
  // HIGH = CW, LOW = CCW
  if (newDir == "CW") {
    digitalWrite(inputAPin, HIGH);
  }
  if (newDir == "CCW") {
    digitalWrite(inputAPin, LOW);
  }
  dir = newDir;
  delay(delayTime);
}

void ClearPathMotor::update(int newTarget) {
  if(mode == "VELOCITY") {
    // max speed set in ClearPath software. 255 = 100% of that value.
    int adjustedSpeedVal = map(newTarget, 680, 15, 0, 200);
    target = constrain(adjustedSpeedVal, 0 , 200);
  }
  if(mode == "TORQUE") {\
    // by percent of max torque
    target = floor((newTarget/100.0) * 255);
  }
}

void ClearPathMotor::loop() {
  if (enabled && !error) {
    analogWrite(inputBPin, target);
  }
}

void ClearPathMotor::stop() {
  update(0);
  stopped = true;
}

void ClearPathMotor::reset() {
  // toggle enable pin to clear error state
  digitalWrite(enablePin, LOW);
  delay(delayTime);
  
  digitalWrite(enablePin, HIGH);
  delay(delayTime);
  
  digitalWrite(enablePin, LOW);
  delay(delayTime);

  analogWrite(inputBPin, 0);
  enabled = false;
  stopped = true;
  error = 0;
  target = 0;
}
