#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <string>

struct NoHuffman {
    char caractere;
    int frequencia;

    NoHuffman* filhoEsq;
    NoHuffman* filhoDir;

    NoHuffman(char c, int f);
};

class Huffman {
public:
    static void comprimir(const std::string& arquivoEntrada, const std::string& arquivoSaida);
    static void descomprimir(const std::string& arquivoEntrada, const std::string& arquivoSaida);
};

#endif