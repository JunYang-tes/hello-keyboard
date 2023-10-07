#ifndef HELLO_KEYBOARD_DISPLAY
#define HELLO_KEYBOARD_DISPLAY
#include <lcdgfx.h>
DisplaySSD1306_128x32_I2C display(-1);
#define FONT_HEIGHT 8
#define FONT_WIDTH 6
class Scene {
public:
  virtual void update();
};
class Display {
private:
  Scene* scene;
public:
  Display() {
    scene = nullptr;
  }
  void setup() {
    display.begin();
    display.setFixedFont(ssd1306xled_font6x8);
    display.clear();
    display.printFixed(0,0,"Hello,Keyboard",STYLE_NORMAL);
  }
  void setScene(Scene* scene) {
    this->scene = scene;
    display.clear();
  }
  void show() {
    if(scene) {
      scene->update();
    }
  }
} Display;

short getRowPix(char row) {
  return row*FONT_HEIGHT;
}
short getColPix(char col){
  return col*FONT_WIDTH;
}

class Status: public Scene {
private:
  bool _fn_locked;
  bool _tp_enabled;
  bool _dirty;
  char* _current_layout;
  KeyPosition pos = none;

public:
  Status() {
    _fn_locked = false;
    _tp_enabled = true;
    _current_layout = "Normal";
    _dirty = true;
  }
  void setFnLocked(bool locked) {
    _fn_locked = locked;
    _dirty = true;
  }
  void setKeyPosition(KeyPosition pos) {
    this->pos = pos;
    _dirty = true;
  }
  void clearKeyPosition() {
    this->pos = none;
    _dirty = true;
  }
  void update() {
    if(_dirty) {
      _dirty = false;
      String firstRow = "";
      firstRow += _current_layout;
      if(_fn_locked) {
        display.printFixed(
        display.width() - 9*FONT_WIDTH,
        0,
        "Fn locked"
        );
      } else {
        display.printFixed(
        display.width() - 9*FONT_WIDTH,
        0,
        "         "
        );
      }
      display.printFixed(
        0,0,
        _current_layout,
        STYLE_NORMAL
      );
      if(pos!=none) {
        String posText="(";
        posText +=(short)pos.getRow();
        posText +=",";
        posText += (short)pos.getCol();
        posText += ")";
        display.printFixed(
          getColPix(0),
          getRowPix(1),
          posText.c_str());
      }
    } 
  }
} status;
#endif
