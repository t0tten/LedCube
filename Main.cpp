#include "LedCube.h"
#include "FileManagement.h"
#include "containers/Coordinate.h"
#include "instructions/Instruction.h"

using namespace std;

/**
 * TODO:
 *  - LedCube:
 *      - sendData() - Implement conversion to binary and send to shift register
 */

int main ()
{
    // Setup
    FileManagement* fileManagement = new FileManagement("input/file.cube");
    LedCube* ledCube = new LedCube(new Coordinate(4, 4, 4), true);

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