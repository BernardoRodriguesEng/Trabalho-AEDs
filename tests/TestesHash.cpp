#include "MotorDeTestes.h"
#include "../include/HashExtensivel.h"

void rodarTestesHash() {
    INICIAR_SUITE("Testes Hash Extensivel");

    HashExtensivel hash(2); // Capacidade de bucket pequena para forcar divisao
    
    hash.inserir(10, 100);
    hash.inserir(20, 200);
    hash.inserir(30, 300); // Deve forcar divisao de bucket

    long offset;
    bool achou = hash.buscar(10, offset);
    TESTAR_VERDADEIRO(achou, "Deve encontrar a chave 10");
    TESTAR_IGUAL(offset, 100, "Offset da chave 10 deve ser 100");

    achou = hash.buscar(30, offset);
    TESTAR_VERDADEIRO(achou, "Deve encontrar a chave 30 apos divisao");
    TESTAR_IGUAL(offset, 300, "Offset da chave 30 deve ser 300");

    achou = hash.buscar(99, offset);
    TESTAR_VERDADEIRO(!achou, "Nao deve encontrar chave inexistente");

    hash.remover(20);
    achou = hash.buscar(20, offset);
    TESTAR_VERDADEIRO(!achou, "Nao deve encontrar chave 20 apos remocao");
}
