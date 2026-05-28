using namespace std;

#include "../../include/Compressao/Huffman.h"

#include <fstream>
#include <queue>
#include <vector>
#include <bitset>
#include <iostream>


HuffmanNode::HuffmanNode(char c, int f){
    ch = c;
    freq = f;

    left = nullptr;
    right = nullptr;
}

struct Compare{
    bool operator()(HuffmanNode* a, HuffmanNode* b){
        return a->freq > b->freq;
    }
};

void buildCodes(HuffmanNode* root, string code, unordered_map<char, string>& huffmanCode){
    if(!root){
        return;
    }
    if(!root->left && !root->right){
        huffmanCode[root->ch] = code;
    }

    buildCodes(root->left, code + "0", huffmanCode);

    buildCodes(root->right, code + "1", huffmanCode);
}

void Huffman::compress(const string& inputFile, const string& outputFile){

    ifstream in(inputFile, ios::binary);

    if(!in){
        cout << "Erro ao abrir arquivo.\n";

        return;
    }

    string text((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());

    in.close();

    unordered_map<char, int> freq;

    for(char ch : text){
        freq[ch]++;
    }

    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> pq;

    for(auto pair : freq){
        pq.push(new HuffmanNode(pair.first, pair.second));
    }

    while(pq.size() != 1){
        HuffmanNode* left = pq.top();
        pq.pop();

        HuffmanNode* right = pq.top();
        pq.pop();

        HuffmanNode* sum = new HuffmanNode('\0', left->freq + right->freq);

        sum->left = left;
        sum->right = right;

        pq.push(sum);
    }

    HuffmanNode* root = pq.top();

    unordered_map<char, string> huffmanCode;

    buildCodes(root, "", huffmanCode);

    string encoded;

    for(char ch : text){
        encoded += huffmanCode[ch];
    }

    ofstream out(outputFile, ios::binary);

    int tableSize = freq.size();

    out.write(reinterpret_cast<char*>(&tableSize), sizeof(int));

    for(auto pair : freq){
        out.write(&pair.first, sizeof(char));

        out.write(reinterpret_cast<char*>(&pair.second), sizeof(int));
    }

    int encodedSize = encoded.size();

    out.write(reinterpret_cast<char*>(&encodedSize), sizeof(int));

    unsigned char byte = 0;
    int bitCount = 0;

    for(char bit : encoded){
        byte <<= 1;

        if(bit == '1'){
            byte |= 1;
        }

        bitCount++;

        if(bitCount == 8){
            out.write(reinterpret_cast<char*>(&byte), 1);

        byte = 0;
            bitCount = 0;
        }
    }

    if(bitCount > 0){
        byte <<= (8 - bitCount);

        out.write(reinterpret_cast<char*>(&byte), 1);
    }

    out.close();

    cout << "Compactacao Huffman concluida!\n";
}

void Huffman::decompress(const string& inputFile, const string& outputFile){
    ifstream in(inputFile, ios::binary);

    if(!in){
        cout << "Erro ao abrir arquivo.\n";
        return;
    }

    unordered_map<char, int> freq;

    int tableSize;

    in.read(reinterpret_cast<char*>(&tableSize), sizeof(int));

    for(int i = 0; i < tableSize; i++){
        char ch;
        int frequency;

        in.read(&ch, sizeof(char));

        in.read(reinterpret_cast<char*>(&frequency), sizeof(int));

        freq[ch] = frequency;
    }

    int originalSize = 0;

    for(auto pair : freq){
        originalSize += pair.second;
    }

    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> pq;

    for(auto pair : freq){
        pq.push(new HuffmanNode(pair.first, pair.second));
    }

    while(pq.size() != 1){
        HuffmanNode* left = pq.top();
        pq.pop();

        HuffmanNode* right = pq.top();
        pq.pop();

        HuffmanNode* sum = new HuffmanNode('\0', left->freq + right->freq);

        sum->left = left;
        sum->right = right;

        pq.push(sum);
    }

    HuffmanNode* root = pq.top();

    int encodedSize;

    in.read(reinterpret_cast<char*>(&encodedSize), sizeof(int));

    string encoded;
    char byte;

    while(in.read(&byte, 1)){
        bitset<8> bits((unsigned char)byte);

        encoded += bits.to_string();
    }

    encoded = encoded.substr(0, encodedSize);

    in.close();

    string decoded;

    HuffmanNode* current = root;

    for(char bit : encoded){
        if(decoded.size() >= originalSize){
            break;
        }
        if(bit == '0'){
            current = current->left;
        }else{
            current = current->right;
        }
        if(!current->left && !current->right){
            decoded += current->ch;
            current = root;
        }
    }

    ofstream out(outputFile, ios::binary);

    out.write(decoded.c_str(), decoded.size());

    out.close();

    cout << "Descompactacao Huffman concluida!\n";
}