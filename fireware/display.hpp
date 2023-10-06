#ifndef HELLO_KEYBOARD_DISPLAY
#define HELLO_KEYBOARD_DISPLAY
#include <lcdgfx.h>
DisplaySSD1306_128x32_I2C display(-1);
class Display {
public:
  Display() {
  }
  void setup() {
    display.begin();
    display.setFixedFont(ssd1306xled_font6x8);
    display.printFixed(0,0,"Hello,Keyboard",STYLE_NORMAL);
  }

  void update() {

  }
} Display;
#endif
