#include "MotorDeTestes.h"
#include "../include/BPlusTree.h"
#include <cstdio>

void rodarTestesArvoreBMais() {
    INICIAR_SUITE("Testes Arvore B+");

    std::string arquivoTree = "teste_tree.bin";
    
    std::remove(arquivoTree.c_str());

    BPlusTree tree(arquivoTree);

    tree.inserir(10.5f, 100);
    tree.inserir(20.5f, 200);
    tree.inserir(5.5f, 50);
    tree.inserir(15.5f, 150);

    long offset = tree.buscar(10.5f);
    TESTAR_IGUAL(offset, 100, "Deve encontrar a chave 10.5");

    offset = tree.buscar(20.5f);
    TESTAR_IGUAL(offset, 200, "Deve encontrar a chave 20.5");

    offset = tree.buscar(99.9f);
    TESTAR_IGUAL(offset, -1, "Nao deve encontrar chave inexistente");

    std::vector<long> resultados = tree.buscarIntervalo(5.0f, 16.0f);
    TESTAR_IGUAL(resultados.size(), 3, "Busca por intervalo deve retornar 3 elementos");

    std::remove(arquivoTree.c_str());
}
