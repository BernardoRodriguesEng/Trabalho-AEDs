using namespace std;

#include "../../include/Compressao/LZW.h"

#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>

void LZW::compress(const string& inputFile, const string& outputFile){
    ifstream in(inputFile, ios::binary);

    if(!in){
        return;
    }

    string data((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());

    in.close();

    unordered_map<string, int> dictionary;

    for(int i = 0; i < 256; i++){
        dictionary[string(1, char(i))] = i;
    }

    string current;
    vector<int> compressed;
    int dictSize = 256;

    for(char c : data){
        string next = current + c;

        if(dictionary.count(next)){
            current = next;
        }else{
            compressed.push_back(dictionary[current]);
            dictionary[next] = dictSize++;
            current = string(1, c);
        }
    }

    if(!current.empty()){
        compressed.push_back(dictionary[current]);
    }

    ofstream out(outputFile, ios::binary);

    for(int code : compressed){
        out.write(reinterpret_cast<char*>(&code), sizeof(int));
    }

    out.close();
}

void LZW::decompress(const string& inputFile, const string& outputFile){
    ifstream in(inputFile, ios::binary);

    if(!in){
        return;
    }

    vector<int> compressed;
    int code;

    while(in.read(reinterpret_cast<char*>(&code), sizeof(int))){
        compressed.push_back(code);
    }

    in.close();

    unordered_map<int, string> dictionary;

    for(int i = 0; i < 256; i++){
        dictionary[i] = string(1, char(i));
    }

    int dictSize = 256;
    string previous = dictionary[compressed[0]];
    string result = previous;

    for(size_t i = 1; i < compressed.size(); i++){
        string entry;

        if(dictionary.count(compressed[i])){
            entry = dictionary[compressed[i]];
        }else{
            entry = previous + previous[0];
        }

        result += entry;

        dictionary[dictSize++] = previous + entry[0];

        previous = entry;
    }

    ofstream out(outputFile, ios::binary);
    out.write(result.c_str(), result.size());
    out.close();
}