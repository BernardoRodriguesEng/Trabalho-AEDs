#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>
#include <cstring>
#include <ctime>

using namespace std;

class LibraryEntry {
public:
    char lapide;
    int idUser;
    int idGame;
    long dataAquisicao; // Timestamp
    int tempoJogado; // Em minutos

    LibraryEntry() : lapide(' '), idUser(-1), idGame(-1), tempoJogado(0) {
        dataAquisicao = (long)time(NULL);
    }

    void setAtivo(bool ativo) { lapide = ativo ? ' ' : '*'; }
    bool isAtivo() const { return lapide == ' '; }

    // Tamanho fixo para facilitar (ou variável se preferir)
    // Vamos usar fixo para a tabela intermediária (mais rápido)
    static int getTamanho() {
        return sizeof(char) + (sizeof(int) * 3) + sizeof(long);
    }

    void serializar(char* buffer) const {
        int pos = 0;
        memcpy(buffer + pos, &lapide, sizeof(char)); pos += sizeof(char);
        memcpy(buffer + pos, &idUser, sizeof(int)); pos += sizeof(int);
        memcpy(buffer + pos, &idGame, sizeof(int)); pos += sizeof(int);
        memcpy(buffer + pos, &dataAquisicao, sizeof(long)); pos += sizeof(long);
        memcpy(buffer + pos, &tempoJogado, sizeof(int)); pos += sizeof(int);
    }

    void desserializar(const char* buffer) {
        int pos = 0;
        memcpy(&lapide, buffer + pos, sizeof(char)); pos += sizeof(char);
        memcpy(&idUser, buffer + pos, sizeof(int)); pos += sizeof(int);
        memcpy(&idGame, buffer + pos, sizeof(int)); pos += sizeof(int);
        memcpy(&dataAquisicao, buffer + pos, sizeof(long)); pos += sizeof(long);
        memcpy(&tempoJogado, buffer + pos, sizeof(int)); pos += sizeof(int);
    }
};

#endif
