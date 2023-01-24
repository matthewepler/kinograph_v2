#include "ArduPID.h"




ArduPID myController;




double input;
double output;

// Arbitrary setpoint and gains - adjust these as fit for your project:
double setpoint = 700;
double p = 0.0;
double i = 0;
double d = 0;

const int enable = 20;
const int inputA = 21;
const int inputB = 22;


void setup()
{
  Serial.begin(115200);

  pinMode(enable, OUTPUT);
  pinMode(inputA, OUTPUT);  // dir
  pinMode(inputB, OUTPUT); // speed
  digitalWrite(enable, LOW);
  delay(10);
  digitalWrite(enable, HIGH);
  delay(10);
  digitalWrite(inputA, LOW);
  analogWrite(inputB, 0);
  
  myController.begin(&input, &output, &setpoint, p, i, d);

  myController.reverse()   ;            // Uncomment if controller output is "reversed"
  // myController.setSampleTime(10);      // OPTIONAL - will ensure at least 10ms have past between successful compute() calls
  myController.setOutputLimits(0, 100);
  myController.setBias(100.0 / 2.0);
  myController.setWindUpLimits(-10, 10); // Groth bounds for the integral term to prevent integral wind-up
  
  myController.start();
  // myController.reset();               // Used for resetting the I and D terms - only use this if you know what you're doing
  // myController.stop();                // Turn off the PID controller (compute() will not do anything until start() is called)
}




void loop()
{
  p = map(analogRead(16), 0, 1023, 0, 10);
  input = analogRead(A5); // Replace with sensor feedback

  myController.compute();
  myController.debug(&Serial, "myController", PRINT_INPUT    | // Can include or comment out any of these terms to print
                                              PRINT_OUTPUT   | // in the Serial plotter
                                              PRINT_SETPOINT |
                                              PRINT_BIAS     |
                                              PRINT_P        |
                                              PRINT_I        |
                                              PRINT_D);
  
  analogWrite(inputB, output); // Replace with plant control signal
  delay(10);
}