/*
  CURRENTLY NOT WORKING. GETTING UNSTABLE SIGNAL FROM MOTOR. 

  HLFB + ===> 5V+ (not MCU) ==> 1.5K ==> 3.3K ==> GND
                                      |
                                    Pin 22 (Teensy 4.1)
  
  Use this code to test an incoming PWM signal, usually from a single ClearPath motor.
  For test multiple incoming PWM signals, you will need to use the "Multi" version of 
  the library. See links below.

  FreqMeasure lib used to read PWM pulses coming into MCU.
  http://www.pjrc.com/teensy/td_libs_FreqMeasure.html
  see also FreqMeasureMulti:
  https://github.com/PaulStoffregen/FreqMeasureMulti 

  HLFB + --> 5V-24V (external from motor). 
  HLFB - --> GND, with an analog pin reading voltage
  See ClearPath user manual for more info.

  !! IMPORTANT !!
  If using a variable input (a knob, etc), make sure it is at its zero
  position (off) before starting, or the reel will start
  moving immediately.   

  TODO
  --
  map to 5/95% range of motor feedback;

*/
// NOTE: check the links above for pins that can be used
#include <FreqMeasure.h>

// takeup motor
const int tk_Enable = 20;
const int tk_InputA = 21;
const int tk_InputB = 37;
const int tk_HLFB = 22;

const int potPin = A2;

double sum=0;
int count=0;

void setup() {
  Serial.begin(9600);

  pinMode(tk_Enable, OUTPUT);
  pinMode(tk_InputA, OUTPUT);  // dir
  pinMode(tk_InputB, OUTPUT); // speed
  digitalWrite(tk_Enable, LOW);
  delay(10);
  digitalWrite(tk_Enable, HIGH);
  delay(10);
  digitalWrite(tk_InputA, LOW);
  analogWrite(tk_InputB, 0);

  FreqMeasure.begin();
}

void loop() {
  int potVal = analogRead(potPin);
  // Serial.print(potVal);
  int output = map(potVal, 0, 1023, 0, 255);
  // Serial.print(" : ");
  // Serial.print(output);
  analogWrite(tk_InputB, output);


  if (FreqMeasure.available()) {
    // average several reading together
    sum = sum + FreqMeasure.read();
    count = count + 1;
    if (count > 10) {
      float frequency = FreqMeasure.countToFrequency(sum / count);
      // Serial.print("HLFB: ");
      Serial.println(frequency);
      sum = 0;
      count = 0;
    }
  }

  // Serial.println();
  // delay(10);
}
