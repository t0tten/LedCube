#include "LedCube.h"
#include <string>
#include <iostream>

LedCube::LedCube(Coordinate* coordinate, bool isMulticolor, bool useSideOptimization)
{
    std::cout << "Creating cube with size of: " << coordinate->get3DSize();
    std::cout << " (x: " << coordinate->getX()<< ", y: " << coordinate->getY() << ", z: " << coordinate->getZ() << ")" << std::endl;
    std::cout << "Cube is multicolor: " << isMulticolor << std::endl;
    std::cout << "Utilize side shift optimization: " << useSideOptimization << std::endl;
    this->coordinate = coordinate;
    this->ledIsMulticolor = isMulticolor;
    this->amount_colors = (isMulticolor) ? 3 : 1;
    this->sideOptimization = useSideOptimization;
    this->ledArray = new bool[(this->coordinate->get2DSize() * this->amount_colors) + this->coordinate->getZ()];

    for (short z = 0; z < this->coordinate->getZ(); z++) {
        this->ledArray[z] = true;
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
    short* threshHoldPins = this->updateLEDArray(arr_pos, ledCoordinate->getZ(), color, true);
    this->sendData(threshHoldPins);
    //delay(delayTime);
    this->updateLEDArray(arr_pos, ledCoordinate->getZ(), color, false);
}

short* LedCube::updateLEDArray(short arr_pos, short level, Color* color, bool turnOn)
{
    short lowestPin = -1;
    short highestPin = (arr_pos * this->amount_colors) + this->coordinate->getZ();
    if (this->ledIsMulticolor)
    {
        if (color->getR() && (arr_pos + this->coordinate->getZ()) < ((this->coordinate->get2DSize() * this->amount_colors) + this->coordinate->getZ()))
        {
            //highestPin = (arr_pos * this->amount_colors) + this->coordinate->getZ();
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

void LedCube::sendData(short* threshHoldPins)
{
    for (short color = 0; color < this->amount_colors; color++)
    {
        for (short y = 0; y < this->coordinate->getY(); y++)
        {
            for (short x = 0; x < this->coordinate->getX(); x++)
            {
                short arr_pos = ((y * this->coordinate->getY()) + (x)) * this->amount_colors + color; // * this->amount_colors));// * color; //this->amount_colors));
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
    if (this->sideOptimization) {
        short mid = (allPins) / 2;
        short lowest = threshHoldPins[0] - this->coordinate->getZ();
        short highest = threshHoldPins[1] - this->coordinate->getZ();
        short lowDiff = lowest - mid;
        short highDiff = mid - highest;

        if (lowDiff >= 0) {
            this->fillFromRight(allPins, threshHoldPins[0]);
        } else if (highDiff >= 0) {
            this->fillFromLeft(allPins, threshHoldPins[1]);
        }
    }
    else {
        this->fillFromLeft(allPins, threshHoldPins[1]);
    }
    delete threshHoldPins;
}

void LedCube::fillFromRight(short allPins, short lowest)
{
    std::cout << "To send from right: ";
    for (short index = allPins; index >= (lowest - this->coordinate->getZ()); index--) {
        std::cout << this->ledArray[index];
    }
    std::cout << std::endl;
    std::cout << "To send from left: ";
    for (short z = 0; z < this->coordinate->getZ(); z++) {
        std::cout << this->ledArray[z];
    }
    std::cout << std::endl;
}
void LedCube::fillFromLeft(short allPins, short highestPin)
{
    std::cout << "To send: ";
    short stop = (highestPin < allPins) ? (highestPin + 1) : allPins;
    for (short index = 0; index < stop; index++) {
        std::cout << this->ledArray[index];
    }
    std::cout << std::endl;
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