#ifndef COORDINATE_H
#define COORDINATE_H

#include "Arduino.h"

class Coordinate {
    private:
        short x, y, z;
        short size2D, size3D;

    public:
        Coordinate(short x, short y, short z);
        ~Coordinate();

        short getX();
        short getY();
        short getZ();

        short get2DSize();
        short get3DSize();
        short getFull3DSize();
};

#endif //COORDINATE_H