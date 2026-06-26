#ifndef BOYERMOORE_H
#define BOYERMOORE_H

#include <string>
#include <vector>

class BoyerMoore {
private:
    static const int ALPHABET_SIZE = 256;
    static std::vector<int> buildBadCharTable(const std::string& pattern);

public:
    /**
     * Busca o padrão no texto fornecido usando Boyer-Moore (Heurística de Bad Character).
     * @param texto A string original onde a busca ocorrerá.
     * @param padrao O padrão que deseja encontrar.
     * @return O índice (0-based) onde o padrão se inicia no texto, ou -1 se não encontrado.
     */
    static int buscar(const std::string& texto, const std::string& padrao);
};

#endif // BOYERMOORE_H
