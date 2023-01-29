#include <HX711.h>
#include <MovingAveragePlus.h>

HX711 hx711;
const int datPin = 31;
const int clkPin = 32;

MovingAveragePlus<unsigned> average(10);
const double interval = 300; // ms
double prevTime = 0;
unsigned int _offset;
const int _scale = 1000;

unsigned int minVal;
unsigned int maxVal;


void setup() {
//   Serial.begin(9600);
  hx711.begin(datPin, clkPin);
  
  // read whatever crazy values it gives us at startup
  // and subtract that from every reading. Aka "tare."
  // this blocks for a few cycles to gather enough samples
  // before reading the average.
  _offset = hx711.read_average();
}

void loop() {
   unsigned int raw = (unsigned int) hx711.read();
   average.push(raw);

  if (millis() - prevTime > interval) {
    unsigned int avg = average.get();
    unsigned int avgAdj = (raw / _scale) - (_offset / _scale);
    
    Serial.print(raw);
    Serial.print(" : ");
    Serial.print(average.get());
    Serial.print(" : ");
    Serial.println(avgAdj);

    prevTime = millis();
  }
}
