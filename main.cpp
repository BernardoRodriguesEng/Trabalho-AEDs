#include <iostream>
#include "transformar-binario.cpp"

using namespace std;

int main() {
    ler_arquivo_csv leitor;
    cout << "Iniciando conversao..." << endl;
    leitor.converter_csv_binario();
    cout << "Conversao concluida com sucesso!" << endl;
    return 0;
}
