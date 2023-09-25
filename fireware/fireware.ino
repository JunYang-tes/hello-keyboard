#include <Keyboard.h>
#define ROWS 5
#include "pins.hpp"
#include "keyboard-scanner.hpp"
#include <Keyboard.h>
#include "normal-layout.hpp"
NormalLayout layout;
KeyboardScanner scanner(
    rows,
    cols,
    ROW_COUNT,
    COL_COUNT
);
void setup()
{
  Keyboard.begin();
}
void loop()
{
  delay(100);
  scanner.scan(
        [](char row, char col) { 
            uint8_t key = layout.keydown(KeyPosition(row,col));
            if(key) {
                Keyboard.press(key);
            }
        },
        [](char row, char col) {
            uint8_t key = layout.keydown(KeyPosition(row,col));
            if(key) {
                Keyboard.release(key);
            } else {
                Keyboard.releaseAll();
            }
        }
    );
}
