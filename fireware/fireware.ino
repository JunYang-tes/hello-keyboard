#include "keyboard-scanner.hpp"
#include "pins.hpp"
#include <Keyboard.h>
KeyboardScanner scanner(
    rows,
    cols,
    ROW_COUNT,
    COL_COUNT
);
void setup()
{
    Serial1.begin(115200);
    Keyboard.begin();
}
void loop()
{
    scanner.scan(
        [](char row, char col) { 
            String t="row:";
            t+=row;
            t+="col:";
            t+=col;
            t+="pressed";
            Serial1.println(t);
        },
        [](char row, char col) {
            String t="row:";
            t+=row;
            t+="col:";
            t+=col;
            t+="released";
            Serial1.println(t);
        }
    );
}