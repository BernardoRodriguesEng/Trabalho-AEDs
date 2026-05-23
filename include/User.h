#ifndef USER_H
#define USER_H

#include <string>
#include <cstring>
#include <vector>

using namespace std;

class User {
public:
    char lapide;
    int idUser;
    string nome;
    string email;

    User() : lapide(' '), idUser(-1) {}

    void setAtivo(bool ativo) { lapide = ativo ? ' ' : '*'; }
    bool isAtivo() const { return lapide == ' '; }

    // Serialização (Tamanho Variável)
    int getTamanho() const {
        return sizeof(char) + sizeof(int) + 
               sizeof(unsigned short) + nome.length() +
               sizeof(unsigned short) + email.length();
    }

    void serializar(char* buffer) const {
        int pos = 0;
        memcpy(buffer + pos, &lapide, sizeof(char)); pos += sizeof(char);
        memcpy(buffer + pos, &idUser, sizeof(int)); pos += sizeof(int);

        unsigned short tamNome = nome.length();
        memcpy(buffer + pos, &tamNome, sizeof(unsigned short)); pos += sizeof(unsigned short);
        memcpy(buffer + pos, nome.c_str(), tamNome); pos += tamNome;

        unsigned short tamEmail = email.length();
        memcpy(buffer + pos, &tamEmail, sizeof(unsigned short)); pos += sizeof(unsigned short);
        memcpy(buffer + pos, email.c_str(), tamEmail); pos += tamEmail;
    }

    void desserializar(const char* buffer) {
        int pos = 0;
        memcpy(&lapide, buffer + pos, sizeof(char)); pos += sizeof(char);
        memcpy(&idUser, buffer + pos, sizeof(int)); pos += sizeof(int);

        unsigned short tamNome;
        memcpy(&tamNome, buffer + pos, sizeof(unsigned short)); pos += sizeof(unsigned short);
        nome.assign(buffer + pos, tamNome); pos += tamNome;

        unsigned short tamEmail;
        memcpy(&tamEmail, buffer + pos, sizeof(unsigned short)); pos += sizeof(unsigned short);
        email.assign(buffer + pos, tamEmail); pos += tamEmail;
    }
};

#endif
