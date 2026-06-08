#include "../external/httplib.h"
#include "MotorDeTestes.h"
#include "../include/GameController.h"
#include <thread>
#include <chrono>
#include <fstream>
#include <cstdio>

void rodarTestesServidor() {
    INICIAR_SUITE("Testes de Servidor HTTP");

    std::string binFile = "teste_server.bin";
    std::string usersFile = "teste_users.bin";
    std::string libraryFile = "teste_library.bin";

    std::ofstream o1(binFile), o2(usersFile), o3(libraryFile);
    o1.close(); o2.close(); o3.close();

    std::thread t([&]() {
        GameController controller(binFile, usersFile, libraryFile);
        controller.run(); 
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    httplib::Client cli("localhost", 8080);
    if (auto res = cli.Get("/api/users")) {
        TESTAR_IGUAL(res->status, 200, "Acesso a /api/users deve retornar 200 OK");
        TESTAR_VERDADEIRO(res->body.find("[") != std::string::npos, "Resposta deve conter array JSON");
    } else {
        TESTAR_VERDADEIRO(false, "Nao foi possivel conectar ao servidor de testes na porta 8080");
    }

    if (auto res = cli.Get("/api/shutdown")) {
        TESTAR_IGUAL(res->status, 200, "Desligamento via /api/shutdown deve retornar 200 OK");
    } else {
        TESTAR_VERDADEIRO(false, "Falha ao enviar comando de shutdown");
    }

    if (t.joinable()) {
        t.join();
    }

    std::remove(binFile.c_str());
    std::remove(usersFile.c_str());
    std::remove(libraryFile.c_str());
}
