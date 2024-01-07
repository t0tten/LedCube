#ifndef COLOR_H
#define COLOR_H

#include "Arduino.h"

class Color {
    private:
        bool r, g, b;

    public:
        Color(bool r, bool g, bool b);
        ~Color();

        bool getR();
        bool getG();
        bool getB();
};

#endif //COLOR_H