/*
   TODO
   ----
   replace dir switch with two-position switch
   controls class
   add control update() for software input
   implement error handling for motors
   if anything is on that shouldn't be, refuse to start\
   conditional loading of libs
    e.g. "ellapsed" timers built-in to Teensyduino,
    if !Teensy && Arduino: https://playground.arduino.cc/Code/ElapsedMillis/
    example: https://github.com/SofaPirate/Chrono/blob/master/Chrono.cpp
  when using manual control, play/stop is not necessary
  if manual...no missing perf detection because speed is not static
*/

#include "ClearPathMotor.h"
#include <Bounce.h>


/* Controls */
const int sysEnabledCtrlPin = 33;
const int sysDirCtrlPin = 35;
const int sysStartStopCtrlPin = 36;
const int buttonPin = 20;
Bounce sysEnabledCtrl = Bounce(sysEnabledCtrlPin, 10);
Bounce sysDirCtrl = Bounce(sysDirCtrlPin, 10);
Bounce sysStartStopCtrl = Bounce(sysStartStopCtrlPin, 10);
Bounce buttonCtrl = Bounce(buttonPin, 10);



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
const int perfSignalPin = 37;
int perfCounter = 1;
int frameCounter = 0;
int prevFrameCount;

/* Camera */
const int camTriggerPin = 32;
const int camTriggerDuration = 50;
elapsedMillis timeSinceCamTrigger = camTriggerDuration + 1;  // see "elapsedMillis" test

void setup() {
  /* CONTROL PANEL */
  pinMode(manSpeedAdjPin, INPUT);
  pinMode(sysEnabledCtrlPin, INPUT_PULLUP);
  pinMode(sysDirCtrlPin, INPUT_PULLUP);
  pinMode(sysStartStopCtrlPin, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);

  /* PERF DETECTOR */
  pinMode(perfSignalPin, INPUT);
  attachInterrupt(perfSignalPin, readSprocket, FALLING);

  /* SYSTEM STATE */
  sysEnabled = false;
  sysStopped = true; // even if the control is not @ "STOP" position;

  /* MOTORS -- temp */
  leftMotor.update(10);
  rightMotor.update(5);

  /* CAMERA */
  pinMode(camTriggerPin, OUTPUT);
  digitalWrite(camTriggerPin, LOW);
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
  checkTimers();

  // Debugging options
//  printState();
//  printPerfs();
//  printTimers();
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
  buttonCtrl.update();
  if (buttonCtrl.fallingEdge()) {
    Serial.println("BUTTON PRESSED");
    perfCounter++;
  }
}


/* PERF DETECTION ISR */
void readSprocket() {
  if (perfCounter % 4 == 0) {
    triggerCamera();
    perfCounter = 1;
  } else {
    perfCounter++;
  }
}

/* CAMERA TRIGGER */
void triggerCamera() {
  digitalWrite(camTriggerPin, LOW);
  timeSinceCamTrigger = 0;
  digitalWrite(LED_BUILTIN, HIGH);
  frameCounter ++;
}

void checkTimers() {
  if (timeSinceCamTrigger >= camTriggerDuration) {
    digitalWrite(camTriggerPin, HIGH);
    digitalWrite(LED_BUILTIN, LOW);
  }
}


/* DEBUGGING STUFF */
void printState() {
//  Serial.print("sysEnabled: ");
//  Serial.print(sysEnabled);
//  Serial.print(", ");
//  Serial.print("sysStopped: ");
//  Serial.print(sysStopped);
}

void printPerfs() {
  Serial.println(perfCounter);
  if (frameCounter != prevFrameCount) {
    Serial.print("******** ");
    Serial.print(frameCounter);
    Serial.println(" ********");
    delay(2);
    prevFrameCount = frameCounter;
  }
}

void printTimers() {
  Serial.println((unsigned long)timeSinceCamTrigger);
}
