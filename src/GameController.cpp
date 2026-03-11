#include "../include/GameController.h"
#include <iostream>
#include <limits>
#include <sstream>
#include <vector>

using namespace std;

// Constructor: Initializes the controller with the database filename and its DAO
GameController::GameController(const string& binFilename) : binFilename(binFilename), dao(binFilename) {}

// Main loop of the controller that displays the menu and handles user
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
            cout << ">> Invalid option." << endl;
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
            case 7: cout << "Exiting..." << endl; break;
            default: cout << ">> Invalid option." << endl;
        }
    } while (op != 7);
}

// Prompts the user for a game name and searches for it in the database
void GameController::handleSearchByName() {
    string name;
    cout << "Enter Name: "; getline(cin, name);
    Game g;
    long pos;
    if (dao.searchByName(name, g, pos)) {
        cout << "\n>> GAME FOUND! <<" << endl;
        g.print();
    } else {
        cout << ">> Game not found." << endl;
    }
}

// Prompts the user for a game ID and searches for it in the database
void GameController::handleSearchByID() {
    int id = safeReadInt("Enter ID: ");
    Game g;
    long pos;
    if (dao.searchById(id, g, pos)) {
        cout << "\n>> GAME FOUND! <<" << endl;
        g.print();
    } else {
        cout << ">> Game not found." << endl;
    }
}

// Collects all required fields from the user and adds a new game to the database
void GameController::handleAddGame() {
    Game g;

    int appidChoice = safeReadInt("Enter AppID (default is 0): ");
    g.appid = (appidChoice == 0) ? dao.getNextAppId() : appidChoice;

    cout << ">> AppID assigned: " << g.appid << endl;

    g.name = safeReadString("Enter Name: ");
    
    g.release_date.year = safeReadInt("Enter Release Year (YYYY): ");
    g.release_date.month = safeReadInt("Enter Release Month (MM): ");
    g.release_date.day = safeReadInt("Enter Release Day (DD): ");
    
    int eng = safeReadInt("Is English supported? (1 for yes, 0 for no): ");
    g.english = (eng == 1);
    
    g.developer = safeReadString("Enter Developer: ");
    g.publisher = safeReadString("Enter Publisher: ");
    g.platforms = safeReadString("Enter Platforms (ex: windows;mac;linux): ");
    g.required_age = safeReadInt("Enter Required Age: ");

    auto splitString = [](const string& s, char delimiter) {
        vector<string> tokens;
        string token;
        istringstream tokenStream(s);
        while (getline(tokenStream, token, delimiter)) {
            if (!token.empty()) tokens.push_back(token);
        }
        return tokens;
    };

    string cats = safeReadString("Enter Categories (separated by ;): ");
    g.categories = splitString(cats, ';');
    
    string gens = safeReadString("Enter Genres (separated by ;): ");
    g.genres = splitString(gens, ';');
    
    string tags = safeReadString("Enter Tags (separated by ;): ");
    g.steamspy_tags = splitString(tags, ';');
    
    g.achievements = safeReadInt("Enter Achievements count: ");
    g.positive_ratings = safeReadInt("Enter Positive Ratings: ");
    g.negative_ratings = safeReadInt("Enter Negative Ratings: ");
    g.average_playtime = safeReadInt("Enter Average Playtime (minutes): ");
    g.median_playtime = safeReadInt("Enter Median Playtime (minutes): ");
    g.owners = safeReadString("Enter Owners range (ex: 10000-20000): ");
    
    g.price = safeReadFloat("Enter Price: ");
    g.setActive(true);

    dao.create(g);
    cout << ">> Game successfully added (ID " << g.appid << ")." << endl;
}

// Finds a game by name and prompts the user to update its price
void GameController::handleUpdateGame(){
    string name;
    cout << "Enter Name of the game to update: ";
    getline(cin, name);
    
    Game g;
    long pos;

    if(!dao.searchByName(name, g, pos)){
        cout << ">> Game not found." << endl;
        return;
    }

    cout << "\n>> Game found! <<" << endl;
    g.print();

    bool editing = true;

    while (editing) {

        cout << "\n>> What do you want to update? <<" << endl;
        cout << "1 - Name" << endl;
        cout << "2 - Release Date\n";
        cout << "3 - Developer\n";
        cout << "4 - Publisher\n";
        cout << "5 - Platforms\n";
        cout << "6 - Price\n";
        cout << "7 - Required Age\n";
        cout << "8 - Achievements\n";
        cout << "9 - Positive Ratings\n";
        cout << "10 - Negative Ratings\n";
        cout << "0 - Finish editing\n";

        int op = safeReadInt("Choice: ");

        switch(op){
            case 1:
                g.name = safeReadString("New name: ");
                break;

            case 2:
                g.release_date.year = safeReadInt("New year: ");
                g.release_date.month = safeReadInt("New month: ");
                g.release_date.day = safeReadInt("New day: ");
                break;

            case 3:
                g.developer = safeReadString("New developer: ");
                break;

            case 4:
                g.publisher = safeReadString("New publisher: ");
                break;

            case 5:
                g.platforms = safeReadString("New platforms: ");
                break;

            case 6:
                g.price = safeReadFloat("New price: ");
                break;

            case 7:
                g.required_age = safeReadInt("New required age: ");
                break;

            case 8:
                g.achievements = safeReadInt("New achievements: ");
                break;

            case 9:
                g.positive_ratings = safeReadInt("New positive ratings: ");
                break;

            case 10:
                g.negative_ratings = safeReadInt("New negative ratings: ");
                break;

            case 0:
                editing = false;
                break;

            default:
                cout << ">> Invalid option." << endl;
        }
    }
    if(dao.update(name, g)){
        cout << ">> Game information updated." << endl;
    } else {
        cout << ">> Error updating the game." << endl;
    }
}

// Locates a game by name and softly deletes it
void GameController::handleDeleteGame() {
    string name;
    cout << "Enter Name of the game to delete: "; getline(cin, name);
    if (dao.remove(name)) {
        cout << ">> Game marked for deletion (logical exclusion)." << endl;
    } else {
        cout << ">> Game not found." << endl;
    }
}

// Lists the 20 most recently added active games in the database
void GameController::handleListGames() {
    dao.listActive(20);
}

// Utility to safely read an integer from standard input, handling invalid formats
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
        cout << ">> Invalid entry. Please type a whole number." << endl;
    }
}

// Utility to safely read a floating point number from standard input
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
        cout << ">> Invalid entry. Please type a real number (ex: 19.90)." << endl;
    }
}

// Utility to safely read a string from standard input
string GameController::safeReadString(const string& prompt) {
    string val;
    cout << prompt;
    getline(cin, val);
    return val;
}