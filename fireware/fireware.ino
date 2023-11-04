#include <Keyboard.h>
#define ROWS 5
#include "pins.hpp"
#include "keyboard-scanner.hpp"
#include <Keyboard.h>
#include "normal-layout.hpp"
#include "TPMouse.hpp"
#include "display.hpp"
#include <memory>

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
  Display.setScene(&status);
  Serial1.begin(115200);
  Keyboard.begin();
  TPMouse.begin();
  Serial1.println("SETUP");
}
void loop()
{
  TPMouse.tick();
  Display.show();
  scanner.scan(
        [](char row, char col) { 
            uint8_t key = layout.keydown(KeyPosition(row,col));
            status.setKeyPosition(KeyPosition(row,col));
            if(key) {
                Keyboard.press(key);
            }
        },
        [](char row, char col) {
            auto key = layout.keyup(KeyPosition(row,col));
            if(key.is_single_key) {
              if(key.key) {
                Keyboard.release(key.key);
              }
            } else {
              auto& key_seq = key.keys;
              if(key_seq->ctrl) {
                Keyboard.press(KEY_LEFT_CTRL);
              }
              for(char i =0;i<key_seq->get_size();i++) {
                Keyboard.press(key_seq->keys[i]);
              }
              for(char i =0;i<key_seq->get_size();i++) {
                Keyboard.release(key_seq->keys[i]);
              }
              if(key_seq->ctrl) {
                Keyboard.release(KEY_LEFT_CTRL);
              }
            }
        }
    );
  status.setFnLocked(layout.isFnLocked());
}
