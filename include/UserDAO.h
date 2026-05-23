#ifndef USERDAO_H
#define USERDAO_H

#include "User.h"
#include "HashExtensivel.h"
#include <fstream>
#include <vector>

class UserDAO {
private:
    string filename;
    HashExtensivel index;

    void reconstruirHash();

public:
    UserDAO(string filename);
    bool criar(User& user);
    User ler(int id);
    bool atualizar(const User& user);
    bool deletar(int id);
    vector<User> listarTodos();
};

#endif
