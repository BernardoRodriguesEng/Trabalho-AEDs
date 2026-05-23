#include "../include/Hash.h"

Bucket::Bucket(int profundidade) {
    profundidadeLocal = profundidade;
}

bool Bucket::inserir(int id, long offset, int capacidade) {
    if (registros.size() >= capacidade)
        return false;

    registros.push_back({id, offset});
    return true;
}

bool Bucket::buscar(int id, long &offset) {
    for (auto &e : registros) {
        if (e.id == id) {
            offset = e.offset;
            return true;
        }
    }
    return false;
}

vector<long> Bucket::buscarTodos(int id) {
    vector<long> offsets;
    for (auto &e : registros) {
        if (e.id == id) {
            offsets.push_back(e.offset);
        }
    }
    return offsets;
}

bool Bucket::remover(int id) {
    for (auto it = registros.begin(); it != registros.end(); ++it) {
        if (it->id == id) {
            registros.erase(it);
            return true;
        }
    }
    return false;
}

bool Bucket::removerPorOffset(int id, long offset) {
    for (auto it = registros.begin(); it != registros.end(); ++it) {
        if (it->id == id && it->offset == offset) {
            registros.erase(it);
            return true;
        }
    }
    return false;
}