/*
  HLFB + --> 5V-24V (external from motor)
  HLFB - --> Analog pin (same voltage as input)
  See ClearPath user manual for more info 

  TODO: add pot to change speed
*/

const int Enable = 20;
const int InputA = 21;
const int InputB = 22;
const int HLFB = A9;

void setup() {
  Serial.begin(9600);

  pinMode(Enable, OUTPUT);
  pinMode(InputA, OUTPUT);  // dir
  pinMode(InputB, OUTPUT); // speed
  digitalWrite(Enable, LOW);
  delay(10);
  digitalWrite(Enable, HIGH);
  delay(10);
  digitalWrite(InputA, LOW);
  analogWrite(InputB, 0);
}

void loop() {
  analogWrite(InputB, 100);

  // TODO: map to 5/95% range of motor feedback;
  
  Serial.println(analogRead(HLFB));
  delay(100);
}
