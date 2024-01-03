#include "Color.h"
#include <iostream>

Color::Color(bool r, bool g, bool b)
{
    this->r = r;
    this->g = g;
    this->b = b;
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
    std::cout << "Color: (r: " << this->getR() << ", g: " << this->getG() << ", b: " << this->getB() << ")" << std::endl;
}