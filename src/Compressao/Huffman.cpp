#include "../../include/Compressao/Huffman.h"
#include "../../include/Compressao/GerenciadorArquivo.h"
#include <iostream>

using namespace std;

NoHuffman::NoHuffman(char c, int f)
    : caractere(c), frequencia(f), filhoEsq(nullptr), filhoDir(nullptr) {}

static void construirCodigos(NoHuffman* no, const string& codigoAtual, string* codigosHuffman) {
    if (!no) return;

    if (!no->filhoEsq && !no->filhoDir) {
        codigosHuffman[static_cast<unsigned char>(no->caractere)] = codigoAtual;
    }

    construirCodigos(no->filhoEsq, codigoAtual + "0", codigosHuffman);
    construirCodigos(no->filhoDir, codigoAtual + "1", codigosHuffman);
}

static void deletarArvore(NoHuffman* no) {
    if (!no) return;
    deletarArvore(no->filhoEsq);
    deletarArvore(no->filhoDir);
    delete no;
}

void Huffman::comprimir(const string& arquivoEntrada, const string& arquivoSaida) {
    GerenciadorArquivo arqEntrada(arquivoEntrada, "rb");
    if (!arqEntrada.estaAberto()) {
        cerr << "Erro: Nao foi possivel abrir o arquivo de entrada: " << arquivoEntrada << endl;
        return;
    }

    string texto = arqEntrada.lerTudo();
    arqEntrada.fechar();

    if (texto.empty()) {
        cerr << "Aviso: O arquivo de entrada esta vazio.\n";
        return;
    }

    int frequencias[256] = {0};
    for (char c : texto) {
        frequencias[static_cast<unsigned char>(c)]++;
    }

    int qtdNosAtivos = 0;
    NoHuffman* nosAtivos[256];
    for (int i = 0; i < 256; i++) {
        if (frequencias[i] > 0) {
            nosAtivos[qtdNosAtivos++] = new NoHuffman(static_cast<char>(i), frequencias[i]);
        }
    }

    while (qtdNosAtivos > 1) {
        int idxMin1 = -1;
        int idxMin2 = -1;

        for (int i = 0; i < qtdNosAtivos; i++) {
            if (idxMin1 == -1 || nosAtivos[i]->frequencia < nosAtivos[idxMin1]->frequencia) {
                idxMin2 = idxMin1;
                idxMin1 = i;
            } else if (idxMin2 == -1 || nosAtivos[i]->frequencia < nosAtivos[idxMin2]->frequencia) {
                idxMin2 = i;
            }
        }

        NoHuffman* esq = nosAtivos[idxMin1];
        NoHuffman* dir = nosAtivos[idxMin2];
        NoHuffman* pai = new NoHuffman('\0', esq->frequencia + dir->frequencia);
        pai->filhoEsq = esq;
        pai->filhoDir = dir;

        int idxRemover1 = max(idxMin1, idxMin2);
        int idxRemover2 = min(idxMin1, idxMin2);

        nosAtivos[idxRemover1] = nosAtivos[qtdNosAtivos - 1];
        qtdNosAtivos--;
        nosAtivos[idxRemover2] = nosAtivos[qtdNosAtivos - 1];
        qtdNosAtivos--;

        nosAtivos[qtdNosAtivos++] = pai;
    }

    NoHuffman* raiz = (qtdNosAtivos > 0) ? nosAtivos[0] : nullptr;

    string codigosHuffman[256];
    construirCodigos(raiz, "", codigosHuffman);

    string textoCodificado;
    for (char c : texto) {
        textoCodificado += codigosHuffman[static_cast<unsigned char>(c)];
    }

    GerenciadorArquivo arqSaida(arquivoSaida, "wb");
    if (!arqSaida.estaAberto()) {
        cerr << "Erro: Nao foi possivel criar o arquivo de saida: " << arquivoSaida << endl;
        deletarArvore(raiz);
        return;
    }

    int charsUnicos = 0;
    for (int i = 0; i < 256; i++) {
        if (frequencias[i] > 0) charsUnicos++;
    }

    arqSaida.escrever(&charsUnicos, sizeof(int), 1);

    for (int i = 0; i < 256; i++) {
        if (frequencias[i] > 0) {
            char ch = static_cast<char>(i);
            arqSaida.escrever(&ch, sizeof(char), 1);
            arqSaida.escrever(&frequencias[i], sizeof(int), 1);
        }
    }

    int tamCodificado = textoCodificado.size();
    arqSaida.escrever(&tamCodificado, sizeof(int), 1);

    unsigned char byteAtual = 0;
    int qtdBits = 0;

    for (char bit : textoCodificado) {
        byteAtual <<= 1;
        if (bit == '1') {
            byteAtual |= 1;
        }

        qtdBits++;
        if (qtdBits == 8) {
            arqSaida.escrever(&byteAtual, 1, 1);
            byteAtual = 0;
            qtdBits = 0;
        }
    }

    if (qtdBits > 0) {
        byteAtual <<= (8 - qtdBits);
        arqSaida.escrever(&byteAtual, 1, 1);
    }

    arqSaida.fechar();
    deletarArvore(raiz);

    cout << "Compactacao Huffman concluida!\n";
}

