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

void LedData::print()
{
    this->log->info("");
    this->coordinate->print();
    if (this->color != nullptr)
    {
        this->color->print();
    }
    this->log->info("Delay: " + std::to_string(this->duration) + "ms");
}

bool LedData::execute(LedCube* ledCube)
{
    this->print();
    ledCube->updateLight(this->coordinate, this->color, this->duration);
    return true;
}