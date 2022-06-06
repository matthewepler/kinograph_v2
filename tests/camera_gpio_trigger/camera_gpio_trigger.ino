void setup() {
  pinMode(D5, OUTPUT);

}

void loop() {
  digitalWrite(D5, HIGH);
  delayMicroseconds(20);
  digitalWrite(D5, LOW);
  
  delay(500);
}
