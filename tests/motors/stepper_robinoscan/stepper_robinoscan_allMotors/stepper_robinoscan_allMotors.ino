#include "Config.h"
#include "ContinuousStepper.h"
#include <Encoder.h>

/*
  Testing and tuning sketch for tension feedback loop. 
  Test one tension zone at a time. For example, feed -> capstan, and capstan -> take-up. 
 
  A knob (potentiometer) is used to change the target tension value. The min/max of the knob
  should match the min/max tension of your machine. In other words, use map()
*/

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
int f_setpoint = 138;
int f_output = 30;
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
int t_setpoint = 487;
int t_output = 20;
int t_gain = 0.0;
const int t_gainPotPin = A3;
int t_gainTotal;
int t_errTotal;

// -- capstan motor
ContinuousStepper stepper(stepPin, dirPin, enablePin);
int s_output;

const int speedPotPin = A2; // A2 = 16
int interval = 1000; // ms
int sampleCounter = 1;
int f_total, t_total, s_total;


void setup() {
  Serial.begin(115200);  
  initMotors();
}

float gain = 1.0;
double prevtime;
int margin = 3;

void loop() {

  if (millis() - prevtime < interval) {

    float speedVal = analogRead(speedPotPin);
    int f_val = map(analogRead(18), 0, 1023, 0, 50);
    int t_val = map(analogRead(17), 0, 1023, 0, 50);

    s_total += speedVal;
    f_total += f_val;
    t_total += t_val;
    sampleCounter ++;

  } else {
    f_output = floor(f_total / sampleCounter);
    t_output = floor(t_total / sampleCounter);
    s_output = floor(s_total / sampleCounter);


    sampleCounter = 1;
    f_total = 0;
    t_total = 0;
    s_total = 0;
    prevtime = millis();
  }
  
  analogWrite(f_inputB, f_output);
  analogWrite(t_inputB, t_output);
  int speed = (int) map(s_output, 0, 1023, 0, 1000);
  stepper.spin(speed);
  stepper.loop();

  Serial.print(f_output);
  Serial.print(" : ");
  Serial.print(t_output);  
  Serial.print(" : ");
  Serial.println(speed);

  // delay(10);
}


void initMotors() {
  // -- feed motor
  pinMode(f_inputA, OUTPUT);
  pinMode(f_inputB, OUTPUT);
  // pinMode(f_enable, OUTPUT); // comment out if using switch

  // -- take-up motor
  pinMode(t_inputA, OUTPUT);
  pinMode(t_inputB, OUTPUT); 
  // pinMode(t_enable, OUTPUT); // comment out if using switch
 
  // -- capstan motor
  stepper.setPulseWidth(2);  // Increase this value if the motor doesn't move
  stepper.setAcceleration(4000);
  stepper.powerOn();
  stepper.setDir(LOW); // HIGH = CW, LOW = CCW

  // set direction. HIGH = CW
  digitalWrite(f_inputA, HIGH);
  digitalWrite(t_inputA, LOW);

  // set all motors to 0 torque/speed
  digitalWrite(f_inputB, 0);
  digitalWrite(t_inputB, 0);
}
