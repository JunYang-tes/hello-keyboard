#ifndef normal_layout
#define normal_layout
#include <Keyboard.h>
#include "layout.h"
#include "key-position.hpp"
KeyPosition fn(4, 1);
KeyPosition fn1(4,13);
unsigned char keys[][14] = {
    {'`', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', KEY_BACKSPACE},
    {KEY_TAB, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\\'},
    {KEY_ESC,'a','s','d','f','g','h','j','k','l',';','\'',0,KEY_RETURN},
    {KEY_LEFT_SHIFT,'z','x','c','v','b','n','m',',','.','/',0,0,KEY_RIGHT_SHIFT},
    {KEY_LEFT_CTRL,0/*fn*/,KEY_LEFT_GUI,0,0,' ',0,0,KEY_RIGHT_ALT,KEY_DOWN_ARROW,KEY_RIGHT_ARROW,KEY_RIGHT_CTRL,0}};
unsigned char fn_keys[][14] = {
    {'`', KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12, KEY_BACKSPACE},
    {KEY_TAB, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\\'},
    {KEY_ESC,'a','s','d','f','g',KEY_LEFT_ARROW,KEY_DOWN_ARROW,KEY_UP_ARROW,KEY_RIGHT_ARROW,';','\'',0,KEY_RETURN},
    {KEY_LEFT_SHIFT,'z','x','c','v','b','n','m',',','.','/',0,0,KEY_RIGHT_SHIFT},
    {KEY_LEFT_CTRL,0/*fn*/,KEY_LEFT_GUI,0,0,' ',0,0,KEY_RIGHT_ALT,KEY_DOWN_ARROW,KEY_RIGHT_ARROW,KEY_RIGHT_CTRL,0}};

class NormalLayout : public Layout
{
private:
    bool _fn_locked;
    bool _fn_down;

public:
    NormalLayout()
    {
        _fn_down = false;
    }
    bool isFnLocked() {
        return _fn_locked;
    }
    SendableKey keydown(KeyPosition pos)
    {
        if (pos == fn)
        {
            _fn_down = true;
            return 0;
        }
        // toggle fn down;
        if(pos == fn1) {
            _fn_down = !_fn_down;
            _fn_locked = _fn_down;
            return 0;
        }
        if(_fn_down) {
            return fn_keys[pos.getRow()][pos.getCol()];
        }
        return keys[pos.getRow()][pos.getCol()];
    }
    SendableKey keyup(KeyPosition pos)
    {
        if (pos == fn)
        {
            _fn_down = false;
            _fn_locked = false;
            return 0;
        };
        if(_fn_down) {
            return fn_keys[pos.getRow()][pos.getCol()];
        }
        return keys[pos.getRow()][pos.getCol()];
    }
};
#endif
