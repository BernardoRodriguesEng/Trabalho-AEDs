#ifndef LIBRARYDAO_H
#define LIBRARYDAO_H

#include "Library.h"
#include "HashExtensivel.h"
#include <vector>
#include <fstream>

class LibraryDAO {
private:
    string filename;
    HashExtensivel userIndex; // idUser -> offsets
    HashExtensivel gameIndex; // idGame -> offsets

    void reconstruirIndices();

public:
    LibraryDAO(string filename);
    bool adicionar(LibraryEntry& entry);
    bool remover(int idUser, int idGame);
    
    // Integridade Referencial (Cascata)
    int removerPorUsuario(int idUser);
    int removerPorJogo(int idGame);
    
    vector<LibraryEntry> buscarPorUsuario(int idUser);
    vector<LibraryEntry> buscarPorJogo(int idGame);
};

#endif
