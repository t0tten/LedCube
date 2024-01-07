#include "LedData.h"

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

bool LedData::execute(LedCube* ledCube)
{
    ledCube->updateLight(this->coordinate, this->color, this->duration);
    return true;
}