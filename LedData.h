#ifndef LED_DATA_H
#define LED_DATA_H

#include "Instruction.h"
#include "Coordinate.h"
#include "Color.h"
#include "Arduino.h"

class LedData: public Instruction {
    private:
        Coordinate* coordinate;
        Color* color;

    public:
        LedData(Coordinate* coordinate, Color* color, short duration);
        virtual ~LedData();

        Coordinate* getCoordinate();
        Color* getColor();
        virtual bool execute(LedCube* ledCube);
};

#endif //LED_DATA_H