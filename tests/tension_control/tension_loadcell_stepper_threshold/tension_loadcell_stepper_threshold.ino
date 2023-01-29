#include <NBHX711.h>


// Motor
const int motorDirPin = 35;
const int motorSignalPin = 36;
//const int f_enable = 34; // disable if using manual switch

// load sensor
NBHX711 hx711(31, 32);
int _output;
int setpoint = 150;
int _offset;
const int _threshold = 10;
const int correction =_threshold / 10;

const double _interval = 500; // ms
double prevTime = 0;
const int _scale = 1000;

const int torqueAdjPin = 18;


void setup() {
  pinMode(torqueAdjPin, INPUT);
  
  initMotors();
  
  hx711.begin();
  for (int i=0; i<100; i++) {
    hx711.update();
    delay(1);
  }
  _offset = hx711.readAverage(10);
  _output = _offset / 10;
}

void loop() {
  hx711.update();
  
  if (millis() - prevTime > _interval) {
    int avg = (int) hx711.readAverage(10);
    int avgAdj = (int) (avg / _scale) - (_offset / _scale);
    Serial.print(avgAdj);
    Serial.print(" : ");
    
    if (avgAdj < (setpoint - _threshold)) _output += correction;
    if (avgAdj > (setpoint + _threshold)) _output -= correction;
    _output = constrain(_output, 0, setpoint);
    Serial.println(_output);

    prevTime = millis();
  }
  int adjOutput = map(_output, 0, setpoint, 0, 35);
  analogWrite(motorSignalPin, adjOutput);

//  Serial.println(adjOutput);
}

void initMotors() {
  // -- feed motor
  pinMode(motorDirPin, OUTPUT);
  pinMode(motorSignalPin, OUTPUT);
  // pinMode(motorEnablePin, OUTPUT);

  // set direction. HIGH = CW
  digitalWrite(motorDirPin, HIGH);

  // set all motors to 0 torque/speed
  digitalWrite(motorSignalPin, 0);

}
