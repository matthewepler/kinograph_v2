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
const int sysPausedCtrlPin = 36;
const int buttonPin = 20;
Bounce sysEnabledCtrl = Bounce(sysEnabledCtrlPin, 10);
Bounce sysDirCtrl = Bounce(sysDirCtrlPin, 10);
Bounce sysPausedCtrl = Bounce(sysPausedCtrlPin, 10);
Bounce buttonCtrl = Bounce(buttonPin, 10);


 /* System State */
  // For safety, the state of manual control switches are ignored
  // on startup so that it doesn't start right away. If either of 
  // the manual controls for these are "on," they will need to be
  // toggled off and then on again to start working.
volatile bool sysEnabled = false; // tension is on
volatile bool sysPaused = true; // tension is on but film is not moving

/* Motors */
volatile bool manSpeedCtrl = true; // speed set with hardware control
const int manSpeedAdjPin = 14;     // manual speed adjustment via hardware (knob, pedal, etc)
ClearPathMotor capstanMotor = ClearPathMotor(11, 10, 9, 8, "CAPSTAN", "VEL", "CCW");
ClearPathMotor leftMotor = ClearPathMotor(3, 2, 1, 0, "LEFT", "TOR", "CW");
ClearPathMotor rightMotor = ClearPathMotor(7, 6, 5, 4, "RIGHT", "TOR", "CCW");

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
  pinMode(sysPausedCtrlPin, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);

  /* PERF DETECTOR */
  pinMode(perfSignalPin, INPUT);
  attachInterrupt(perfSignalPin, readSprocket, FALLING);

  /* CAMERA */
  pinMode(camTriggerPin, OUTPUT);
  digitalWrite(camTriggerPin, LOW);
}

void loop() {
  readControls();
  checkTimers();
  
  if (sysEnabled) {
    leftMotor.loop();
    rightMotor.loop();
    capstanMotor.loop();

    if (manSpeedCtrl && !sysPaused) {
      capstanMotor.update(analogRead(manSpeedAdjPin));
    }
  }

  
  // Debugging options
//  printSysState();
//  printPerfs();
//  printTimers();
//  capstanMotor.debug();
//  leftMotor.debug();
}

void readControls() {
  // -- SYSTEM ENABLED -- //
  // If machine is enabled on startup, it is ignored and
  // you must toggle the control to turn it on
  sysEnabledCtrl.update();
  if (sysEnabledCtrl.risingEdge()) {
//    Serial.println("RISING");
    // HIGH = OFF
    leftMotor.disable();
    rightMotor.disable();    capstanMotor.stop(); // TODO << this should be unecessary
    capstanMotor.disable();
    sysEnabled = false;
    sysPaused = true;
  } else if (sysEnabledCtrl.fallingEdge()) {
//    Serial.println("FALLING");
    // LOW = ON
    leftMotor.enable();
    rightMotor.enable();
    capstanMotor.enable();
    sysEnabled = true;
    sysPaused = true; // never want "enable" to be the go button
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

  // -- SYSTEM PAUSE/START -- //
  // If machine is on "START" @ startup, it is ignored and
  // you must toggle the control to turn it ong
  sysPausedCtrl.update();
  if (sysPausedCtrl.fallingEdge()) {
//    Serial.println("FALLING = UN-PAUSE");
    // LOW = GO
    sysPaused = false;
    capstanMotor.start();
  } else if(sysPausedCtrl.risingEdge()) {
//    Serial.println("RISING = PAUSE");
    // HIGH = STOP
    sysPaused = true;
    capstanMotor.stop();
  }
  // -- AUX BUTTON -- /
  buttonCtrl.update();
  if (buttonCtrl.fallingEdge()) {
    if (sysPaused) {
      capstanMotor.reset();
      leftMotor.reset();
      rightMotor.reset();
    } else {
      perfCounter++;
    }
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
void printSysState() {
  Serial.print("sysEnabled: ");
  Serial.print(sysEnabled);
  Serial.print(", ");
  Serial.print("sysStopped: ");
  Serial.print(sysPaused);
  Serial.println();
  delay(2);
}

void printPerfs() {
  Serial.println(perfCounter);
  if (frameCounter != prevFrameCount) {
    Serial.print("******** ");
    Serial.print(frameCounter);
    Serial.println(" ********");
    Serial.println();
    delay(2);
    prevFrameCount = frameCounter;
  }
}

void printTimers() {
  Serial.println((unsigned long)timeSinceCamTrigger);
  delay(2);
}
