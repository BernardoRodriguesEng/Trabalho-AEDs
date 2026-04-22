#include "../include/Review.h"
#include <cstring>

using namespace std;

Review::Review() {
    lapide = ' ';
    idReview = 0;
    idJogo = 0;
    nota = 0;
}

void Review::setAtivo(bool ativo) {
    lapide = ativo ? ' ' : '*';
}

bool Review::isAtivo() const {
    return lapide == ' ';
}

int Review::getTamanho() const {
    return sizeof(char) + sizeof(int)*2 +
           sizeof(unsigned short) + usuario.length() +
           sizeof(unsigned short) + comentario.length() +
           sizeof(float);
}

void writeString(char*& ptr, const string& str) {
    unsigned short len = str.length();
    memcpy(ptr, &len, sizeof(unsigned short));
    ptr += sizeof(unsigned short);
    memcpy(ptr, str.c_str(), len);
    ptr += len;
}

void readString(const char*& ptr, string& str) {
    unsigned short len;
    memcpy(&len, ptr, sizeof(unsigned short));
    ptr += sizeof(unsigned short);

    str.assign(ptr, len);
    ptr += len;
}

void Review::serializar(char* buffer) const {
    char* ptr = buffer;

    memcpy(ptr, &lapide, sizeof(char)); ptr += sizeof(char);
    memcpy(ptr, &idReview, sizeof(int)); ptr += sizeof(int);
    memcpy(ptr, &idJogo, sizeof(int)); ptr += sizeof(int);

    writeString(ptr, usuario);
    writeString(ptr, comentario);

    memcpy(ptr, &nota, sizeof(float));
}

void Review::desserializar(const char* buffer) {
    const char* ptr = buffer;

    memcpy(&lapide, ptr, sizeof(char)); ptr += sizeof(char);
    memcpy(&idReview, ptr, sizeof(int)); ptr += sizeof(int);
    memcpy(&idJogo, ptr, sizeof(int)); ptr += sizeof(int);

    readString(ptr, usuario);
    readString(ptr, comentario);

    memcpy(&nota, ptr, sizeof(float));
}