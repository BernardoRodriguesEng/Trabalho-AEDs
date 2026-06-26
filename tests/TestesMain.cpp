#include <iostream>
#include "MotorDeTestes.h"

// Protótipos das funções de teste
void rodarTestesJogo();
void rodarTestesCompressao();
void rodarTestesHash();
void rodarTestesArvoreBMais();
void rodarTestesOrdenacao();
void rodarTestesGameDAO();
void rodarTestesServidor();
void rodarTestesCriptografia();

int main() {
    std::cout << "\n========================================\n";
    std::cout << " INICIANDO BATERIA DE TESTES DO SISTEMA\n";
    std::cout << "========================================\n";

    rodarTestesJogo();
    rodarTestesCompressao();
    rodarTestesHash();
    rodarTestesArvoreBMais();
    rodarTestesOrdenacao();
    rodarTestesGameDAO();
    rodarTestesServidor();
    rodarTestesCriptografia();

    MotorDeTestes::getInstancia().finalizarTestes();

    int falhas = MotorDeTestes::getInstancia().totalFalhas();
    if (falhas > 0) {
        std::cout << "\nAlguns testes falharam. Pressione ENTER para sair..." << std::endl;
        std::cin.get();
        return 1;
    }

    std::cout << "\nTodos os testes passaram! Pressione ENTER para sair..." << std::endl;
    std::cin.get();
    return 0;
}
