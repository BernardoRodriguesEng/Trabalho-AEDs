#include "../../include/CasamentoPadroes/BoyerMoore.h"
#include <algorithm>

using namespace std;

vector<int> BoyerMoore::buildBadCharTable(const string& pattern) {
    int m = pattern.length();
    vector<int> badChar(ALPHABET_SIZE, -1);

    for (int i = 0; i < m; i++) {
        badChar[(int)(unsigned char)pattern[i]] = i;
    }

    return badChar;
}

int BoyerMoore::buscar(const string& texto, const string& padrao) {
    int n = texto.length();
    int m = padrao.length();

    if (m == 0) return 0;
    if (n == 0 || m > n) return -1;

    vector<int> badChar = buildBadCharTable(padrao);

    int s = 0; // Deslocamento do padrão em relação ao texto

    while (s <= (n - m)) {
        int j = m - 1;

        // Diminui o j enquanto os caracteres do padrão casarem com o texto
        while (j >= 0 && padrao[j] == texto[s + j]) {
            j--;
        }

        if (j < 0) {
            return s; // Encontrou o padrão
            // Se quisesse buscar todas as ocorrências:
            // s += (s + m < n) ? m - badChar[texto[s + m]] : 1;
        } else {
            // Desloca o padrão para alinhar com o bad character no texto
            s += max(1, j - badChar[(int)(unsigned char)texto[s + j]]);
        }
    }

    return -1; // Não encontrou
}
