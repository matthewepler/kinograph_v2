 // A4 = pin 18 on Teensy 4.1
 // A5 = pin 19 "
 
const int signalPin = A5;

void setup() {
 Serial.begin(9600);

}

void loop() {
  Serial.println(analogRead(signalPin));
}
