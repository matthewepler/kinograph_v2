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

// Take-up Motor
 const int Enable = 2;
 const int InputA = 3;
 const int InputB = 4;

// Feed Motor
//const int Enable = 31;
//const int InputA = 32;
//const int InputB = 33;

const int startSpeed = 10; // TODO: change name to account for torque or other non-speed modes
const bool usePot = true;
const int potPin = 16;
int speedVal;

const int resetButtonPin = 22;

void setup() {
  pinMode(Enable, OUTPUT);
  pinMode(InputA, OUTPUT);
  pinMode(InputB, OUTPUT);

  bounce.attach( resetButtonPin ,  INPUT_PULLUP ); // USE INTERNAL PULL-UP
  bounce.interval(5); // interval in ms


  // toggle enable to reset from any leftover error state
  digitalWrite(Enable, LOW);
  delay(10);
  digitalWrite(Enable, HIGH);
  delay(10);

  // set direction. HIGH = CW
  digitalWrite(InputA, LOW);
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
  delay(10);
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
