#ifndef LED_CUBE_H
#define LED_CUBE_H

#include "containers/Coordinate.h"
#include "containers/Color.h"
#include <string>

class LedCube {
    private:
        Coordinate* coordinate;
        bool* ledArray;
        short amount_colors;
        bool ledIsMulticolor;

        /* FUNCTIONS */
        void sendData(short highestPin);
        short updateLEDArray(short arr_pos, short level, Color* color, bool turnOn);

    public:
        LedCube(Coordinate* coordinate, bool isMulticolor);
        ~LedCube();

        void updateLight(Coordinate* coordinate, Color* color, short delayTime);
        short getSize();

        void showErrorSign();

        bool isMulticolor();
};

#endif //LED_CUBE_H