#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>

using namespace std;

// --- DATA STRUCTURE ---
struct Game {
    int appid;
    char name[150];
    char release_date[15];
    bool english;
    char developer[150];
    char publisher[150];
    char platforms[100];
    int required_age;
    char categories[300];
    char genres[150];
    char steamspy_tags[300];
    int achievements;
    int positive_ratings;
    int negative_ratings;
    int average_playtime;
    int median_playtime;
    char owners[50];
    float price;
    bool active; // Delete marker
};

void safelySetString(char* dest, const string& src, int size) {
    memset(dest, 0, size);
    strncpy(dest, src.c_str(), size - 1);
}

// --- CSV PARSING ---
class CSVParser {
private:
    ifstream csvFile;
public:
    CSVParser(const string& filename) {
        csvFile.open(filename);
    }

    ~CSVParser() {
        if (csvFile.is_open()) csvFile.close();
    }

    bool isOpen() const {
        return csvFile.is_open();
    }

    bool isEOF() {
        return csvFile.peek() == EOF;
    }

    Game parseLine() {
        Game g = {}; // Initialize with zeros
        string line;
        if (!getline(csvFile, line) || line.empty()) return g;

        stringstream ss(line);
        string field;

        getline(ss, field, ','); if(!field.empty()) g.appid = stoi(field);
        getline(ss, field, ','); safelySetString(g.name, field, sizeof(g.name));
        getline(ss, field, ','); safelySetString(g.release_date, field, sizeof(g.release_date));
        getline(ss, field, ','); g.english = (field == "1");
        getline(ss, field, ','); safelySetString(g.developer, field, sizeof(g.developer));
        getline(ss, field, ','); safelySetString(g.publisher, field, sizeof(g.publisher));
        getline(ss, field, ','); safelySetString(g.platforms, field, sizeof(g.platforms));
        getline(ss, field, ','); if(!field.empty()) g.required_age = stoi(field);
        getline(ss, field, ','); safelySetString(g.categories, field, sizeof(g.categories));
        getline(ss, field, ','); safelySetString(g.genres, field, sizeof(g.genres));
        getline(ss, field, ','); safelySetString(g.steamspy_tags, field, sizeof(g.steamspy_tags));
        getline(ss, field, ','); if(!field.empty()) g.achievements = stoi(field);
        getline(ss, field, ','); if(!field.empty()) g.positive_ratings = stoi(field);
        getline(ss, field, ','); if(!field.empty()) g.negative_ratings = stoi(field);
        getline(ss, field, ','); if(!field.empty()) g.average_playtime = stoi(field);
        getline(ss, field, ','); if(!field.empty()) g.median_playtime = stoi(field);
        getline(ss, field, ','); safelySetString(g.owners, field, sizeof(g.owners));
        getline(ss, field);      if(!field.empty()) g.price = stof(field); 

        g.active = true; // New object is not to be deleted
        return g;
    }
};

// --- CRUD ENGINE ---
class SteamCRUD {
private:
    string fileName = "steam.bin";

public:
    // Create
    void create(const Game& g) {
        ofstream outFile(fileName, ios::binary | ios::app);
        if (outFile) {
            Game newGame = g;
            outFile.write(reinterpret_cast<const char*>(&newGame), sizeof(Game));
            outFile.close();
        }
    }

    string toLowerCase(string str) {
        for (char &c : str) {
            if (c >= 'A' && c <= 'Z') c += ('a' - 'A');
        }
        return str;
    }
    
    // Read
    bool searchByName(const string& targetName, Game& found, long& pos) {
        ifstream inFile(fileName, ios::binary);
        if (!inFile) return false;
    
        Game temp;
        string lowerTargetName = toLowerCase(targetName);
    
        while (inFile.read(reinterpret_cast<char*>(&temp), sizeof(Game))) {
            string lowerTempName = toLowerCase(temp.name);
    
            if (temp.active && lowerTempName == lowerTargetName) { 
                found = temp;
                pos = static_cast<long>(inFile.tellg()) - sizeof(Game);
                inFile.close();
                return true;
            }
        }
        inFile.close();
        return false;
    }

