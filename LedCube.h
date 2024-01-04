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
        bool bidirectional;
        bool splitOutput;

        /* FUNCTIONS */
        void sendData(Color* color, short* threshHoldPins);
        short* updateLEDArray(short arr_pos, short level, Color* color, bool turnOn);
        void fillFromRight(Color* color, short allPins, short lowest);
        void fillFromLeft(Color* color, short allPins, short highestPin);
        void sendToShiftRegister(short index, bool* array, short start, short stop);
        void sendToShiftRegisterRight(short shiftRegister, bool* array, short start, short stop);

    public:
        LedCube();
        ~LedCube();

        void updateLight(Coordinate* coordinate, Color* color, short delayTime);
        short getSize();

        void showErrorSign();

        bool isMulticolor();
};

#endif //LED_CUBE_H