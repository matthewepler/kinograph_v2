/********************************************************
 * PID Basic Example
 * Reading analog input 0 to control analog PWM output 3
 ********************************************************/

#include <PID_v2.h>

#define PIN_INPUT A5;
#define PIN_OUTPUT 22

const int enable = 20;
const int inputA = 21;
const int inputB = 22;

// Specify the links and initial tuning parameters
double Kp = 2, Ki = 5, Kd = 1;
PID_v2 myPID(Kp, Ki, Kd, PID::Direct);

void setup() {
  pinMode(enable, OUTPUT);
  pinMode(inputA, OUTPUT);  // dir
  pinMode(inputB, OUTPUT); // speed
  digitalWrite(enable, LOW);
  delay(10);
  digitalWrite(enable, HIGH);
  delay(10);
  digitalWrite(inputA, LOW);
  analogWrite(inputB, 0);

  myPID.Start(analogRead(PIN_INPUT),  // input
              0,                      // current output
              700);                   // setpoint

      
}

void loop() {
  const double input = analogRead(PIN_INPUT);
  const double output = myPID.Run(input);
  analogWrite(PIN_OUTPUT, output);
}
