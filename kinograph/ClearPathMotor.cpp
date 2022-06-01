// ClearPathMotor.cpp
#include "ClearPathMotor.h"
#include "Arduino.h"

ClearPathMotor::ClearPathMotor(int _enablePin, int _inputAPin, int _inputBPin, int _HLFBPin, String _dirStr){
  enablePin = _enablePin;
  inputAPin = _inputAPin;
  inputBPin = _inputBPin;
  HLFBPin = _inputBPin;
  dir = _dirStr;
  target = 0;

  pinMode(enablePin, OUTPUT);
  pinMode(inputAPin, OUTPUT);
  pinMode(inputBPin, OUTPUT);
  // HLFB LOW = MOTOR DISABLED, HIGH = ENABLED
  pinMode(HLFBPin, INPUT_PULLDOWN); // HLFB+ to MCU Voltage (3.3V for Teensy), HLFB- to DIO pin

 init();
}

void ClearPathMotor::init() {
  digitalWrite(enablePin, LOW);
  while(digitalRead(HLFBPin) == !LOW) {
    delay(20);
  }
  analogWrite(inputBPin, 0);
  setDirection(dir);
  delay(20);
}

void ClearPathMotor::enable() {
  digitalWrite(enablePin, HIGH);
//   while(digitalRead(HLFBPin) == !HIGH) {
//    delay(20);
//  }
  delay(20);
  Serial.println("enabled");
  delay(20);
}

void ClearPathMotor::disable() {
  digitalWrite(enablePin, LOW);
  while(digitalRead(HLFBPin) == !LOW) {
    delay(20);
  }
  Serial.println("disabled");
  delay(20);
}

void ClearPathMotor::setDirection(String dirStr) {
  // HIGH = CW, LOW = CCW
  if (dirStr == "CW") {
    digitalWrite(inputAPin, HIGH);
    Serial.println("dir set to CW");
  } else if (dirStr == "CCW") {
    digitalWrite(inputAPin, LOW);
    Serial.println("dir set to CCW");
  }
  delay(20);
}

void ClearPathMotor::update(int newTarget) {
  target = constrain(newTarget, 0 , 255);
}

void ClearPathMotor::loop() {
  analogWrite(inputBPin, target);
//  if (digitalRead(HLFBPin == LOW)) {
//    analogWrite(inputBPin, target);
//  }
}

void ClearPathMotor::stop() {
  digitalWrite(enablePin, LOW);
}

void ClearPathMotor::reset() {
  digitalWrite(enablePin, LOW);
  delay(15);
  digitalWrite(enablePin, HIGH);
  delay(15);
  while(digitalRead(HLFBPin == !LOW)) {
    delay(20);
  }
  digitalWrite(enablePin, LOW);
}
