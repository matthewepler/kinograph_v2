/*
 * Test of elapsedMillis type.
 * This funcionality is built-in to the Teensy. See https://www.pjrc.com/teensy/td_timing_elaspedMillis.html
 * For other boards, see this library: https://playground.arduino.cc/Code/ElapsedMillis/
 
 
 */

#include <Bounce.h>

const int buttonPin = 20;
Bounce buttonCtrl = Bounce(buttonPin, 10);

const int timerDuration = 3000;

// optional, if you want to run your 
// "time's up" logic on the first loop.
// in our case, that logic is the "off" state 
// and so we want that to happen ASAP.
elapsedMillis timeAfterButtonPress = timerDuration + 1;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {;
  if (timeAfterButtonPress >= 3000) {
    digitalWrite(LED_BUILTIN, LOW);
  }
  
  buttonCtrl.update();
  if (buttonCtrl.fallingEdge()) {
    Serial.println("Pressed");
    digitalWrite(LED_BUILTIN, HIGH);

    timeAfterButtonPress = 0;
  }
}
