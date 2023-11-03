#include <Arduino.h>
#include <functional>
#include <memory>
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
  std::unique_ptr<char[]> _currentPressMap;

public:
  KeyboardScanner(char *rowPins, char *colPins, char rowCount, char colCount)
      : _rowPins(rowPins), _colPins(colPins), _rowCount(rowCount),
        _colCount(colCount) {
    _mapLength = ceil(rowCount * colCount / 8.0);
    _currentPressMap = std::make_unique<char[]>(_mapLength);
    //  disable all rows & cols
    for(char c = 0;c<_colCount;c++) {
      pinMode(_colPins[c],INPUT);
    }
    for(char r = 0;r<_rowCount;r++) {
      pinMode(_rowPins[r],INPUT);
    }
  };
  void scan(std::function<void(char, char)> onKeyPressed,
            std::function<void(char, char)> onKeyReleased) {
    for (char rowIndex = 0; rowIndex < _rowCount; rowIndex++) {
      char rowPin = _rowPins[rowIndex];
      pinMode(rowPin, OUTPUT);
      digitalWrite(rowPin, LOW);
      for (char colIndex = 0; colIndex < _colCount; colIndex++) {
        char colPin = _colPins[colIndex];
        pinMode(colPin, INPUT_PULLUP);
        char mapIndex =colIndex + rowIndex * (_colCount); 
        char byteIndex = mapIndex / 8;
        char bitIndex = mapIndex % 8;
        if (digitalRead(colPin) == LOW) {
          delay(10);
          if (digitalRead(colPin) == LOW) {
            // pressed
            if (!is_pressed(_currentPressMap, byteIndex, bitIndex)) {
              // newly pressed
              set_pressed(_currentPressMap, byteIndex, bitIndex);
              if (onKeyPressed) {
                onKeyPressed(rowIndex,colIndex);
              }
            }
          }
        } else if (is_pressed(_currentPressMap, byteIndex, bitIndex)) {
          // newly released
          clear_pressed(_currentPressMap, byteIndex, bitIndex);
          if (onKeyReleased) {
            onKeyReleased(rowIndex,colIndex);
          }
        }
        pinMode(colPin, INPUT);
      }
      pinMode(rowPin, INPUT);
    }
  }
};

#undef each_row
