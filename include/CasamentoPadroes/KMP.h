#ifndef KMP_H
#define KMP_H

#include <string>
#include <vector>

class KMP {
private:
    static std::vector<int> buildLPS(const std::string& pattern);

public:
    /**
     * Busca o padrão no texto fornecido.
     * @param texto A string original onde a busca ocorrerá.
     * @param padrao O padrão que deseja encontrar.
     * @return O índice (0-based) onde o padrão se inicia no texto, ou -1 se não encontrado.
     */
    static int buscar(const std::string& texto, const std::string& padrao);
};

#endif // KMP_H
