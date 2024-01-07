#include "FileManagement.h"

FileManagement::FileManagement(String filepath)
{
    this->fileIsOpen = true;
    this->fileSize = 24;
    this->currentIndex = 0;
    this->file = new Instruction*[this->fileSize];

    short delayTime = 1000;
    this->file[0] = new LedData(new Coordinate(0, 0, 0), new Color(1, 0, 0), delayTime);
    this->file[1] = new LedData(new Coordinate(1, 0, 0), new Color(1, 0, 0), delayTime);
    this->file[2] = new LedData(new Coordinate(0, 1, 0), new Color(1, 0, 0), delayTime);
    this->file[3] = new LedData(new Coordinate(1, 1, 0), new Color(1, 0, 0), delayTime);

    this->file[4] = new LedData(new Coordinate(0, 0, 1), new Color(1, 0, 0), delayTime);
    this->file[5] = new LedData(new Coordinate(1, 0, 1), new Color(1, 0, 0), delayTime);
    this->file[6] = new LedData(new Coordinate(0, 1, 1), new Color(1, 0, 0), delayTime);
    this->file[7] = new LedData(new Coordinate(1, 1, 1), new Color(1, 0, 0), delayTime);

    this->file[8] = new LedData(new Coordinate(0, 0, 0), new Color(0, 1, 0), delayTime);
    this->file[9] = new LedData(new Coordinate(1, 0, 0), new Color(0, 1, 0), delayTime);
    this->file[10] = new LedData(new Coordinate(0, 1, 0), new Color(0, 1, 0), delayTime);
    this->file[11] = new LedData(new Coordinate(1, 1, 0), new Color(0, 1, 0), delayTime);

    this->file[12] = new LedData(new Coordinate(0, 0, 1), new Color(0, 1, 0), delayTime);
    this->file[13] = new LedData(new Coordinate(1, 0, 1), new Color(0, 1, 0), delayTime);
    this->file[14] = new LedData(new Coordinate(0, 1, 1), new Color(0, 1, 0), delayTime);
    this->file[15] = new LedData(new Coordinate(1, 1, 1), new Color(0, 1, 0), delayTime);

    this->file[16] = new LedData(new Coordinate(0, 0, 0), new Color(0, 0, 1), delayTime);
    this->file[17] = new LedData(new Coordinate(1, 0, 0), new Color(0, 0, 1), delayTime);
    this->file[18] = new LedData(new Coordinate(0, 1, 0), new Color(0, 0, 1), delayTime);
    this->file[19] = new LedData(new Coordinate(1, 1, 0), new Color(0, 0, 1), delayTime);

    this->file[20] = new LedData(new Coordinate(0, 0, 1), new Color(0, 0, 1), delayTime);
    this->file[21] = new LedData(new Coordinate(1, 0, 1), new Color(0, 0, 1), delayTime);
    this->file[22] = new LedData(new Coordinate(0, 1, 1), new Color(0, 0, 1), delayTime);
    this->file[23] = new LedData(new Coordinate(1, 1, 1), new Color(0, 0, 1), delayTime);
}

FileManagement::~FileManagement()
{

}

bool FileManagement::isOpen()
{
    return this->fileIsOpen;
}

Instruction* FileManagement::readNextLine()
{
    Instruction* instruction = nullptr;
    if (this->fileIsOpen)
    {
        while (instruction == nullptr) {
            if (this->currentIndex >= this->fileSize)
            {
                this->currentIndex = 0;
                return nullptr;
            }
            instruction = this->file[this->currentIndex++];
        }
    }
    return instruction;
}
