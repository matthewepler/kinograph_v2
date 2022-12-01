// ClearPathMotor.cpp
#include "ClearPathMotor.h"
#include "Arduino.h"

const int delayTime = 20;
const int defaultTorqueLeft = 10;
const int defaultTorqueRight = 10;

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
  target = 0; // target output value
  curr = 0;   // current output value

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
  curr = 0;
  
  if (mode == "VEL") {
    target = 0;
  } else if (mode == "TOR") {
    // set default tension for both left and right motors
    target = name.indexOf("LEFT") > -1 ? defaultTorqueLeft : defaultTorqueRight;
  }
 
}

void ClearPathMotor::enable() {
  digitalWrite(enablePin, HIGH);
  // TODO: make this a while loop with an actual read so we can confirm the state
  delay(delayTime);
  enabled = true;

  if (mode == "TOR") {
    // Torque motors are either enabled or disabled, on/off
    stopped = false;
  }
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
  if(!enabled || stopped) return;

  
// TODO: If abs(newTarget - target) > X, lerp. save as target


  if(mode == "VEL") {
    // Velocity mode. Max rpm set in ClearPath software. 
    int adjustedSpeedVal = map(newTarget, 690, 25, 0, 200);
    target = constrain(adjustedSpeedVal, 0 , 200);;
  } else if(mode == "TOR") {      // TORQUE MODE
    // by percent of max torque
    target = floor((newTarget/100.0) * 255);
  }
}

void ClearPathMotor::start() {
  if (mode == "VEL") {
    // Capstan should ramp up from 0 (see loop)
    update(0);
    stopped = false;
  } else if (mode == "TOR") {
    // Not needed, but if you want to use it, go for it.
    loop();
  }
}

void ClearPathMotor::stop() {
  update(0);
  stopped = true;
}

void ClearPathMotor::loop() {
  // TODO: check abs(curr - target) and lerp/save to curr
  if(!enabled || stopped) return;
  analogWrite(inputBPin, target);
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
  target = 0;
}

void ClearPathMotor::debug() {
  // NAME //
  Serial.print("Name: ");
  Serial.print(name);
  Serial.print("\t");
  
  // MODE //
  Serial.print("Mode: ");
  Serial.print(mode);
  Serial.print("\t");
    
  // STATE // 
  Serial.print("Dir: ");
  Serial.print(dir);
  Serial.print("\t");
  Serial.print("Enabled: ");
  Serial.print(enabled);
  Serial.print("\t");
  Serial.print("Stopped: ");
  Serial.print(stopped);
  Serial.print("\t");
  Serial.print("Target: ");
  Serial.print(target);
  Serial.print("\t");

  // PINS //
  Serial.print("enablePin: ");
  Serial.print(enablePin);
  Serial.print(", ");
  Serial.print(digitalRead(enablePin));
  Serial.print("\t");
  Serial.print("inputAPin: ");
  Serial.print(inputAPin);
  Serial.print(", ");
  Serial.print(digitalRead(inputAPin));
  Serial.print("\t");
  Serial.print("inputBPin: ");
  Serial.print(inputBPin);
  Serial.print(", ");
  Serial.print(digitalRead(inputBPin));
  Serial.print("\t");
  Serial.print("HLFBPin: ");
  Serial.print(HLFBPin);
  Serial.print(", ");
  Serial.print(digitalRead(HLFBPin));
  Serial.print("\t");

  Serial.println();
  delay(10);
}
