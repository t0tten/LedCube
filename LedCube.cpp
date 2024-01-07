#include "LedCube.h"
#include "Arduino.h"

/* SETTINGS */
#define X_SIZE                  2
#define Y_SIZE                  2
#define Z_SIZE                  2
#define IS_MULTICOLOR           true

/* SHIFT REGISTERS */
#define SHIFT_REGISTERS         4                   // Add the corresponding shift register pin layout below
#define INSTRUCTIONS            4
const short PIN_LAYOUT[SHIFT_REGISTERS][INSTRUCTIONS] = {
    {8, 7, 4, 2},       // RED
    {9, -1, -1, -1},    // GREEN
    {3, -1, -1, -1},    // BLUE
    {11, -1, -1, -1}    // Levels
};
/* 74HC595 Shift register */
#define DATA                    0
#define CLOCK                   1
#define LATCH                   2
#define RESET                   3

LedCube::LedCube()
{
    this->coordinate = new Coordinate(X_SIZE, Y_SIZE, Z_SIZE);
    this->splitOutput = (SHIFT_REGISTERS > 1) ? true : false;
    this->ledIsMulticolor = IS_MULTICOLOR;
    this->amount_colors = (this->ledIsMulticolor && !this->splitOutput) ? 3 : 1;
 
    this->ledArray = new bool[(this->coordinate->get2DSize() * this->amount_colors)];
    for (short i = 0; i < (this->coordinate->get2DSize() * this->amount_colors); i++) {
        this->ledArray[i] = false;
    }
    
    this->levelArray = new bool[this->coordinate->getZ()];
    for (short z = 0; z < this->coordinate->getZ(); z++) {
        this->levelArray[z] = true;
    }

    short numOfOutputs = (this->coordinate->get2DSize() * this->amount_colors) + this->coordinate->getZ();
    for (short shiftRegister = 0; shiftRegister < SHIFT_REGISTERS; shiftRegister++)
    {
        for (short pinIndex = 0; pinIndex < INSTRUCTIONS; pinIndex++)
        {
            if (PIN_LAYOUT[shiftRegister][pinIndex] != -1)
            {
                pinMode(PIN_LAYOUT[shiftRegister][pinIndex], OUTPUT);
            }
        }
    }
}

LedCube::~LedCube()
{
    delete this->ledArray;
    delete this->coordinate;
}

void LedCube::updateLight(Coordinate* ledCoordinate, Color* color, short delayTime)
{
    short arr_pos = (ledCoordinate->getY() * this->coordinate->getY()) + ledCoordinate->getX();
    this->updateArrays(arr_pos, ledCoordinate->getZ(), color, true);
    this->sendData(color);
    delay(delayTime);
    this->updateArrays(arr_pos, ledCoordinate->getZ(), color, false);
}

void LedCube::updateArrays(short arr_pos, short level, Color* color, bool turnOn)
{
    short highestPin = arr_pos * this->amount_colors;
    if (this->ledIsMulticolor && !this->splitOutput)
    {
        if (color->getR() && (arr_pos + this->coordinate->getZ()) < ((this->coordinate->get2DSize() * this->amount_colors) + this->coordinate->getZ()))
        {
            this->ledArray[highestPin] = turnOn;
        }

        if (color->getG() && ((arr_pos + this->coordinate->getZ()) + 1) < ((this->coordinate->get2DSize() * this->amount_colors) + this->coordinate->getZ()))
        {
            this->ledArray[highestPin + 1] = turnOn;
        }

        if (color->getB() && ((arr_pos + this->coordinate->getZ()) + 2) < ((this->coordinate->get2DSize() * this->amount_colors) + this->coordinate->getZ()))
        {
            this->ledArray[highestPin + 2] = turnOn;
        }
    }
    else
    {
        this->ledArray[highestPin] = turnOn;
    }
    this->levelArray[level] = !turnOn;
}

short LedCube::getSize()
{
    return this->coordinate->get3DSize();
}

void LedCube::sendData(Color* color)
{
    if (this->splitOutput)
    {
        for (short index = (this->coordinate->get2DSize() * this->amount_colors) - 1; index >= 0; index--)
        {
            if (this->ledArray[index])
            {
                if (color->getR()) digitalWrite(PIN_LAYOUT[0][DATA], 1);
                if (color->getG()) digitalWrite(PIN_LAYOUT[1][DATA], 1);
                if (color->getB()) digitalWrite(PIN_LAYOUT[2][DATA], 1);
            }

            if (index < this->coordinate->getZ())
            {
                if (this->levelArray[index]) digitalWrite(PIN_LAYOUT[3][DATA], 1);
            }

            digitalWrite(PIN_LAYOUT[0][CLOCK], 1);
            digitalWrite(PIN_LAYOUT[0][CLOCK], 0);

            if (this->ledArray[index])
            {
                if (color->getR()) digitalWrite(PIN_LAYOUT[0][DATA], 0);
                if (color->getG()) digitalWrite(PIN_LAYOUT[1][DATA], 0);
                if (color->getB()) digitalWrite(PIN_LAYOUT[2][DATA], 0);
            }

            if (index < this->coordinate->getZ())
            {
                if (this->levelArray[index]) digitalWrite(PIN_LAYOUT[3][DATA], 0);
            }
        }
        digitalWrite(PIN_LAYOUT[0][LATCH], 1);
        digitalWrite(PIN_LAYOUT[0][LATCH], 0);
    }
    else
    {
        for (short index = (this->coordinate->get2DSize() * this->amount_colors) - 1; index >= 0; index--)
        {
            if (this->ledArray[index]) {
                digitalWrite(PIN_LAYOUT[0][DATA], 1);
            }

            digitalWrite(PIN_LAYOUT[0][CLOCK], 1);
            digitalWrite(PIN_LAYOUT[0][CLOCK], 0);

            if (this->ledArray[index]) {
                digitalWrite(PIN_LAYOUT[0][DATA], 0);
            }
        }

        for (short index = this->coordinate->getZ() - 1; index >= 0; index--)
        {
            if (this->levelArray[index]) {
                digitalWrite(PIN_LAYOUT[0][DATA], 1);
            }

            digitalWrite(PIN_LAYOUT[0][CLOCK], 1);
            digitalWrite(PIN_LAYOUT[0][CLOCK], 0);

            if (this->levelArray[index]) {
                digitalWrite(PIN_LAYOUT[0][DATA], 0);
            }
        }
    }
}