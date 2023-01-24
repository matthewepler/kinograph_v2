/*
  Testing and tuning sketch for tension feedback loop. 
  Test one tension zone at a time. For example, feed -> capstan, and capstan -> take-up. 
 
  A knob (potentiometer) is used to change the target tension value. The min/max of the knob
  should match the min/max tension of your machine. In other words, use map()
*/

// tension sensor
const int sensorPin = A5;
float gainVal = .1;
int target;
// const bool inverted = true; // true if sensor reads 1023 at rest, false if 0.

// -- feed motor
// const int f_enable = 34; // using kill switch
const int f_inputA = 35;
const int f_inputB = 36;
const int f_sensor = A14; // A14 = 38
int f_setpoint = 138;
int f_output = 30;
int f_prevOutput = 0;
float f_gain = 0.0;
const int f_gainPotPin = 18; // A4 = 18
float f_gainTotal = 0.0;
int f_errTotal = 0;

// -- takeup motor
// const int t_enable = 20; // using kill switch
const int t_inputA = 21;
const int t_inputB = 22;
const int t_sensor = A5 ;// A5 = 19
int t_setpoint = 487;
int t_output = 20;
int t_gain = 0.0;
const int t_gainPotPin = A3;
int t_gainTotal;
int t_errTotal;

// -- capstan motor
const int c_enable = 7;
const int c_inputA = 6;
const int c_inputB = 5;

const int speedPotPin = A2;
float speedSampleTotal = 0; 

int sampleCounter = 1;
int numSamples = 8;


void setup() {
  Serial.begin(115200);
  initMotors();

}

float gain = 1.0;
double prevtime;
int margin = 3;
void loop() {

    
  float speedVal = map(analogRead(speedPotPin), 0, 1023, 0, 100);
  analogWrite(c_inputB, floor(speedVal));  

  if (millis() - prevtime > 300) {
        
    int f_sensorVal = analogRead(f_sensor);
    if (f_sensorVal > f_setpoint + margin) f_output -= gain;
    if (f_sensorVal < f_setpoint - margin) f_output += gain;  

    int t_sensorVal = analogRead(t_sensor);
    if (t_sensorVal > t_setpoint - margin) t_output += gain;
    if (t_sensorVal < t_setpoint + margin) t_output -= gain;
    
    prevtime = millis();
  } 

  analogWrite(f_inputB, constrain(f_output, 0, 50));
  analogWrite(t_inputB, constrain(t_output, 0, 50));

  // Serial.println(t_output);

  delay(10);
}


void initMotors() {
  // -- feed motor
  pinMode(f_inputA, OUTPUT);
  pinMode(f_inputB, OUTPUT);
  // pinMode(f_enable, OUTPUT);

  // -- take-up motor
  pinMode(t_inputA, OUTPUT);
  pinMode(t_inputB, OUTPUT); 
  // pinMode(t_enable, OUTPUT);
 
  // -- capstan motor
  pinMode(c_inputA, OUTPUT);
  pinMode(c_inputB, OUTPUT);
  pinMode(c_enable, OUTPUT);
  // digitalWrite(c_enable, LOW);
  // delay(10);
  // digitalWrite(c_enable, HIGH);
  // delay(10);

  // set direction. HIGH = CW
  digitalWrite(f_inputA, HIGH);
  digitalWrite(t_inputA, LOW);
  digitalWrite(c_inputA, LOW);

  // set all motors to 0 torque/speed
  digitalWrite(f_inputB, 0);
  digitalWrite(t_inputB, 0);
  digitalWrite(c_inputB, 0);

}
