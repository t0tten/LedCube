#include "LedCube.h"
#include <string>
#include <iostream>

/* SETTINGS */
#define X_SIZE                  8
#define Y_SIZE                  8
#define Z_SIZE                  8
#define IS_MULTICOLOR           true
#define IS_BIDIRECTIONAL        false    // If active - add +1 to PIN_LAYOUT (RIGHT)

/* SHIFT REGISTERS */
#define RESET                   0
#define ENABLE                  1
#define STORE                   2
#define SEND                    3
#define RIGHT                   4       // Used when IS_BIDIRECTIONAL is active

#define SHIFT_REGISTERS         3       // Add the corresponding pin layout below
const short PIN_LAYOUT[SHIFT_REGISTERS][4] = {
        {0, 1, 2, 3},
        {5, 6, 7, 8},
        {10, 11, 12, 13}
};

LedCube::LedCube()
{
    this->coordinate = new Coordinate(X_SIZE, Y_SIZE, Z_SIZE);
    std::cout << "Creating cube with size of: " << this->coordinate->get3DSize();
    std::cout << " (x: " << this->coordinate->getX() << ", y: " << this->coordinate->getY() << ", z: " << this->coordinate->getZ() << ")" << std::endl;
    this->splitOutput = (SHIFT_REGISTERS > 1) ? true : false;
    this->ledIsMulticolor = IS_MULTICOLOR;
    this->bidirectional = IS_BIDIRECTIONAL;
    this->amount_colors = (this->ledIsMulticolor && !this->splitOutput) ? 3 : 1;

    this->ledArray = new bool[(this->coordinate->get2DSize() * this->amount_colors) + this->coordinate->getZ()];

    for (short z = 0; z < this->coordinate->getZ(); z++) {
        this->ledArray[z] = true;
    }
    std::cout << "Number of outputs: " << (this->coordinate->get2DSize() * this->amount_colors) + this->coordinate->getZ() << std::endl;
    std::cout << "Cube is multicolor: " << this->ledIsMulticolor << std::endl;
    std::cout << "Utilize side shift optimization: " << this->bidirectional << std::endl;
}

LedCube::~LedCube()
{
    delete this->ledArray;
    delete this->coordinate;
}

void LedCube::updateLight(Coordinate* ledCoordinate, Color* color, short delayTime)
{
    short arr_pos = (ledCoordinate->getY() * this->coordinate->getY()) + ledCoordinate->getX();
    short* threshHoldPins = this->updateLEDArray(arr_pos, ledCoordinate->getZ(), color, true);
    this->sendData(color, threshHoldPins);
    //delay(delayTime);
    this->updateLEDArray(arr_pos, ledCoordinate->getZ(), color, false);
}

short* LedCube::updateLEDArray(short arr_pos, short level, Color* color, bool turnOn)
{
    short lowestPin = -1;
    short highestPin = (arr_pos * this->amount_colors) + this->coordinate->getZ();
    if (this->ledIsMulticolor && !this->splitOutput)
    {
        if (color->getR() && (arr_pos + this->coordinate->getZ()) < ((this->coordinate->get2DSize() * this->amount_colors) + this->coordinate->getZ()))
        {
            lowestPin = highestPin;
            this->ledArray[highestPin] = turnOn;
        }

        if (color->getG() && ((arr_pos + this->coordinate->getZ()) + 1) < ((this->coordinate->get2DSize() * this->amount_colors) + this->coordinate->getZ()))
        {
            highestPin = (arr_pos * this->amount_colors) + this->coordinate->getZ() + 1;
            if (lowestPin == -1)
            {
                lowestPin = highestPin;
            }
            this->ledArray[highestPin] = turnOn;
        }

        if (color->getB() && ((arr_pos + this->coordinate->getZ()) + 2) < ((this->coordinate->get2DSize() * this->amount_colors) + this->coordinate->getZ()))
        {
            highestPin = (arr_pos * this->amount_colors) + this->coordinate->getZ() + 2;
            if (lowestPin == -1)
            {
                lowestPin = highestPin;
            }
            this->ledArray[highestPin] = turnOn;
        }
    }
    else
    {
        this->ledArray[highestPin] = turnOn;
        lowestPin = highestPin;
    }
    this->ledArray[level] = !turnOn;
    short* thresholdPins = new short[2];
    thresholdPins[0] = lowestPin;
    thresholdPins[1] = highestPin;
    return thresholdPins;
}

