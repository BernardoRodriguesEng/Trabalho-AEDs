#include "../include/UserDAO.h"
#include <ctime>

UserDAO::UserDAO(string filename) : filename(filename), index(4) {
    ifstream file(filename);
    if (!file.is_open()) {
        ofstream create(filename, ios::binary);
        create.close();
    } else {
        file.close();
        reconstruirHash();
    }
}

void UserDAO::reconstruirHash() {
    ifstream file(filename, ios::binary);
    while (file.peek() != EOF) {
        long offset = file.tellg();
        int tam;
        file.read((char*)&tam, sizeof(int));
        char* buffer = new char[tam];
        file.read(buffer, tam);
        
        User u;
        u.desserializar(buffer);
        if (u.isAtivo()) {
            index.inserir(u.idUser, offset);
        }
        delete[] buffer;
    }
    file.close();
}

bool UserDAO::criar(User& user) {
    if (user.idUser == -1) {
        // Logica simples de ID incremental (poderia ser melhorada)
        user.idUser = (int)time(NULL) % 1000000;
    }
    
    ofstream file(filename, ios::binary | ios::app);
    long offset = file.tellp();
    int tam = user.getTamanho();
    file.write((char*)&tam, sizeof(int));
    char* buffer = new char[tam];
    user.serializar(buffer);
    file.write(buffer, tam);
    delete[] buffer;
    file.close();
    index.inserir(user.idUser, offset);
    return true;
}

User UserDAO::ler(int id) {
    long offset;
    if (!index.buscar(id, offset)) return User();
    
    ifstream file(filename, ios::binary);
    file.seekg(offset, ios::beg);
    int tam;
    file.read((char*)&tam, sizeof(int));
    char* buffer = new char[tam];
    file.read(buffer, tam);
    User u;
    u.desserializar(buffer);
    delete[] buffer;
    file.close();
    return u;
}

bool UserDAO::deletar(int id) {
    long offset;
    if (!index.buscar(id, offset)) return false;
    
    fstream file(filename, ios::binary | ios::in | ios::out);
    file.seekp(offset + sizeof(int), ios::beg); // Pula o campo 'tamanho'
    char lapide = '*';
    file.write(&lapide, sizeof(char));
    file.close();
    
    return index.remover(id);
}

vector<User> UserDAO::listarTodos() {
    vector<User> users;
    ifstream file(filename, ios::binary);
    while (file.peek() != EOF) {
        int tam;
        file.read((char*)&tam, sizeof(int));
        char* buffer = new char[tam];
        file.read(buffer, tam);
        User u;
        u.desserializar(buffer);
        if (u.isAtivo()) users.push_back(u);
        delete[] buffer;
    }
    file.close();
    return users;
}
