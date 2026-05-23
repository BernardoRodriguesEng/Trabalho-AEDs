#include "../include/HashExtensivel.h"

HashExtensivel::HashExtensivel(int capacidade) {
    profundidadeGlobal = 1;
    capacidadeBucket = capacidade;

    diretorio.resize(2);

    for (int i = 0; i < 2; i++) {
        diretorio[i] = new Bucket(1);
    }
}

int HashExtensivel::hash(int id) {
    return id & ((1 << profundidadeGlobal) - 1);
}

bool HashExtensivel::buscar(int id, long &offset) {
    int indice = hash(id);
    return diretorio[indice]->buscar(id, offset);
}

std::vector<long> HashExtensivel::buscarTodos(int id) {
    int indice = hash(id);
    return diretorio[indice]->buscarTodos(id);
}

void HashExtensivel::inserir(int id, long offset) {
    int indice = hash(id);

    if (diretorio[indice]->inserir(id, offset, capacidadeBucket))
        return;

    dividirBucket(indice);
    inserir(id, offset);
}

void HashExtensivel::dividirBucket(int indice) {
    Bucket* bucket = diretorio[indice];

    if (bucket->profundidadeLocal == profundidadeGlobal) {
        int tamanho = diretorio.size();
        diretorio.resize(tamanho * 2);

        for (int i = 0; i < tamanho; i++) {
            diretorio[i + tamanho] = diretorio[i];
        }

        profundidadeGlobal++;
    }

    Bucket* novoBucket = new Bucket(bucket->profundidadeLocal + 1);
    bucket->profundidadeLocal++;

    int mask = (1 << bucket->profundidadeLocal) - 1;

    for (int i = 0; i < diretorio.size(); i++) {
        if (diretorio[i] == bucket && ((i & mask) != (indice & mask))) {
            diretorio[i] = novoBucket;
        }
    }

    auto antigos = bucket->registros;
    bucket->registros.clear();

    for (auto &e : antigos) {
        inserir(e.id, e.offset);
    }
}

bool HashExtensivel::remover(int id) {
    int indice = hash(id);

    return diretorio[indice]->remover(id);
}

bool HashExtensivel::removerPorOffset(int id, long offset) {
    int indice = hash(id);
    return diretorio[indice]->removerPorOffset(id, offset);
}