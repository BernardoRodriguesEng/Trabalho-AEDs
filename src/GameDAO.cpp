#include "../include/GameDAO.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

#include "../include/BPlusTree.h"

using namespace std;

GameDAO::GameDAO(const string& fileName) 
    : fileName(fileName), hash(4) {

    priceIndex = new BPlusTree("price_index.bin");

    indexFileName = fileName;
    size_t dotPos = indexFileName.find_last_of(".");
    if (dotPos != string::npos) indexFileName = indexFileName.substr(0, dotPos);
    indexFileName += "_index.bin";
    
    loadLastID();
    reconstruirHash();
}

GameDAO::~GameDAO() {
    delete priceIndex;
}

void GameDAO::loadLastID() {
    ifstream inFile(fileName, ios::binary);
    if (inFile) {
        inFile.read(reinterpret_cast<char*>(&lastID), sizeof(int));
        inFile.close();
    } else {
        lastID = 0;
    }
}

void GameDAO::saveLastID() {
    fstream outFile(fileName, ios::binary | ios::in | ios::out);
    if (outFile) {
        outFile.seekp(0, ios::beg);
        outFile.write(reinterpret_cast<const char*>(&lastID), sizeof(int));
        outFile.close();
    }
}

void GameDAO::reconstruirHash() {
    ifstream file(fileName, ios::binary);

    if (!file) return;

    file.seekg(sizeof(int), ios::beg);

    while (file.peek() != EOF) {
        long pos = file.tellg();

        Game g;
        g.readFromStream(file);

        if (g.isActive()) {
            hash.inserir(g.appid, pos);
            priceIndex->inserir(g.price, pos);
        }
    }

    file.close();
}

void GameDAO::create(Game& g) {
    fstream outFile(fileName, ios::binary | ios::in | ios::out);

    if (!outFile) {
        outFile.open(fileName, ios::binary | ios::out);
        int zero = 0;
        outFile.write(reinterpret_cast<char*>(&zero), sizeof(int));
        outFile.close();
        outFile.open(fileName, ios::binary | ios::in | ios::out);
    }

    outFile.seekp(0, ios::end);
    
    // Verifica se a chave primária é duplicada
    if (g.appid != 0) {
        long existingOffset;
        if (hash.buscar(g.appid, existingOffset)) {
            outFile.close();
            throw runtime_error("AppID duplicado: " + to_string(g.appid));
        }
    }

    long offset = outFile.tellp();

    if (g.appid == 0) {
        lastID++;
        g.appid = lastID;
    } else if (g.appid > lastID) {
        lastID = g.appid;
    }

    int size = g.getSerializationSize();
    char* buffer = new char[size];

    g.serialize(buffer);
    outFile.write(buffer, size);

    delete[] buffer;
    outFile.close();

    saveLastID();

    hash.inserir(g.appid, offset);
    priceIndex->inserir(g.price, offset);
}

bool GameDAO::searchById(int appid, Game& found, long& pos) {
    long offset;
    bool achou = hash.buscar(appid, offset);

    if (!achou) return false;

    ifstream inFile(fileName, ios::binary);
    if (!inFile) return false;

    inFile.seekg(offset);
    found.readFromStream(inFile);
    pos = offset;

    inFile.close();
    return found.isActive();
}

