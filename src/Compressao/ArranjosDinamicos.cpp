#include "../../include/Compressao/ArranjosDinamicos.h"

ArrayDinamicoInt::ArrayDinamicoInt(int capacidadeInicial) {
    capacidade = capacidadeInicial;
    tamanho = 0;
    dados = new int[capacidade];
}

ArrayDinamicoInt::~ArrayDinamicoInt() {
    delete[] dados;
}

void ArrayDinamicoInt::adicionar(int valor) {
    if (tamanho >= capacidade) {
        capacidade *= 2;
        int* novosDados = new int[capacidade];
        for (int i = 0; i < tamanho; i++) {
            novosDados[i] = dados[i];
        }
        delete[] dados;
        dados = novosDados;
    }
    dados[tamanho++] = valor;
}

ArrayDinamicoString::ArrayDinamicoString(int capacidadeInicial) {
    capacidade = capacidadeInicial;
    tamanho = 0;
    dados = new std::string[capacidade];
}

ArrayDinamicoString::~ArrayDinamicoString() {
    delete[] dados;
}

void ArrayDinamicoString::adicionar(const std::string& valor) {
    if (tamanho >= capacidade) {
        capacidade *= 2;
        std::string* novosDados = new std::string[capacidade];
        for (int i = 0; i < tamanho; i++) {
            novosDados[i] = dados[i];
        }
        delete[] dados;
        dados = novosDados;
    }
    dados[tamanho++] = valor;
}
