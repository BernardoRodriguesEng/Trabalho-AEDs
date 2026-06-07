#ifndef SAIDA_H
#define SAIDA_H

#include <string>

namespace CompressaoSaida {
    void lidarComInicializacao(const std::string& arquivoOriginal);
    void lidarComEncerramento(const std::string& arquivoOriginal);
}

#endif // SAIDA_H
