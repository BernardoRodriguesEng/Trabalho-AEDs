#include "../include/ReviewDAO.h"
#include <fstream>

using namespace std;

ReviewDAO::ReviewDAO(string nomeArquivo)
    : nomeArquivo(nomeArquivo), hash(10), hashJogo(10) {
    ultimoID = 0;
    reconstruirHashes();
}

void ReviewDAO::reconstruirHashes() {
    ifstream file(nomeArquivo, ios::binary);
    if (!file) return;

    while (file.peek() != EOF) {
        long offset = file.tellg();
        int size;
        if (!file.read(reinterpret_cast<char*>(&size), sizeof(int))) break;

        char* buffer = new char[size];
        file.read(buffer, size);

        Review r;
        r.desserializar(buffer);

        if (r.isAtivo()) {
            hash.inserir(r.idReview, offset);
            hashJogo.inserir(r.idJogo, offset);
            if (r.idReview > ultimoID) ultimoID = r.idReview;
        }

        delete[] buffer;
    }
    file.close();
}

void ReviewDAO::adicionarIndice(int idJogo, long offset) {
    // Indexador legado, agora usando HashJogo
}

// ---------------- CREATE ----------------
void ReviewDAO::criar(Review& r) {
    fstream file(nomeArquivo, ios::binary | ios::app);
    if (!file) {
        file.open(nomeArquivo, ios::binary | ios::out);
        file.close();
        file.open(nomeArquivo, ios::binary | ios::app);
    }

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

    hash.inserir(r.idReview, offset);
    hashJogo.inserir(r.idJogo, offset);
}

vector<Review> ReviewDAO::buscarPorJogo(int idJogo) {
    vector<Review> lista;
    vector<long> offsets = hashJogo.buscarTodos(idJogo);

    if (offsets.empty()) return lista;

    ifstream file(nomeArquivo, ios::binary);
    if (!file) return lista;

    for (long offset : offsets) {
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
    }

    file.close();
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

    // Remove do hash
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