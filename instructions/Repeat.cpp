#include "Repeat.h"

Repeat::Repeat(short amount, short duration): Instruction(duration)
{
    this->amount_instructions = amount;
    this->amount_current = 0;
    this->index = 0;
    this->instructionArray = new Instruction*[this->amount_instructions];
}

Repeat::~Repeat()
{
    for (short index = 0; index < this->amount_current; index++)
    {
        delete this->instructionArray[index];
    }
    delete this->instructionArray;
}

void Repeat::addInstruction(Instruction* instruction)
{
    if (this->amount_current < this->amount_instructions)
    {
        this->instructionArray[this->amount_current++] = instruction;
    }
}

Instruction* Repeat::getNextInstruction()
{
    if (this->amount_current != 0)
    {
        Instruction* instruction = this->instructionArray[this->index++];
        this->timeLeft -= instruction->getDuration();
        if (this->index >= this->amount_current)
        {
            this->index = 0;
        }
        return instruction;
    }
    return nullptr;
}

bool Repeat::shouldRepeat()
{
    if (this->amount_current > 0) {
        int timeleft_tmp = this->timeLeft - this->instructionArray[0]->getDuration();;
        return (timeleft_tmp) >= 0;
    }
    return false;
}

short Repeat::getAmount()
{
    return this->amount_instructions;
}

short Repeat::getCurrentAmount()
{
    return this->amount_current;
}

void Repeat::print()
{
    this->log->info("Repeat(" + std::to_string(this->amount_instructions) + ", " + std::to_string(this->duration) + "): ");
    for (short index = 0; index < this->amount_current; index++)
    {
        this->instructionArray[index]->print();
    }
}

bool Repeat::execute(LedCube* ledCube)
{
    while (this->shouldRepeat()) {
        for (short index = 0; index < this->getCurrentAmount(); index++) {
            Instruction* instruction = this->getNextInstruction();
            if (instruction != nullptr)
            {
                instruction->resetTimeLeft();
                if (!instruction->execute(ledCube))
                {
                    return false;
                }
            }
        }
    }
    return true;
}