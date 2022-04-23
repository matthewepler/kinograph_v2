  /*
 * TODO: Licensing is a work in progress...TBD.
 * 
 * This test should be performed with one motor and one tension sensor.
 * The test should be performed while holding the film and adjusting its 
 * position manually.
 * 
 * Kinograph - Tension sensor test
 * Copyright 2022
 * 
 */

 /* 
  *  Sensor: https://smile.amazon.com/gp/product/B08KRWY43Y/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&th=1 
  */
  
#include "CytronMotorDriver.h"
CytronMD reel_motor(PWM_DIR, D6, D7);   // feed PWM 2 = Pin 6, DIR 2 = Pin 7.
//  CytronMD reel_motor(PWM_DIR, D9, D8);   // take-up PWM 1 = Pin 9, DIR 1 = Pin 8.

const int hx711_clk = D0;
const int hx711_dt = D1;

const int tensionSetpoint = 900000;
const int tensionBadReading = 33500000;
int tensionDiff = 0;
int tension = 900000;
int oldTension = 900000;
float tensionDiffAvg = 0;
const float tensionAvgCoeff = 0.08;

int motor_pwr = 0;


int read_hx711() {
  int val = 0;
  int pin = 0;

  for (int i = 0; i < 25; i++) {
    digitalWrite(hx711_clk, HIGH);
    delayMicroseconds(1);
    digitalWrite(hx711_clk, LOW);
    pin = digitalRead(hx711_dt) & digitalRead(hx711_dt) & digitalRead(hx711_dt);
    val = (val << 1) | (pin & 1 );
    delayMicroseconds(1);
  }
  
  return val;
}


void setup() {
  Serial.begin(115200);
  while (!Serial);
  delay(100);

  pinMode(hx711_clk, OUTPUT);
  pinMode(hx711_dt, INPUT);
  digitalWrite(hx711_clk, LOW);
}

void loop() {
  float motor_pwr;

  // read tension arm`
  oldTension = tension;
  tension = read_hx711();
  
  if (tension < tensionBadReading) {
    tensionDiff = tension - tensionSetpoint;
    tensionDiffAvg = tensionDiffAvg * (1 - tensionAvgCoeff) + tensionDiff * tensionAvgCoeff;
  }

  Serial.print("tension arm: ");
  Serial.print(tension);
  Serial.print(" avg diff ");
  Serial.print(tensionDiffAvg); 

  motor_pwr -= (int16_t)(tensionDiffAvg / 40000);  // feed a little of tension arm to make feed back-tension reduce at end of reel

  reel_motor.setSpeed((int16_t)motor_pwr);

  delay(200);
}
