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
#include "src/Review.cpp"
#include "src/ReviewDAO.cpp"
#include "src/Hash.cpp"
#include "src/HashExtensivel.cpp"
#include "src/OrdenacaoExterna.cpp"

using namespace std;

int main() {
    string csvFilename = "steam.csv";
    string binFilename = "steam.bin";

    // Verifies if the binary file exists
    ifstream check(binFilename);
    if (!check.is_open()) {
        cout << binFilename << " not found. Converting from " << csvFilename << "..." << endl;
        CSVConverter converter(csvFilename);
        if (!converter.convertToBinary(binFilename)) {
            cerr << "Erro fatal na conversão do CSV!" << endl;
            return 1;
        }
    }
    check.close();

    OrdenacaoExterna ordenacao;
    ordenacao.ordenarPorNome(binFilename, "steam_ordenado.bin");

    // Awakes controller (MVC)
    GameController controller(binFilename);
    
    // Starts application
    controller.run();

    return 0;
}