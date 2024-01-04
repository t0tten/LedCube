#include "LedCube.h"
#include "FileManagement.h"
#include "instructions/Instruction.h"

using namespace std;

int main ()
{
    // Setup
    FileManagement* fileManagement = new FileManagement("input/sequence.cube");
    LedCube* ledCube = new LedCube();

    // Loop
    if (fileManagement->isOpen())
    {
        Instruction* instruction = fileManagement->readNextLine();
        while (instruction != nullptr)
        {
            instruction->execute(ledCube);
            delete instruction;
            instruction = fileManagement->readNextLine();
        }
    }
    else
    {
        ledCube->showErrorSign();
    }

    delete ledCube;
    delete fileManagement;

    return 0;
}