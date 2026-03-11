#include "../include/Game.h"
#include <cstring>
#include <iostream>
#include <vector>

using namespace std;

Game::Game() : lapide(' '), appid(0), english(false), required_age(0), 
               achievements(0), positive_ratings(0), negative_ratings(0), 
               average_playtime(0), median_playtime(0), price(0.0f) {
    release_date = {0, 0, 0};
}

void Game::setActive(bool active) { lapide = active ? ' ' : '*'; }
bool Game::isActive() const { return lapide == ' '; }
string Game::getName() const { return name; }
int Game::getAppId() const { return appid; }
float Game::getPrice() const { return price; }
string Game::getDeveloper() const { return developer; }

void Game::print() const {
    if (!isActive()) return;
    cout << "   [" << appid << "] " << name << endl;
    cout << "   Release Date: "; release_date.print(); cout << endl;
    cout << "   Developer: " << developer << " | Publisher: " << publisher << endl;
    cout << "   Price: $" << price << endl;
    cout << "   Genres: ";
    for (size_t i = 0; i < genres.size(); ++i) {
        cout << genres[i] << (i == genres.size() - 1 ? "" : ", ");
    }
    cout << endl;
    cout << "-----------------------------------" << endl;
}

int Game::getSerializationSize() const {
    int size = sizeof(char) + sizeof(int); // lapide + appid
    size += sizeof(unsigned short) + name.length();
    size += sizeof(Date);
    size += sizeof(bool); // english
    size += sizeof(unsigned short) + developer.length();
    size += sizeof(unsigned short) + publisher.length();
    size += sizeof(unsigned short) + platforms.length();
    size += sizeof(int); // age
    
    // Vectors
    size += sizeof(unsigned short); // categories count
    for(const auto& s : categories) size += sizeof(unsigned short) + s.length();
    
    size += sizeof(unsigned short); // genres count
    for(const auto& s : genres) size += sizeof(unsigned short) + s.length();
    
    size += sizeof(unsigned short); // tags count
    for(const auto& s : steamspy_tags) size += sizeof(unsigned short) + s.length();

    size += sizeof(int) * 5; // ratings, playtime
    size += sizeof(unsigned short) + owners.length();
    size += sizeof(float);
    return size;
}

void Game::writeString(char*& ptr, const string& str) const {
    unsigned short len = (unsigned short)str.length();
    memcpy(ptr, &len, sizeof(unsigned short));
    ptr += sizeof(unsigned short);
    memcpy(ptr, str.c_str(), len);
    ptr += len;
}

void Game::writeVector(char*& ptr, const vector<string>& vec) const {
    unsigned short count = (unsigned short)vec.size();
    memcpy(ptr, &count, sizeof(unsigned short));
    ptr += sizeof(unsigned short);
    for (const auto& s : vec) {
        writeString(ptr, s);
    }
}

void Game::serialize(char* buffer) const {
    char* ptr = buffer;
    memcpy(ptr, &lapide, sizeof(char)); ptr += sizeof(char);
    memcpy(ptr, &appid, sizeof(int)); ptr += sizeof(int);
    writeString(ptr, name);
    memcpy(ptr, &release_date, sizeof(Date)); ptr += sizeof(Date);
    memcpy(ptr, &english, sizeof(bool)); ptr += sizeof(bool);
    writeString(ptr, developer);
    writeString(ptr, publisher);
    writeString(ptr, platforms);
    memcpy(ptr, &required_age, sizeof(int)); ptr += sizeof(int);
    
    writeVector(ptr, categories);
    writeVector(ptr, genres);
    writeVector(ptr, steamspy_tags);

    memcpy(ptr, &achievements, sizeof(int)); ptr += sizeof(int);
    memcpy(ptr, &positive_ratings, sizeof(int)); ptr += sizeof(int);
    memcpy(ptr, &negative_ratings, sizeof(int)); ptr += sizeof(int);
    memcpy(ptr, &average_playtime, sizeof(int)); ptr += sizeof(int);
    memcpy(ptr, &median_playtime, sizeof(int)); ptr += sizeof(int);
    writeString(ptr, owners);
    memcpy(ptr, &price, sizeof(float)); ptr += sizeof(float);
}

