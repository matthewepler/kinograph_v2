#include <ContinuousStepper.h>

const int stepPin = 10;
const int dirPin = 9;
const int enablePin = 8;
ContinuousStepper stepper(stepPin, dirPin, enablePin);
// ^ motor enabled by default

const int potPin = 16;

void setup() {

//  Serial.begin(115200);
//  while (!Serial) {};

  stepper.setPulseWidth(2);  // Increase this value if the motor doesn't move
  stepper.setAcceleration(4000);
  
  // try these one at a time
  // stepper.spin(5000);
  // stepper.setAcceleration(1000);
  // stepper.stop();
  stepper.powerOn();
  // stepper.powerOff();
  // Serial.println(stepper.isSpinning());
  // Serial.println(stepper.speed());
}

void loop() {
  int potVal = analogRead(potPin);
  Serial.println(potVal);
  if (potVal < 10) {
    stepper.stop();
  } else {  
    int speed = (int) map(potVal, 10, 1023, 0, 1000);
    stepper.spin(speed);
  }

  stepper.loop();
  // Serial.print(stepper.isSpinning());
  // Serial.print(" : ");
  // Serial.print(stepper.speed());
  // Serial.println();
}
