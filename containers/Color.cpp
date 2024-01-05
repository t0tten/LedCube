#include "Color.h"
#include "../Log.h"

Color::Color(bool r, bool g, bool b)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->log = Log::GetInstance();
}

Color::~Color()
{

}

bool Color::getR()
{
    return this->r;
}

bool Color::getG()
{
    return this->g;
}

bool Color::getB()
{
    return this->b;
}

void Color::print()
{
    this->log->info("Color: (r: " + std::to_string(this->getR()) + ", g: " + std::to_string(this->getG()) + ", b: " + std::to_string(this->getB()) + ")");
}