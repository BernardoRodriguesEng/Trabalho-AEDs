#include <iostream>
#include <string>
#include <cassert>
#include "../include/CasamentoPadroes/KMP.h"
#include "../include/CasamentoPadroes/BoyerMoore.h"

using namespace std;

void rodarTestesCasamentoPadroes() {
    cout << "\n----------------------------------------\n";
    cout << "TESTES: CASAMENTO DE PADROES (KMP e BM)" << endl;
    cout << "----------------------------------------\n";

    string texto = "aabacaabxaaabmac";
    string padrao1 = "aabx";
    string padrao2 = "mac";
    string padrao3 = "nada";

    // KMP
    assert(KMP::buscar(texto, padrao1) == 5);
    assert(KMP::buscar(texto, padrao2) == 13);
    assert(KMP::buscar(texto, padrao3) == -1);
    cout << "KMP: Testes basicos OK!" << endl;

    // Boyer-Moore
    assert(BoyerMoore::buscar(texto, padrao1) == 5);
    assert(BoyerMoore::buscar(texto, padrao2) == 13);
    assert(BoyerMoore::buscar(texto, padrao3) == -1);
    cout << "Boyer-Moore: Testes basicos OK!" << endl;
    
    // Casos limite
    assert(KMP::buscar("", "a") == -1);
    assert(KMP::buscar("a", "") == 0);
    assert(BoyerMoore::buscar("", "a") == -1);
    assert(BoyerMoore::buscar("a", "") == 0);
    cout << "Casos Limite OK!" << endl;
}
