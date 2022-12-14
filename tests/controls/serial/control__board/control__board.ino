/*
 * NOTE: if using an Arduino Uno, you must disconnect wires
 * from Tx/Rx pins during code upload, or it will not work.
*/



#include <EasyTransfer.h>

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
  
  //first, lets read our potentiometer and button and store it in our data structure
  txdata.potVal = analogRead(potPin);
  
  
  if(!digitalRead(buttonPin))
    txdata.buttonstate = HIGH;
  else
    txdata.buttonstate = LOW;
 
  if (CNTRL_in.receiveData()) {
    if (rxdata.poll) {
      CNTRL_out.sendData();
    }
  }
 
  //delay for good measure
  delay(10);
}
