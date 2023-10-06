#ifndef HELLO_KEYBOARD_DISPLAY
#define HELLO_KEYBOARD_DISPLAY
#include <ssd1306.h>
class Display {
public:
  Display() {
  }
  void setup() {
    ssd1306_128x32_i2c_init();
    ssd1306_setFixedFont(ssd1306xled_font6x8);
    ssd1306_clearScreen();
    ssd1306_printFixed(0, 0, "Hello Keyboard", STYLE_NORMAL);
  }
  void update() {

  }
} Display;
#endif
