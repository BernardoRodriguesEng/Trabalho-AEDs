#include "MotorDeTestes.h"
#include "../include/Game.h"
#include <string.h>

void rodarTestesJogo() {
    INICIAR_SUITE("Testes Jogo (Entidade Game)");

    Game g1;
    g1.appid = 1234;
    g1.name = "Jogo Teste";
    g1.developer = "Dev Legal";
    g1.price = 19.99f;
    g1.achievements = 10;
    g1.genres.push_back("Acao");
    g1.genres.push_back("Aventura");

    int tamanho = g1.getSerializationSize();
    TESTAR_VERDADEIRO(tamanho > 0, "Tamanho da serializacao deve ser maior que 0");

    char* buffer = new char[tamanho];
    g1.serialize(buffer);

    Game g2;
    g2.deserialize(buffer);

    TESTAR_IGUAL(g2.appid, 1234, "AppID deve ser igual apos desserializacao");
    TESTAR_IGUAL(g2.name, std::string("Jogo Teste"), "Nome deve ser igual");
    TESTAR_IGUAL(g2.developer, std::string("Dev Legal"), "Developer deve ser igual");
    TESTAR_IGUAL(g2.genres.size(), 2, "Quantidade de generos deve ser a mesma");
    if(g2.genres.size() == 2) {
        TESTAR_IGUAL(g2.genres[0], std::string("Acao"), "Genero 1 correto");
    }

    delete[] buffer;
}
