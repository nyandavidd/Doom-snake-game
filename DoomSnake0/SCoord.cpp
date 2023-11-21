#include "SCoord.h"
SCoord& SCoord::operator +=(const SCoord& op) {
    x += op.x;
    y += op.y;
    return *this;
}

SCoord operator +(const SCoord& op1, const SCoord& op2) {
    return SCoord(op1.x + op2.x, op1.y + op2.y);
}

bool operator ==(const SCoord& op1, const SCoord& op2) {
    return op1.x == op2.x && op1.y == op2.y;
}



CFood::CFood() {

}


CSweet::CSweet() {

}

CPoison::CPoison() {

}

int CFood::eaten()
{
    Beep(0, 0);
    return 0;
}

int CSweet::eaten()
{
    Beep(622, 100);
    return grow_size;
}



int CPoison::eaten()
{
    Beep(1244, 1000);
    return 0;
}
