#ifndef LZW_H
#define LZW_H

#include <string>

class LZW {
public:
    static void comprimir(const std::string& arquivoEntrada, const std::string& arquivoSaida);
    static void descomprimir(const std::string& arquivoEntrada, const std::string& arquivoSaida);
};

#endif