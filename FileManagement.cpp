#include "FileManagement.h"
#include "instructions/LedData.h"
#include "containers/Color.h"
#include "containers/Coordinate.h"
#include "instructions/Instruction.h"
#include <vector>
#include <string>
#include <regex>

FileManagement::FileManagement(std::string filepath)
{
    this->log = Log::GetInstance();
    this->log->info("Opening file: " + filepath);
    this->myfile.open(filepath);
    this->fileIsOpen = this->myfile.is_open();
}

FileManagement::~FileManagement()
{

}

bool FileManagement::isOpen()
{
    return this->fileIsOpen;
}

std::vector<std::string> FileManagement::split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

bool FileManagement::isDigit(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

Instruction* FileManagement::readNextLine()
{
    Instruction* instruction = nullptr;
    if (myfile)
    {
        while (instruction == nullptr) {
            std::string line = "";
            if (!std::getline(this->myfile, line)) {
                return nullptr;
            }
            instruction = this->translate(line);
        }
    }
    return instruction;
}

bool FileManagement::match(std::string str, std::string reg) {
    return std::regex_match (str, std::regex(reg));
}

Instruction* FileManagement::translate(const std::string& s)
{
    if (this->match(s, "^repeat\\([0-9]*,[0-9]*\\):$"))
    {
        return this->processRepeat(s);
    }
    else if (this->match(s, "^[0-9]*,[0-9]*,[0-9]*,[0-9]*,[0-9]*,[0-9]*,[0-9]*$"))
    {
        return processLedData(s);
    }
    else if (this->match(s, "^[0-9]*,[0-9]*,[0-9]*,[0-9]*$"))
    {
        return processLedData(s);
    }
    return nullptr;
}

LedData* FileManagement::processLedData(const std::string& s)
{
    std::vector<std::string> items = this->split(s, ",");
    if (items.size() == 4 || items.size() == 7)
    {
        bool isDigits = true;
        for (short index = 0; index < items.size() && isDigits; index++)
        {
            if (!this->isDigit(items.at(index))) {
                isDigits = false;
            }
        }

        if (isDigits)
        {
            short delayPos = 3;
            short x = std::stoi(items.at(0));
            short y = std::stoi(items.at(1));
            short z = std::stoi(items.at(2));

            if (x > 0 && y > 0 && z > 0)
            {
                Coordinate *coordinate = new Coordinate(x - 1, y - 1, z - 1);
                Color *color = nullptr;
                if (items.size() == 7)
                {
                    delayPos = 6;
                    color = new Color(std::stoi(items.at(3)), std::stoi(items.at(4)), std::stoi(items.at(5)));
                }


                return new LedData(coordinate, color, std::stoi(items.at(delayPos)));
            }
        }
    }
    return nullptr;
}

Repeat* FileManagement::processRepeat(const std::string& s)
{
    std::vector<std::string> items = this->split(s.substr(0, s.size() - 2), "(");
    items = this->split(items.at(1), ",");
    Repeat* repeat = new Repeat(std::stoi(items.at(0)), std::stoi(items.at(1)));

    for (short repeats = 0; repeats < repeat->getAmount(); repeats++)
    {
        Instruction* instruction = this->readNextLine();
        if (instruction != nullptr)
        {
            repeat->addInstruction(instruction);
        }
    }
    return repeat;
}