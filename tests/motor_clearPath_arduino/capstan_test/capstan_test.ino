/*
 * Test for Teknic motor in any PWM Unipolar mode
 * with potentiometer for manual speed adjustment.
 * 
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
 
const int Enable = 7;
const int InputA = 6;
const int InputB = 5;
//const int HLFB = 3; // can't get this to work as expected
const int potPin = 14;
const int dwellTime = 100; // ms

bool usePot = false;

void setup() {
//  pinMode(HLFB, INPUT_PULLUP);
  pinMode(Enable, OUTPUT);
  pinMode(InputA, OUTPUT);
  pinMode(InputB, OUTPUT);

  pinMode(potPin, INPUT);
  Serial.begin(115200);

  // set direction. HIGH = CW
  digitalWrite(InputA, LOW);
  // set speed to 0;
  analogWrite(InputB, 0);
  // set inital state
  digitalWrite(Enable, LOW);
  delay(dwellTime);
  // enable motor
  digitalWrite(Enable, HIGH);
  delay(dwellTime);
}

void loop() {
  int signalVal;
  
  if (!usePot) {
    signalVal = 20;
  } else {
    int potVal = analogRead(potPin);
    int mapVal = map(potVal, 685, 15, 0, 255); // see potTestSimple
    signalVal = constrain(mapVal, 0, 255);
  }

//  Serial.println(signalVal);

  analogWrite(InputB, signalVal);
}
