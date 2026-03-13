#ifndef CSVCONVERTER_H
#define CSVCONVERTER_H

#include <string>
#include <fstream>
#include "Game.h"

class CSVConverter {
public:
    CSVConverter(const std::string& csvFilename);
    ~CSVConverter();

    bool convertToBinary(const std::string& binFilename);

private:
    std::ifstream csvFile;
    Game parseLine();
};

#endif
