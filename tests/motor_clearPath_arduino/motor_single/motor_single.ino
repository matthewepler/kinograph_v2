/*
 * Basic test for Teknic ClearPath motor
 * Mode = Torque or Velocity, PWM Unipolar mode
 */

const int Enable = 11;
const int InputA =10;
const int InputB = 9;
const int HLFB = 8;
const int myLED = 13;

int startSpeed = 10;
int usePot = true;
int potPin = A0;

void setup() {
  pinMode(Enable, OUTPUT);
  pinMode(InputA, OUTPUT);
  pinMode(InputB, OUTPUT);
  pinMode(HLFB, INPUT_PULLUP);

  // toggle enable to reset from any leftover error state
  digitalWrite(Enable, LOW);
  delay(10);
  digitalWrite(Enable, HIGH);
  delay(10);

  // set direction. HIGH = CW
  digitalWrite(InputA, HIGH);

}

void loop() {
  int speedVal;
  if (usePot) {
    int potVal = analogRead(potPin);
    speedVal = map(potVal, 0, 1023, 0, 255);
  } else {
    speedVal = startSpeed;
  }
  analogWrite(InputB, speedVal);
}
