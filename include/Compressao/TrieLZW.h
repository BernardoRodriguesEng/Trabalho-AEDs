#ifndef TRIE_LZW_H
#define TRIE_LZW_H

// Estrutura manual de árvore Trie para compressão LZW
struct NoTrieLZW {
    int codigo;
    NoTrieLZW* filhos[256];
    
    NoTrieLZW(int valorCodigo = -1);
    ~NoTrieLZW();
};

#endif
