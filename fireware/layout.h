#ifndef LAYOUT
#define LAYOUT
#include <cstdint>
#include <key-position.hpp>
#include <memory>
#define MAX_KEYS 6
class KeySequence {
private:
     char size;
public:
    std::unique_ptr<unsigned char[]> keys;
    bool ctrl;

    KeySequence() {
        size = 0;
        ctrl =false;
        keys = std::make_unique<unsigned char[]>(MAX_KEYS);
    }
    void put(unsigned char ch) {
        if(size<MAX_KEYS-1) {
            keys[size++]=ch;
        }
    }
    bool delete_key(unsigned char key) {
        char index = -1;
        for(char i = 0;i<this->size;i++) {
            if(keys[i] == key) {
                index = i;
            }
        }
        if(index>-1) {
            for(char i = index;i<this->size;i++) {
                keys[i] = keys[i+1];
            }
            this->size--;
            return true;
        }
        return false;
    }
    char get_size() {
      return this->size;
    }
};

typedef uint8_t SendableKey;
struct ReleaseKey {
  bool is_single_key;
  uint8_t key;
  std::unique_ptr<KeySequence> keys;
  ReleaseKey(uint8_t key) {
    is_single_key = true;
    this->key = key;
  }
  ReleaseKey(std::unique_ptr<KeySequence> keys) {
    is_single_key = false;
    this->keys = std::move(keys);
  }
};
class Layout {
public:
    virtual SendableKey keydown (KeyPosition pos) = 0;
    virtual ReleaseKey keyup(KeyPosition pos) = 0;
};

#endif
