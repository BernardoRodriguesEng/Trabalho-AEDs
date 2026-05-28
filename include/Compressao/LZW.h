using namespace std;

#ifndef LZW_H
#define LZW_H

#include <string>
#include <vector>

class LZW{
    public:
        static void compress(const string& inputFile, const string& outputFile);
        static void decompress(const string& inputFile, const string&outputFile);
        
};

#endif