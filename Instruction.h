#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "LedCube.h"
#include "Arduino.h"

class Instruction {
protected:
    short duration;
    short timeLeft;

public:
    Instruction(short duration);
    virtual ~Instruction();

    void resetTimeLeft();
    short getTimeLeft();
    short getDuration();

    virtual bool execute(LedCube* ledCube) = 0;
};

#endif //INSTRUCTION_H