#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#define ORDEM 100 // Ordem da Árvore B+ (m)

struct Nodo {
    bool folha;
    int n; // número de chaves atuais
    float chaves[ORDEM];
    long filhos_ou_offsets[ORDEM + 1]; // offsets para outros nodos ou para o arquivo de dados
    long proximo; // Apenas para nodos folha (lista ligada)
    long pai;

    Nodo() {
        folha = false;
        n = 0;
        proximo = -1;
        pai = -1;
        for (int i = 0; i < ORDEM; i++) {
            chaves[i] = -1;
            filhos_ou_offsets[i] = -1;
        }
        filhos_ou_offsets[ORDEM] = -1;
    }
};

class BPlusTree {
private:
    string filename;
    long raiz_offset;
    fstream* persistent_file;
    bool conexao_aberta;

    long lerRaiz();
    void gravarRaiz(long offset);
    Nodo lerNodo(long offset);
    void gravarNodo(long offset, const Nodo& nodo);
    long criarNodo(bool folha);

    // Funções auxiliares de inserção
    void inserirNaFolha(long folha_offset, float chave, long data_offset);
    void splitFolha(long folha_offset);
    void splitInterno(long interno_offset);
    void inserirNoPai(long filho_offset, float chave_subiu, long novo_filho_offset);

public:
    BPlusTree(string filename);
    ~BPlusTree();
    
    void abrirConexao();
    void fecharConexao();
    
    void inserir(float chave, long data_offset);
    long buscar(float chave);
    vector<long> buscarIntervalo(float min, float max); // Range Search
    
    void print(); // Para debug
};

#endif
