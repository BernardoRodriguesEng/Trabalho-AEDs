/**
 * MÓDULO DE CRIPTOGRAFIA XOR (ONE-TIME PAD)
 * 
 * Lógica do Algoritmo:
 * Este módulo utiliza a operação lógica XOR (Ou Exclusivo) para criptografar e descriptografar arquivos.
 * A grande vantagem do XOR é a sua simetria: (Dado ^ Chave) = Criptografado  =>  (Criptografado ^ Chave) = Dado.
 * 
 * - Criptografia: O programa lê o arquivo original em blocos (buckets), gera bytes aleatórios equivalentes ao tamanho 
 *                 do arquivo (funcionando como um "One-Time Pad" prático), aplica o XOR byte a byte, e salva o 
 *                 resultado ininteligível e a chave gerada em arquivos separados.
 * - Descriptografia: O programa lê simultaneamente o arquivo criptografado e o arquivo de chave, aplicando 
 *                    novamente a operação XOR byte a byte para restaurar a informação original.
 */

#include "../../include/Criptografia/XOR.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Tamanho do bloco para leitura/escrita em disco
const size_t TAMANHO_BUFFER = 8192;

bool XOR::criptografar(const string& arquivoEntrada, const string& arquivoSaida, const string& arquivoChave) {
    ifstream entrada(arquivoEntrada, ios::binary | ios::ate);
    ofstream saida(arquivoSaida, ios::binary);
    ofstream chaveOut(arquivoChave, ios::binary);

    if (!entrada.is_open() || !saida.is_open() || !chaveOut.is_open()) {
        cerr << "XOR: Erro ao abrir arquivos para criptografia." << endl;
        return false;
    }

    streamsize bytesRestantes = entrada.tellg();
    entrada.seekg(0, ios::beg);

    // Inicializa a semente (seed) do gerador de numeros aleatorios basico do C
    srand(static_cast<unsigned int>(time(nullptr)));

    // Arrays manuais no estilo C em vez de std::vector
    char bufferIn[TAMANHO_BUFFER];
    char bufferKey[TAMANHO_BUFFER];
    char bufferOut[TAMANHO_BUFFER];

    while (bytesRestantes > 0) {
        entrada.read(bufferIn, TAMANHO_BUFFER);
        streamsize bytesLidos = entrada.gcount();
        if (bytesLidos == 0) break;

        // Gera a chave manualmente e aplica o XOR
        for (streamsize i = 0; i < bytesLidos; i++) {
            char k = static_cast<char>(rand() % 256);
            bufferKey[i] = k;
            bufferOut[i] = bufferIn[i] ^ k;
        }

        chaveOut.write(bufferKey, bytesLidos);
        saida.write(bufferOut, bytesLidos);
        
        bytesRestantes -= bytesLidos;
    }

    return true;
}

bool XOR::descriptografar(const string& arquivoEntrada, const string& arquivoSaida, const string& arquivoChave) {
    ifstream entrada(arquivoEntrada, ios::binary);
    ifstream chaveIn(arquivoChave, ios::binary);
    ofstream saida(arquivoSaida, ios::binary);

    if (!entrada.is_open() || !chaveIn.is_open() || !saida.is_open()) {
        cerr << "XOR: Erro ao abrir arquivos para descriptografia." << endl;
        return false;
    }

    // Arrays manuais no estilo C
    char bufferIn[TAMANHO_BUFFER];
    char bufferKey[TAMANHO_BUFFER];
    char bufferOut[TAMANHO_BUFFER];

    while (entrada) {
        entrada.read(bufferIn, TAMANHO_BUFFER);
        streamsize bytesLidos = entrada.gcount();
        if (bytesLidos == 0) break;

        chaveIn.read(bufferKey, bytesLidos);
        
        if (chaveIn.gcount() != bytesLidos) {
            cerr << "XOR: A chave fornecida tem tamanho incompativel." << endl;
            return false;
        }

        // Aplica o XOR iterando manualmente pelo array
        for (streamsize i = 0; i < bytesLidos; i++) {
            bufferOut[i] = bufferIn[i] ^ bufferKey[i];
        }

        saida.write(bufferOut, bytesLidos);
    }

    return true;
}
