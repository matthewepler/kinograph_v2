#include "TensionLoop.h"
 
// -- feed motor
// const int f_enable = 34; // using kill switch
const int f_inputA = 35;
const int f_inputB = 36;
TensionLoop feedLoop = TensionLoop(0, A14, 36, 158, false);

// -- takeup motor
// const int t_enable = 20; // using kill switch
const int t_inputA = 21;
const int t_inputB = 22;
TensionLoop takeUpLoop = TensionLoop(1, A5, 22, 450, true);

// -- capstan motor
const int c_enable = 7;
const int c_inputA = 6;
const int c_inputB = 5;

const int speedPotPin = A2;
int numSamples = 10;


void setup() {
  Serial.begin(115200);
  initMotors();
}

void loop() {
  // disable loop updating if calibrating.
  // feedLoop.calibrate();
  // takeUpLoop.calibrate();

  feedLoop.update();
  takeUpLoop.update();


  float speedVal = map(analogRead(speedPotPin), 0, 1023, -5, 200);
  analogWrite(c_inputB, floor(speedVal));

  Serial.println();
}


void initMotors() {
  // -- feed motor
  pinMode(f_inputA, OUTPUT);
  pinMode(f_inputB, OUTPUT);
  // pinMode(t_enable, OUTPUT);

  // -- take-up motor
  pinMode(t_inputA, OUTPUT);
  pinMode(t_inputB, OUTPUT); 
  // pinMode(f_enable, OUTPUT);
 
  // -- capstan motor
  pinMode(c_inputA, OUTPUT);
  pinMode(c_inputB, OUTPUT);
  pinMode(c_enable, OUTPUT);
  digitalWrite(c_enable, LOW);
  delay(10);
  digitalWrite(c_enable, HIGH);
  delay(10);
  

  // set direction. HIGH = CW
  digitalWrite(f_inputA, HIGH);
  digitalWrite(t_inputA, LOW);
  digitalWrite(c_inputA, LOW);

  // set all motors to 0 torque/speed
  digitalWrite(f_inputB, 0);
  digitalWrite(t_inputB, 0);
  digitalWrite(c_inputB, 0);

}
