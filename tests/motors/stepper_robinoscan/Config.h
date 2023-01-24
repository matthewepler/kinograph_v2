#pragma once

#if defined(ARDUINO_TEENSY41)

const uint8_t stepPin = 10;
const uint8_t dirPin = 9;
const uint8_t enablePin = 8;
HardwareSerial &io = Serial1;

#elif defined(ARDUINO_AVR_MEGA2560)

const uint8_t stepPin = 34;
const uint8_t dirPin = 35;
const uint8_t enablePin = 33;
HardwareSerial &io = Serial3;

#else

#error Hardware not supported

#endif
