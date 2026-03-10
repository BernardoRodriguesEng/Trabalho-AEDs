#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <iostream>

struct Date {
    int day;
    int month;
    int year;

    void print() const {
        std::cout << day << "/" << month << "/" << year;
    }
};

class Game {
public:
    Game();
    
    void setActive(bool active);
    bool isActive() const;
    std::string getName() const;
    int getAppId() const;
    float getPrice() const;
    std::string getDeveloper() const;
    void print() const;

    int getSerializationSize() const;
    void serialize(char* buffer) const;
    void deserialize(const char* buffer);
    void readFromStream(std::istream& is);

    char lapide;
    int appid;
    std::string name;
    Date release_date;
    bool english;
    std::string developer;
    std::string publisher;
    std::string platforms;
    int required_age;
    std::vector<std::string> categories;
    std::vector<std::string> genres;
    std::vector<std::string> steamspy_tags;
    int achievements;
    int positive_ratings;
    int negative_ratings;
    int average_playtime;
    int median_playtime;
    std::string owners;
    float price;

private:
    void writeString(char*& ptr, const std::string& str) const;
    void readString(const char*& ptr, std::string& str);
    void writeVector(char*& ptr, const std::vector<std::string>& vec) const;
    void readVector(const char*& ptr, std::vector<std::string>& vec);
    
    void readStringFromStream(std::istream& is, std::string& str);
    void readVectorFromStream(std::istream& is, std::vector<std::string>& vec);
};

#endif
