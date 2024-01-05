#ifndef COORDINATE_H
#define COORDINATE_H

#include "../Log.h"

class Coordinate {
    private:
        short x, y, z;
        short size2D, size3D;
        Log* log;

    public:
        Coordinate(short x, short y, short z);
        ~Coordinate();

        short getX();
        short getY();
        short getZ();

        short get2DSize();
        short get3DSize();
        short getFull3DSize();

        void print();
};

#endif //COORDINATE_H