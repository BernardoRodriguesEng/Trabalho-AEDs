#include "../../include/Compressao/Saida.h"
#include "../../include/Compressao/LZW.h"
#include "../../include/Compressao/Huffman.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

using namespace std;

namespace CompressaoSaida {

    // -1 = Nenhum (primeira vez), 0 = LZW, 1 = Huffman
    static int ultimoMetodo = -1;

    bool arquivoExiste(const std::string& nomeArquivo) {
        ifstream f(nomeArquivo.c_str());
        return f.good();
    }

    void animarCarregamento(const string& titulo) {
        cout << "\n============================================\n";
        cout << " " << titulo << "\n";
        cout << "============================================\n";
        cout << "Progresso: [";
        for (int i = 0; i < 40; ++i) {
            cout << "=";
            cout.flush();
            this_thread::sleep_for(chrono::milliseconds(30));
        }
        cout << "] 100%\n";
    }

    void lidarComInicializacao(const std::string& arquivoOriginal) {
        string arqLzw = arquivoOriginal + ".lzw";
        string arqHuffman = arquivoOriginal + ".huffman";

        if (arquivoExiste(arqLzw)) {
            ultimoMetodo = 0;
            cout << "\n[!] Banco de dados comprimido via LZW detectado.\n";
            cout << "Pressione ENTER para descompactar e iniciar o servidor...";
            cin.get();
            animarCarregamento("DESCOMPRESSAO LZW EM ANDAMENTO");
            LZW::descomprimir(arqLzw, arquivoOriginal);
            remove(arqLzw.c_str());
            cout << ">> Restauracao LZW concluida!\n\n";
        } 
        else if (arquivoExiste(arqHuffman)) {
            ultimoMetodo = 1;
            cout << "\n[!] Banco de dados comprimido via Huffman detectado.\n";
            cout << "Pressione ENTER para descompactar e iniciar o servidor...";
            cin.get();
            animarCarregamento("DESCOMPRESSAO HUFFMAN EM ANDAMENTO");
            Huffman::descomprimir(arqHuffman, arquivoOriginal);
            remove(arqHuffman.c_str());
            cout << ">> Restauracao Huffman concluida!\n\n";
        }
    }

    void lidarComEncerramento(const std::string& arquivoOriginal) {
        if (!arquivoExiste(arquivoOriginal)) {
            return;
        }

        int escolha;
        if (ultimoMetodo == 0) {
            escolha = 1; // Se era LZW, inverte para Huffman
        } else if (ultimoMetodo == 1) {
            escolha = 0; // Se era Huffman, inverte para LZW
        } else {
            escolha = 0; // Se for a primeira vez (sem histórico de descompressão), começa com LZW
        }

        if (escolha == 0) {
            animarCarregamento("COMPRESSAO LZW DE ENCERRAMENTO");
            string arqLzw = arquivoOriginal + ".lzw";
            LZW::comprimir(arquivoOriginal, arqLzw);
            remove(arquivoOriginal.c_str());
            cout << ">> Banco de dados empacotado via LZW.\n\n";
        } else {
            animarCarregamento("COMPRESSAO HUFFMAN DE ENCERRAMENTO");
            string arqHuffman = arquivoOriginal + ".huffman";
            Huffman::comprimir(arquivoOriginal, arqHuffman);
            remove(arquivoOriginal.c_str());
            cout << ">> Banco de dados empacotado via Huffman.\n\n";
        }
    }

}
