/*
 * Basic test for Teknic ClearPath motor
 * Mode = Torque or Velocity, PWM Unipolar mode
 * Connector wiring:
 * PIN   COLOR   FUNCTION
 *  1     GRN     HLFB +
 *  2     BLK     INPUT B +
 *  3     WHIT    INPUT A +
 *  4     BLU     ENABLE +
 *  5     RED     HLFB -
 *  6     YEL     INPUT B -
 *  7     BRN     INPUT A -
 *  8     ORN     ENABLE -
 */

const int Enable = 3;
const int InputA = 2;
const int InputB = 1;
const int HLFB = 0;
const int myLED = 13;

int startSpeed = 10;
int usePot = false;
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
