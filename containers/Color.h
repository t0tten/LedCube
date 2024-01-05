#ifndef COLOR_H
#define COLOR_H

#include "../Log.h"

class Color {
    private:
        bool r, g, b;
        Log* log;

    public:
        Color(bool r, bool g, bool b);
        ~Color();

        bool getR();
        bool getG();
        bool getB();

        void print();
};

#endif //COLOR_H