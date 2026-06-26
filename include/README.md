# Diretório Include

Este diretório contém todos os arquivos de cabeçalho (**headers**) do projeto. Eles definem as interfaces, classes e estruturas de dados utilizadas em todo o sistema.

## Principais Arquivos:

- **Game.h**: Define a classe principal de dados `Game`, incluindo as estruturas para datas e os métodos de serialização para persistência binária.
- **Review.h**: Define a classe `Review` para armazenar as avaliações dos usuários vinculadas aos jogos.
- **GameDAO.h / ReviewDAO.h**: Definem as interfaces de Acesso a Dados (Data Access Objects) para gerenciar a persistência no disco.
- **GameController.h**: Interface do controlador que gerencia a ponte entre as requisições web e o banco de dados.
- **Hash.h / HashExtensivel.h**: Definições das estruturas de dados para o índice de Hash Extensível.
- **OrdenacaoExterna.h**: Interface para o algoritmo de intercalação balanceada de vários caminhos.
- **CSVConverter.h**: Define a lógica para importar dados brutos de arquivos CSV para o formato binário otimizado.
- **CasamentoPadroes/**: Subpasta contendo as declarações de `KMP.h` e `BoyerMoore.h` para algoritmos de buscas em strings de alto desempenho.

> [!NOTE]
> Todos os arquivos aqui utilizam *include guards* (`#ifndef`) para evitar múltiplas inclusões durante a compilação.
