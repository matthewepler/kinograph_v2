// Manual speed control test

const int manSpeedAdjPin = 14;


void setup() {
  pinMode(manSpeedAdjPin, INPUT);
  Serial.begin(115200);
}

void loop() {
  int controlVal = analogRead(manSpeedAdjPin);
  Serial.println(controlVal);
  delay(100);
}
