#ifndef COLOR_H
#define COLOR_H

class Color {
    private:
        bool r, g, b;

    public:
        Color(bool r, bool g, bool b);
        ~Color();

        bool getR();
        bool getG();
        bool getB();

        void print();
};

#endif //COLOR_H