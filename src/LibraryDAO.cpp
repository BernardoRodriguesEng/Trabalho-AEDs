#include "../include/LibraryDAO.h"

LibraryDAO::LibraryDAO(string filename) : 
    filename(filename), 
    userIndex(4), 
    gameIndex(4) 
{
    ifstream file(filename);
    if (!file.is_open()) {
        ofstream create(filename, ios::binary);
        create.close();
    } else {
        file.close();
        reconstruirIndices();
    }
}

void LibraryDAO::reconstruirIndices() {
    ifstream file(filename, ios::binary);
    int tam = LibraryEntry::getTamanho();
    char* buffer = new char[tam];
    
    while (file.read(buffer, tam)) {
        long offset = (long)file.tellg() - tam;
        LibraryEntry e;
        e.desserializar(buffer);
        if (e.isAtivo()) {
            userIndex.inserir(e.idUser, offset);
            gameIndex.inserir(e.idGame, offset);
        }
    }
    delete[] buffer;
    file.close();
}

bool LibraryDAO::adicionar(LibraryEntry& entry) {
    ofstream file(filename, ios::binary | ios::app);
    long offset = file.tellp();
    int tam = LibraryEntry::getTamanho();
    char* buffer = new char[tam];
    entry.serializar(buffer);
    file.write(buffer, tam);
    delete[] buffer;
    file.close();
    
    userIndex.inserir(entry.idUser, offset);
    gameIndex.inserir(entry.idGame, offset);
    return true;
}

bool LibraryDAO::remover(int idUser, int idGame) {
    auto offsets = userIndex.buscarTodos(idUser);
    fstream file(filename, ios::binary | ios::in | ios::out);
    int tam = LibraryEntry::getTamanho();
    char* buffer = new char[tam];
    bool removido = false;

    for (long offset : offsets) {
        file.seekg(offset, ios::beg);
        file.read(buffer, tam);
        LibraryEntry e;
        e.desserializar(buffer);
        
        if (e.idGame == idGame && e.isAtivo()) {
            e.setAtivo(false);
            e.serializar(buffer);
            file.seekp(offset, ios::beg);
            file.write(buffer, tam);
            
            userIndex.removerPorOffset(idUser, offset);
            gameIndex.removerPorOffset(idGame, offset);
            removido = true;
            break;
        }
    }
    delete[] buffer;
    file.close();
    return removido;
}

vector<LibraryEntry> LibraryDAO::buscarPorUsuario(int idUser) {
    auto offsets = userIndex.buscarTodos(idUser);
    vector<LibraryEntry> res;
    ifstream file(filename, ios::binary);
    int tam = LibraryEntry::getTamanho();
    char* buffer = new char[tam];

    for (long offset : offsets) {
        file.seekg(offset, ios::beg);
        file.read(buffer, tam);
        LibraryEntry e;
        e.desserializar(buffer);
        if(e.isAtivo()) res.push_back(e);
    }
    delete[] buffer;
    return res;
}

vector<LibraryEntry> LibraryDAO::buscarPorJogo(int idGame) {
    auto offsets = gameIndex.buscarTodos(idGame);
    vector<LibraryEntry> res;
    ifstream file(filename, ios::binary);
    int tam = LibraryEntry::getTamanho();
    char* buffer = new char[tam];

    for (long offset : offsets) {
        file.seekg(offset, ios::beg);
        file.read(buffer, tam);
        LibraryEntry e;
        e.desserializar(buffer);
        if(e.isAtivo()) res.push_back(e);
    }
    delete[] buffer;
    return res;
}

int LibraryDAO::removerPorUsuario(int idUser) {
    auto offsets = userIndex.buscarTodos(idUser);
    fstream file(filename, ios::binary | ios::in | ios::out);
    int tam = LibraryEntry::getTamanho();
    char* buffer = new char[tam];
    int count = 0;

    for (long offset : offsets) {
        file.seekg(offset, ios::beg);
        file.read(buffer, tam);
        LibraryEntry e;
        e.desserializar(buffer);
        
        if (e.isAtivo()) {
            e.setAtivo(false);
            e.serializar(buffer);
            file.seekp(offset, ios::beg);
            file.write(buffer, tam);
            
            userIndex.removerPorOffset(idUser, offset);
            gameIndex.removerPorOffset(e.idGame, offset);
            count++;
        }
    }
    
    delete[] buffer;
    file.close();
    return count;
}

int LibraryDAO::removerPorJogo(int idGame) {
    auto offsets = gameIndex.buscarTodos(idGame);
    fstream file(filename, ios::binary | ios::in | ios::out);
    int tam = LibraryEntry::getTamanho();
    char* buffer = new char[tam];
    int count = 0;

    for (long offset : offsets) {
        file.seekg(offset, ios::beg);
        file.read(buffer, tam);
        LibraryEntry e;
        e.desserializar(buffer);
        
        if (e.isAtivo()) {
            e.setAtivo(false);
            e.serializar(buffer);
            file.seekp(offset, ios::beg);
            file.write(buffer, tam);
            
            userIndex.removerPorOffset(e.idUser, offset);
            gameIndex.removerPorOffset(idGame, offset);
            count++;
        }
    }
    
    delete[] buffer;
    file.close();
    return count;
}
