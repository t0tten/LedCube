#include "LedCube.h"
#include <string>
#include <iostream>

LedCube::LedCube(Coordinate* coordinate, bool isMulticolor)
{
    std::cout << "Creating cube with size of: " << coordinate->get3DSize() << std::endl;
    this->coordinate = coordinate;
    this->ledIsMulticolor = isMulticolor;
    this->amount_colors = (isMulticolor) ? 3 : 1;
    this->ledArray = new bool[(this->coordinate->get2DSize() * this->amount_colors) + this->coordinate->getZ()];

    for (short z = 0; z < this->coordinate->getZ(); z++) {
        this->ledArray[(this->coordinate->get2DSize() * this->amount_colors) + z] = true;
    }
}

LedCube::~LedCube()
{
    delete this->ledArray;
    delete this->coordinate;
}

void LedCube::updateLight(Coordinate* coordinate, Color* color, short delayTime)
{
    short arr_pos = (coordinate->getY() * this->coordinate->getY()) + coordinate->getX();
    this->updateLEDArray(arr_pos, coordinate->getZ(), color, true);
    this->sendData();
    //delay(delayTime);
    this->updateLEDArray(arr_pos, coordinate->getZ(), color, false);
}

void LedCube::updateLEDArray(short arr_pos, short level, Color* color, bool turnOn)
{
    if (this->ledIsMulticolor)
    {
        short offset = this->coordinate->getX() * this->coordinate->getY();
        if (color->getR() && arr_pos < this->coordinate->get2DSize())
        {
            this->ledArray[arr_pos] = turnOn;
        }

        if (color->getG() && (arr_pos + offset) < (this->coordinate->get2DSize() * 2))
        {
            this->ledArray[arr_pos + offset] = turnOn;
        }

        if (color->getB() && (arr_pos + (offset * 2)) < (this->coordinate->get2DSize() * 3))
        {
            this->ledArray[arr_pos + (offset * 2)] = turnOn;
        }
    }
    else
    {
        this->ledArray[arr_pos] = turnOn;
    }
    this->ledArray[(this->coordinate->get2DSize() * this->amount_colors) + level] = !turnOn;
}

short LedCube::getSize()
{
    return this->coordinate->get3DSize();
}

void LedCube::sendData()
{
    short offset = this->coordinate->getX() * this->coordinate->getY();
    for (short color = 0; color < this->amount_colors; color++)
    {
        for (short y = 0; y < this->coordinate->getY(); y++)
        {
            for (short x = 0; x < this->coordinate->getX(); x++)
            {
                short arr_pos = ((y * this->coordinate->getY()) + x) + (offset * color);
                std::cout << this->ledArray[arr_pos] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    for (short z = 0; z < this->coordinate->getZ(); z++) {
        std::cout << this->ledArray[(this->coordinate->get2DSize() * this->amount_colors) + z] << " ";
    }
    std::cout << std::endl;

    std::cout << "To send: ";
    for (short index = 0; index < (this->coordinate->get2DSize() * this->amount_colors) + this->coordinate->getZ(); index++)
    {
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