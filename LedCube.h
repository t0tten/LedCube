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
        bool sideOptimization;

        /* FUNCTIONS */
        void sendData(short* threshHoldPins);
        short* updateLEDArray(short arr_pos, short level, Color* color, bool turnOn);
        void fillFromRight(short allPins, short lowest);
        void fillFromLeft(short allPins, short highestPin);

    public:
        LedCube(Coordinate* coordinate, bool isMulticolor, bool useSideOptimization);
        ~LedCube();

        void updateLight(Coordinate* coordinate, Color* color, short delayTime);
        short getSize();

        void showErrorSign();

        bool isMulticolor();
};

#endif //LED_CUBE_H