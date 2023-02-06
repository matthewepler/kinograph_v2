/*
 * Perf detector + camera GPIO test
 * See perfDetector and cam_gpio sketches for more detail
 */
 
const int camTriggerPin = 32;
const int camDelay = 50;
elapsedMillis timeSinceCamTrigger = camDelay + 1;  // see "elapsedMillis" test
volatile bool camFlag = false;

const int perfSignalPin = 37;
int perfCounter = 1;
int frameCounter = 0;
int prevFrameCount;


 void setup() {
  pinMode(camTriggerPin, OUTPUT);
  digitalWrite(camTriggerPin, HIGH);
  pinMode(perfSignalPin, INPUT);
  attachInterrupt(perfSignalPin, readSprocket, FALLING);
}

void loop() {
  if (timeSinceCamTrigger >= camDelay) {
    digitalWrite(camTriggerPin, HIGH);
    digitalWrite(LED_BUILTIN, LOW);
  }

//  Serial.println(perfCounter);
//  delay(10);
}

/* PERF DETECTION ISR */
void readSprocket() {
  if (perfCounter % 5 == 0) {
    triggerCamera();
    perfCounter = 1;
  } else {
    perfCounter++;
  }
}

/* CAMERA TRIGGER */
void triggerCamera() {
  digitalWrite(camTriggerPin, LOW);
  timeSinceCamTrigger = 0;
  digitalWrite(LED_BUILTIN, HIGH);
  frameCounter ++;
}
