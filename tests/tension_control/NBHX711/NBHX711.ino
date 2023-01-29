#include <NBHX711.h>

NBHX711 hx711(29, 30);
void setup() {
  Serial.begin(9600);
  hx711.begin();
}

void loop() {
  static unsigned long lastRead;
  hx711.update();
  if (millis() - lastRead >= 500) {
    lastRead = millis();
    Serial.println(hx711.readAverage(10));
  }
}
