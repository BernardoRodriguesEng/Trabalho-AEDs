#include "../../include/CasamentoPadroes/KMP.h"

using namespace std;

vector<int> KMP::buildLPS(const string& pattern) {
    int m = pattern.length();
    vector<int> lps(m, 0);
    int len = 0;
    int i = 1;

    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

int KMP::buscar(const string& texto, const string& padrao) {
    int n = texto.length();
    int m = padrao.length();

    if (m == 0) return 0;
    if (n == 0 || m > n) return -1;

    vector<int> lps = buildLPS(padrao);

    int i = 0; // índice para texto
    int j = 0; // índice para padrao

    while (i < n) {
        if (padrao[j] == texto[i]) {
            j++;
            i++;
        }

        if (j == m) {
            return i - j; // Encontrou o padrão
        } else if (i < n && padrao[j] != texto[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }

    return -1; // Não encontrou
}
