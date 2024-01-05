#include "LedCube.h"
#include "Log.h"
#include <string>

/* SETTINGS */
#define X_SIZE                  3
#define Y_SIZE                  3
#define Z_SIZE                  3
#define IS_MULTICOLOR           true
#define IS_BIDIRECTIONAL        false               // If active - add +1 to PIN_LAYOUT INSTRUCTION (RIGHT)
/* SHIFT REGISTERS */
#define SHIFT_REGISTERS         1                   // Add the corresponding shift register pin layout below
#define INSTRUCTIONS            4
const short PIN_LAYOUT[SHIFT_REGISTERS][INSTRUCTIONS] = {
        {8, 7, 4, 2},          // RESET, ENABLE, STORE, SEND, [RIGHT]
        //{5, 6, 7, 8},
        //{10, 11, 12, 13}
};
#define DATA                    0
#define CLOCK                   1
#define LATCH                   2
#define RESET                   3
#define RIGHT                   4                   // Used when IS_BIDIRECTIONAL is active

LedCube::LedCube()
{
    this->log = Log::GetInstance();
    this->coordinate = new Coordinate(X_SIZE, Y_SIZE, Z_SIZE);
    this->log->info("Creating cube with size of: " + std::to_string(this->coordinate->get3DSize()));
    this->log->info(" (x: " + std::to_string(this->coordinate->getX()) + ", y: " + std::to_string(this->coordinate->getY()) + ", z: "
                  + std::to_string(this->coordinate->getZ()) + ")");
    this->splitOutput = (SHIFT_REGISTERS > 1) ? true : false;
    this->ledIsMulticolor = IS_MULTICOLOR;
    this->bidirectional = IS_BIDIRECTIONAL && (INSTRUCTIONS > 4);
    this->amount_colors = (this->ledIsMulticolor && !this->splitOutput) ? 3 : 1;

    this->ledArray = new bool[(this->coordinate->get2DSize() * this->amount_colors) + this->coordinate->getZ()];

    for (short z = 0; z < this->coordinate->getZ(); z++) {
        this->ledArray[z] = true;
    }

    short numOfOutputs = (this->coordinate->get2DSize() * this->amount_colors) + this->coordinate->getZ();
    this->log->info("Number of outputs: " + std::to_string(numOfOutputs));
    this->log->info("Cube is multicolor: " + std::to_string(this->ledIsMulticolor));
    this->log->info("Utilize side shift optimization: " + std::to_string(this->bidirectional));

    for (short shiftRegister = 0; shiftRegister < SHIFT_REGISTERS; shiftRegister++)
    {
        for (short pinIndex = 0; pinIndex < INSTRUCTIONS; pinIndex++)
        {
            //pinMode(PIN_LAYOUT[shiftRegister][pinIndex], OUTPUT);
        }
    }
}

LedCube::~LedCube()
{
    delete this->ledArray;
    delete this->coordinate;
}

void LedCube::updateLight(Coordinate* ledCoordinate, Color* color, short delayTime)
{
    short arr_pos = (ledCoordinate->getY() * this->coordinate->getY()) + ledCoordinate->getX();
    short* threshHoldPins = this->updateLEDArray(arr_pos, ledCoordinate->getZ(), color, true);
    this->sendData(color, threshHoldPins);
    //delay(delayTime);
    this->updateLEDArray(arr_pos, ledCoordinate->getZ(), color, false);
}

short* LedCube::updateLEDArray(short arr_pos, short level, Color* color, bool turnOn)
{
    short lowestPin = -1;
    short highestPin = (arr_pos * this->amount_colors) + this->coordinate->getZ();
    if (this->ledIsMulticolor && !this->splitOutput)
    {
        if (color->getR() && (arr_pos + this->coordinate->getZ()) < ((this->coordinate->get2DSize() * this->amount_colors) + this->coordinate->getZ()))
        {
            lowestPin = highestPin;
            this->ledArray[highestPin] = turnOn;
        }

        if (color->getG() && ((arr_pos + this->coordinate->getZ()) + 1) < ((this->coordinate->get2DSize() * this->amount_colors) + this->coordinate->getZ()))
        {
            highestPin = (arr_pos * this->amount_colors) + this->coordinate->getZ() + 1;
            if (lowestPin == -1)
            {
                lowestPin = highestPin;
            }
            this->ledArray[highestPin] = turnOn;
        }

        if (color->getB() && ((arr_pos + this->coordinate->getZ()) + 2) < ((this->coordinate->get2DSize() * this->amount_colors) + this->coordinate->getZ()))
        {
            highestPin = (arr_pos * this->amount_colors) + this->coordinate->getZ() + 2;
            if (lowestPin == -1)
            {
                lowestPin = highestPin;
            }
            this->ledArray[highestPin] = turnOn;
        }
    }
    else
    {
        this->ledArray[highestPin] = turnOn;
        lowestPin = highestPin;
    }
    this->ledArray[level] = !turnOn;
    short* thresholdPins = new short[2];
    thresholdPins[0] = lowestPin;
    thresholdPins[1] = highestPin;
    return thresholdPins;
}

