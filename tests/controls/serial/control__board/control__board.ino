/*
 * NOTE: if using an Arduino Uno, you must disconnect wires
 * from Tx/Rx pins during code upload, or it will not work.

   Moving average code from: https://maker.pro/arduino/tutorial/how-to-clean-up-noisy-sensor-data-with-a-moving-average-filter
*/



#include <EasyTransfer.h>
#define WINDOW_SIZE 5

// moving average filter to remove noise from readings
int INDEX = 0;
int VALUE = 0;
int SUM = 0;
int READINGS[WINDOW_SIZE];
int AVERAGED = 0;

//create two objects
EasyTransfer CNTRL_in, CNTRL_out; 

struct RECEIVE_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  uint16_t poll;
};

struct SEND_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  uint16_t buttonstate;
  int potVal;
};

//give a name to the group of data
RECEIVE_DATA_STRUCTURE rxdata;
SEND_DATA_STRUCTURE txdata;

const int buttonPin = 12;
const int potPin = A5;

void setup(){
  Serial.begin(9600);
  //start the library, pass in the data details and the name of the serial port. Can be Serial, Serial1, Serial2, etc.
  CNTRL_in.begin(details(rxdata), &Serial);
  CNTRL_out.begin(details(txdata), &Serial);
  
  pinMode(buttonPin, INPUT_PULLUP);  
//  pinMode(potPin, INPUT); // unnecessary to declare analog inputs
  
}

void loop(){
  int potVal = analogRead(potPin);
  
  // for mapping, determine the value of the sensor in two positions:
  // at rest (low map val), and at max tension (high map val)
  int mappedVal = map(potVal, 0, 1023, 750, 700);
  
  // reduce noise by averaging readings, save for transmission
  txdata.potVal = movingAverage(mappedVal);
  
  if(!digitalRead(buttonPin))
    txdata.buttonstate = HIGH;
  else
    txdata.buttonstate = LOW;
 
 
  if (CNTRL_in.receiveData()) {
    if (rxdata.poll) {
      Serial.println("sending...");
      CNTRL_out.sendData();
    }
  }
 
  // If not polling before sending data,
  // delay must be longer than time it takes to complete loop() on main board
  delay(10);
}

int movingAverage(int latestReading) {
  SUM = SUM - READINGS[INDEX];       // Remove the oldest entry from the sum
  READINGS[INDEX] = latestReading;       // Add the newest reading to the window
  SUM = SUM + latestReading;                 // Add the newest reading to the sum
  INDEX = (INDEX+1) % WINDOW_SIZE;   // Increment the index, and wrap to 0 if it exceeds the window size
  AVERAGED = (int) SUM / WINDOW_SIZE;      // Divide the sum of the window by the window size for the result
  return AVERAGED;
}
