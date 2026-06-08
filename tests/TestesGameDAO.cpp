#include "MotorDeTestes.h"
#include "../include/GameDAO.h"
#include "../include/Game.h"
#include <fstream>
#include <iostream>

using namespace std;

void rodarTestesGameDAO() {
    MotorDeTestes::getInstancia().iniciarSuite("Testes do GameDAO (CRUD e Indices)");

    // Limpar arquivos residuais antes de começar
    remove("test_dao.bin");
    remove("test_dao_index.bin");
    remove("price_index.bin");

    // 1. Instanciar o DAO para criar os arquivos e testar a inicialização dos índices
    {
        GameDAO dao("test_dao.bin");
        
        Game g1;
        g1.appid = 1001;
        g1.name = "Test Game 1";
        g1.price = 10.5f;
        g1.release_date = {2021, 5, 15};
        
        Game g2;
        g2.appid = 1002;
        g2.name = "Test Game 2";
        g2.price = 20.0f;
        g2.release_date = {2022, 10, 10};

        // Testar Create
        dao.create(g1);
        dao.create(g2);
        
        TESTAR_VERDADEIRO(true, "Insercao via GameDAO executada"); // Chegou até aqui sem crashar
    }

    // 2. Testar persistencia e reconstrução de índices abrindo um novo DAO
    {
        GameDAO dao2("test_dao.bin");
        
        // Buscar por AppID usando Hash (via DAO)
        Game achado;
        long pos = -1;
        bool result = dao2.searchById(1001, achado, pos);
        TESTAR_VERDADEIRO(result, "Read: Deve encontrar o jogo com AppID 1001");
        TESTAR_IGUAL(achado.name, std::string("Test Game 1"), "Read: O jogo encontrado deve ser 'Test Game 1'");
        TESTAR_IGUAL(achado.price, 10.5f, "Read: O preco do jogo encontrado deve ser 10.5");

        // Testar Update
        achado.price = 15.0f;
        achado.name = "Test Game 1 Updated";
        bool updated = dao2.update("Test Game 1", achado);
        TESTAR_VERDADEIRO(updated, "Update: A atualizacao deve retornar verdadeiro");

        Game atualizado;
        dao2.searchById(1001, atualizado, pos);
        TESTAR_IGUAL(atualizado.name, std::string("Test Game 1 Updated"), "Update: O nome deve estar atualizado");
        TESTAR_IGUAL(atualizado.price, 15.0f, "Update: O preco deve estar atualizado");

        // Buscar Todos por Nome (busca sequencial)
        vector<Game> games = dao2.searchAllByName("Test Game");
        TESTAR_IGUAL(games.size(), (size_t)2, "SearchByName: Deve retornar os 2 jogos contendo 'Test Game'");

        // Testar exclusão lógica
        bool deleted = dao2.remove("Test Game 2");
        TESTAR_VERDADEIRO(deleted, "Delete: A delecao deve retornar verdadeiro");

        Game deletado;
        bool foundDeleted = dao2.searchById(1002, deletado, pos);
        TESTAR_VERDADEIRO(!foundDeleted, "Delete: Um jogo deletado nao deve ser retornado na busca");
    }

    // Limpar arquivos após o teste
    remove("test_dao.bin");
    remove("test_dao_index.bin");
    remove("price_index.bin");
}
