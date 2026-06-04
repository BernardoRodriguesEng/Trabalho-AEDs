# Módulo de Compressão

Este diretório contém as implementações de algoritmos de compressão sem perdas: **Huffman** e **LZW**.

## 🏗️ Arquitetura do Módulo

O sistema de compressão foi dividido seguindo o princípio da *Separação de Preocupações* (SoC). Os componentes operam em conjunto, mas possuem responsabilidades isoladas:

### 1. Algoritmos Core (`Huffman.cpp` e `LZW.cpp`)
Atuam como os orquestradores da lógica matemática. Eles recebem os dados brutos, utilizam as estruturas auxiliares e invocam a escrita do resultado.
* **Huffman:** Baseia-se na frequência estatística dos caracteres do arquivo. Uma árvore binária é construída, designando menos bits para os caracteres mais frequentes e criando prefixos.
* **LZW (Lempel-Ziv-Welch):** Algoritmo baseado em dicionário que "aprende" padrões no arquivo enquanto roda. Ele começa conhecendo a tabela ASCII básica e vai mapeando novas palavras para códigos inteiros progressivamente.

### 2. Gerenciador de Arquivos (`GerenciadorArquivo.cpp`)
Para realizar operações de Leitura/Escrita no disco utilizamos o `GerenciadorArquivo`. É um "wrapper" construído em cima das funções raiz da linguagem C padrão (`fopen`, `fread`, `fwrite`, `fclose`), operando manipulações baseadas em buffers de bytes (`FILE*`).

### 3. Estruturas Auxiliares (`ArranjosDinamicos.cpp` e `TrieLZW.cpp`)
* **`ArrayDinamicoInt` e `ArrayDinamicoString`**: Gerenciadores de listas contíguas que redimensionam a si mesmos automaticamente usando limites de capacidade (se encher, ele dobra o tamanho, aloca nova memória, copia os valores antigos e descarta a memória velha).
* **`NoTrieLZW`**: Estrutura em formato de Árvore de Prefixos (Trie) usada na Compressão LZW para permitir buscas de caminhos de strings no dicionário em complexidade $O(1)$.

---

## Como Utilizar no Código

Ambos os algoritmos oferecem APIs estáticas que podem ser chamadas em qualquer local do projeto (como no `main.cpp`):

### Exemplo de LZW:
```cpp
#include "include/Compressao/LZW.h"

// Para Comprimir
LZW::comprimir("banco_dados.bin", "banco_dados_lzw.bin");

// Para Descomprimir
LZW::descomprimir("banco_dados_lzw.bin", "banco_restaurado.bin");
```

### Exemplo de Huffman:
```cpp
#include "include/Compressao/Huffman.h"

// Para Comprimir
Huffman::comprimir("banco_dados.bin", "banco_dados_huffman.bin");

// Para Descomprimir
Huffman::descomprimir("banco_dados_huffman.bin", "banco_restaurado.bin");
```

> **Aviso de Tratamento de Erros:** Alertas são registrados no console (`std::cerr`) em caso de tentativas de compressão de arquivos vazios, corrompidos ou falhas de permissão ao criar o disco.

## 🧠 Onde Encontrar Cada Parte?

* **Interfaces (`/include/Compressao/`)**: Todos os Cabeçalhos (`.h`) contendo o esqueleto estrutural das funções e structs.
* **Corpo Lógico (`/src/compressao/`)**: A lógica concreta e implementação algorítmica (`.cpp`).
