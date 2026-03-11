#include "../include/CSVConverter.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

CSVConverter::CSVConverter(const string& csvFilename) {
    csvFile.open(csvFilename);
}

CSVConverter::~CSVConverter() {
    if (csvFile.is_open()) csvFile.close();
}

vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        if (!token.empty()) tokens.push_back(token);
    }
    return tokens;
}

Date parseDate(const string& s) {
    Date d = {0, 0, 0};
    // Expected format: YYYY-MM-DD
    if (s.length() >= 10 && s[4] == '-' && s[7] == '-') {
        try {
            d.year = stoi(s.substr(0, 4));
            d.month = stoi(s.substr(5, 2));
            d.day = stoi(s.substr(8, 2));
        } catch (...) {}
    }
    return d;
}

Game CSVConverter::parseLine() {
    string line;
    if (!getline(csvFile, line) || line.empty()) return Game();

    vector<string> fields;
    string field;
    bool inQuotes = false;
    for (size_t i = 0; i < line.length(); ++i) {
        if (line[i] == '"') {
            inQuotes = !inQuotes;
        } else if (line[i] == ',' && !inQuotes) {
            fields.push_back(field);
            field.clear();
        } else {
            field += line[i];
        }
    }
    fields.push_back(field);

    if (fields.size() < 18) return Game();

    try {
        Game g;
        g.appid = stoi(fields[0]);
        g.name = fields[1];
        g.release_date = parseDate(fields[2]);
        g.english = (fields[3] == "1");
        g.developer = fields[4];
        g.publisher = fields[5];
        g.platforms = fields[6];
        g.required_age = stoi(fields[7]);
        g.categories = split(fields[8], ';');
        g.genres = split(fields[9], ';');
        g.steamspy_tags = split(fields[10], ';');
        g.achievements = stoi(fields[11]);
        g.positive_ratings = stoi(fields[12]);
        g.negative_ratings = stoi(fields[13]);
        g.average_playtime = stoi(fields[14]);
        g.median_playtime = stoi(fields[15]);
        g.owners = fields[16];
        g.price = stof(fields[17]);

        return g;
    } catch (...) {
        return Game();
    }
}

bool CSVConverter::convertToBinary(const string& binFilename) {
    if (!csvFile.is_open()) return false;

    ofstream binFile(binFilename, ios::binary);
    if (!binFile.is_open()) return false;

    // Cabeçalho: guarda o maior AppID encontrado (4 bytes)
    int maxAppId = 0;
    binFile.write(reinterpret_cast<char*>(&maxAppId), sizeof(int));

    string indexFilename = binFilename;
    size_t dotPos = indexFilename.find_last_of(".");
    if (dotPos != string::npos) indexFilename = indexFilename.substr(0, dotPos);
    indexFilename += "_index.bin";

    ofstream indexFile(indexFilename, ios::binary);
    if (!indexFile.is_open()) return false;

    string headerLine;
    getline(csvFile, headerLine);

    int count = 0;
    while (csvFile.peek() != EOF) {
        Game g = parseLine();
        if (g.appid != 0) {
            long offset = binFile.tellp();
            if (g.appid > maxAppId) maxAppId = g.appid;

            int size = g.getSerializationSize();
            char* buffer = new char[size];
            g.serialize(buffer);
            binFile.write(buffer, size);
            delete[] buffer;

            indexFile.write(reinterpret_cast<const char*>(&g.appid), sizeof(int));
            indexFile.write(reinterpret_cast<const char*>(&offset), sizeof(long));
            count++;
        }
    }

    binFile.seekp(0, ios::beg);
    binFile.write(reinterpret_cast<char*>(&maxAppId), sizeof(int));

    binFile.close();
    indexFile.close();
    cout << "Convertidos " << count << " registros com sucesso!" << endl;
    return true;
}
