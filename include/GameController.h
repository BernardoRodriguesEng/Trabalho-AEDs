#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "GameDAO.h"
#include <string>

class GameController {
private:
    GameDAO dao;
    std::string binFilename;

    void handleSearchByName();
    void handleSearchByID();
    void handleAddGame();
    void handleUpdateGame();
    void handleDeleteGame();
    void handleListGames();

    int safeReadInt(const std::string& prompt);
    float safeReadFloat(const std::string& prompt);
    std::string safeReadString(const std::string& prompt);

public:
    GameController(const std::string& binFilename);
    void run();
};

#endif
