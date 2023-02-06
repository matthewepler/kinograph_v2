#include <NBHX711.h>
#include <ContinuousStepper.h>

const int stepPin = 10;
const int dirPin = 9;
const int enablePin = 8;
ContinuousStepper stepper(stepPin, dirPin, enablePin);

// "f" = feed motor
const int f_enable = 31;
const int f_inputA = 32;
const int f_inputB = 33;

// "t" = take-up motor
const int t_enable = 2;
const int t_inputA = 3;
const int t_inputB = 4;

// shared tension vars
const double _interval = 500; // ms
double prevTime = 0;
const int _scale = 1000;
const int _threshold = 10;
const int _correct = _threshold / 10;

// Feed tension zone
NBHX711 f_hx711(26, 27);
int f_offset;
// setpoint is determined with calibrate()
int f_setpoint = 145;
int f_output;

// Take-up tension zone
NBHX711 t_hx711(24, 25);
int t_offset;
// setpoint is determined with calibrate()
int t_setpoint = 255;
int t_output;

// controls
const int speedPotPin = 16;
//const int f_torqueAdjPin = 18; // TODO
//const int t_torqueAdjPin = 17; // TODO

bool calibrated = true;
bool debug = false;

// Cam + perf sensor
const int triggerPin = 15;
const int perfSignalPin = 14;
const int ledPin = 23;
int perfCounter = 1;
int frameCounter = 0;
int prevFrameCount;
int camDelay = 50; // micro seconds
elapsedMillis timeSinceCamTrigger = camDelay + 1; // this sets condition to trigger in the first loop


void setup() {
//  while (!Serial) {};
  
  pinMode(speedPotPin, INPUT);
//  pinMode(f_torqueAdjPin, INPUT);
//  pinMode(t_torqueAdjPin, INPUT);
 
  // cam + perf sensor
  pinMode(ledPin, OUTPUT);
  pinMode(triggerPin, OUTPUT);
  pinMode(perfSignalPin, INPUT);
  digitalWrite(triggerPin, HIGH);
  attachInterrupt(perfSignalPin, readSprocket, FALLING);

  initMotors();

  f_hx711.begin();
  t_hx711.begin();
  for (int i=0; i<100; i++) {
    f_hx711.update();
    t_hx711.update();
    delay(10);
  }
  f_offset = f_hx711.readAverage(10);
  t_offset = t_hx711.readAverage(10);
  f_output = f_offset / (f_offset > 100000 ? 100: 10);
  t_output = t_offset / (t_offset > 100000 ? 100: 10);

  
}

void loop() {
  adjustSpeed();

  if (timeSinceCamTrigger > camDelay) {
    // adds a little delay since we can't use delay in the interrupt routine
    digitalWrite(triggerPin, HIGH);
    digitalWrite(ledPin, LOW) ;
  }

  f_hx711.update();
  t_hx711.update();

  if (millis() - prevTime > _interval) {
    int f_avg = (int) f_hx711.readAverage(10);
    int f_avgAdj = (int) (f_avg / _scale) - (f_offset / _scale);
    if (f_avgAdj < (f_setpoint - _threshold)) f_output += _correct;
    if (f_avgAdj > (f_setpoint + _threshold)) f_output -= _correct;
    f_output = constrain(f_output, 0, f_setpoint);

    int t_avg = (int) t_hx711.readAverage(10);
    int t_avgAdj = (int) (t_avg / _scale) - (t_offset / _scale);
    if (t_avgAdj < (t_setpoint - _threshold)) t_output += _correct;
    if (t_avgAdj > (t_setpoint + _threshold)) t_output -= _correct;
    t_output = constrain(t_output, 0, t_setpoint);

    prevTime = millis();

    if (!calibrated || debug) {
      calibrateSetpoints(f_avg, t_avg, f_avgAdj, t_avgAdj);
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

void calibrateSetpoints(int fAvg, int tAvg, int fAvgAdj, int tAvgAdj) {
  // apply tension to the sensors and note the values for each
  // determine your target value and set the "setpoint" vars for each
  
  Serial.print("\t");
  Serial.print("fAvg: ");
  Serial.print(fAvg / _scale);
  Serial.print(" -> ");
  Serial.print(fAvgAdj);

  Serial.print("\t");
  Serial.print("tAvg: ");
  Serial.print(tAvg / _scale);
  Serial.print(" -> ");
  Serial.println(tAvgAdj);
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

/* PERF DETECTION ISR */
void readSprocket() {
  if (perfCounter % 4 == 0) {
    triggerCamera();
    perfCounter = 1;
  } else {
    perfCounter++;
  }
}

/* CAMERA TRIGGER */
void triggerCamera() {
  digitalWrite(triggerPin, LOW);
  digitalWrite(ledPin, HIGH);
  timeSinceCamTrigger = 0;
  frameCounter ++;
}

void initMotors() {
  // -- feed motor
  pinMode(f_enable, OUTPUT);
  pinMode(f_inputA, OUTPUT);
  pinMode(f_inputB, OUTPUT);
  digitalWrite(f_enable, HIGH);

  // -- take-up motor
  pinMode(t_enable, OUTPUT);
  pinMode(t_inputA, OUTPUT);
  pinMode(t_inputB, OUTPUT);
  digitalWrite(t_enable, HIGH);

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
