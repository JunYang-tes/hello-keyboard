#include <Keyboard.h>
#define ROWS 5
#include "pins.hpp"
#include "keyboard-scanner.hpp"
#include <Keyboard.h>
#include "normal-layout.hpp"
#include "TPMouse.hpp"
#include "display.hpp"

NormalLayout layout;
KeyboardScanner scanner(
    rows,
    cols,
    ROW_COUNT,
    COL_COUNT
);
void setup()
{
  Display.setup();
  Serial1.begin(115200);
  Keyboard.begin();
  TPMouse.begin();
  Serial1.println("SETUP");
}
void loop()
{
  TPMouse.tick();
  scanner.scan(
        [](char row, char col) { 
            uint8_t key = layout.keydown(KeyPosition(row,col));
            if(key) {
                Keyboard.press(key);
            }
        },
        [](char row, char col) {
            uint8_t key = layout.keyup(KeyPosition(row,col));
            if(key) {
                Keyboard.release(key);
            } else {
                Keyboard.releaseAll();
            }
        }
    );
}
