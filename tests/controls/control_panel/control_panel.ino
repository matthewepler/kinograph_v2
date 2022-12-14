// Switches - SPDT
//const int SPDT1 = ; 
//const int SPDT2 = ;

// Switches - toggle
//const int TGL1 = ;
//const int TGL2 = ;

// Buttons - momentary
//const int BTN1 = ;
//const int BTN2 = ;

// Knobs - 10K pot
//const int KNB1 = ;
//const int KNB2 = ;
//const int KNB3 = ;
//const int KNB4 = ;

// CONTROL FEATURE MAPPING


void setup() {
  Serial.begin(38400);
}

void loop() {
  int seventeen = analogRead(3);
  int sixteen = analogRead(2);
  int fifteen = analogRead(1);
  int fourteen = analogRead(0);

  Serial.print(analogRead(3));
  Serial.print(" : ");
  Serial.print(analogRead(2));
  Serial.print(" : ");
  Serial.print(analogRead(1));
  Serial.print(" : ");
  Serial.println(analogRead(0));
  
  delay(100);
}
