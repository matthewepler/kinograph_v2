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
 #include <Bounce2.h>
Bounce bounce = Bounce();


// const int Enable = 20;
// const int InputA = 21;
// const int InputB = 22;
// const int HLFB = A9; // 23

const int Enable = 34;
const int InputA = 35;
const int InputB = 36;
// const int HLFB = A9; // 23

const int startSpeed = 10;
const bool usePot = true;
const int potPin = A2; // 16
int speedVal;

const int resetButtonPin = 37;

void setup() {
  Serial.begin(9600);

  pinMode(Enable, OUTPUT);
  pinMode(InputA, OUTPUT);
  pinMode(InputB, OUTPUT);
  // pinMode(resetButtonPin, INPUT_PULLUP);
  bounce.attach( resetButtonPin ,  INPUT_PULLUP ); // USE INTERNAL PULL-UP
  bounce.interval(5); // interval in ms


  // toggle enable to reset from any leftover error state
  digitalWrite(Enable, LOW);
  delay(10);
  digitalWrite(Enable, HIGH);
  delay(10);

  // set direction. HIGH = CW
  digitalWrite(InputA, HIGH);
}

void loop() {
  checkButton();

  if (usePot) {
    int potVal = analogRead(potPin);
    // Serial.print(potVal);
    speedVal = map(potVal, 0, 1023, 0, 255);
  } else {
    speedVal = startSpeed;
  }
  
  analogWrite(InputB, speedVal);
  
  Serial.println(speedVal);
}

void checkButton() {
  bounce.update();
   if ( bounce.changed() ) {
    // THE STATE OF THE INPUT CHANGED
    // GET THE STATE
    int deboucedInput = bounce.read();
    // IF THE CHANGED VALUE IS LOW
    if ( deboucedInput == LOW ) {
      Serial.println("reset");
      digitalWrite(Enable, LOW);
      delay(10);
      digitalWrite(Enable, HIGH);
      delay(10);
    }
  }
}
