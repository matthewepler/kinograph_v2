#include "Config.h"
#include "ContinuousStepper.h"

ContinuousStepper stepper(stepPin, dirPin, enablePin);

enum class command : uint8_t {
  spin,
  stop,
  setAcceleration,
  powerOn,
  powerOff,
  isSpinning,
  speed,
};

using float_t = float;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.begin(115200);
  Serial.println("Boot");

  io.begin(19200);
  io.setTimeout(100);

  stepper.setPulseWidth(2);  // Increase this value if the motor doesn't move

  stepper.setAcceleration(4000);

  Serial.println("Ready");
}

void loop() {
  if (io.available()) {
    command cmd;
    read(cmd);

    switch (cmd) {
    case command::spin: {
      float_t speed;
      if (read(speed)) {
        Serial.print("spin(");
        Serial.print(speed);
        Serial.println(")");
        stepper.spin(speed);
      } else {
        Serial.println("spin(INVALID)");
      }
      break;
    }

    case command::setAcceleration: {
      float_t acceleration;
      if (read(acceleration)) {
        Serial.print("setAcceleration(");
        Serial.print(acceleration);
        Serial.println(")");
        stepper.setAcceleration(acceleration);
      } else {
        Serial.println("setAcceleration(INVALID)");
      }
      break;
    }

    case command::stop:
      Serial.println("stop()");
      stepper.stop();
      break;

    case command::powerOn:
      Serial.println("powerOn()");
      stepper.powerOn();
      break;

    case command::powerOff:
      Serial.println("powerOff()");
      stepper.powerOff();
      break;

    case command::isSpinning: {
      bool isSpinning = stepper.isSpinning();
      Serial.print("isSpinning() -> ");
      Serial.println(isSpinning ? "true" : "false");
      write(isSpinning);
      break;
    }

    case command::speed: {
      float_t speed = stepper.speed();
      Serial.print("speed() -> ");
      Serial.println(speed);
      write(speed);
      break;
    }

    default:
      Serial.print("Unknown command 0x");
      Serial.println((int)cmd, HEX);
      break;
    }
  }

  stepper.loop();
}

template <typename T>
bool read(T &value) {
  return io.readBytes(reinterpret_cast<char *>(&value), sizeof(value)) == sizeof(value);
}

template <typename T>
void write(const T &value) {
  io.write(reinterpret_cast<const char *>(&value), sizeof(value));
}
