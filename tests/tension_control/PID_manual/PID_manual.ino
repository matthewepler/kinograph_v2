 #include <Bounce2.h>
Bounce bounce = Bounce();

// takeup motor
// const int enable = 20;
// const int inputA = 21;
// const int inputB = 22;
// feed motor
const int enable = 37;
const int inputA = 35;
const int inputB = 36;

double sensed_output, control_signal;
double setpoint = 130;
double Kp = 1.3; //proportional gain
double Ki = 0.000; //integral gain
double Kd = 0; //derivative gain

int T = 10; //sample time in milliseconds (ms)
unsigned long last_time;
double total_error, last_error;
int max_control = 200;
int min_control = 0;

const int resetButtonPin = 37;


void setup(){ 
// init motor
  pinMode(enable, OUTPUT);
  pinMode(inputA, OUTPUT);  // dir
  pinMode(inputB, OUTPUT); // speed
  digitalWrite(enable, LOW);
  delay(10);
  digitalWrite(enable, HIGH);
  delay(10);
  digitalWrite(inputA, HIGH);
  analogWrite(inputB, 0);

   bounce.attach( resetButtonPin ,  INPUT_PULLUP ); // USE INTERNAL PULL-UP
  bounce.interval(5); // interval in ms
}

void loop(){ 
  checkButton();

  setpoint = map(analogRead(16), 0, 1023, 109, 150);
  sensed_output = analogRead(A14);
  // Serial.println(sensed_output);

  // Kp = map(analogRead(16), 0, 1023, 0, 1000)/100.0; // oscillates at 4. Divide by 2. Kp set to 2.
  // Ki = map(analogRead(17), 0, 1023, -1000, 1000)/50000.0; // set to 0
  // Kd = map(analogRead(18), 0, 1023, -1000, 1000)/1000.0; // not used

  PID_Control(); //calls the PID function every T interval and outputs a control signal 
  delay(10);
}

void PID_Control(){

  unsigned long current_time = millis(); //returns the number of milliseconds passed since the Arduino started running the program

  int delta_time = current_time - last_time; //delta time interval 

  if (delta_time >= T){

    double error = setpoint - sensed_output;
    // error *= -1; // onlhy used for takeup

    total_error += error; //accumalates the error - integral term
    if (total_error >= max_control) total_error = max_control;
    else if (total_error <= min_control) total_error = min_control;

    double delta_error = error - last_error; //difference of error for derivative term

    control_signal = Kp*error + (Ki*T)*total_error + (Kd/T)*delta_error; //PID control compute
    if (control_signal >= max_control) control_signal = max_control;
    else if (control_signal <= min_control) control_signal = min_control;

    last_error = error;
    last_time = current_time;

    // Serial.print(Ki, 4);
    Serial.print(setpoint);
    Serial.print(" : ");
    Serial.print(sensed_output);
    Serial.print(" : ");
    Serial.println(control_signal);
    analogWrite(inputB, control_signal);
  } 
}

void checkButton() {
  // bounce.update();
  //  if ( bounce.changed() ) {
  //   // THE STATE OF THE INPUT CHANGED
  //   // GET THE STATE
  //   int deboucedInput = bounce.read();
  //   // IF THE CHANGED VALUE IS LOW
  //   if ( deboucedInput == LOW ) {
  //     // Serial.println("reset");
  //     digitalWrite(enable, LOW);
  //     delay(100);
  //     digitalWrite(enable, HIGH);
  //     delay(100);
  //   }
  // }
}