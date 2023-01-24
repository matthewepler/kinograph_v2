/*
  Testing and tuning sketch for tension feedback loop. 
  Test one tension zone at a time. For example, feed -> capstan, and capstan -> take-up. 
 
  A knob (potentiometer) is used to change the target tension value. The min/max of the knob
  should match the min/max tension of your machine. In other words, use map()
*/

#include "Config.h"
#include "ContinuousStepper.h"

// tension sensor
const int sensorPin = A5;
float gainVal = .1;
int target;
// const bool inverted = true; // true if sensor reads 1023 at rest, false if 0.

// -- feed motor
// const int f_enable = 34; // using kill switch
const int f_inputA = 35;
const int f_inputB = 36;
const int f_sensor = A14; // A14 = 38
int f_setpoint = 135;
// int f_output = 0;
int f_prevOutput = 0;
float f_gain = 0.0;
const int f_gainPotPin = 18; // A4 = 18
float f_gainTotal = 0.0;
int f_errTotal = 0;

// -- takeup motor
// const int t_enable = 20; // using kill switch
const int t_inputA = 21;
const int t_inputB = 22;
const int t_sensor = A5 ;// A5 = 19
int t_setpoint = 475;
// int t_output = 0;
int t_prevOutput = 0;
float t_gain = 0.0;
// const int t_gainPotPin = 17; // A3 = 17
int t_gainTotal;
int t_errTotal;

// -- capstan motor
// const int c_enable = 7;
// const int c_inputA = 6;
// const int c_inputB = 5;
ContinuousStepper stepper(stepPin, dirPin, enablePin);
int s_output;

const int speedPotPin = A2;
float speedSampleTotal = 0; 

int sampleCounter = 1;
int numSamples = 8;


void setup() {
  Serial.begin(115200);
  initMotors();
}

void loop() {  
  // TODO: enter this mode if the reading is way off 
  // really we're talking about proportional gain here. gain is proportional to diff between setpoint and target. Big diff, smaller gain to compensatge?;

  if (sampleCounter < numSamples) {
    sampleCounter += 1;

    float speedVal = map(analogRead(speedPotPin), 0, 1023, 0, 1000);
    speedSampleTotal += speedVal;

    int f_sensorVal = analogRead(f_sensor);
    int f_err = f_setpoint - f_sensorVal;
    f_errTotal += f_err;

    int t_sensorVal = analogRead(t_sensor);
    int t_err = t_sensorVal - t_setpoint;
    t_errTotal += t_err;

    // adjust gain manually
    // f_gain = (float) map(analogRead(f_gainPotPin), 0, 1023, 30, 100.0)/100.0;
    // t_gain = (float) map(analogRead(t_gainPotPin), 0, 1023, 30, 100.0)/100.0;

  } else {

    int speedVal = (int) speedSampleTotal / numSamples;
    // analogWrite(c_inputB, floor(speedVal));
    stepper.spin(speedVal);

    int f_errAvg = floor(f_errTotal / numSamples);
    float f_gain = map(f_errAvg, 0.0, 14.0, 20, 70.0)/100.0; // proportional gain
    int f_correct = (int) f_errAvg * f_gain;
    int f_output = f_prevOutput + f_correct;
    f_output = f_prevOutput + constrain(f_correct, 0, 10);
    analogWrite(f_inputB, constrain(f_output, 0, 40));
    f_prevOutput = f_output;

    int t_errAvg = floor(t_errTotal / numSamples);
    float t_gain = map(t_errAvg, 0.0, 14.0, 20, 70.0)/100.0; // proportional gain
    int t_correct = (int) t_errAvg * t_gain;
    int t_output = t_prevOutput + t_correct;
    t_output = t_prevOutput + constrain(t_correct, 0, 10);
    analogWrite(t_inputB, constrain(t_output, 0, 40));
    t_prevOutput = t_output;

    Serial.print(f_errAvg);
    Serial.print(" : ");
    Serial.println(f_gain);

    sampleCounter = 1;
    speedSampleTotal = 0;
    f_errTotal = 0;
    f_gainTotal = 0;    
    t_errTotal = 0;
    t_gainTotal = 0;
  }
  stepper.loop();
  
  // if (debug) {
  //   Serial.print(f_errAvg);
  //   Serial.print(" : ");
  //   Serial.print(f_gain);
  //   Serial.print(" : ");
  //   Serial.print(f_correct);
  //   Serial.print(" : ");
  //   Serial.print(constrain(f_output, 0, 60));
  //   Serial.print(" : ");
  //   Serial.print(f_setpoint);
  //   Serial.print("/");
  //   Serial.println(analogRead(f_sensor));
  // }

  // delay(10);
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
  // pinMode(c_inputA, OUTPUT);
  // pinMode(c_inputB, OUTPUT);
  // pinMode(c_enable, OUTPUT);
  // digitalWrite(c_enable, LOW);
  // delay(10);
  // digitalWrite(c_enable, HIGH);
  // delay(10);
  stepper.setPulseWidth(2);  // Increase this value if the motor doesn't move
  stepper.setAcceleration(4000);
  stepper.powerOn();
  stepper.setDir(LOW); // HIGH = CW, LOW = CCW
  

  // set direction. HIGH = CW
  digitalWrite(f_inputA, HIGH);
  digitalWrite(t_inputA, LOW);
  // digitalWrite(c_inputA, LOW);

  // set all motors to 0 torque/speed
  digitalWrite(f_inputB, 0);
  digitalWrite(t_inputB, 0);
  // digitalWrite(c_inputB, 0);

}
