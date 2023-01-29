#include <LoadCell.h>

// Motor
const int motorDirPin = 35;
const int motorSignalPin = 36;
//const int f_enable = 34; // disable if using manual switch

// load sensor
LoadCell cell = LoadCell(31, 32);

const int torqueAdjPin = 18;


void setup() {
  pinMode(torqueAdjPin, INPUT);
  initMotors();
  cell.setpoint = 150;
}

void loop() {
  cell.loop();
  int mtrSignal = map(cell.read(), 0, cell.setpoint, 0, 35);
  analogWrite(motorSignalPin, mtrSignal);
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
