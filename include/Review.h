#ifndef REVIEW_H
#define REVIEW_H

#include <string>
using namespace std;

class Review {
public:
    char lapide;
    int idReview;
    int idJogo; // Chave estrangeira
    string usuario;
    string comentario;
    float nota; // 1 a 5

    Review();

    void setAtivo(bool ativo);
    bool isAtivo() const;

    int getTamanho() const;
    void serializar(char* buffer) const;
    void desserializar(const char* buffer);
};

#endif