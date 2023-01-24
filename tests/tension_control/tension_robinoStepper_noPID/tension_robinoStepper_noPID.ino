#include "Config.h"
#include "ContinuousStepper.h"

ContinuousStepper stepper(stepPin, dirPin, enablePin);

// "f" = feed motor
// const int f_enable = 34; // using kill switch
const int f_inputA = 35;
const int f_inputB = 36;
const int potPin = 16;

// "t" = take-up motor
// const int t_enable = 20; // using kill switch
const int t_inputA = 21;
const int t_inputB = 22;

// "c" = capstan motor
// -- capstan motor
// const int c_enable = 7;
const int c_inputA = 6;
const int c_inputB = 5;

const int speedPotPin = 16;
const int f_tensionPin = 18;
const int t_tensionPin = 17;

void setup() {
  Serial.begin(115200);
  while (!Serial) {};
  initMotors();
};


void loop() {
  
  int speedVal =analogRead(speedPotPin);
  int f_tensionVal = analogRead(f_tensionPin);
  int t_tensionVal = analogRead(t_tensionPin);

  if (speedVal < 10) {
    stepper.stop();
  } else {  
    int speed = (int) map(speedVal, 10, 1023, 0, 1000);
    // Serial.print(" : ");
    // Serial.println(speed);
    stepper.spin(speed);
  }
  stepper.loop();

  int t_tension = map(t_tensionVal, 0, 1023, 0, 20);
  analogWrite(t_inputB, t_tension);

  int f_tension = map(f_tensionVal, 0, 1023, 0, 20);
  analogWrite(f_inputB, f_tension);
}

void initMotors() {
  // -- feed motor
  pinMode(f_inputA, OUTPUT);
  pinMode(f_inputB, OUTPUT);
  // pinMode(f_enable, OUTPUT);

  // -- take-up motor
  pinMode(t_inputA, OUTPUT);
  pinMode(t_inputB, OUTPUT); 
  // pinMode(t_enable, OUTPUT);
 
  // -- capstan motor
  stepper.powerOn();
  stepper.setPulseWidth(2);  // Increase this value if the motor doesn't move
  stepper.setAcceleration(4000);

  // set direction. HIGH = CW
  digitalWrite(f_inputA, HIGH);
  digitalWrite(t_inputA, LOW);
  digitalWrite(c_inputA, LOW);

  // set all motors to 0 torque/speed
  digitalWrite(f_inputB, 0);
  digitalWrite(t_inputB, 0);
  stepper.stop();
}