short LedCube::getSize()
{
    return this->coordinate->get3DSize();
}

void LedCube::sendData(Color* color, short* threshHoldPins)
{
    std::string matrix = "";
    for (short color = 0; color < this->amount_colors; color++) {
        for (short y = 0; y < this->coordinate->getY(); y++) {
            for (short x = 0; x < this->coordinate->getX(); x++) {
                short arr_pos = ((y * this->coordinate->getY()) + (x)) * this->amount_colors + color;
                matrix += std::to_string(this->ledArray[arr_pos + this->coordinate->getZ()]) + " ";
            }
            matrix += "\n";
        }
        matrix += "\n";
    }

    for (short z = 0; z < this->coordinate->getZ(); z++) {
        matrix += std::to_string(this->ledArray[z]) + " ";
    }
    this->log->info(matrix);

    short allPins = (this->coordinate->get2DSize() * this->amount_colors) + this->coordinate->getZ();
    /*
     * Optimize bits to send from either left or right depending on
     * how far away we are from the middle of the array
     */
    if (this->bidirectional) {
        short mid = (allPins) / 2;
        short lowest = threshHoldPins[0] - this->coordinate->getZ();
        short highest = threshHoldPins[1] - this->coordinate->getZ();
        short lowDiff = lowest - mid;
        short highDiff = mid - highest;

        if (lowDiff >= 0) {
            this->fillFromRight(color, allPins, threshHoldPins[0]);
        } else if (highDiff >= 0) {
            this->fillFromLeft(color, allPins, threshHoldPins[1]);
        }
    }
    else {
        this->fillFromLeft(color, allPins, threshHoldPins[1]);
    }
    delete threshHoldPins;
}

void LedCube::fillFromRight(Color* color, short allPins, short lowest) {
    /* Reset all shift registers */
    for (short shiftRegister = 0; shiftRegister < SHIFT_REGISTERS; shiftRegister++) {
        this->log->debug("RESET on pin: " + std::to_string(PIN_LAYOUT[shiftRegister][RESET]));
        //digitalWrite(PIN_LAYOUT[shiftRegister][RESET], 1);
        //digitalWrite(PIN_LAYOUT[shiftRegister][RESET], 0);
    }

    std::string output = "";
    this->log->debug("");
    short stop = (this->splitOutput) ? lowest : (lowest - this->coordinate->getZ());
    for (short index = (allPins - 1); index >= stop; index--) {
        output += this->ledArray[index] ? "1" : "0";
    }
    short data = 0;
    if (this->splitOutput)
    {
        if (color->getR())
        {
            std::string outputPlus = "";
            for (short z = 0; z < this->coordinate->getZ(); z++)
            {
                outputPlus += "0";
            }
            data += (allPins - lowest) + this->coordinate->getZ();
            this->log->debug("");
            this->log->debug("DATA: " + std::to_string(data));
            this->log->debug("Sending to RED from RIGHT: "  + output + outputPlus);
            this->sendToShiftRegisterRight(0, this->ledArray, allPins, lowest - this->coordinate->getZ());
        }
        if (color->getG())
        {
            data += allPins - lowest;
            this->log->debug("");
            this->log->debug("Sending to GREEN from RIGHT: " + output);
            this->sendToShiftRegisterRight(1, this->ledArray, allPins, lowest);
        }
        if (color->getB())
        {
            data += allPins - lowest;
            this->log->debug("");
            this->log->debug("Sending to BLUE from RIGHT: " + output);
            this->sendToShiftRegisterRight(2, this->ledArray, allPins, lowest);
        }
    }
    else {
        data += (allPins - lowest) + this->coordinate->getZ();
        this->log->debug("");
        this->log->debug("Sending from RIGHT: " + output);
        this->sendToShiftRegisterRight(0, this->ledArray, allPins, lowest - this->coordinate->getZ());
    }

    this->log->debug("");
    std::string dataArray = "";
    for (short z = 0; z < this->coordinate->getZ(); z++) {
        dataArray += std::to_string(this->ledArray[z]);
    }
    this->log->debug("To send from left: " + dataArray);
    this->log->debug("");

    data += this->coordinate->getZ();
    this->sendToShiftRegister(0, this->ledArray, 0, this->coordinate->getZ());

    this->log->info("Data to send: " + std::to_string(data));
}
void LedCube::fillFromLeft(Color* color, short allPins, short highestPin)
{
    short stop = (highestPin < allPins) ? (highestPin + 1) : allPins;
    short start = (this->splitOutput) ? this->coordinate->getZ() : 0;
    std::string output = "";
    for (short index = start; index < stop; index++) {
        output += (this->ledArray[index]) ? "1" : "0";
    }

    /* Reset all shift registers */
    for (short shiftRegister = 0; shiftRegister < SHIFT_REGISTERS; shiftRegister++)
    {
        this->log->debug("RESET on pin: " + std::to_string(PIN_LAYOUT[shiftRegister][RESET]));
        //digitalWrite(PIN_LAYOUT[shiftRegister][RESET], 1);
        //digitalWrite(PIN_LAYOUT[shiftRegister][RESET], 0);
    }

    short data = 0;
    if (this->splitOutput)
    {

        std::string layers = "";
        for (short index = 0; index < this->coordinate->getZ(); index++) {
            layers += (this->ledArray[index]) ? "1" : "0";
        }
        this->log->debug("");
        this->log->debug("Sending to RED: " + layers);

        this->sendToShiftRegister(0, this->ledArray, 0, this->coordinate->getZ());
        data += this->coordinate->getZ();

        if (color->getR())
        {
            this->log->debug("");
            this->log->debug("Sending to RED: " + output);
            this->sendToShiftRegister(0, this->ledArray, start, stop);
            data += stop - start;
        }
        if (color->getG())
        {
            this->log->debug("");
            this->log->debug("Sending to GREEN: " + output);
            this->sendToShiftRegister(1, this->ledArray, start, stop);
            data += stop - start;
        }
        if (color->getB())
        {
            this->log->debug("");
            this->log->debug("Sending to BLUE: " + output);
            this->sendToShiftRegister(2, this->ledArray, start, stop);
            data += stop - start;
        }
    }
    else
    {
        this->log->debug("");
        this->log->debug("Sending: " + output);
        this->sendToShiftRegister(0, this->ledArray, start, stop);
        data += stop - start;
    }
    this->log->info("Data to send: " + std::to_string(data));
}

