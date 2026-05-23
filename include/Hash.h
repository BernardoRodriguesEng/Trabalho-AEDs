#ifndef HASH_H
#define HASH_H

#include <vector>
using namespace std;

struct Entrada {
    int id;
    long offset;
};

class Bucket {
public:
    int profundidadeLocal;
    vector<Entrada> registros;

    Bucket(int profundidade);

    bool inserir(int id, long offset, int capacidade);
    bool buscar(int id, long &offset);
    vector<long> buscarTodos(int id);

    bool remover(int id);
    bool removerPorOffset(int id, long offset);
};

#endif