#include "../include/GameDAO.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;

GameDAO::GameDAO(const string& fileName) : fileName(fileName) {
    indexFileName = fileName;
    size_t dotPos = indexFileName.find_last_of(".");
    if (dotPos != string::npos) indexFileName = indexFileName.substr(0, dotPos);
    indexFileName += "_index.bin";
    
    loadLastID();
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

void GameDAO::create(Game& g) {
    ofstream outFile(fileName, ios::binary | ios::app);
    if (outFile) {
        if (outFile.tellp() == 0) {
            int zero = 0;
            outFile.write(reinterpret_cast<char*>(&zero), sizeof(int));
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

        ofstream idxFile(indexFileName, ios::binary | ios::app);
        if (idxFile) {
            idxFile.write(reinterpret_cast<const char*>(&g.appid), sizeof(int));
            idxFile.write(reinterpret_cast<const char*>(&offset), sizeof(long));
            idxFile.close();
        }
    }
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
    for (int i = 0; i < count; ++i) {
        skipString(is);
    }
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

long GameDAO::getOffsetFromIndex(int appid) {
    ifstream idxFile(indexFileName, ios::binary);
    if (!idxFile) return -1;
    
    int id;
    long offset;
    while (idxFile.read(reinterpret_cast<char*>(&id), sizeof(int))) {
        idxFile.read(reinterpret_cast<char*>(&offset), sizeof(long));
        if (id == appid) {
            idxFile.close();
            return offset;
        }
    }
    idxFile.close();
    return -1;
}

bool GameDAO::searchById(int appid, Game& found, long& pos) {
    long offset = getOffsetFromIndex(appid);
    if (offset == -1) return false;
    
    ifstream inFile(fileName, ios::binary);
    if (!inFile) return false;
    
    inFile.seekg(offset);
    found.readFromStream(inFile);
    pos = offset;

    inFile.close();
    return found.isActive();
}

bool GameDAO::update(const string& targetName, const Game& updatedGame) {
    Game g;
    long pos;
    if (searchByName(targetName, g, pos)) {
        remove(targetName);
        Game temp = updatedGame;
        create(temp);
        return true;
    }
    return false;
}

int GameDAO::getNextAppId() {
    return lastID + 1;
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
    // Abre o índice no final para pegar o tamanho
    ifstream idxFile(indexFileName, ios::binary | ios::ate);
    if (!idxFile) {
        cout << ">> Error while opening the index file." << endl;
        return;
    }

    long fileSize = idxFile.tellg();
    int entrySize = sizeof(int) + sizeof(long);
    if (fileSize < entrySize) {
        cout << ">> The database is empty." << endl;
        idxFile.close();
        return;
    }

    int totalEntries = fileSize / entrySize;
    int toRead = (totalEntries < limit) ? totalEntries : limit;

    idxFile.seekg(fileSize - (toRead * entrySize), ios::beg);

    vector<long> offsets;
    int id;
    long offset;
    // Lê exatamente a quantidade solicitada
    for (int i = 0; i < toRead; ++i) {
        if (idxFile.read(reinterpret_cast<char*>(&id), sizeof(int)) &&
            idxFile.read(reinterpret_cast<char*>(&offset), sizeof(long))) {
            offsets.push_back(offset);
        }
    }
    idxFile.close();

    ifstream inFile(fileName, ios::binary);
    if (!inFile) return;

    cout << "\n--- Showing the last few games in the database " << offsets.size() << " active entries ---" << endl;
    for (long off : offsets) {
        inFile.seekg(off, ios::beg);
        Game temp;
        temp.readFromStream(inFile);
        if (temp.isActive()) {
            temp.print();
        }
    }
    inFile.close();
}
