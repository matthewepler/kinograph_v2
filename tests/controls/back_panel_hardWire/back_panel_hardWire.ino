void setup() {
//  pinMode(23, OUTPUT);
  pinMode(22, INPUT_PULLUP);
}

void loop() {
//  Serial.println(analogRead(16));

  Serial.println(digitalRead(22));

//  digitalWrite(23, HIGH);
//  delay(100);
//  digitalWrite(23, LOW);
//  delay(100);
  
  delay(10);
}
