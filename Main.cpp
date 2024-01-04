#include "LedCube.h"
#include "FileManagement.h"
#include "containers/Coordinate.h"
#include "instructions/Instruction.h"

/* SETTINGS */
#define X_SIZE 3
#define Y_SIZE 3
#define Z_SIZE 3
#define IS_MULTICOLOR false
#define USE_SIDE_SHIFT_OPTIMIZATION true
#define FILEPATH "input/file.cube"

using namespace std;

/**
 * TODO:
 *  - LedCube:
 *      - sendData() - Implement conversion to binary and send to shift register
 */

int main ()
{
    // Setup
    FileManagement* fileManagement = new FileManagement(FILEPATH);
    LedCube* ledCube = new LedCube(new Coordinate(X_SIZE, Y_SIZE, Z_SIZE), IS_MULTICOLOR, USE_SIDE_SHIFT_OPTIMIZATION);

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