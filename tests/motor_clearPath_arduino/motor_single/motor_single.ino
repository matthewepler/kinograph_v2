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
 *  
 *  Note: HLFB requires its own power source. See ClearPath docs.
 */

const int Enable = 34;
const int InputA = 35;
const int InputB = 36;
//const int HLFB = 4;
const int myLED = 13;

const int startSpeed = 100;
const bool usePot = false;
const int potPin = A0;
int speedVal;

void setup() {
  pinMode(Enable, OUTPUT);
  pinMode(InputA, OUTPUT);
  pinMode(InputB, OUTPUT);
//  pinMode(HLFB, INPUT_PULLUP);

  // toggle enable to reset from any leftover error state
  digitalWrite(Enable, LOW);
  delay(10);
  digitalWrite(Enable, HIGH);
  delay(10);

  // set direction. HIGH = CW
  digitalWrite(InputA, HIGH);

}

void loop() {
  if (usePot) {
    int potVal = analogRead(potPin);
    speedVal = map(potVal, 0, 1023, 0, 255);
  } else {
    speedVal = startSpeed;
  }
  analogWrite(InputB, 6);
}
