#ifndef REPEAT_H
#define REPEAT_H

#include "Instruction.h"
#include "Arduino.h"

class Repeat: public Instruction {
private:
    short amount_instructions, amount_current, index;
    Instruction** instructionArray;

public:
    Repeat(short amount, short duration);
    virtual ~Repeat();

    void addInstruction(Instruction* instruction);
    Instruction* getNextInstruction();

    bool shouldRepeat();
    short getAmount();
    short getCurrentAmount();

    virtual bool execute(LedCube* ledCube);
};

#endif //REPEAT_H