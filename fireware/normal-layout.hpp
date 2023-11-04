#ifndef normal_layout
#define normal_layout
#include "key-position.hpp"
#include "layout.h"
#include <Keyboard.h>
#include <memory>
KeyPosition fn(4, 1);
KeyPosition fn1(4,12);
KeyPosition esc(2, 0);
unsigned char keys[][14] = {
    {'`', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
     KEY_BACKSPACE},
    {KEY_TAB, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\\'},
    {KEY_ESC,'a','s','d','f','g','h','j','k','l',';','\'',KEY_RETURN,0},
    {KEY_LEFT_SHIFT,'z','x','c','v','b','n','m',',','.','/',0,KEY_RIGHT_SHIFT,0},
    {KEY_LEFT_CTRL,0/*fn*/,KEY_LEFT_GUI,0,' ',0,0,0,KEY_RIGHT_ALT,KEY_DOWN_ARROW,KEY_RIGHT_ARROW,KEY_RIGHT_CTRL,0}};
unsigned char fn_keys[][14] = {
    {'`', KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8,
     KEY_F9, KEY_F10, KEY_F11, KEY_F12, KEY_BACKSPACE},
    {KEY_TAB, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\\'},
    {KEY_ESC,'a','s','d','f','g',KEY_LEFT_ARROW,KEY_DOWN_ARROW,KEY_UP_ARROW,KEY_RIGHT_ARROW,';','\'',KEY_RETURN,0},
    {KEY_LEFT_SHIFT,'z','x','c','v','b','n','m',',','.','/',0,KEY_RIGHT_SHIFT,0},
    {KEY_LEFT_CTRL,0/*fn*/,KEY_LEFT_GUI,0,' ',0,0,0,KEY_RIGHT_ALT,KEY_DOWN_ARROW,KEY_RIGHT_ARROW,KEY_RIGHT_CTRL,0}};

class NormalLayout : public Layout {
private:
  bool _fn_locked;
  bool _fn_down;
  bool _esc_down;
  bool _anykey_pressed_after_esc;

public:
  NormalLayout() { _fn_down = false; }
  bool isFnLocked() { return _fn_locked; }
  SendableKey keydown(KeyPosition pos) {
    if (pos == esc) {
      _esc_down = true;
      return 0;
    }
    if (_esc_down) {
      if (_anykey_pressed_after_esc) {
        return 0;
      }
      _anykey_pressed_after_esc = true;
      return KEY_LEFT_CTRL;
    }
    if (pos == fn) {
      _fn_down = true;
      return 0;
    }
    // toggle fn down;
    if (pos == fn1) {
      _fn_down = !_fn_down;
      _fn_locked = _fn_down;
      return 0;
    }
    if (_fn_down) {
      return fn_keys[pos.getRow()][pos.getCol()];
    }
    return keys[pos.getRow()][pos.getCol()];
  }
  ReleaseKey keyup(KeyPosition pos) {
    if (_esc_down) {
      if (pos != esc) {
        auto key_seq = std::make_unique<KeySequence>();
        key_seq->put(keys[pos.getRow()][pos.getCol()]);
        return ReleaseKey(std::move(key_seq));
      } else {
        _esc_down = false;
        if (_anykey_pressed_after_esc) {
          _anykey_pressed_after_esc = false;
          return ReleaseKey(KEY_LEFT_CTRL);
        } else {
          auto key_seq = std::make_unique<KeySequence>();
          key_seq->put(KEY_ESC);
          return ReleaseKey(std::move(key_seq));
        }
      }
    }

    if (pos == fn) {
      _fn_down = false;
      _fn_locked = false;
      return ReleaseKey(0);
    };
    if (_fn_down) {
      return ReleaseKey(fn_keys[pos.getRow()][pos.getCol()]);
    }
    return ReleaseKey(keys[pos.getRow()][pos.getCol()]);
  }
};
#endif
