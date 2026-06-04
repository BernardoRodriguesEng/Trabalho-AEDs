#include "../../include/Compressao/TrieLZW.h"

NoTrieLZW::NoTrieLZW(int valorCodigo) : codigo(valorCodigo) {
    for(int i = 0; i < 256; i++) {
        filhos[i] = nullptr;
    }
}

NoTrieLZW::~NoTrieLZW() {
    for(int i = 0; i < 256; i++) {
        if(filhos[i]) {
            delete filhos[i];
        }
    }
}
