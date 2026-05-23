#ifndef HASHEXTENSIVEL_H
#define HASHEXTENSIVEL_H

#include "Hash.h"
#include <vector>

class HashExtensivel {
private:
    int profundidadeGlobal;
    int capacidadeBucket;

    std::vector<Bucket*> diretorio;

    int hash(int id);
    void dividirBucket(int indice);

public:
    HashExtensivel(int capacidade);

    void inserir(int id, long offset);
    bool buscar(int id, long &offset);
    std::vector<long> buscarTodos(int id);

    bool remover(int id);
    bool removerPorOffset(int id, long offset);
};

#endif