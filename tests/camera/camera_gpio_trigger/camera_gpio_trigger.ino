/*
 * TODO: license, etc.
 * 
 * Simple test to confirm camera is triggered on 
 * digital signal. Actual implementation may be different. 
 * 
 * Circuit for FLIR 6 pin camera:
 * 
 * RED -> PIN (INPUT), LOW = Trigger
 * BROWN -> 1K res -> GND
 */

#include <Bounce.h>

const int buttonPin = 20;
const int triggerPin = 32;

Bounce buttonCtrl = Bounce(buttonPin, 10);


void setup() {
  pinMode(triggerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(triggerPin, HIGH);
}

void loop() {
  buttonCtrl.update();
  if (buttonCtrl.fallingEdge()) {
    Serial.println("falling");
    digitalWrite(triggerPin, LOW);
    digitalWrite(LED_BUILTIN, HIGH);
    delayMicroseconds(100);
    digitalWrite(triggerPin, HIGH);
    digitalWrite(LED_BUILTIN, LOW);
  }
}
