#ifndef REVIEWDAO_H
#define REVIEWDAO_H

#include "Review.h"
#include "HashExtensivel.h"
#include <vector>
#include <string>

using namespace std;

class ReviewDAO {
private:
    string nomeArquivo;
    int ultimoID;

    HashExtensivel hash;
    HashExtensivel hashJogo;

    void reconstruirHashes();
    void adicionarIndice(int idJogo, long offset);

public:
    ReviewDAO(string nomeArquivo);

    void criar(Review& r);
    vector<Review> buscarPorJogo(int idJogo);
    bool buscarPorId(int idReview, Review& r);

    bool atualizar(int idReview, const Review& r);
    bool remover(int idReview);
};

#endif