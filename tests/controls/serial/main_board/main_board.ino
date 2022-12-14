/*
 * Kinograph - Control Board Serial Comms Test
 * Main Board
 * 
 * EasyTransfer lib: https://github.com/madsci1016/Arduino-EasyTransfer
*/



#include <EasyTransfer.h>

//create two objects
EasyTransfer MAIN_in, MAIN_out; 


struct RECEIVE_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  uint16_t buttonstate;
  uint16_t potVal;
};

struct SEND_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  uint16_t poll;
};

//give a name to the group of data
RECEIVE_DATA_STRUCTURE rxdata;
SEND_DATA_STRUCTURE txdata;


void setup(){
  Serial1.begin(9600);
  //start the library, pass in the data details and the name of the serial port. Can be Serial, Serial1, Serial2, etc.
  MAIN_in.begin(details(rxdata), &Serial1);
  MAIN_out.begin(details(txdata), &Serial1);  

  txdata.poll = true;
}

void loop(){
  delay(1000);
  MAIN_out.sendData();

  if (MAIN_in.receiveData()) {
    Serial.print(rxdata.buttonstate);
    Serial.print(" : ");
    Serial.println(rxdata.potVal);
  } else {
    Serial.println("NO DATA RCV'D");
  }
}
