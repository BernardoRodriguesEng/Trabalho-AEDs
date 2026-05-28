using namespace std;

#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <string>
#include <unordered_map>

struct HuffmanNode{
    char ch;
    int freq;

    HuffmanNode* left;
    HuffmanNode* right;
    HuffmanNode(char c, int f);
};

class Huffman{
    public:
        static void compress(const string& inputFile, const string& outputFile);
        static void decompress(const string& inputFile, const string& outputFile);

};

#endif