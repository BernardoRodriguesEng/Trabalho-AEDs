#include "MotorDeTestes.h"
#include "../include/Criptografia/XOR.h"
#include <fstream>
#include <string>

void rodarTestesCriptografia() {
    INICIAR_SUITE("Testes de Criptografia (XOR)");

    std::string originalStr = "Este e um arquivo ultra secreto com senhas: 12345.";
    std::string originalFile = "teste_cripto_original.txt";
    std::string encryptedFile = "teste_cripto_enc.bin";
    std::string decryptedFile = "teste_cripto_restored.txt";
    std::string keyFile = "teste_cripto_chave.key";

    // Criar arquivo original
    std::ofstream out(originalFile, std::ios::binary);
    out << originalStr;
    out.close();

    // Testar Criptografia
    bool encOk = XOR::criptografar(originalFile, encryptedFile, keyFile);
    TESTAR_IGUAL(encOk, true, "Criptografia XOR deve retornar true (sucesso)");

    // Testar Descriptografia
    bool decOk = XOR::descriptografar(encryptedFile, decryptedFile, keyFile);
    TESTAR_IGUAL(decOk, true, "Descriptografia XOR deve retornar true (sucesso)");

    // Verificar integridade do dado restaurado
    std::ifstream inRestored(decryptedFile, std::ios::binary);
    std::string resultRestored((std::istreambuf_iterator<char>(inRestored)), std::istreambuf_iterator<char>());
    inRestored.close();

    TESTAR_IGUAL(resultRestored, originalStr, "Restauracao XOR deve ser perfeitamente identica ao original");

    // Limpar arquivos temporários
    std::remove(originalFile.c_str());
    std::remove(encryptedFile.c_str());
    std::remove(decryptedFile.c_str());
    std::remove(keyFile.c_str());
}