void Huffman::descomprimir(const string& arquivoEntrada, const string& arquivoSaida) {
    GerenciadorArquivo arqEntrada(arquivoEntrada, "rb");
    if (!arqEntrada.estaAberto()) {
        cerr << "Erro: Nao foi possivel abrir o arquivo compactado: " << arquivoEntrada << endl;
        return;
    }

    int charsUnicos = 0;
    if (arqEntrada.ler(&charsUnicos, sizeof(int), 1) != 1) {
        charsUnicos = 0;
    }

    int frequencias[256] = {0};
    int tamTextoOriginal = 0;

    for (int i = 0; i < charsUnicos; i++) {
        char c;
        int freq;

        arqEntrada.ler(&c, sizeof(char), 1);
        arqEntrada.ler(&freq, sizeof(int), 1);

        frequencias[static_cast<unsigned char>(c)] = freq;
        tamTextoOriginal += freq;
    }

    if (tamTextoOriginal == 0) {
        cerr << "Aviso: Arquivo de entrada sem dados de frequencia validos.\n";
        arqEntrada.fechar();
        return;
    }

    int qtdNosAtivos = 0;
    NoHuffman* nosAtivos[256];
    for (int i = 0; i < 256; i++) {
        if (frequencias[i] > 0) {
            nosAtivos[qtdNosAtivos++] = new NoHuffman(static_cast<char>(i), frequencias[i]);
        }
    }

    while (qtdNosAtivos > 1) {
        int idxMin1 = -1;
        int idxMin2 = -1;

        for (int i = 0; i < qtdNosAtivos; i++) {
            if (idxMin1 == -1 || nosAtivos[i]->frequencia < nosAtivos[idxMin1]->frequencia) {
                idxMin2 = idxMin1;
                idxMin1 = i;
            } else if (idxMin2 == -1 || nosAtivos[i]->frequencia < nosAtivos[idxMin2]->frequencia) {
                idxMin2 = i;
            }
        }

        NoHuffman* esq = nosAtivos[idxMin1];
        NoHuffman* dir = nosAtivos[idxMin2];
        NoHuffman* pai = new NoHuffman('\0', esq->frequencia + dir->frequencia);
        pai->filhoEsq = esq;
        pai->filhoDir = dir;

        int idxRemover1 = max(idxMin1, idxMin2);
        int idxRemover2 = min(idxMin1, idxMin2);

        nosAtivos[idxRemover1] = nosAtivos[qtdNosAtivos - 1];
        qtdNosAtivos--;
        nosAtivos[idxRemover2] = nosAtivos[qtdNosAtivos - 1];
        qtdNosAtivos--;

        nosAtivos[qtdNosAtivos++] = pai;
    }

    NoHuffman* raiz = (qtdNosAtivos > 0) ? nosAtivos[0] : nullptr;

    int tamCodificado = 0;
    arqEntrada.ler(&tamCodificado, sizeof(int), 1);

    string bitsCodificados;
    unsigned char byteLido;

    while (arqEntrada.ler(&byteLido, 1, 1) == 1) {
        for (int i = 7; i >= 0; i--) {
            char bit = ((byteLido >> i) & 1) ? '1' : '0';
            bitsCodificados += bit;
        }
    }
    arqEntrada.fechar();

    if (bitsCodificados.size() > static_cast<size_t>(tamCodificado)) {
        bitsCodificados = bitsCodificados.substr(0, tamCodificado);
    }

    string textoDecodificado;
    textoDecodificado.reserve(tamTextoOriginal);

    NoHuffman* noAtual = raiz;
    for (char bit : bitsCodificados) {
        if (textoDecodificado.size() >= static_cast<size_t>(tamTextoOriginal)) {
            break;
        }

        noAtual = (bit == '0') ? noAtual->filhoEsq : noAtual->filhoDir;

        if (!noAtual->filhoEsq && !noAtual->filhoDir) {
            textoDecodificado += noAtual->caractere;
            noAtual = raiz;
        }
    }

    deletarArvore(raiz);

    GerenciadorArquivo arqSaida(arquivoSaida, "wb");
    if (!arqSaida.estaAberto()) {
        cerr << "Erro: Nao foi possivel criar o arquivo de saida descompactado: " << arquivoSaida << endl;
        return;
    }

    arqSaida.escrever(textoDecodificado.c_str(), 1, textoDecodificado.size());
    arqSaida.fechar();

    cout << "Descompactacao Huffman concluida.\n";
}