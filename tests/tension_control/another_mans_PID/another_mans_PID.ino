// #include <PID_v2.h>
// double f_Kp = 1.3, f_Ki = 0.0, f_Kd = 0.0;
// double t_Kp = 1.3, t_Ki = 0.0, t_Kd = 0.0;
// PID_v2 f_PID(f_Kp, f_Ki, f_Kd, PID::Direct);
// PID_v2 t_PID(f_Kp, f_Ki, f_Kd, PID::Direct);

double t_tension, t_output;
double t_setpoint = 475; // typical range 690 - 740
double t_Kp = 1.5, t_Ki = 0.0, t_Kd = 0.0;
int t_max =60; // of 255. Reduce if RMS is maxing out.
int t_min = 0;
double t_total_error, t_last_error;

double f_tension, f_output;
double f_setpoint = 135; // typical range 109 - 150
double f_Kp = 1.5, f_Ki = 0.0, f_Kd = 0.0;
int f_max = 60; // of 255. Reduce if RMS is maxing out.
int f_min = 0;
double f_total_error, f_last_error;

int T = 1; //sample time in milliseconds (ms)
unsigned long last_time;



// -- feed motor
// const int f_enable = 34; // using kill switch
const int f_inputA = 35;
const int f_inputB = 36;
const int f_tensionSensor = 38;
// -- takeup motor
// const int t_enable = 20; // using kill switch
const int t_inputA = 21;
const int t_inputB = 22;
const int t_tensionSensor = 19;
// -- capstan motor
const int c_enable = 7;
const int c_inputA = 6;
const int c_inputB = 5;

const int speedPotPin = 16;
int speedCounter = 0;
int speedSamples = 20;
float speedTotal = 0;  

void setup() {
  Serial.begin(115200);

  initMotors();
}

void loop() {
  float speedVal = map(analogRead(speedPotPin), 0, 1023, 0, 100)/1.0;
  speedTotal += speedVal;
  speedCounter++;
  if (speedCounter > speedSamples-1)  {
    speedVal = speedTotal / speedSamples;
    analogWrite(c_inputB, (int)floor(speedVal));
    speedTotal = 0;
    speedCounter = 0;  
    // Serial.println((int)floor(speedVal));
  
  }
  if (speedVal > 2) {
  }
  


  t_tension = analogRead(t_tensionSensor);
  f_tension = analogRead(f_tensionSensor);
  Serial.print(t_tension);
  Serial.print(" : ");
  Serial.println(f_tension);

  PID_Control();
  delay(10);
}

// TODO: make motor objects that can be passed around 
void PID_Control() {

  unsigned long current_time = millis(); //returns the number of milliseconds passed since the Arduino started running the program
  int delta_time = current_time - last_time; //delta time interval 
  if (delta_time >= T){

    double t_error = t_setpoint - t_tension;
    t_error *= -1; // sensor reads in reverse (descending)
    t_total_error += t_error; //accumalates the error - integral term
    if (t_total_error >= t_max) t_total_error = t_max;
    else if (t_total_error <= t_min) t_total_error = t_min;

    double t_delta_error = t_error - t_last_error; //difference of error for derivative term

    t_output = t_Kp*t_error + (t_Ki*T)*t_total_error + (t_Kd/T)*t_delta_error; //PID control compute
    if (t_output >= t_max) t_output = t_max;
    else if (t_output <= t_min) t_output = t_min;

    t_last_error = t_error;
    last_time = current_time;

    analogWrite(t_inputB, t_output);

    // Serial.print(Ki, 4);
    // Serial.print(t_setpoint);
    // Serial.print(" : ");
    // Serial.print(t_tension);
    // Serial.print(" : ");
    // Serial.println(t_output);

    double f_error = f_setpoint - f_tension;
    // f_error *= -1; // sensor reads in reverse (descending). take-up only
    f_total_error += f_error; //accumalates the error - integral term
    if (f_total_error >= f_max) f_total_error = f_max;
    else if (f_total_error <= f_min) f_total_error = f_min;

    double f_delta_error = f_error - f_last_error; //difference of error for derivative term

    f_output = f_Kp*t_error + (f_Ki*T)*f_total_error + (f_Kd/T)*f_delta_error; //PID control compute
    if (f_output >= f_max) f_output = f_max;
    else if (f_output <= f_min) f_output = f_min;

    f_last_error = f_error;
    last_time = current_time;

    analogWrite(f_inputB, f_output);

    // Serial.print(Ki, 4);
    // Serial.print(t_setpoint);
    // Serial.print(" : ");
    // Serial.print(t_tension);
    // Serial.print(" : ");
    // Serial.println(f_output);

    // Serial.print(f_output);
    // Serial.print(" : ");
    // Serial.println(t_output);
  } 
}

void initMotors() {
  // -- feed motor
  pinMode(f_inputA, OUTPUT);
  pinMode(f_inputB, OUTPUT);
  // pinMode(t_enable, OUTPUT);

  // -- take-up motor
  pinMode(t_inputA, OUTPUT);
  pinMode(t_inputB, OUTPUT); 
  // pinMode(f_enable, OUTPUT);
 
  // -- capstan motor
  pinMode(c_inputA, OUTPUT);
  pinMode(c_inputB, OUTPUT);
  pinMode(c_enable, OUTPUT);
  digitalWrite(c_enable, LOW);
  delay(10);
  digitalWrite(c_enable, HIGH);
  delay(10);
  

  // set direction. HIGH = CW
  digitalWrite(f_inputA, HIGH);
  digitalWrite(t_inputA, LOW);
  digitalWrite(c_inputA, LOW);

  // set all motors to 0 torque/speed
  digitalWrite(f_inputB, 0);
  digitalWrite(t_inputB, 0);
  digitalWrite(c_inputB, 0);

}
