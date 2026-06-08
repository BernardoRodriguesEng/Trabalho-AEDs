#include "MotorDeTestes.h"
#include "../include/Compressao/LZW.h"
#include "../include/Compressao/Huffman.h"
#include <fstream>
#include <string>

void rodarTestesCompressao() {
    INICIAR_SUITE("Testes de Compressao (LZW e Huffman)");

    std::string originalStr = "BANANA BANANA MACACO MACACA BANANA";
    std::string originalFile = "teste_compressao_original.txt";
    std::string lzwFile = "teste_compressao_lzw.bin";
    std::string lzwRestored = "teste_lzw_restored.txt";
    std::string huffmanFile = "teste_compressao_huffman.bin";
    std::string huffmanRestored = "teste_huffman_restored.txt";

    // Criar arquivo original
    std::ofstream out(originalFile, std::ios::binary);
    out << originalStr;
    out.close();

    // Testar LZW
    LZW::comprimir(originalFile, lzwFile);
    LZW::descomprimir(lzwFile, lzwRestored);

    std::ifstream inLzw(lzwRestored, std::ios::binary);
    std::string resultLzw((std::istreambuf_iterator<char>(inLzw)), std::istreambuf_iterator<char>());
    inLzw.close();

    TESTAR_IGUAL(resultLzw, originalStr, "Restauracao LZW deve ser identica ao original");

    // Testar Huffman
    Huffman::comprimir(originalFile, huffmanFile);
    Huffman::descomprimir(huffmanFile, huffmanRestored);

    std::ifstream inHuff(huffmanRestored, std::ios::binary);
    std::string resultHuff((std::istreambuf_iterator<char>(inHuff)), std::istreambuf_iterator<char>());
    inHuff.close();

    TESTAR_IGUAL(resultHuff, originalStr, "Restauracao Huffman deve ser identica ao original");

    // Limpar arquivos temporários
    std::remove(originalFile.c_str());
    std::remove(lzwFile.c_str());
    std::remove(lzwRestored.c_str());
    std::remove(huffmanFile.c_str());
    std::remove(huffmanRestored.c_str());
}
