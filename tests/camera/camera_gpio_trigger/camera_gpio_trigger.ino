/*
 * TODO: license, etc.
 * 
 * Simple test to confirm camera is triggered on 
 * digital signal. Actual implementation may be different. 
 * 
 * Circuit for FLIR 6 pin camera:
 * 
 * Red + Brown = Line 2 (Flir Blackfly)
 * RED -> PIN (INPUT_PULLUP, 3.3 or 5V)
 * BROWN -> 1K res -> GND
 */

#include <Bounce.h>

const int buttonPin = 22;
const int triggerPin = 15;

Bounce buttonCtrl = Bounce(buttonPin, 20);


void setup() {
  pinMode(triggerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(triggerPin, HIGH);
}

void loop() {
  buttonCtrl.update();
  if(buttonCtrl.risingEdge()) {
    Serial.println("rising");
  }
  if (buttonCtrl.fallingEdge()) {
    Serial.println("falling");
    digitalWrite(triggerPin, LOW);
    digitalWrite(LED_BUILTIN, HIGH);
    delayMicroseconds(100);
    digitalWrite(triggerPin, HIGH);
    digitalWrite(LED_BUILTIN, LOW);
  }
}
