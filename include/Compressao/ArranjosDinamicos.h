#ifndef ARRANJOS_DINAMICOS_H
#define ARRANJOS_DINAMICOS_H

#include <string>

// Array dinâmico simples para inteiros
struct ArrayDinamicoInt {
    int* dados;
    int capacidade;
    int tamanho;

    ArrayDinamicoInt(int capacidadeInicial = 1024);
    ~ArrayDinamicoInt();
    void adicionar(int valor);
};

// Array dinâmico simples para strings
struct ArrayDinamicoString {
    std::string* dados;
    int capacidade;
    int tamanho;

    ArrayDinamicoString(int capacidadeInicial = 4096);
    ~ArrayDinamicoString();
    void adicionar(const std::string& valor);
};

#endif
