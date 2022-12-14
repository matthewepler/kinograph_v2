// TODO: add pot for changing target

const int sensorPin = A5;
const float gainVal = 0.5;
int target = 725;
int prevOutput;
const bool inverted = true; // true if sensor reads 1023 at rest, false if 0.

// capstan
const int cap_Enable = 7;
const int cap_InputA = 6;
const int cap_InputB = 5;

// reel motor
// takeup
const int tk_Enable = 20;
const int tk_InputA = 21;
const int tk_InputB = 22;


void setup() {
  Serial.begin(9600);
  
  pinMode(cap_Enable, OUTPUT);
  pinMode(cap_InputA, OUTPUT);  // dir
  pinMode(cap_InputB, OUTPUT);  // speed
  digitalWrite(cap_InputA, LOW);
  digitalWrite(cap_Enable, LOW); 
  delay(10);
  digitalWrite(cap_Enable, HIGH);
  delay(10);
  analogWrite(cap_InputB, 0);


  pinMode(tk_Enable, OUTPUT);
  pinMode(tk_InputA, OUTPUT);  // dir
  pinMode(tk_InputB, OUTPUT); // speed
  digitalWrite(tk_Enable, LOW);
  delay(10);
  digitalWrite(tk_Enable, HIGH);
  delay(10);
  digitalWrite(tk_InputA, LOW);
  analogWrite(tk_InputB, 0);

  prevOutput = 1;
}

void loop() {  
  // we need something more than 0 to get started
//  if (prevOutput < 1) prevOutput = 1;

  int currVal = analogRead(sensorPin);
  int diff = target - currVal;

//  Serial.print(diff);
//  Serial.print(" : ");
  
  if (inverted) diff*= -1;

  if (diff < 0) {
    analogWrite(tk_InputB, 0);
  } else {
    float percentDiff = float(diff)/float(target);
//    Serial.print(percentDiff);
//    Serial.print(" : ");
    
    float outputChange = gainVal * (percentDiff * float(prevOutput));
    Serial.print(outputChange);
    Serial.print(" : ");
    
    float currOutput = prevOutput + ceil(outputChange);
    Serial.print(currOutput);
    Serial.print(" : ");
    
    constrain(currOutput, 0, 255);
    Serial.print(currOutput);
    Serial.print(" : ");
    
//    analogWrite(tk_InputB, currOutput);
    prevOutput = ceil(currOutput);
    
    
    
    
    Serial.print(currOutput);
  }

//  Serial.print(currVal);
//  Serial.print(" : ");
//  Serial.print(diff);
//  Serial.print(" : ");
//  Serial.println(currOutput);

  

  Serial.println();
  delay(50);
}