string GameDAO::toLowerCase(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

void skipString(istream& is) {
    unsigned short len;
    is.read(reinterpret_cast<char*>(&len), sizeof(unsigned short));
    is.seekg(len, ios::cur);
}

void skipVector(istream& is) {
    unsigned short count;
    is.read(reinterpret_cast<char*>(&count), sizeof(unsigned short));
    for (int i = 0; i < count; ++i) skipString(is);
}

bool GameDAO::searchByName(const string& targetName, Game& found, long& pos) {
    ifstream inFile(fileName, ios::binary);
    if (!inFile) return false;

    inFile.seekg(sizeof(int), ios::beg);

    string lowerTargetName = toLowerCase(targetName);

    while (inFile.peek() != EOF) {
        long currentPos = inFile.tellg();

        char lapide;
        if (!inFile.read(&lapide, 1)) break;

        int appid;
        inFile.read(reinterpret_cast<char*>(&appid), sizeof(int));

        unsigned short nameLen;
        inFile.read(reinterpret_cast<char*>(&nameLen), sizeof(unsigned short));

        char* nameBuf = new char[nameLen + 1];
        inFile.read(nameBuf, nameLen);
        nameBuf[nameLen] = '\0';

        string currentName(nameBuf);
        delete[] nameBuf;

        if (lapide == ' ' && toLowerCase(currentName).find(lowerTargetName) == 0) {
            inFile.seekg(currentPos);
            found.readFromStream(inFile);
            pos = currentPos;
            inFile.close();
            return true;
        } else {
            inFile.seekg(sizeof(Date), ios::cur);
            inFile.seekg(sizeof(bool), ios::cur);
            skipString(inFile);
            skipString(inFile);
            skipString(inFile);
            inFile.seekg(sizeof(int), ios::cur);
            skipVector(inFile);
            skipVector(inFile);
            skipVector(inFile);
            inFile.seekg(sizeof(int) * 5, ios::cur);
            skipString(inFile);
            inFile.seekg(sizeof(float), ios::cur);
        }
    }

    inFile.close();
    return false;
}

vector<Game> GameDAO::searchAllByName(const string& targetName) {
    vector<Game> results;
    ifstream inFile(fileName, ios::binary);
    if (!inFile) return results;

    inFile.seekg(sizeof(int), ios::beg);
    string lowerTargetName = toLowerCase(targetName);

    while (inFile.peek() != EOF) {
        long currentPos = inFile.tellg();

        char lapide;
        if (!inFile.read(&lapide, 1)) break;

        int appid;
        inFile.read(reinterpret_cast<char*>(&appid), sizeof(int));

        unsigned short nameLen;
        inFile.read(reinterpret_cast<char*>(&nameLen), sizeof(unsigned short));

        char* nameBuf = new char[nameLen + 1];
        inFile.read(nameBuf, nameLen);
        nameBuf[nameLen] = '\0';

        string currentName(nameBuf);
        delete[] nameBuf;

        if (lapide == ' ' && toLowerCase(currentName).find(lowerTargetName) != string::npos) {
            inFile.seekg(currentPos);
            Game g;
            g.readFromStream(inFile);
            results.push_back(g);
        } else {
            inFile.seekg(sizeof(Date), ios::cur);
            inFile.seekg(sizeof(bool), ios::cur);
            skipString(inFile);
            skipString(inFile);
            skipString(inFile);
            inFile.seekg(sizeof(int), ios::cur);
            skipVector(inFile);
            skipVector(inFile);
            skipVector(inFile);
            inFile.seekg(sizeof(int) * 5, ios::cur);
            skipString(inFile);
            inFile.seekg(sizeof(float), ios::cur);
        }
    }

    inFile.close();
    return results;
}

bool GameDAO::update(const string& targetName, const Game& updatedGame){
    Game oldGame;
    long pos;

    if(!searchByName(targetName, oldGame, pos)) return false;

    fstream file(fileName, ios::binary | ios::in | ios::out);
    file.seekp(pos);
    char lapide = '*';
    file.write(&lapide, 1);
    file.close();

    // Remove do hash para que create() não dispare o check de duplicata
    hash.remover(oldGame.appid);

    Game newGame = updatedGame;
    newGame.appid = oldGame.appid;
    newGame.setActive(true);

    create(newGame);

    return true;
}

bool GameDAO::remove(const string& targetName) {
    Game g;
    long pos; 

    if (searchByName(targetName, g, pos)) {
        fstream file(fileName, ios::binary | ios::in | ios::out);
        file.seekp(pos);
        char lapide = '*';
        file.write(&lapide, 1);
        file.close();
        return true;
    }
    return false;
}

void GameDAO::listActive(int limit) {
    ifstream inFile(fileName, ios::binary);
    if (!inFile) return;

    inFile.seekg(sizeof(int), ios::beg);

    cout << "\n--- Listando Jogos ---\n";

    int count = 0;

    while (inFile.peek() != EOF && count < limit) {
        Game g;
        g.readFromStream(inFile);

        if (g.isActive()) {
            g.print();
            count++;
        }
    }

    inFile.close();
}

int GameDAO::getNextAppId() {
    return lastID + 1;
}

vector<Game> GameDAO::searchByPriceRange(float min, float max) {
    auto offsets = priceIndex->buscarIntervalo(min, max);
    vector<Game> results;
    
    ifstream inFile(fileName, ios::binary);
    if (!inFile) return results;

    for (long offset : offsets) {
        inFile.seekg(offset);
        Game g;
        g.readFromStream(inFile);
        if (g.isActive()) {
            results.push_back(g);
        }
    }
    
    inFile.close();
    return results;
}