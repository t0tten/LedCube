#include "Instruction.h"

Instruction::Instruction(short duration)
{
    this->duration = duration;
    this->timeLeft = duration;
    this->log = Log::GetInstance();
}

Instruction::~Instruction()
{

}

short Instruction::getDuration()
{
    return this->duration;
}

short Instruction::getTimeLeft()
{
    return this->timeLeft;
}

void Instruction::resetTimeLeft()
{
    this->timeLeft = this->duration;
}