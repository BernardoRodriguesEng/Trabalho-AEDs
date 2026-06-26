#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#define COR_VERDE "\033[32m"
#define COR_VERMELHA "\033[31m"
#define COR_RESET "\033[0m"

class MotorDeTestes {
private:
    int testesPassaram = 0;
    int testesFalharam = 0;
    std::string suiteAtual = "";

    MotorDeTestes() {}

public:
    static MotorDeTestes& getInstancia() {
        static MotorDeTestes instancia;
        return instancia;
    }

    void iniciarSuite(const std::string& nome) {
        suiteAtual = nome;
        std::cout << "\n========================================\n";
        std::cout << "INICIANDO SUITE DE TESTES: " << nome << "\n";
        std::cout << "========================================\n";
    }

    void testarVerdadeiro(bool condicao, const std::string& nomeDoTeste) {
        if (condicao) {
            std::cout << COR_VERDE << "[PASSOU] " << COR_RESET << nomeDoTeste << "\n";
            testesPassaram++;
        } else {
            std::cout << COR_VERMELHA << "[FALHOU] " << COR_RESET << nomeDoTeste << "\n";
            testesFalharam++;
        }
    }

    template <typename T, typename U>
    void testarIgual(const T& obtido, const U& esperado, const std::string& nomeDoTeste) {
        if (obtido == esperado) {
            std::cout << COR_VERDE << "[PASSOU] " << COR_RESET << nomeDoTeste << "\n";
            testesPassaram++;
        } else {
            std::cout << COR_VERMELHA << "[FALHOU] " << COR_RESET << nomeDoTeste << "\n";
            std::cout << "         Esperado: " << esperado << "\n";
            std::cout << "         Obtido:   " << obtido << "\n";
            testesFalharam++;
        }
    }

    void finalizarTestes() {
        std::cout << "\n========================================\n";
        std::cout << "RESULTADOS FINAIS:\n";
        std::cout << COR_VERDE << "PASSARAM: " << testesPassaram << COR_RESET << "\n";
        if (testesFalharam > 0) {
            std::cout << COR_VERMELHA << "FALHARAM: " << testesFalharam << COR_RESET << "\n";
        } else {
            std::cout << COR_VERDE << "TODOS OS TESTES PASSARAM." << COR_RESET << "\n";
        }
        std::cout << "========================================\n";
    }
    
    int totalFalhas() {
        return testesFalharam;
    }
};

// Macros convenientes
#define INICIAR_SUITE(nome) MotorDeTestes::getInstancia().iniciarSuite(nome)
#define TESTAR_VERDADEIRO(condicao, nome) MotorDeTestes::getInstancia().testarVerdadeiro(condicao, nome)
#define TESTAR_IGUAL(obtido, esperado, nome) MotorDeTestes::getInstancia().testarIgual(obtido, esperado, nome)
