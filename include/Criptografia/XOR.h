#ifndef XOR_H
#define XOR_H

#include <string>

class XOR {
public:
    // Aplica o XOR para criptografar (gera a chave se não existir ou se for um novo processo)
    // Retorna true em caso de sucesso.
    static bool criptografar(const std::string& arquivoEntrada, const std::string& arquivoSaida, const std::string& arquivoChave);

    // Aplica o XOR para descriptografar (precisa da chave existente)
    static bool descriptografar(const std::string& arquivoEntrada, const std::string& arquivoSaida, const std::string& arquivoChave);
};

#endif
