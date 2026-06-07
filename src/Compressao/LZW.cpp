#include "../../include/Compressao/LZW.h"
#include "../../include/Compressao/GerenciadorArquivo.h"
#include "../../include/Compressao/TrieLZW.h"
#include "../../include/Compressao/ArranjosDinamicos.h"
#include <iostream>
#include <string>

using namespace std;

static const int TAM_INICIAL_DICIONARIO = 256;

void LZW::comprimir(const string& arquivoEntrada, const string& arquivoSaida) {
    GerenciadorArquivo arqEntrada(arquivoEntrada, "rb");
    if (!arqEntrada.estaAberto()) {
        cerr << "Erro: Nao foi possivel abrir o arquivo de entrada: " << arquivoEntrada << endl;
        return;
    }

    string dadosArquivo = arqEntrada.lerTudo();
    arqEntrada.fechar();

    if (dadosArquivo.empty()) {
        cerr << "Aviso: O arquivo de entrada esta vazio.\n";
        return;
    }

    NoTrieLZW* dicionario = new NoTrieLZW();
    for (int i = 0; i < TAM_INICIAL_DICIONARIO; i++) {
        dicionario->filhos[i] = new NoTrieLZW(i);
    }

    int tamAtualDicionario = TAM_INICIAL_DICIONARIO;
    NoTrieLZW* noAtual = dicionario;
    ArrayDinamicoInt codigosComprimidos;

    for (char caractere : dadosArquivo) {
        unsigned char uc = static_cast<unsigned char>(caractere);
        
        if (noAtual->filhos[uc] != nullptr) {
            noAtual = noAtual->filhos[uc];
        } else {
            codigosComprimidos.adicionar(noAtual->codigo);
            
            noAtual->filhos[uc] = new NoTrieLZW(tamAtualDicionario++);
            
            noAtual = dicionario->filhos[uc];
        }
    }

    if (noAtual != dicionario) {
        codigosComprimidos.adicionar(noAtual->codigo);
    }

    GerenciadorArquivo arqSaida(arquivoSaida, "wb");
    if (!arqSaida.estaAberto()) {
        cerr << "Erro: Nao foi possivel criar o arquivo de saida: " << arquivoSaida << endl;
        delete dicionario;
        return;
    }

    for (int i = 0; i < codigosComprimidos.tamanho; i++) {
        arqSaida.escrever(&codigosComprimidos.dados[i], sizeof(int), 1);
    }

    arqSaida.fechar();
    delete dicionario;

    cout << "Compactacao LZW concluida!\n";
}

void LZW::descomprimir(const string& arquivoEntrada, const string& arquivoSaida) {
    GerenciadorArquivo arqEntrada(arquivoEntrada, "rb");
    if (!arqEntrada.estaAberto()) {
        cerr << "Erro: Nao foi possivel abrir o arquivo compactado: " << arquivoEntrada << endl;
        return;
    }

    ArrayDinamicoInt codigosComprimidos;
    int codigoAtual;
    while (arqEntrada.ler(&codigoAtual, sizeof(int), 1) == 1) {
        codigosComprimidos.adicionar(codigoAtual);
    }
    arqEntrada.fechar();

    if (codigosComprimidos.tamanho == 0) {
        cerr << "Aviso: O arquivo compactado esta vazio ou corrompido.\n";
        return;
    }

    ArrayDinamicoString dicionario;
    
    for (int i = 0; i < TAM_INICIAL_DICIONARIO; i++) {
        dicionario.adicionar(string(1, static_cast<char>(i)));
    }

    int tamAtualDicionario = TAM_INICIAL_DICIONARIO;
    string stringAnterior = dicionario.dados[codigosComprimidos.dados[0]];
    string resultadoDescomprimido = stringAnterior;

    for (int i = 1; i < codigosComprimidos.tamanho; i++) {
        int codigo = codigosComprimidos.dados[i];
        string entrada;

        if (codigo < dicionario.tamanho) {
            entrada = dicionario.dados[codigo];
        } else if (codigo == tamAtualDicionario) {
            entrada = stringAnterior + stringAnterior[0];
        } else {
            cerr << "Erro: Codigo LZW invalido detectado no arquivo compactado.\n";
            return;
        }

        resultadoDescomprimido += entrada;

        dicionario.adicionar(stringAnterior + entrada[0]);
        tamAtualDicionario++;
        
        stringAnterior = entrada;
    }

    GerenciadorArquivo arqSaida(arquivoSaida, "wb");
    if (!arqSaida.estaAberto()) {
        cerr << "Erro: Nao foi possivel criar o arquivo de saida descompactado: " << arquivoSaida << endl;
        return;
    }

    arqSaida.escrever(resultadoDescomprimido.c_str(), 1, resultadoDescomprimido.size());
    arqSaida.fechar();

    cout << "Descompactacao LZW concluida.\n";
}