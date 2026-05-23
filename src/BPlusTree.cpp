#include "../include/BPlusTree.h"

BPlusTree::BPlusTree(string filename) : filename(filename) {
    raiz_offset = lerRaiz();
    if (raiz_offset == -1) {
        raiz_offset = criarNodo(true);
        gravarRaiz(raiz_offset);
    }
}

long BPlusTree::lerRaiz() {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) return -1;
    long offset;
    file.read((char*)&offset, sizeof(long));
    file.close();
    return offset;
}

void BPlusTree::gravarRaiz(long offset) {
    fstream file(filename, ios::binary | ios::in | ios::out);
    if (!file.is_open()) {
        file.open(filename, ios::binary | ios::out);
    }
    file.seekp(0, ios::beg);
    file.write((char*)&offset, sizeof(long));
    file.close();
    raiz_offset = offset;
}

Nodo BPlusTree::lerNodo(long offset) {
    Nodo nodo;
    ifstream file(filename, ios::binary);
    file.seekg(offset, ios::beg);
    file.read((char*)&nodo, sizeof(Nodo));
    file.close();
    return nodo;
}

void BPlusTree::gravarNodo(long offset, const Nodo& nodo) {
    fstream file(filename, ios::binary | ios::in | ios::out);
    file.seekp(offset, ios::beg);
    file.write((char*)&nodo, sizeof(Nodo));
    file.close();
}

long BPlusTree::criarNodo(bool folha) {
    Nodo nodo;
    nodo.folha = folha;
    
    ofstream file(filename, ios::binary | ios::app);
    long offset = file.tellp();
    file.write((char*)&nodo, sizeof(Nodo));
    file.close();
    return offset;
}

long BPlusTree::buscar(float chave) {
    long curr_offset = raiz_offset;
    while (curr_offset != -1) {
        Nodo nodo = lerNodo(curr_offset);
        if (nodo.folha) {
            for (int i = 0; i < nodo.n; i++) {
                if (nodo.chaves[i] == chave) return nodo.filhos_ou_offsets[i];
            }
            return -1;
        } else {
            int i = 0;
            while (i < nodo.n && chave >= nodo.chaves[i]) i++;
            curr_offset = nodo.filhos_ou_offsets[i];
        }
    }
    return -1;
}

vector<long> BPlusTree::buscarIntervalo(float min, float max) {
    vector<long> resultados;
    long curr_offset = raiz_offset;
    
    // 1. Encontrar a primeira folha que pode conter min
    while (curr_offset != -1) {
        Nodo nodo = lerNodo(curr_offset);
        if (nodo.folha) break;
        int i = 0;
        while (i < nodo.n && min >= nodo.chaves[i]) i++;
        curr_offset = nodo.filhos_ou_offsets[i];
    }

    // 2. Percorrer a lista ligada de folhas
    while (curr_offset != -1) {
        Nodo nodo = lerNodo(curr_offset);
        for (int i = 0; i < nodo.n; i++) {
            if (nodo.chaves[i] >= min && nodo.chaves[i] <= max) {
                resultados.push_back(nodo.filhos_ou_offsets[i]);
            }
            if (nodo.chaves[i] > max) return resultados;
        }
        curr_offset = nodo.proximo;
    }
    return resultados;
}

void BPlusTree::inserir(float chave, long data_offset) {
    long curr_offset = raiz_offset;
    while (curr_offset != -1) {
        Nodo nodo = lerNodo(curr_offset);
        if (nodo.folha) {
            inserirNaFolha(curr_offset, chave, data_offset);
            break;
        } else {
            int i = 0;
            while (i < nodo.n && chave >= nodo.chaves[i]) i++;
            curr_offset = nodo.filhos_ou_offsets[i];
        }
    }
}

void BPlusTree::inserirNaFolha(long folha_offset, float chave, long data_offset) {
    Nodo nodo = lerNodo(folha_offset);
    int i = nodo.n - 1;
    while (i >= 0 && nodo.chaves[i] > chave) {
        nodo.chaves[i+1] = nodo.chaves[i];
        nodo.filhos_ou_offsets[i+1] = nodo.filhos_ou_offsets[i];
        i--;
    }
    nodo.chaves[i+1] = chave;
    nodo.filhos_ou_offsets[i+1] = data_offset;
    nodo.n++;
    gravarNodo(folha_offset, nodo);

    if (nodo.n == ORDEM) {
        splitFolha(folha_offset);
    }
}

