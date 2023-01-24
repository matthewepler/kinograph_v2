#include "ArduPID.h"
#include "Config.h"
#include "ContinuousStepper.h"


ArduPID fPID;
double f_in, f_out;
double fp = 0.0, fi=0, fd=0;
double f_setpoint = 135;

ArduPID tPID;
double t_in, t_out;
double tp = 0.0, ti=0, td=0;
double t_setpoint = 475;

// -- feed motor
// const int f_enable = 34; // using kill switch
const int f_inputA = 35;
const int f_inputB = 36;
const int f_sensor = 38;
// -- takeup motor
// const int t_enable = 20; // using kill switch
const int t_inputA = 21;
const int t_inputB = 22;
const int t_sensor = 19;
// -- capstan motor
// const int c_enable = 7;
// const int c_inputA = 6;
// const int c_inputB = 5;
ContinuousStepper stepper(stepPin, dirPin, enablePin);
int s_output;

const int speedPotPin = 16;
int speedCounter = 0;
int speedSamples = 20;
float speedTotal = 0;  

void setup() {
  Serial.begin(115200);

  initMotors();

  fPID.begin(&f_in, &f_out, &f_setpoint, fp, fi, fd);
  fPID.setOutputLimits(0, 50);
  fPID.setBias(100.0 / 2.0);
  fPID.setWindUpLimits(-10, 10); // Groth bounds for the integral term to prevent integral wind-up
  fPID.start();

  tPID.begin(&t_in, &t_out, &t_setpoint, tp, ti, td);
  tPID.reverse();
  tPID.setOutputLimits(0, 50);
  tPID.setBias(100.0 / 2.0);
  tPID.setWindUpLimits(-10, 10); // Groth bounds for the integral term to prevent integral wind-up
  tPID.start();
}

void loop() {
  int speedVal = analogRead(speedPotPin);
  Serial.println(speedVal);
  stepper.spin(speedVal);
  // speedTotal += speedVal;
  // speedCounter++;
  // if (speedCounter > speedSamples-1)  {
  //   speedVal = (int) floor(speedTotal / speedSamples);
  //   // analogWrite(c_inputB, (int)floor(speedVal));
  //   stepper.spin(speedVal);
  //   speedTotal = 0;
  //   speedCounter = 0;  
  //   // Serial.println((int)floor(speedVal));
  // }
  stepper.loop();

  // PID value adjustments
  fp = map(analogRead(A4), 0, 1023, 0, 10)/1.0;
  tp = map(analogRead(A3), 0, 1023, 0, 10)/1.0;
  // Serial.print(fp);
  // Serial.print(" : ");
  // Serial.println(tp);

  int f_in = analogRead(f_sensor);
  int t_in = analogRead(t_sensor);
  // Serial.print(f_in);
  // Serial.print(" : ");
  // Serial.println(t_in);
   
  fPID.compute();
  tPID.compute();
  
  analogWrite(f_inputB, f_out);
  analogWrite(t_inputB, t_out);
  // Serial.print(f_out);
  // Serial.print(" : ");
  // Serial.println(t_out);

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