void Game::readString(const char*& ptr, string& str) {
    unsigned short len;
    memcpy(&len, ptr, sizeof(unsigned short));
    ptr += sizeof(unsigned short);
    str.assign(ptr, len);
    ptr += len;
}

void Game::readVector(const char*& ptr, vector<string>& vec) {
    unsigned short count;
    memcpy(&count, ptr, sizeof(unsigned short));
    ptr += sizeof(unsigned short);
    vec.clear();
    for (int i = 0; i < count; ++i) {
        string s;
        readString(ptr, s);
        vec.push_back(s);
    }
}

void Game::deserialize(const char* buffer) {
    const char* ptr = buffer;
    memcpy(&lapide, ptr, sizeof(char)); ptr += sizeof(char);
    memcpy(&appid, ptr, sizeof(int)); ptr += sizeof(int);
    readString(ptr, name);
    memcpy(&release_date, ptr, sizeof(Date)); ptr += sizeof(Date);
    memcpy(&english, ptr, sizeof(bool)); ptr += sizeof(bool);
    readString(ptr, developer);
    readString(ptr, publisher);
    readString(ptr, platforms);
    memcpy(&required_age, ptr, sizeof(int)); ptr += sizeof(int);
    
    readVector(ptr, categories);
    readVector(ptr, genres);
    readVector(ptr, steamspy_tags);

    memcpy(&achievements, ptr, sizeof(int)); ptr += sizeof(int);
    memcpy(&positive_ratings, ptr, sizeof(int)); ptr += sizeof(int);
    memcpy(&negative_ratings, ptr, sizeof(int)); ptr += sizeof(int);
    memcpy(&average_playtime, ptr, sizeof(int)); ptr += sizeof(int);
    memcpy(&median_playtime, ptr, sizeof(int)); ptr += sizeof(int);
    readString(ptr, owners);
    memcpy(&price, ptr, sizeof(float)); ptr += sizeof(float);
}

void Game::readStringFromStream(istream& is, string& str) {
    unsigned short len;
    is.read(reinterpret_cast<char*>(&len), sizeof(unsigned short));
    char* buf = new char[len + 1];
    is.read(buf, len);
    buf[len] = '\0';
    str = string(buf);
    delete[] buf;
}

void Game::readVectorFromStream(istream& is, vector<string>& vec) {
    unsigned short count;
    is.read(reinterpret_cast<char*>(&count), sizeof(unsigned short));
    vec.clear();
    for (int i = 0; i < count; ++i) {
        string s;
        readStringFromStream(is, s);
        vec.push_back(s);
    }
}

void Game::readFromStream(istream& is) {
    is.read(&lapide, sizeof(char));
    is.read(reinterpret_cast<char*>(&appid), sizeof(int));
    readStringFromStream(is, name);
    is.read(reinterpret_cast<char*>(&release_date), sizeof(Date));
    is.read(reinterpret_cast<char*>(&english), sizeof(bool));
    readStringFromStream(is, developer);
    readStringFromStream(is, publisher);
    readStringFromStream(is, platforms);
    is.read(reinterpret_cast<char*>(&required_age), sizeof(int));
    
    readVectorFromStream(is, categories);
    readVectorFromStream(is, genres);
    readVectorFromStream(is, steamspy_tags);

    is.read(reinterpret_cast<char*>(&achievements), sizeof(int));
    is.read(reinterpret_cast<char*>(&positive_ratings), sizeof(int));
    is.read(reinterpret_cast<char*>(&negative_ratings), sizeof(int));
    is.read(reinterpret_cast<char*>(&average_playtime), sizeof(int));
    is.read(reinterpret_cast<char*>(&median_playtime), sizeof(int));
    readStringFromStream(is, owners);
    is.read(reinterpret_cast<char*>(&price), sizeof(float));
}