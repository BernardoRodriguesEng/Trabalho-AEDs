#ifndef GAMEDAO_H
#define GAMEDAO_H

#include <string>
#include <fstream>
#include "Game.h"
#include "HashExtensivel.h"

class GameDAO {
private:
    std::string fileName;
    std::string indexFileName;
    int lastID;

    HashExtensivel hash;

    void loadLastID();
    void saveLastID();
    std::string toLowerCase(std::string str);

    void reconstruirHash();

public:
    GameDAO(const std::string& fileName);
    
    void create(Game& g);
    bool searchByName(const std::string& targetName, Game& found, long& pos);
    bool searchById(int appid, Game& found, long& pos);
    bool update(const std::string& targetName, const Game& updatedGame);
    bool remove(const std::string& targetName);
    void listActive(int limit);
    
    int getNextAppId();
};

#endif