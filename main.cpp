#include <iostream>
#include <fstream>
#include "include/Game.h"
#include "include/CSVConverter.h"
#include "include/GameDAO.h"
#include "include/GameController.h"
#include "include/Review.h"
#include "include/ReviewDAO.h"
#include "include/OrdenacaoExterna.h"

#include "src/Game.cpp"
#include "src/CSVConverter.cpp"
#include "src/GameDAO.cpp"
#include "src/GameController.cpp"
#include "src/User.cpp"
#include "src/Review.cpp"
#include "src/ReviewDAO.cpp"
#include "src/Hash.cpp"
#include "src/HashExtensivel.cpp"
#include "src/UserDAO.cpp"
#include "src/LibraryDAO.cpp"
#include "src/BPlusTree.cpp"
#include "src/OrdenacaoExterna.cpp"

using namespace std;

int main() {
    string csvFilename = "steam.csv";
    string binFilename = "steam.bin";

    // Verifica se os arquivos de dados existem. Se não, reconstrói o banco.
    ifstream check(binFilename);
    if (!check.is_open()) {
        cout << "--------------------------------------------" << endl;
        cout << "PRIMEIRA EXECUCAO DETECTADA" << endl;
        cout << "Convertendo " << csvFilename << " para binario..." << endl;
        
        CSVConverter converter(csvFilename);
        if (!converter.convertToBinary(binFilename)) {
            cerr << "Erro critico: Nao foi possivel criar o banco de dados." << endl;
            return 1;
        }
        
        cout << "Gerando indices e ordenacao inicial..." << endl;
        OrdenacaoExterna ordenacao;
        ordenacao.ordenarPorNome(binFilename, "steam_ordenado.bin");
        cout << "Processo concluido com sucesso!" << endl;
        cout << "--------------------------------------------" << endl;
    } else {
        check.close();
        // Se desejar garantir que o arquivo ordenado sempre exista:
        ifstream checkOrd("steam_ordenado.bin");
        if (!checkOrd.is_open()) {
            OrdenacaoExterna ordenacao;
            ordenacao.ordenarPorNome(binFilename, "steam_ordenado.bin");
        }
    }

    // Desperta o controller (MVC) com suporte a Usuários e Biblioteca (N:N)
    GameController controller(binFilename, "users.bin", "library.bin");
    
    // Inicia a aplicação
    controller.run();

    return 0;
}