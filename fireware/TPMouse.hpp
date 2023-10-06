#ifndef TPMOUSE
#define TPMOUSE
#include "HardwareSerial.h"
#include "Trackpoint.hpp"
#include "wiring_time.h"
#include <Mouse.h>
#define CLK PB1
#define DATA PB11
#define REST PB10
#define DEBOUNCE_MS 0
#include <ssd1306.h>
class TPMouse
{
private:
  uint32_t _lastReadTime;
  Trackpoint _tp = Trackpoint(CLK, DATA, REST);

public:
  TPMouse() {}
  void begin()
  {
    _tp.hardReset();
    _tp.writeThenRead(TP_CMD_SET_REMOTE_MODE);
    Mouse.begin();
  }
  void tick()
  {
    auto now = millis();
    if (now - _lastReadTime > DEBOUNCE_MS)
    {
      _lastReadTime = now;
      auto data = _tp.readData();
      if (isTPButtonPressed(data.state, TP_MIDDLE) && data.y !=0)
      {
        Mouse.move(0, 0, isYNegative(data.state) ? -1 : 1);
      }
      else
      {
        Mouse.move(data.x, -data.y);
      }
      if (isTPButtonPressed(data.state, TP_LEFT))
      {
        Mouse.press(MOUSE_LEFT);
      }
      else if (Mouse.isPressed(MOUSE_LEFT))
      {
        Mouse.release(MOUSE_LEFT);
      }
      if (isTPButtonPressed(data.state, TP_RIGHT))
      {
        Mouse.press(MOUSE_RIGHT);
      }
      else if (Mouse.isPressed(MOUSE_RIGHT))
      {
        Mouse.release(MOUSE_RIGHT);
      }
    }
  }
} TPMouse;
#endif
