#ifndef GERENCIADOR_ARQUIVO_H
#define GERENCIADOR_ARQUIVO_H

#include <string>
#include <cstdio>

class GerenciadorArquivo {
private:
    FILE* ponteiroArq;

public:
    GerenciadorArquivo(const std::string& nomeArquivo, const char* modo);
    ~GerenciadorArquivo();

    bool estaAberto() const;
    std::string lerTudo();
    size_t escrever(const void* dados, size_t tamanho, size_t quantidade);
    size_t ler(void* dados, size_t tamanho, size_t quantidade);
    void fechar();
};

#endif
