#include "../include/OrdenacaoExterna.h"
#include "../include/Game.h"
#include <fstream>
#include <vector>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

using namespace std;

bool compararPorNome(const Game& a, const Game& b) {
    return a.name < b.name;
}

vector<string> criarBlocos(const string& arquivoEntrada, int tamanhoBloco) {
    ifstream in(arquivoEntrada, ios::binary);
    vector<string> arquivos;

    in.seekg(sizeof(int));

    int indice = 0;

    while (in.peek() != EOF) {
        vector<Game> buffer;

        for (int i = 0; i < tamanhoBloco && in.peek() != EOF; i++) {
            Game g;
            g.readFromStream(in);

            if (g.isActive()) {
                buffer.push_back(g);
            }
        }

        sort(buffer.begin(), buffer.end(), compararPorNome);
        
        if (!fs::exists("temp")) fs::create_directory("temp");

        string nomeTemp = "temp/bloco_" + to_string(indice++) + ".bin";
        ofstream out(nomeTemp, ios::binary);

        for (auto& g : buffer) {
            int size = g.getSerializationSize();
            char* dados = new char[size];

            g.serialize(dados);
            out.write(dados, size);

            delete[] dados;
        }

        out.close();
        arquivos.push_back(nomeTemp);
    }

    in.close();
    return arquivos;
}

void intercalarDois(const string& a, const string& b, const string& saida) {
    ifstream f1(a, ios::binary);
    ifstream f2(b, ios::binary);
    ofstream out(saida, ios::binary);

    Game g1, g2;

    bool tem1 = (f1.peek() != EOF);
    bool tem2 = (f2.peek() != EOF);

    if (tem1) g1.readFromStream(f1);
    if (tem2) g2.readFromStream(f2);

    while (tem1 && tem2) {
        if (g1.name < g2.name) {
            int size = g1.getSerializationSize();
            char* dados = new char[size];
            g1.serialize(dados);
            out.write(dados, size);
            delete[] dados;

            tem1 = (f1.peek() != EOF);
            if (tem1) g1.readFromStream(f1);
        } else {
            int size = g2.getSerializationSize();
            char* dados = new char[size];
            g2.serialize(dados);
            out.write(dados, size);
            delete[] dados;

            tem2 = (f2.peek() != EOF);
            if (tem2) g2.readFromStream(f2);
        }
    }

    while (tem1) {
        int size = g1.getSerializationSize();
        char* dados = new char[size];
        g1.serialize(dados);
        out.write(dados, size);
        delete[] dados;

        tem1 = (f1.peek() != EOF);
        if (tem1) g1.readFromStream(f1);
    }

    while (tem2) {
        int size = g2.getSerializationSize();
        char* dados = new char[size];
        g2.serialize(dados);
        out.write(dados, size);
        delete[] dados;

        tem2 = (f2.peek() != EOF);
        if (tem2) g2.readFromStream(f2);
    }

    f1.close();
    f2.close();
    out.close();
}

void OrdenacaoExterna::ordenarPorNome(const string& arquivoEntrada, const string& arquivoSaida) {
    if (!fs::exists("temp")) fs::create_directory("temp");
    
    vector<string> blocos = criarBlocos(arquivoEntrada, 100);
    int mergeCount = 0;

    while (blocos.size() > 1) {
        vector<string> novos;

        for (size_t i = 0; i < blocos.size(); i += 2) {
            if (i + 1 < blocos.size()) {
                string nomeSaida = "temp/merge_" + to_string(mergeCount++) + ".bin";
                intercalarDois(blocos[i], blocos[i + 1], nomeSaida);
                
                // Limpa explicitamente quaisquer associações de sistema de arquivos se possível, 
                // mas fechar os fluxos dentro de intercalarDois é a correção real.
                try {
                    if (fs::exists(blocos[i])) fs::remove(blocos[i]);
                    if (fs::exists(blocos[i + 1])) fs::remove(blocos[i + 1]);
                } catch (const fs::filesystem_error& e) {
                    cerr << "Aviso: Não foi possível remover o arquivo temporário: " << e.what() << endl;
                }
                
                novos.push_back(nomeSaida);
            } else {
                novos.push_back(blocos[i]);
            }
        }

        blocos = novos;
    }

    if (!blocos.empty()) {
        try {
            if (fs::exists(arquivoSaida)) fs::remove(arquivoSaida);
            fs::rename(blocos[0], arquivoSaida);
        } catch (const fs::filesystem_error& e) {
            cerr << "Erro ao finalizar a ordenação: " << e.what() << endl;
        }
    }
    
    // Limpeza final do diretório temporário se estiver vazio
    try {
        if (fs::exists("temp") && fs::is_empty("temp")) {
            fs::remove("temp");
        }
    } catch (...) {}
}