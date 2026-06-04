# Interfaces do Módulo de Compressão 📑

Esta pasta (`/include/Compressao/`) guarda todos os **Arquivos de Cabeçalho (.h)** do módulo de compressão. 

O objetivo de manter essas definições separadas do código fonte é expor apenas as **Interfaces** (APIs) para o restante do programa. Assim, se você estiver desenvolvendo o `main.cpp` ou um *Controller*, só precisa olhar para estes arquivos para saber **o que** as classes fazem, sem se preocupar em ler **como**.

## O que temos aqui?

### 1. APIs dos Algoritmos (O que você vai chamar)
* **`Huffman.h`**: Expõe a classe `Huffman` com seus métodos estáticos públicos (`comprimir` e `descomprimir`). Ele também revela a estrutura da árvore (`NoHuffman`) caso você precise debugar ou expandir o código.
* **`LZW.h`**: Expõe a classe `LZW`. As assinaturas dos métodos são idênticas às de Huffman, o que facilita trocar de algoritmo na aplicação sem alterar muito código.

### 2. Estruturas de Dados Customizadas
* **`ArranjosDinamicos.h`**: Define nossos gerenciadores de memória. Ele te mostra a estrutura de um `ArrayDinamicoInt` e `ArrayDinamicoString` (capacidade, tamanho, ponteiros soltos de memória).
* **`TrieLZW.h`**: Mostra como um nó da árvore de dicionário LZW (`NoTrieLZW`) foi desenhado para criar ramificações com 256 filhos possíveis.

### 3. Operações em Disco
* **`GerenciadorArquivo.h`**: Abstração da biblioteca (`<cstdio>`) para manipulação de bytes. Se quiser ver os métodos de baixo nível de disco disponíveis para leitura/escrita, é aqui que você procura.

---

## 🔌 Como Importar

Para utilizar a compressão em qualquer lugar do projeto, basta direcionar o `#include`:

```cpp
#include "include/Compressao/Huffman.h"
#include "include/Compressao/LZW.h"

int main() {
    Huffman::comprimir("entrada.bin", "saida.huff");
    LZW::comprimir("entrada.bin", "saida.lzw");
    return 0;
}
```

> **Dica:** Para ler um aprofundamento mais técnico sobre as decisões de baixo nível, leia o `README.md` localizado na pasta dos códigos-fonte: `/src/compressao/README.md`.
