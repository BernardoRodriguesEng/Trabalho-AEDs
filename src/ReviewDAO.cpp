#include "../include/ReviewDAO.h"
#include <fstream>

using namespace std;

ReviewDAO::ReviewDAO(string nomeArquivo)
    : nomeArquivo(nomeArquivo), hash(3) {
    ultimoID = 0;
}

void ReviewDAO::adicionarIndice(int idJogo, long offset) {
    ofstream index("review_index.bin", ios::binary | ios::app);

    index.write(reinterpret_cast<char*>(&idJogo), sizeof(int));
    index.write(reinterpret_cast<char*>(&offset), sizeof(long));

    index.close();
}

// ---------------- CREATE ----------------
void ReviewDAO::criar(Review& r) {
    fstream file(nomeArquivo, ios::binary | ios::app);

    long offset = file.tellp();

    ultimoID++;
    r.idReview = ultimoID;
    r.setAtivo(true);

    int size = r.getTamanho();
    char* buffer = new char[size];

    r.serializar(buffer);

    file.write(reinterpret_cast<char*>(&size), sizeof(int));
    file.write(buffer, size);

    delete[] buffer;
    file.close();

    adicionarIndice(r.idJogo, offset);

    hash.inserir(r.idReview, offset);
}

vector<Review> ReviewDAO::buscarPorJogo(int idJogo) {
    vector<Review> lista;

    ifstream index("review_index.bin", ios::binary);
    if (!index) return lista;

    int id;
    long offset;

    while (index.read(reinterpret_cast<char*>(&id), sizeof(int))) {
        index.read(reinterpret_cast<char*>(&offset), sizeof(long));

        if (id == idJogo) {
            ifstream file(nomeArquivo, ios::binary);

            file.seekg(offset);

            int size;
            file.read(reinterpret_cast<char*>(&size), sizeof(int));

            char* buffer = new char[size];
            file.read(buffer, size);

            Review r;
            r.desserializar(buffer);

            if (r.isAtivo())
                lista.push_back(r);

            delete[] buffer;
            file.close();
        }
    }

    index.close();
    return lista;
}

bool ReviewDAO::buscarPorId(int idReview, Review& r) {
    long offset;

    if (!hash.buscar(idReview, offset))
        return false;

    ifstream file(nomeArquivo, ios::binary);
    if (!file) return false;

    file.seekg(offset);

    int size;
    file.read(reinterpret_cast<char*>(&size), sizeof(int));

    char* buffer = new char[size];
    file.read(buffer, size);

    r.desserializar(buffer);

    delete[] buffer;
    file.close();

    return r.isAtivo();
}

bool ReviewDAO::atualizar(int idReview, const Review& novo) {
    long offset;

    if (!hash.buscar(idReview, offset))
        return false;
    hash.remover(idReview);

    fstream file(nomeArquivo, ios::binary | ios::in | ios::out);
    if (!file)
        return false;
    file.seekp(offset);
    char lapide = '*';
    file.write(reinterpret_cast<char*>(&lapide), sizeof(char));
    file.close();

    Review r = novo;
    r.idReview = idReview;
    r.setAtivo(true);

    fstream out(nomeArquivo, ios::binary | ios::app);
    long newOffset = out.tellp();

    int size = r.getTamanho();
    char* buffer = new char[size];

    r.serializar(buffer);

    out.write(reinterpret_cast<char*>(&size), sizeof(int));
    out.write(buffer, size);

    delete[] buffer;
    out.close();

    hash.inserir(idReview, newOffset);

    return true;
}

bool ReviewDAO::remover(int idReview) {
    long offset;

    if (!hash.buscar(idReview, offset))
        return false;

    // remove do hash (CORREÇÃO IMPORTANTE)
    hash.remover(idReview);

    fstream file(nomeArquivo, ios::binary | ios::in | ios::out);
    if (!file)
        return false;

    file.seekp(offset);
    char lapide = '*';
    file.write(reinterpret_cast<char*>(&lapide), sizeof(char));

    file.close();

    return true;
}