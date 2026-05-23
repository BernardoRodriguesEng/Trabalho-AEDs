#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "GameDAO.h"
#include "UserDAO.h"
#include "LibraryDAO.h"
#include <string>

class GameController {
private:
    GameDAO dao;
    UserDAO uDao;
    LibraryDAO lDao;
    std::string binFilename;

public:
    GameController(const std::string& binFilename, const std::string& userBin, const std::string& libBin);
    void run();
};

#endif
