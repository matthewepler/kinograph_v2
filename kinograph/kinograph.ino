/*
 * TODO
 * ----
 * replace dir switch with two-position switch
 * abstract controls
 * add control update() for software input
 */
 
#include "ClearPathMotor.h"

/* Controls */
const int tensionPin = 33; // grey, turns tension on/off
const int dirPin = 35;     // blue, left-to-right or right-to-right
const int goStopPin = 36;  // yellow, start/stop scanning
bool tensionOn;
int moveDir;               // 0 = left to right, 1 = right to left
bool stopped;

/* Motors */
bool useSpeedPreset = false;       // presets are in frames-per-second 
const int manSpeedAdjPin = 14;     // manual speed adjustment via hardware (knob, pedal, etc)
ClearPathMotor capstanMotor = ClearPathMotor(11, 10, 9, 8, "CCW");


void setup() {
  /* Controls */
  pinMode(tensionPin, INPUT_PULLUP);
  pinMode(dirPin, INPUT_PULLUP);
  pinMode(goStopPin, INPUT_PULLUP);
  
  /* Motors */
  pinMode(manSpeedAdjPin, INPUT);
  capstanMotor.enable();
}

void loop() {
//  int potVal = analogRead(potPin);
//  int mapVal = map(potVal, 15, 680, 255, 0);
//  int speedVal = constrain(mapVal, 0, 255);;
//  
//  feedMotor.update(speedVal);
//  feedMotor.loop();

  checkControls();

  Serial.println();
  delay(100);
}

void checkControls() {
  int tensionPinVal = digitalRead(tensionPin); // HIGH = OFF, LOW = ON
  int dirPinVal = digitalRead(dirPin);        // HIGH = LEFT-TO-RIGHT, LOW = RIGHT-TO-LEFT
  int goStopPinVal = digitalRead(goStopPin);   // HIGH = STOP, LOW = GO  

  tensionOn = tensionPinVal == HIGH ? false : true;
  moveDir = dirPinVal == HIGH ? 0 : 1;
  stopped = goStopPinVal == HIGH ? true : false;

  Serial.print(tensionPinVal);
  Serial.print(", ");
  Serial.print(dirPinVal);
  Serial.print(", ");
  Serial.print(goStopPinVal);
  Serial.print("\t");
  Serial.print("tensionOn: ");
  Serial.print(tensionOn);
  Serial.print(", moveDir: ");
  Serial.print(moveDir);
  Serial.print(", stopped: ");
  Serial.print(stopped);
  
}
