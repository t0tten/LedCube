#include "Coordinate.h"

Coordinate::Coordinate(short x, short y, short z)
{
    this->x = x;
    this->y = y;
    this->z = z;

    this->size2D = this->x * this->y;
    this->size3D = this->x * this->y * this->z;
    
}

Coordinate::~Coordinate()
{

}

short Coordinate::getX()
{
    return this->x;
}

short Coordinate::getY()
{
    return this->y;
}

short Coordinate::getZ()
{
    return this->z;
}

short Coordinate::get2DSize()
{
    return this->size2D;
}

short Coordinate::get3DSize()
{
    return this->size3D;
}