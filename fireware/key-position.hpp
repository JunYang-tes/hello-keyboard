#ifndef KEY_POSITION
#define KEY_POSITION
struct KeyPosition {
    unsigned char position;
    KeyPosition(char r, char c) {
      position = r << 4 | c;
    }
    char getRow() {
        return this->position >> 4;
    }
    char getCol() {
        return this->position & 0b1111;
    }
};
bool operator==(const KeyPosition& lhs, const KeyPosition& rhs) {
    return lhs.position == rhs.position;
}
bool operator!=(const KeyPosition& lhs, const KeyPosition& rhs) {
    return !(lhs==rhs);
}
KeyPosition none(0x1111,0x1111);
#endif