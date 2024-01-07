#ifndef LED_CUBE_H
#define LED_CUBE_H

#include "Coordinate.h"
#include "Color.h"
#include "Arduino.h"

class LedCube {
    private:
        Coordinate* coordinate;
        bool* ledArray;
        bool* levelArray;

        short amount_colors;
        bool ledIsMulticolor;
        bool splitOutput;

        /* FUNCTIONS */
        void sendData(Color* color);
        void updateArrays(short arr_pos, short level, Color* color, bool turnOn);

    public:
        LedCube();
        ~LedCube();

        void updateLight(Coordinate* coordinate, Color* color, short delayTime);
        short getSize();
};

#endif //LED_CUBE_H