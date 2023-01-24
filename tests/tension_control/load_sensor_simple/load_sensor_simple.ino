// Load sensor test - simple
// Based off of example "Read_1x_load_call" from HX711_ADC lib: https://github.com/olkal/HX711_ADC

#include "HX711.h"

#include <HX711_ADC.h>
#if defined(AVR)
#include <EEPROM.h>
#endif

uint8_t dataPin = 6;
uint8_t clockPin = 7;

//pins:
const int HX711_dout = 4; //mcu > HX711 dout pin
const int HX711_sck = 5; //mcu > HX711 sck pin

//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

void setup() {
  Serial.begin(115200); delay(10);
  Serial.println();
  Serial.println("Starting...");
  
  LoadCell.begin();
  //LoadCell.setReverseOutput(); //uncomment to turn a negative output value to positive
  calibrateTensionSensor();
}

void loop() {
  Serial.println(LoadCell.getData());
}

void calibrateTensionSensor() {
//    float calibrationValue = 696.0;
  float calibrationValue = 1.0;
  //EEPROM.get(calVal_eepromAdress, calibrationValue); // uncomment this if you want to fetch the calibration value from eeprom
  
  unsigned long stabilizingtime = 2000;
  LoadCell.start(stabilizingtime, true);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
    Serial.println("Startup is complete");
  }
}
