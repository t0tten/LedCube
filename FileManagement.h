#ifndef FILE_MANAGEMENT_H
#define FILE_MANAGEMENT_H

#include "Instruction.h"
#include "LedData.h"
#include "Repeat.h"
#include "Coordinate.h"
#include "Arduino.h"
#include "Color.h"

class FileManagement {
private:
    bool fileIsOpen;
    Instruction** file;

    short fileSize, currentIndex;

public:
    FileManagement(String filepath);
    ~FileManagement();

    bool isOpen();
    bool hasNextLine();
    Instruction* readNextLine();
};

#endif //FILE_MANAGEMENT_H