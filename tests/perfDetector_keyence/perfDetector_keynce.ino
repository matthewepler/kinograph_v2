const int perfSignalPin = 37;
int counter = 0;
int prevCounter = 0;

void setup() {
  pinMode(perfSignalPin, INPUT);
  attachInterrupt(perfSignalPin, readSprocket, FALLING);
}

void loop() {
  if (counter > prevCounter) {
    Serial.println(counter);
    delay(10);
  }
  prevCounter = counter;
}

void readSprocket() {
  counter++;
}
