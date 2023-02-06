/*
 * Test output signal from perforation detection circuit.
 * 
 * Tested with Keyence FS-N41P amplifier unit + fiber-optic reflective sensor
 * Circuit:
 * 
 * BROWN ---- 24V+
 * BLACK ---- 1K res --┬-- 180 res ---- GND
 *                    PIN (3.66 V+)
 * BLUE ----GND
 */

const int perfSignalPin = 14;
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
