#include "wiring_constants.h"
#include "wiring_digital.h"
#include "wiring_time.h"
#ifndef HELLO_KEYBOARD_TRACKPOINT
#define HELLO_KEYBOARD_TRACKPOINT
#include <Arduino.h>
#define TP_CMD_SET_REMOTE_MODE 0xf0
#define TP_CMD_READ_DATA 0xeb
#define TP_LEFT 0
#define TP_MIDDLE 2
#define TP_RIGHT 1
#define isTPButtonPressed(state, button) (((state) >> (button)) & 1)
#define isYNegative(state) ((state >> 5) & 1)
class Trackpoint {
private:
  uint8_t _clkPin;
  uint8_t _dataPin;
  uint8_t _restPin;
  void golo(uint8_t pin) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
  void gohi(uint8_t pin) { pinMode(pin, INPUT_PULLUP); }
  void waitClock() {
    while (digitalRead(_clkPin) == LOW)
      ;
    while (digitalRead(_clkPin) == HIGH)
      ;
  }
  inline void startWrite() {
    gohi(_clkPin);
    gohi(_dataPin);
    delayMicroseconds(300);
    golo(_clkPin);
    delayMicroseconds(100);
    golo(_dataPin); // start bit
    delayMicroseconds(10);
    gohi(_clkPin); // pullup clock,let it TPC enable to write
    while (digitalRead(_clkPin))
      ;
  }
  inline void writeBit(bool bit) {
    if (bit)
      gohi(_dataPin);
    else
      golo(_dataPin);
    waitClock();
  }
  inline byte readBit() {
    while (digitalRead(_clkPin) == HIGH)
      ;
    byte bit = digitalRead(_dataPin);
    while (digitalRead(_clkPin) == LOW)
      ;
    return bit;
  }

public:
  struct Data {
    byte state;
    byte x;
    byte y;
  };
  Trackpoint(uint8_t clkPin, uint8_t dataPin, uint8_t restPin)
      : _clkPin(clkPin), _dataPin(dataPin), _restPin(restPin) {
    golo(_clkPin);
    pinMode(_restPin, OUTPUT);
  }
  void idle() {
    gohi(_clkPin);
    gohi(_dataPin);
  }
  void inhibit(void) {
    gohi(_dataPin);
    golo(_clkPin);
  }
  void high(byte pin) { gohi(pin); }
  void low(byte pin) { golo(pin); }
  void hardReset() {
    digitalWrite(_restPin, HIGH);
    delay(200);
    digitalWrite(_restPin, LOW);
  }
  void write(byte data) {
    startWrite();
    byte parity = 1;
    for (byte i = 0; i < 8; i++) {
      writeBit(data & 1);
      parity = parity ^ (data & 0x01);
      data = data >> 1;
    }
    writeBit(parity);
    writeBit(1);
    golo(_clkPin);
  }
  byte writeThenRead(byte data) {
    write(data);
    return read();
  }
  Data readData() {
    writeThenRead(TP_CMD_READ_DATA);
    Data data;
    data.state = read();
    data.x = read();
    data.y = read();
    return data;
  }
  byte read() {
    byte data = 0x00;

    gohi(_clkPin);
    gohi(_dataPin);
    delayMicroseconds(50);
    readBit(); // start
    for (byte i = 0; i < 8; i++) {
      data = data | (readBit() << i);
    }
    readBit();     // pairty
    readBit();     // stop
    golo(_clkPin); // hold incoming data

    return data;
  }
};
#endif
