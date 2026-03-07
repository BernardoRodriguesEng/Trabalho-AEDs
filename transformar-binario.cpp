#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>

using namespace std;

// Esta é a estrutura que você definiu. 
// Note que aumentei um pouco alguns tamanhos para garantir que caibam todos os dados do CSV.
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
};



class transformar_binario{
    private:
        ifstream csvFile("steam.csv");
        string line;

    public:
        transformar_binario();
        ~transformar_binario();
        void transformar();
};

transformar_binario::transformar_binario(){
    if (!csvFile.is_open()) {
        cerr << "Erro ao abrir o arquivo CSV!" << endl;
        return;
    }
    getline(csvFile, line);
}

transformar_binario::~transformar_binario(){
    
}

void transformar_binario::transformar(){
    while (getline(csvFile, line)) {
        stringstream ss(line);
        string field;
        Game g;
        getline(ss, field, ','); g.appid = stoi(field);
        getline(ss, field, ','); safelySetString(g.name, field, sizeof(g.name));
        getline(ss, field, ','); safelySetString(g.release_date, field, sizeof(g.release_date));
        getline(ss, field, ','); g.english = (field == "1");
        getline(ss, field, ','); safelySetString(g.developer, field, sizeof(g.developer));
        getline(ss, field, ','); safelySetString(g.publisher, field, sizeof(g.publisher));
        getline(ss, field, ','); safelySetString(g.platforms, field, sizeof(g.platforms));
        getline(ss, field, ','); g.required_age = stoi(field);
        getline(ss, field, ','); safelySetString(g.categories, field, sizeof(g.categories));
        getline(ss, field, ','); safelySetString(g.genres, field, sizeof(g.genres));
        getline(ss, field, ','); safelySetString(g.steamspy_tags, field, sizeof(g.steamspy_tags));
        getline(ss, field, ','); g.achievements = stoi(field);
        getline(ss, field, ','); g.positive_ratings = stoi(field);
        getline(ss, field, ','); g.negative_ratings = stoi(field);
        getline(ss, field, ','); g.average_playtime = stoi(field);
        getline(ss, field, ','); g.median_playtime = stoi(field);
        getline(ss, field, ','); safelySetString(g.owners, field, sizeof(g.owners));
        getline(ss, field, ','); g.price = stof(field);


        
    }
}