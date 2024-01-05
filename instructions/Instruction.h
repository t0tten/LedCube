#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "../LedCube.h"
#include "../Log.h"

class Instruction {
protected:
    short duration;
    short timeLeft;
    Log* log;

public:
    Instruction(short duration);
    virtual ~Instruction();

    void resetTimeLeft();
    short getTimeLeft();
    short getDuration();

    virtual void print() = 0;
    virtual bool execute(LedCube* ledCube) = 0;
};

#endif //INSTRUCTION_H