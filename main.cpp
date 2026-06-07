#include <iostream>
#include <fstream>
#include "include/Game.h"
#include "include/CSVConverter.h"
#include "include/GameDAO.h"
#include "include/GameController.h"
#include "include/Review.h"
#include "include/ReviewDAO.h"
#include "include/OrdenacaoExterna.h"
#include "include/Compressao/LZW.h"
#include "include/Compressao/Huffman.h"
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
#include "src/Compressao/GerenciadorArquivo.cpp"
#include "src/Compressao/ArranjosDinamicos.cpp"
#include "src/Compressao/TrieLZW.cpp"
#include "src/Compressao/LZW.cpp"
#include "src/Compressao/Huffman.cpp"
#include "include/Compressao/Saida.h"
#include "src/Compressao/Saida.cpp"

using namespace std;

long getFileSize(const string& filename) {
    ifstream file(filename, ios::binary | ios::ate);

    if (!file)
        return -1;

    return file.tellg();
}

void testarCompressoes() {
    cout << "\n========================================\n";
    cout << "TESTE DE COMPRESSAO\n";
    cout << "========================================\n";

    string original = "steam.bin";

    // ---------------- LZW ----------------

    cout << "\n[LZW]\n";

    LZW::comprimir(original, "steam_lzw.bin");
    LZW::descomprimir("steam_lzw.bin", "steam_lzw_restored.bin");

    long originalSize = getFileSize(original);
    long compressedLZW = getFileSize("steam_lzw.bin");
    long restoredLZW = getFileSize("steam_lzw_restored.bin");

    cout << "Original:      " << originalSize << " bytes\n";
    cout << "Comprimido:    " << compressedLZW << " bytes\n";
    cout << "Restaurado:    " << restoredLZW << " bytes\n";

    float taxaLZW = (1.0f - ((float)compressedLZW / originalSize)) * 100;
    cout << "Taxa de compressao: " << taxaLZW << "%\n";

    if(restoredLZW == originalSize){
        cout << "LZW OK!\n";
    }
    else{
        cout << "RestauracaoLZW DIFERENTE!\n";
    }

    // ---------------- HUFFMAN ----------------

    cout << "\n[HUFFMAN]\n";

    Huffman::comprimir(original, "steam_huffman.bin");
    Huffman::descomprimir("steam_huffman.bin", "steam_huffman_restored.bin");

    long compressedHuffman = getFileSize("steam_huffman.bin");
    long restoredHuffman = getFileSize("steam_huffman_restored.bin");

    cout << "Original:      " << originalSize << " bytes\n";
    cout << "Comprimido:    " << compressedHuffman << " bytes\n";
    cout << "Restaurado:    " << restoredHuffman << " bytes\n";

    float taxaHuffman = (1.0f - ((float)compressedHuffman / originalSize)) * 100;
    cout << "Taxa de compressao: " << taxaHuffman << "%\n";

    if(restoredHuffman == originalSize){
        cout << "Huffman OK!\n";
    }
    else{
        cout << "Restauracao Huffman DIFERENTE!\n";
    }

    cout << "========================================\n";
}

int main() {
    string csvFilename = "steam.csv";
    string binFilename = "steam.bin";

    // Descomprime caso o programa tenha sido fechado anteriormente
    CompressaoSaida::lidarComInicializacao(binFilename);

    // Verifica se os arquivos de dados existem. Se não, reconstrói o banco.
    ifstream check(binFilename);
    if (!check.is_open()) {
        cout << "--------------------------------------------" << endl;
        cout << "PRIMEIRA EXECUCAO DETECTADA" << endl;
        cout << "Convertendo " << csvFilename << " para binario..." << endl;
        
        CSVConverter converter(csvFilename);
        if (!converter.convertToBinary(binFilename)) {
            cerr << "Erro critico: Nao foi possivel criar o banco de dados." << endl;
            cout << "\nPressione ENTER para fechar a janela..." << endl;
            cin.get();
            return 1;
        }
        
        cout << "Gerando indices e ordenacao inicial..." << endl;
        OrdenacaoExterna ordenacao;
        ordenacao.ordenarPorNome(binFilename, "steam_ordenado.bin");
        cout << "Concluido." << endl;
        cout << "--------------------------------------------" << endl;
        cout << "\nPressione ENTER para fechar essa janela..." << endl;
        cin.get();
        return 0;
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

    // Comprime ao encerrar
    CompressaoSaida::lidarComEncerramento(binFilename);

    cout << "\nPressione ENTER para fechar a janela..." << endl;
    cin.get();

    return 0;
}