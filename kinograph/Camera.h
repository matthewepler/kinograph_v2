/* IMPORTED FROM PREVIOUS SKETCH -- DO NOT USE //
 *  
 */
//#ifdef ENABLE_CAMERA_TRIGGER
//// Init STM32 timer TIM3
//STM32Timer ITimer0(TIM15);
//STM32Timer ITimer1(TIM16);
//#endif
//
//const int camera_trigger = D5;
//
//#ifdef ENABLE_CAMERA_TRIGGER
//  pinMode(camera_trigger, OUTPUT);
//  digitalWrite(camera_trigger, LOW);
//#endif
//
//void cameraTrigger()
//{
//  uint32_t t = millis();
//  digitalWrite(camera_trigger, HIGH);
//  delayMicroseconds(20);
//  digitalWrite(camera_trigger, LOW);
//  ITimer1.stopTimer();
//  lastFrameTime = lastCameraTrigger;
//  lastCameraTrigger = t;
//  frameCount++;
//  frameRate = t - lastFrameTime;
//}
//
