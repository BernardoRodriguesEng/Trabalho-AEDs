#include "../../include/Compressao/GerenciadorArquivo.h"

GerenciadorArquivo::GerenciadorArquivo(const std::string& nomeArquivo, const char* modo) {
    ponteiroArq = fopen(nomeArquivo.c_str(), modo);
}

GerenciadorArquivo::~GerenciadorArquivo() {
    fechar();
}

bool GerenciadorArquivo::estaAberto() const {
    return ponteiroArq != nullptr;
}

void GerenciadorArquivo::fechar() {
    if (ponteiroArq) {
        fclose(ponteiroArq);
        ponteiroArq = nullptr;
    }
}

std::string GerenciadorArquivo::lerTudo() {
    if (!ponteiroArq) return "";

    fseek(ponteiroArq, 0, SEEK_END);
    long tamanhoArquivo = ftell(ponteiroArq);
    fseek(ponteiroArq, 0, SEEK_SET);

    std::string dadosArquivo;
    if (tamanhoArquivo > 0) {
        char* buffer = new char[tamanhoArquivo];
        size_t bytesLidos = fread(buffer, 1, tamanhoArquivo, ponteiroArq);
        dadosArquivo.assign(buffer, bytesLidos);
        delete[] buffer;
    }
    return dadosArquivo;
}

size_t GerenciadorArquivo::escrever(const void* dados, size_t tamanho, size_t quantidade) {
    if (!ponteiroArq) return 0;
    return fwrite(dados, tamanho, quantidade, ponteiroArq);
}

size_t GerenciadorArquivo::ler(void* dados, size_t tamanho, size_t quantidade) {
    if (!ponteiroArq) return 0;
    return fread(dados, tamanho, quantidade, ponteiroArq);
}
