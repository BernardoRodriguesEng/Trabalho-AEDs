#include "MotorDeTestes.h"
#include "../include/OrdenacaoExterna.h"
#include "../include/Game.h"
#include <fstream>
#include <cstdio>

void rodarTestesOrdenacao() {
    INICIAR_SUITE("Testes Ordenacao Externa");

    std::string arqDesordenado = "teste_desordenado.bin";
    std::string arqOrdenado = "teste_ordenado.bin";

    std::remove(arqDesordenado.c_str());
    std::remove(arqOrdenado.c_str());

    Game g1, g2, g3;
    g1.name = "Zebra Game";
    g2.name = "Apple Game";
    g3.name = "Monkey Game";

    std::ofstream out(arqDesordenado, std::ios::binary);
    int dummyHeader = 3;
    out.write((char*)&dummyHeader, sizeof(int));

    int t1 = g1.getSerializationSize();
    int t2 = g2.getSerializationSize();
    int t3 = g3.getSerializationSize();

    char* b1 = new char[t1]; g1.serialize(b1);
    char* b2 = new char[t2]; g2.serialize(b2);
    char* b3 = new char[t3]; g3.serialize(b3);

    out.write(b1, t1);
    out.write(b2, t2);
    out.write(b3, t3);
    
    delete[] b1; delete[] b2; delete[] b3;
    out.close();

    OrdenacaoExterna::ordenarPorNome(arqDesordenado, arqOrdenado);

    std::ifstream in(arqOrdenado, std::ios::binary);
    TESTAR_VERDADEIRO(in.is_open(), "Arquivo ordenado deve ter sido gerado");

    if (in.is_open()) {
        Game r1; r1.readFromStream(in);
        Game r2; r2.readFromStream(in);
        Game r3; r3.readFromStream(in);

        TESTAR_IGUAL(r1.name, std::string("Apple Game"), "Primeiro jogo deve ser Apple");
        TESTAR_IGUAL(r2.name, std::string("Monkey Game"), "Segundo jogo deve ser Monkey");
        TESTAR_IGUAL(r3.name, std::string("Zebra Game"), "Terceiro jogo deve ser Zebra");

        in.close();
    }

    std::remove(arqDesordenado.c_str());
    std::remove(arqOrdenado.c_str());
}
