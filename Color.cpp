#include "Color.h"

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