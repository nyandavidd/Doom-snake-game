#pragma once
#include <windows.h>
struct SCoord {
    int x, y;
    SCoord() {  x = 0;  y = 0; }
    SCoord(int _x, int _y) { x = _x; y = _y; }
    SCoord& operator +=(const SCoord& op);
};
SCoord operator +(const SCoord& op1, const SCoord& op2);
bool operator ==(const SCoord& op1, const SCoord& op2);

class CFood : public SCoord {
public:
    int grow_size = 40;
    CFood();
    CFood(SCoord scr) { x = scr.x; y = scr.y; };
    virtual int eaten();
};
class CSweet : public CFood {
public:
    CSweet();
    CSweet(SCoord scr) { x = scr.x; y = scr.y; };
    int eaten();
};
class CPoison : public CFood {
public:
    CPoison();
    CPoison(SCoord scr) { x = scr.x; y = scr.y; };
    int eaten();
};
