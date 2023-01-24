// const int potApin = 
const int potBpin = A2; // 16
const int potCpin = A4; // 18
const int potDpin = A3; // 17

void setup() {
  Serial.begin(9600);
}

void loop() {
  int potBval = analogRead(potBpin);
  int potCval = analogRead(potCpin);
  int potDval = analogRead(potDpin);

  Serial.print(potBval);
  Serial.print(" : ");
  Serial.print(potCval);
  Serial.print(" : ");
  Serial.println(potDval);

  delay(50);
}