void BPlusTree::splitFolha(long folha_offset) {
    Nodo l = lerNodo(folha_offset);
    long r_offset = criarNodo(true);
    Nodo r = lerNodo(r_offset);

    int m = ORDEM / 2;
    r.n = ORDEM - m;
    for (int i = 0; i < r.n; i++) {
        r.chaves[i] = l.chaves[m + i];
        r.filhos_ou_offsets[i] = l.filhos_ou_offsets[m + i];
    }
    l.n = m;
    r.proximo = l.proximo;
    l.proximo = r_offset;
    r.pai = l.pai;

    gravarNodo(folha_offset, l);
    gravarNodo(r_offset, r);

    inserirNoPai(folha_offset, r.chaves[0], r_offset);
}

void BPlusTree::inserirNoPai(long filho_offset, float chave_subiu, long novo_filho_offset) {
    Nodo filho = lerNodo(filho_offset);
    if (filho.pai == -1) {
        long nova_raiz_offset = criarNodo(false);
        Nodo nova_raiz = lerNodo(nova_raiz_offset);
        nova_raiz.chaves[0] = chave_subiu;
        nova_raiz.filhos_ou_offsets[0] = filho_offset;
        nova_raiz.filhos_ou_offsets[1] = novo_filho_offset;
        nova_raiz.n = 1;
        gravarNodo(nova_raiz_offset, nova_raiz);
        
        filho.pai = nova_raiz_offset;
        gravarNodo(filho_offset, filho);
        
        Nodo novo_filho = lerNodo(novo_filho_offset);
        novo_filho.pai = nova_raiz_offset;
        gravarNodo(novo_filho_offset, novo_filho);
        
        gravarRaiz(nova_raiz_offset);
        return;
    }

    long pai_offset = filho.pai;
    Nodo pai = lerNodo(pai_offset);
    
    int i = pai.n - 1;
    while (i >= 0 && pai.chaves[i] > chave_subiu) {
        pai.chaves[i+1] = pai.chaves[i];
        pai.filhos_ou_offsets[i+2] = pai.filhos_ou_offsets[i+1];
        i--;
    }
    pai.chaves[i+1] = chave_subiu;
    pai.filhos_ou_offsets[i+2] = novo_filho_offset;
    pai.n++;
    gravarNodo(pai_offset, pai);

    Nodo novo_filho = lerNodo(novo_filho_offset);
    novo_filho.pai = pai_offset;
    gravarNodo(novo_filho_offset, novo_filho);

    if (pai.n == ORDEM) {
        splitInterno(pai_offset);
    }
}

void BPlusTree::splitInterno(long interno_offset) {
    Nodo l = lerNodo(interno_offset);
    long r_offset = criarNodo(false);
    Nodo r = lerNodo(r_offset);

    int m = ORDEM / 2;
    float chave_que_sobe = l.chaves[m];
    
    r.n = ORDEM - m - 1;
    for (int i = 0; i < r.n; i++) {
        r.chaves[i] = l.chaves[m + 1 + i];
        r.filhos_ou_offsets[i] = l.filhos_ou_offsets[m + 1 + i];
    }
    r.filhos_ou_offsets[r.n] = l.filhos_ou_offsets[ORDEM];
    
    l.n = m;
    r.pai = l.pai;

    gravarNodo(interno_offset, l);
    gravarNodo(r_offset, r);

    // Atualizar o pai de todos os filhos que foram para o novo nodo r
    for (int i = 0; i <= r.n; i++) {
        Nodo filho_de_r = lerNodo(r.filhos_ou_offsets[i]);
        filho_de_r.pai = r_offset;
        gravarNodo(r.filhos_ou_offsets[i], filho_de_r);
    }

    inserirNoPai(interno_offset, chave_que_sobe, r_offset);
}