    // Update
    bool update(const string& targetName, const Game& updatedGame) {
        Game g;
        long pos;
        if (searchByName(targetName, g, pos)) {
            fstream file(fileName, ios::binary | ios::in | ios::out);
            file.seekp(pos); 
            file.write(reinterpret_cast<const char*>(&updatedGame), sizeof(Game));
            file.close();
            return true;
        }
        return false;
    }

    // Delete
    bool remove(const string& targetName) {
        Game g;
        long pos; 
        if (searchByName(targetName, g, pos)) { 
            fstream file(fileName, ios::binary | ios::in | ios::out);
            g.active = false;
            file.seekp(pos); 
            file.write(reinterpret_cast<const char*>(&g), sizeof(Game));
            file.close();
            return true;
        }
        return false;
    }
};

int main() {
    cout << "Initializing Database..." << endl;
    
    // Check if binary file exists; if not, convert CSV
    ifstream check("steam.bin");
    if (!check) {
        CSVParser parser("steam.csv");
        if (parser.isOpen()) {
            cout << "steam.bin not found. Converting from steam.csv. Please wait..." << endl;
            ofstream arquivoBin("steam.bin", ios::binary);
            
            while (!parser.isEOF()) {
                Game g = parser.parseLine();
                if (g.appid != 0) { 
                    arquivoBin.write(reinterpret_cast<char*>(&g), sizeof(Game));
                }
            }
            arquivoBin.close();
            cout << "Conversion complete!" << endl;
        } else {
            cerr << "Error: Could not open steam.csv." << endl;
            return 1; 
        }
    }
    check.close();

    SteamCRUD db;
    int op = 0;

    while (op != 5) {
        cout << "\n=== STEAM DATABASE (BINARY CRUD) ===" << endl;
        cout << "1. Search Game" << endl;
        cout << "2. Add New Game" << endl;
        cout << "3. Update Game" << endl;
        cout << "4. Delete Game" << endl;
        cout << "5. Exit" << endl;
        cout << "Choice: ";
        cin >> op;
        cin.ignore();

        if (op == 1) {
            string name;
            cout << "Enter Name: "; getline(cin, name);
            Game g;
            long pos;
            
            if (db.searchByName(name, g, pos)) { 
                cout << "\n>> FOUND! <<" << endl;
                cout << "ID: " << g.appid << " | Name: " << g.name << endl;
                cout << "Dev: " << g.developer << " | Price: $" << g.price << endl;
            } else {
                cout << ">> Game not found." << endl;
            }
        } 
        else if (op == 2) {
            Game g = {}; // Clear memory
            cout << "Enter AppID: "; cin >> g.appid; cin.ignore();
            cout << "Enter Name: "; cin.getline(g.name, 150);
            cout << "Enter Developer: "; cin.getline(g.developer, 150);
            cout << "Enter Price: "; cin >> g.price;
            g.active = true;
            
            db.create(g);
            cout << ">> Game added successfully." << endl;
        }
        else if (op == 3) {
            string name;
            cout << "Enter Name of the game to update: "; getline(cin, name);
            
            Game g;
            long pos;
            if (db.searchByName(name, g, pos)) {
                cout << "Game found! Enter new details (or re-enter current ones)." << endl;
                cout << "Current Price: $" << g.price << " -> New Price: "; 
                cin >> g.price; cin.ignore();
                
                if (db.update(name, g)) {
                    cout << ">> Game updated successfully." << endl;
                }
            } else {
                cout << ">> Game not found." << endl;
            }
        }
        else if (op == 4) {
            string name;
            cout << "Enter Name to delete: "; getline(cin, name);
            if (db.remove(name)) {
                cout << ">> Game marked as deleted." << endl;
            } else {
                cout << ">> Game not found." << endl;
            }
        }
    }
    
    cout << "Exiting program." << endl;
    return 0;
}