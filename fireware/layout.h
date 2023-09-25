#ifndef LAYOUT
#define LAYOUT
#include <key-position.hpp>
typedef uint8_t SendableKey;
class Layout {
public:
    virtual SendableKey keydown (KeyPosition pos) = 0;
    virtual SendableKey keyup(KeyPosition pos) = 0;
};

#endif
