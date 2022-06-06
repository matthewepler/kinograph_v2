/*
   TODO
   ----
   replace dir switch with two-position switch
   controls class
   add control update() for software input
   implement error handling for motors
   if anything is on that shouldn't be, refuse to start
*/

#include "ClearPathMotor.h"
#include <Bounce.h>

const bool DEBUG = false;

/* Controls */
const int sysEnabledCtrlPin = 33;
const int sysDirCtrlPin = 35;
const int sysStartStopCtrlPin = 36;
Bounce sysEnabledCtrl = Bounce(sysEnabledCtrlPin, 10);
Bounce sysDirCtrl = Bounce(sysDirCtrlPin, 10);
Bounce sysStartStopCtrl = Bounce(sysStartStopCtrlPin, 10);


/* State */
volatile bool sysEnabled;
volatile bool sysStopped;

/* Motors */
volatile bool manSpeedCtrl = true; // speed set with hardware control
const int manSpeedAdjPin = 14;     // manual speed adjustment via hardware (knob, pedal, etc)
ClearPathMotor capstanMotor = ClearPathMotor(11, 10, 9, 8, "CAPSTAN", "VELOCITY", "CCW");
ClearPathMotor leftMotor = ClearPathMotor(3, 2, 1, 0, "LEFT", "TORQUE", "CW");
ClearPathMotor rightMotor = ClearPathMotor(7, 6, 5, 4, "RIGHT", "TORQUE", "CCW");

/* PERF DETECTOR */
const int perfSignalPin = 41;

void setup() {
  /* CONTROL PANEL */
  pinMode(manSpeedAdjPin, INPUT);
  pinMode(sysEnabledCtrlPin, INPUT_PULLUP);
  pinMode(sysDirCtrlPin, INPUT_PULLUP);
  pinMode(sysStartStopCtrlPin, INPUT_PULLUP);

  /* PERF DETECTOR */
  pinMode(perfSignalPin, INPUT);
  attachInterrupt(perfSignalPin, readSprocket, FALLING);

  /* SYSTEM STATE */
  sysEnabled = false;
  sysStopped = true; // even if the control is not @ "STOP" position;

  /* MOTORS -- temp */
  leftMotor.update(10);
  rightMotor.update(5);
}

void loop() {
  if (sysEnabled) {
    leftMotor.loop();
    rightMotor.loop();
    if (sysStopped) {
      capstanMotor.stop();
    } else {
      if (manSpeedCtrl) {
        capstanMotor.update(analogRead(manSpeedAdjPin));
      }
      capstanMotor.loop();
    }
  }

  readControls();

//  Serial.println(analogRead(manSpeedAdjPin));

//  Serial.print("sysEnabled: ");
//  Serial.print(sysEnabled);
//  Serial.print(", ");
//  Serial.print("sysStopped: ");
//  Serial.print(sysStopped);

  Serial.print(leftMotor.target);
  
  Serial.println();
  delay(100);
}

void initControls() {
  
}

void readControls() {
  // -- SYSTEM ENABLED -- //
  sysEnabledCtrl.update();
  if (sysEnabledCtrl.risingEdge()) {
//    Serial.println("RISING");
    // HIGH = OFF
    leftMotor.disable();
    rightMotor.disable();
    capstanMotor.disable();
    sysEnabled = false;
  } else if (sysEnabledCtrl.fallingEdge()) {
//    Serial.println("FALLING");
    // LOW = ON
    leftMotor.enable();
    rightMotor.enable();
    capstanMotor.enable();
    sysEnabled = true;
  }
  // -- SYSTEM DIRECTION -- //
  sysDirCtrl.update();
  if (sysDirCtrl.fallingEdge()) {
    // LOW = LEFT-TO-RIGHT (state 0)
    capstanMotor.setDirection("CCW");
  } else if (sysDirCtrl.risingEdge()) {
    // HIGH = RIGHT-TO-LEFT (state 1)
    capstanMotor.setDirection("CW");
  }

  // -- SYSTEM START/STOP -- //
  sysStartStopCtrl.update();
  if (sysStartStopCtrl.fallingEdge()) {
    // LOW = GO
    sysStopped = false;
  } else if (sysStartStopCtrl.risingEdge()) {
    // HIGH = STOP
    capstanMotor.stop();
    sysStopped = true;
  }
}
