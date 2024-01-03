#ifndef FILE_MANAGEMENT_H
#define FILE_MANAGEMENT_H

#include "instructions/Instruction.h"
#include "instructions/LedData.h"
#include "instructions/Repeat.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class FileManagement {
private:
    bool fileIsOpen;
    std::ifstream myfile;

    /* FUNCTIONS */
    std::vector<std::string> split(std::string s, std::string delimiter);
    bool isDigit(const std::string& s);

    LedData* processLedData(const std::string& s);
    Repeat* processRepeat(const std::string& s);

    Instruction* translate(const std::string& s);
    bool match(std::string str, std::string reg);

public:
    FileManagement(std::string filepath);
    ~FileManagement();

    bool isOpen();
    bool hasNextLine();
    Instruction* readNextLine();
};

#endif //FILE_MANAGEMENT_H