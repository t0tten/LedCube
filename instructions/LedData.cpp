#include "LedData.h"
#include <iostream>

LedData::LedData(Coordinate* coordinate, Color* color, short duration): Instruction(duration)
{
    this->coordinate = coordinate;
    this->color = color;
}

LedData::~LedData()
{
    delete this->coordinate;
    delete this->color;
}

Coordinate* LedData::getCoordinate()
{
    return this->coordinate;
}

Color* LedData::getColor()
{
    return this->color;
}

void LedData::print()
{
    std::cout << std::endl;
    this->coordinate->print();
    if (this->color != nullptr)
    {
        this->color->print();
    }
    std::cout << "Delay: " << this->duration << "ms" << std::endl;
}

bool LedData::execute(LedCube* ledCube)
{
    this->print();
    ledCube->updateLight(this->coordinate, this->color, this->duration);
    return true;
}