short LedCube::getSize()
{
    return this->coordinate->get3DSize();
}

void LedCube::sendData(Color* color, short* threshHoldPins)
{
    for (short color = 0; color < this->amount_colors; color++)
    {
        for (short y = 0; y < this->coordinate->getY(); y++)
        {
            for (short x = 0; x < this->coordinate->getX(); x++)
            {
                short arr_pos = ((y * this->coordinate->getY()) + (x)) * this->amount_colors + color;
                std::cout << this->ledArray[arr_pos + this->coordinate->getZ()] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    for (short z = 0; z < this->coordinate->getZ(); z++) {
        std::cout << this->ledArray[z] << " ";
    }
    std::cout << std::endl;

    short allPins = (this->coordinate->get2DSize() * this->amount_colors) + this->coordinate->getZ();
    /*
     * Optimize bits to send from either left or right depending on
     * how far away we are from the middle of the array
     */
    if (this->bidirectional) {
        short mid = (allPins) / 2;
        short lowest = threshHoldPins[0] - this->coordinate->getZ();
        short highest = threshHoldPins[1] - this->coordinate->getZ();
        short lowDiff = lowest - mid;
        short highDiff = mid - highest;

        if (lowDiff >= 0) {
            this->fillFromRight(color, allPins, threshHoldPins[0]);
        } else if (highDiff >= 0) {
            this->fillFromLeft(color, allPins, threshHoldPins[1]);
        }
    }
    else {
        this->fillFromLeft(color, allPins, threshHoldPins[1]);
    }
    delete threshHoldPins;
}

void LedCube::fillFromRight(Color* color, short allPins, short lowest)
{
    /* Reset all shift registers */
    for (short shiftRegister = 0; shiftRegister < SHIFT_REGISTERS; shiftRegister++)
    {
        std::cout << "RESET on pin: " << PIN_LAYOUT[shiftRegister][RESET] << std::endl;
    }

    std::string output = "";
    short stop = (this->splitOutput) ? lowest : (lowest - this->coordinate->getZ());
    for (short index = (allPins - 1); index >= stop; index--) {
        output += this->ledArray[index] ? "1" : "0";
    }
    std::cout << std::endl;
    short data = 0;
    if (this->splitOutput)
    {
        if (color->getR())
        {
            std::string outputPlus = "";
            for (short z = 0; z < this->coordinate->getZ(); z++)
            {
                outputPlus += "0";
            }
            data += (allPins - lowest + 1) + this->coordinate->getZ();
            std::cout << "DATA: " << data << std::endl;
            std::cout << "Sending to RED from RIGHT: "  << output << outputPlus << std::endl;
            this->sendToShiftRegisterRight(0, this->ledArray, allPins, lowest - this->coordinate->getZ());
            std::cout << std::endl;
        }
        if (color->getG())
        {
            data += allPins - lowest + 1;
            std::cout << "Sending to GREEN from RIGHT: "  << output << std::endl;
            this->sendToShiftRegisterRight(1, this->ledArray, allPins, lowest);
            std::cout << std::endl;
        }
        if (color->getB())
        {
            data += allPins - lowest + 1;
            std::cout << "Sending to BLUE from RIGHT: "  << output << std::endl;
            this->sendToShiftRegisterRight(2, this->ledArray, allPins, lowest);
            std::cout << std::endl;
        }
    }
    else {
        data += (allPins - lowest + 1) + this->coordinate->getZ();
        std::cout << "Sending from RIGHT: "  << output << std::endl;
        this->sendToShiftRegisterRight(0, this->ledArray, allPins, lowest - this->coordinate->getZ());
        std::cout << std::endl;
    }

    std::cout << "To send from left: ";
    for (short z = 0; z < this->coordinate->getZ(); z++) {
        std::cout << this->ledArray[z];
    }
    std::cout << std::endl;
    data += this->coordinate->getZ();
    this->sendToShiftRegister(0, this->ledArray, 0, this->coordinate->getZ());

    std::cout << std::endl;
    std::cout << "Data to send: " << data << std::endl;
}
void LedCube::fillFromLeft(Color* color, short allPins, short highestPin)
{
    std::string output = "";
    short stop = (highestPin < allPins) ? (highestPin + 1) : allPins;
    short start = (this->splitOutput) ? this->coordinate->getZ() : 0;
    for (short index = start; index < stop; index++) {
        output += (this->ledArray[index]) ? "1" : "0";
    }

    /* Reset all shift registers */
    for (short shiftRegister = 0; shiftRegister < SHIFT_REGISTERS; shiftRegister++)
    {
        std::cout << "RESET on pin: " << PIN_LAYOUT[shiftRegister][RESET] << std::endl;
    }

    short data = 0;
    if (this->splitOutput)
    {
        std::string layers = "";
        for (short index = 0; index < this->coordinate->getZ(); index++)
        {
            layers += (this->ledArray[index]) ? "1" : "0";
        }
        std::cout << "Sending to RED: " << layers << std::endl;
        this->sendToShiftRegister(0, this->ledArray, 0, this->coordinate->getZ());
        data += this->coordinate->getZ();
        std::cout << std::endl;
        if (color->getR())
        {
            data += output.size();
            std::cout << "Sending to RED: "  << output << std::endl;
            this->sendToShiftRegister(0, this->ledArray, this->coordinate->getZ(), stop);
            std::cout << std::endl;
        }
        if (color->getG())
        {
            data += output.size();
            std::cout << "Sending to GREEN: " << output << std::endl;
            this->sendToShiftRegister(1, this->ledArray, this->coordinate->getZ(), stop);
            std::cout << std::endl;
        }
        if (color->getB())
        {
            data += output.size();
            std::cout << "Sending to BLUE: " << output << std::endl;
            this->sendToShiftRegister(2, this->ledArray, this->coordinate->getZ(), stop);
            std::cout << std::endl;
        }
    }
    else
    {
        std::cout << "Sending: " << output << std::endl;
        data += output.size();
        this->sendToShiftRegister(0, this->ledArray, start, stop);
        std::cout << std::endl;
    }
    std::cout << "Data to send: " << data << std::endl;
}

void LedCube::sendToShiftRegister(short shiftRegister, bool* array, short start, short stop)
{
    for (short index = start; index < stop; index++) {
        if (array[index]) {
            std::cout << "ENABLE on pin: " << PIN_LAYOUT[shiftRegister][ENABLE] << std::endl;
        }
        std::cout << "STORE on pin: " << PIN_LAYOUT[shiftRegister][STORE] << std::endl;
    }
    std::cout << "SEND on pin: " << PIN_LAYOUT[shiftRegister][SEND] << std::endl;
}

void LedCube::sendToShiftRegisterRight(short shiftRegister, bool* array, short start, short stop)
{
    for (short index = start - 1; index >= stop; index--) {
        if (array[index]) {
            std::cout << "ENABLE on pin: " << PIN_LAYOUT[shiftRegister][ENABLE] << std::endl;
        }
        std::cout << "STORE on pin: " << PIN_LAYOUT[shiftRegister][STORE] << std::endl;
    }
    std::cout << "Enable RIGHT on pin: " << PIN_LAYOUT[shiftRegister][RIGHT] << std::endl;
    std::cout << "SEND on pin: " << PIN_LAYOUT[shiftRegister][SEND] << std::endl;
}

bool LedCube::isMulticolor()
{
    return this->ledIsMulticolor;
}

void LedCube::showErrorSign()
{
    while (1) {
        // flash red
        this->updateLight(new Coordinate(3, 0, 3), new Color(true, false, false), 50);
        this->updateLight(new Coordinate(0, 3, 3), new Color(true, false, false), 50);
        this->updateLight(new Coordinate(3, 3, 3), new Color(true, false, false), 50);
        this->updateLight(new Coordinate(0, 0, 3), new Color(true, false, false), 50);

        this->updateLight(new Coordinate(3, 0, 0), new Color(true, false, false), 50);
        this->updateLight(new Coordinate(0, 3, 0), new Color(true, false, false), 50);
        this->updateLight(new Coordinate(3, 3, 0), new Color(true, false, false), 50);
        this->updateLight(new Coordinate(0, 0, 0), new Color(true, false, false), 50);
    }
}