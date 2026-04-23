#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "GameDAO.h"
#include <string>

class GameController {
private:
    GameDAO dao;
    std::string binFilename;

public:
    GameController(const std::string& binFilename);
    void run();
};

#endif
