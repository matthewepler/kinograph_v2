#include <NBHX711.h>
#include <ContinuousStepper.h>

const int stepPin = 10;
const int dirPin = 9;
const int enablePin = 8;
ContinuousStepper stepper(stepPin, dirPin, enablePin);

// "f" = feed motor
// const int f_enable = 34; // using kill switch
const int f_inputA = 35;
const int f_inputB = 36;

// "t" = take-up motor
// const int f_enable = 20; // using kill switch
const int t_inputA = 21;
const int t_inputB = 22;

// shared tension vars
const double _interval = 500; // ms
double prevTime = 0;
const int _scale = 1000;
const int _threshold = 10;
const int _correct = _threshold / 10;

// Feed tension zone
NBHX711 f_hx711(31, 32);
int f_offset;
// setpoint is determined with calibrate()
int f_setpoint = 180;
int f_output;

// Take-up tension zone
NBHX711 t_hx711(29, 30);
int t_offset;
// setpoint is determined with calibrate()
int t_setpoint = 300;
int t_output;

// controls
const int speedPotPin = 16;
const int f_torqueAdjPin = 18;
const int t_torqueAdjPin = 17;

bool calibrated = true;
bool debug = false;


void setup() {
  pinMode(speedPotPin, INPUT);
  pinMode(f_torqueAdjPin, INPUT);
  pinMode(t_torqueAdjPin, INPUT);

  initMotors();

  f_hx711.begin();
  t_hx711.begin();
  for (int i=0; i<100; i++) {
    f_hx711.update();
    t_hx711.update();
    delay(1);
  }
  f_offset = f_hx711.readAverage(10);
  t_offset = t_hx711.readAverage(10);
  f_output = f_offset / 10;
  t_output = t_offset / 10;
}

void loop() {
  adjustSpeed();

  f_hx711.update();
  t_hx711.update();

  if (millis() - prevTime > _interval) {
    //    unsigned int f_avg = f_avgArr.get();
    int f_avg = (int) f_hx711.readAverage(10);
    int f_avgAdj = (int) (f_avg / _scale) - (f_offset / _scale);
    if (f_avgAdj < (f_setpoint - _threshold)) f_output += _correct;
    if (f_avgAdj > (f_setpoint + _threshold)) f_output -= _correct;
    f_output = constrain(f_output, 0, f_setpoint);

    //    unsigned int t_avg = t_avgArr.get();
    int t_avg = (int) t_hx711.readAverage(10);
    int t_avgAdj = (int) (t_avg / _scale) - (t_offset / _scale);
    if (t_avgAdj < (t_setpoint - _threshold)) t_output += _correct;
    if (t_avgAdj > (t_setpoint + _threshold)) t_output -= _correct;
    t_output = constrain(t_output, 0, t_setpoint);

    prevTime = millis();

    if (!calibrated || debug) {
      calibrateSetpoints(f_avg, t_avg);
    }
  }

  int f_adjOutput = map(f_output, 0, f_setpoint, 0, 35); // motor signal min/max
  int t_adjOutput = map(t_output, 0, t_setpoint, 0, 35); // motor signal min/max

  if (debug) {
    Serial.print("\t");
    Serial.print("f_adjOut: ");
    Serial.print(f_adjOutput);
    Serial.print("\t");
    Serial.print("t_adjOut: ");
    Serial.print(t_adjOutput);
  }
  
  if (calibrated) {
    analogWrite(f_inputB, f_adjOutput);
    analogWrite(t_inputB, t_adjOutput);
  }

  if (debug) Serial.println();
}

void calibrateSetpoints(int fAvg, int tAvg) {
  // apply tension to the sensors and note the values for each
  // determine your target value and set the "setpoint" vars for each
  Serial.print("\t");
  Serial.print("fAvg: ");
  Serial.print(fAvg / _scale);

  Serial.print("\t");
  Serial.print("tAvg: ");
  Serial.println(tAvg / _scale);
}

void adjustSpeed() {
  int speedVal = analogRead(speedPotPin);
  if (speedVal < 10) {
    stepper.stop();
  } else {
    int speed = (int) map(speedVal, 10, 1023, 0, 1000);
    stepper.spin(speed);
    if (debug) {
      Serial.print("\t");
      Serial.print("speed : ");
      Serial.print(speed);
    }
  }
  stepper.loop();
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
  stepper.setDir("CCW");
  stepper.setAcceleration(4000);

  // set direction. HIGH = CW
  digitalWrite(f_inputA, HIGH);
  digitalWrite(t_inputA, LOW);

  // set all motors to 0 torque/speed
  digitalWrite(f_inputB, 0);
  digitalWrite(t_inputB, 0);
  stepper.stop();
}
