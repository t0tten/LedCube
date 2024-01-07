#include "LedCube.h"
#include "FileManagement.h"
#include "Instruction.h"
#include "Arduino.h"

FileManagement* fileManagement;
LedCube* ledCube;

void setup() {
    fileManagement = new FileManagement("input/sequence.cube");
    ledCube = new LedCube();

    /*if (fileManagement->isOpen())
    {
        Instruction* instruction = fileManagement->readNextLine();
        while (instruction != nullptr)
        {
            instruction->execute(ledCube);
            instruction = fileManagement->readNextLine();
        }
    }*/
}

void loop() {
    if (fileManagement->isOpen())
    {
        Instruction* instruction = fileManagement->readNextLine();
        while (instruction != nullptr)
        {
            instruction->execute(ledCube);
            instruction = fileManagement->readNextLine();
        }
    }
}
