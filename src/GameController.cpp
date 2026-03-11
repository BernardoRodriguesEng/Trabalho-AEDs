#include "../include/GameController.h"
#include <iostream>
#include <limits>

using namespace std;

GameController::GameController(const string& binFilename) : binFilename(binFilename), dao(binFilename) {}

void GameController::run() {
    int op = 0;
    do {
        cout << "\n=== STEAM DATABASE ===" << endl;
        cout << "1. Search Game by Name" << endl;
        cout << "2. Search Game by ID" << endl;
        cout << "3. Add New Game" << endl;
        cout << "4. Update Game" << endl;
        cout << "5. Delete Game" << endl;
        cout << "6. List Games" << endl;
        cout << "7. Exit" << endl;
        cout << "Choice: ";
        
        if (!(cin >> op)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << ">> Opcao invalida." << endl;
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (op) {
            case 1: handleSearchByName(); break;
            case 2: handleSearchByID(); break;
            case 3: handleAddGame(); break;
            case 4: handleUpdateGame(); break;
            case 5: handleDeleteGame(); break;
            case 6: handleListGames(); break;
            case 7: cout << "Saindo..." << endl; break;
            default: cout << ">> Opcao invalida." << endl;
        }
    } while (op != 7);
}

void GameController::handleSearchByName() {
    string name;
    cout << "Enter Name: "; getline(cin, name);
    Game g;
    long pos;
    if (dao.searchByName(name, g, pos)) {
        cout << "\n>> JOGO ENCONTRADO! <<" << endl;
        g.print();
    } else {
        cout << ">> Jogo nao encontrado." << endl;
    }
}

void GameController::handleSearchByID() {
    int id = safeReadInt("Enter ID: ");
    Game g;
    long pos;
    if (dao.searchById(id, g, pos)) {
        cout << "\n>> JOGO ENCONTRADO! <<" << endl;
        g.print();
    } else {
        cout << ">> Jogo nao encontrado." << endl;
    }
}

void GameController::handleAddGame() {
    Game g;
    int appid = dao.getNextAppId();
    char name[150], dev[150];

    cout << ">> Automatic AppID assigned: " << appid << endl;
    cout << "Enter Name: "; cin.getline(name, 150);
    cout << "Enter Developer: "; cin.getline(dev, 150);
    float price = safeReadFloat("Enter Price: ");

    g.appid = appid;
    g.name = string(name);
    g.developer = string(dev);
    g.price = price;
    g.release_date = {0, 0, 0};
    g.english = true;

    dao.create(g);
    cout << ">> Jogo adicionado com sucesso (ID " << appid << ")." << endl;
}

void GameController::handleUpdateGame() {
    string name;
    cout << "Enter Name of the game to update: "; getline(cin, name);
    Game g;
    long pos;
    if (dao.searchByName(name, g, pos)) {
        cout << "Jogo encontrado! Preco Atual: $" << g.price << endl;
        g.price = safeReadFloat("Novo Preco: ");
        if (dao.update(name, g)) {
            cout << ">> Jogo atualizado com sucesso." << endl;
        }
    } else {
        cout << ">> Jogo nao encontrado." << endl;
    }
}

void GameController::handleDeleteGame() {
    string name;
    cout << "Enter Name of the game to delete: "; getline(cin, name);
    if (dao.remove(name)) {
        cout << ">> Jogo marcado como deletado (exclusao logica)." << endl;
    } else {
        cout << ">> Jogo nao encontrado." << endl;
    }
}

void GameController::handleListGames() {
    dao.listActive(20);
}

int GameController::safeReadInt(const string& prompt) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << ">> Entrada invalida. Digite um numero inteiro." << endl;
    }
}

float GameController::safeReadFloat(const string& prompt) {
    float val;
    while (true) {
        cout << prompt;
        if (cin >> val) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << ">> Entrada invalida. Digite um numero real (ex: 19.90)." << endl;
    }
}
