#include "ClearPathMotor.h"

const int potPin = 14;
ClearPathMotor feedMotor = ClearPathMotor(11, 10, 9, 8, "CCW");


void setup() {
  pinMode(potPin, INPUT);

  feedMotor.enable();
}

void loop() {
  int potVal = analogRead(potPin);
  int mapVal = map(potVal, 15, 680, 255, 0);
  int speedVal = constrain(mapVal, 0, 255);;
  
  feedMotor.update(speedVal);
  feedMotor.loop();
}
