#include <Arduino.h>
#include <functional>
#define each_row                                                               \
  for (char r = 0, rowPin = _rowPins[r]; r < _rowCount;                        \
       r++, rowPin = _rowPins[r])
#define read_map(map, byteIndex, bitIndex) ((map[byteIndex]) >> (bitIndex))
#define is_pressed(map, byteIndex, bitIndex)                                   \
  (((read_map(map, byteIndex, bitIndex))) & 1)
#define set_pressed(map, byteIndex, bitIndex) (map[byteIndex] |= (1 << bitIndex))
#define clear_pressed(map, byteIndex, bitIndex) (map[byteIndex] &= ~(1 << bitIndex))
class KeyboardScanner {
private:
  char *_rowPins;
  char *_colPins;
  char _rowCount;
  char _colCount;

  // bytes
  char _mapLength;
  char *_currentPressMap;
  char *_pressMap;

public:
  KeyboardScanner(char *rowPins, char *colPins, char rowCount, char colCount)
      : _rowPins(rowPins), _colPins(colPins), _rowCount(rowCount),
        _colCount(colCount) {
    _mapLength = ceil(rowCount * colCount / 8.0);
    _pressMap = new char(_mapLength);
    _currentPressMap = new char(_mapLength);
    memset(_pressMap, 0, _mapLength);
    memset(_currentPressMap, 0, _mapLength);
    //  disable all rows & cols
    each_row { pinMode(rowPin, INPUT); }
  };
  void scan(std::function<void(char, char)> onKeyPressed,
            std::function<void(char, char)> onKeyReleased) {
    for (char c = 0; c < _colCount; c++) {
      char colPin = _colPins[c];
      pinMode(colPin, OUTPUT);
      digitalWrite(colPin, LOW);
      for (char r = 0; r < _rowCount; r++) {
        char rowPin = _rowPins[r];
        pinMode(rowPin, INPUT_PULLUP);
        char mapIndex = c + r * (_colCount);
        char byteIndex = mapIndex / 8;
        char bitIndex = mapIndex % 8;
        if (digitalRead(rowPin) == LOW) {
          delay(10);
          if (digitalRead(rowPin) == LOW) {
            // pressed
            if (!is_pressed(_currentPressMap, byteIndex, bitIndex)) {
              // newly pressed
              set_pressed(_currentPressMap, byteIndex, bitIndex);
              if (onKeyPressed) {
                onKeyPressed(r, c);
              }
            }
          }
        } else if (is_pressed(_currentPressMap, byteIndex, bitIndex)) {
          // newly released
          clear_pressed(_currentPressMap, byteIndex, bitIndex);
          if (onKeyReleased) {
            onKeyReleased(r, c);
          }
        }
        pinMode(rowPin, INPUT);
      }
      pinMode(colPin, INPUT);
    }
  }
};

#undef each_row
