///* IMPORTED FROM PREVIOUS SKETCH -- DO NOT USE */
//
//const int sprocket_int = D2;
//volatile uint32_t lastSprocketTime = 0;
//volatile uint32_t sprocketTimeMs = 0;
//volatile float avgSprocketMs = 0.0;
//volatile uint32_t sprocketCount = 0;
//volatile uint32_t lastMissingTime = 0;
//volatile uint32_t lastCameraTrigger = 0;
//volatile uint32_t recentMissingSprockets = 0;
//volatile uint32_t totalMissingSprockets = 0;
//volatile uint8_t sprocketFrameCount = 0;
//volatile uint8_t sprocketMissingFrameCount = 0;
//volatile uint32_t frameCount = 0;
//volatile uint32_t frameRate = 0;
//volatile uint32_t lastFrameTime = 0;
//
//pinMode(sprocket_int, INPUT);
//attachInterrupt(sprocket_int, readSprocket, FALLING);
//
//#ifdef ENABLE_CAMERA_TRIGGER
//// Interval in microsecs
//if (ITimer0.attachInterruptInterval(100 * 1000, missingSprocket))
//{
//  SerialPort.print("Starting  Timer0 OK, millis() = ");
//  SerialPort.print(millis());
//}
//else
//  SerialPort.print("Can't set ITimer0. Select another freq. or timer");
//
//// Interval in microsecs
//if (ITimer1.attachInterruptInterval(100 * 1000, missingSprocket))
//{
//  SerialPort.print("Starting  Timer1 OK, millis() = ");
//  SerialPort.print(millis());
//  ITimer1.stopTimer();
//}
//else
//  SerialPort.print("Can't set ITimer0. Select another freq. or timer");
//
//#endif
//
//void readSprocket()
//{
//  uint32_t t = millis();
//  sprocketTimeMs = t - lastSprocketTime;
//  avgSprocketMs = (1.0 - SPROCKET_AVG) * avgSprocketMs + SPROCKET_AVG * sprocketTimeMs;
//  sprocketCount++;
//#ifdef ENABLE_CAMERA_TRIGGER
//  if ( avgSprocketMs > SPROCKET_MIN_MS )
//    ITimer0.setInterval((uint32_t)(avgSprocketMs * 1000), missingSprocket);
//
//  if (((t - lastSprocketTime) > SPROCKET_MIN_MS)
//      && ((t - lastSprocketTime) >= (int)(avgSprocketMs / 2))
//      && ((t - lastMissingTime) >= (int)(avgSprocketMs / 2)))
//  {
//    if (sprocketFrameCount == sprocketMissingFrameCount)
//    {
//      sprocketFrameCount = (sprocketFrameCount + 1) % SPROCKETS_PER_FRAME;
//      sprocketMissingFrameCount = sprocketFrameCount;
//    }
//    else
//    {
//      sprocketFrameCount = sprocketMissingFrameCount;
//    }
//
//    if ( sprocketFrameCount == FRAME_OFFSET_SPROCKET )
//    {
//      ITimer1.restartTimer();
//      ITimer1.setInterval((uint32_t)(FRAME_OFFSET_MS * 1000), cameraTrigger);
//    }
//  }
//#endif
//  recentMissingSprockets = 0;
//  lastSprocketTime = t;
//}
//
///// interrupt for timer
//
//void missingSprocket()
//{
//  uint32_t t = millis();
//#ifdef ENABLE_CAMERA_TRIGGER
//  sprocketFrameCount = (sprocketFrameCount + 1) % SPROCKETS_PER_FRAME;
//
//  if (((t - lastSprocketTime) > SPROCKET_MIN_MS)
//      && ((t - lastSprocketTime) >= (int)(avgSprocketMs / 2))
//      && ((t - lastMissingTime) >= (int)(avgSprocketMs / 2)))
//  {
//    //    sprocketFrameCount = (sprocketFrameCount + 1) % SPROCKETS_PER_FRAME;
//    sprocketMissingFrameCount = (sprocketMissingFrameCount + 1) %  SPROCKETS_PER_FRAME;
//
//    if ( sprocketMissingFrameCount == FRAME_OFFSET_SPROCKET )
//    {
//      ITimer1.restartTimer();
//      ITimer1.setInterval((uint32_t)(FRAME_OFFSET_MS * 1000), cameraTrigger);
//    }
//
//    if ( sprocketCount > 0 )
//    {
//      recentMissingSprockets++;
//      totalMissingSprockets++;
//    }
//  }
//#endif
//
//  lastMissingTime = t;
//}