void LedCube::sendToShiftRegister(short shiftRegister, bool* array, short start, short stop)
{
    for (short index = start; index < stop; index++) {
        if (array[index]) {
            this->log->debug("DATA on pin: " + std::to_string(PIN_LAYOUT[shiftRegister][DATA]));
            //digitalWrite(PIN_LAYOUT[shiftRegister][DATA], 1);
        }
        this->log->debug("CLOCK on pin: " + std::to_string(PIN_LAYOUT[shiftRegister][CLOCK]));
        //digitalWrite(PIN_LAYOUT[shiftRegister][CLOCK], 1);
        //digitalWrite(PIN_LAYOUT[shiftRegister][CLOCK], 0);
        if (array[index]) {
            //digitalWrite(PIN_LAYOUT[shiftRegister][DATA], 0);
        }
    }
    this->log->debug("LATCH on pin: " + std::to_string(PIN_LAYOUT[shiftRegister][LATCH]));
    //digitalWrite(PIN_LAYOUT[shiftRegister][LATCH], 1);
    //digitalWrite(PIN_LAYOUT[shiftRegister][LATCH], 0);
}

void LedCube::sendToShiftRegisterRight(short shiftRegister, bool* array, short start, short stop)
{
    for (short index = start - 1; index >= stop; index--) {
        if (array[index]) {
            this->log->debug("DATA on pin: " + std::to_string(PIN_LAYOUT[shiftRegister][DATA]));
            //digitalWrite(PIN_LAYOUT[shiftRegister][DATA], 1);
        }
        this->log->debug("CLOCK on pin: " + std::to_string(PIN_LAYOUT[shiftRegister][CLOCK]));
        //digitalWrite(PIN_LAYOUT[shiftRegister][CLOCK], 1);
        //digitalWrite(PIN_LAYOUT[shiftRegister][CLOCK], 0);
        if (array[index]) {
            //digitalWrite(PIN_LAYOUT[shiftRegister][DATA], 0);
        }
    }
    this->log->debug("Enable RIGHT on pin: " + std::to_string(PIN_LAYOUT[shiftRegister][RIGHT]));
    //digitalWrite(PIN_LAYOUT[shiftRegister][RIGHT], 1);
    this->log->debug("LATCH on pin: " + std::to_string(PIN_LAYOUT[shiftRegister][LATCH]));
    //digitalWrite(PIN_LAYOUT[shiftRegister][LATCH], 1);
    //digitalWrite(PIN_LAYOUT[shiftRegister][LATCH], 0);
    //digitalWrite(PIN_LAYOUT[shiftRegister][RIGHT], 0);
}

bool LedCube::isMulticolor()
{
    return this->ledIsMulticolor;
}

void LedCube::showErrorSign()
{
    while (1) {
        // flash red
        this->updateLight(new Coordinate(3, 0, 3), new Color(true, false, false), 50);
        this->updateLight(new Coordinate(0, 3, 3), new Color(true, false, false), 50);
        this->updateLight(new Coordinate(3, 3, 3), new Color(true, false, false), 50);
        this->updateLight(new Coordinate(0, 0, 3), new Color(true, false, false), 50);

        this->updateLight(new Coordinate(3, 0, 0), new Color(true, false, false), 50);
        this->updateLight(new Coordinate(0, 3, 0), new Color(true, false, false), 50);
        this->updateLight(new Coordinate(3, 3, 0), new Color(true, false, false), 50);
        this->updateLight(new Coordinate(0, 0, 0), new Color(true, false, false), 50);
    }
}