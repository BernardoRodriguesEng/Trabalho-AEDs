# Diretório Source (src)

Este diretório contém as implementações em C++ (`.cpp`) de toda a lógica do banco de dados e do servidor.

## Organização da Lógica:

### 1. Camada de Persistência (DAOs)
- **GameDAO.cpp / ReviewDAO.cpp**: Implementam o CRUD completo. Utilizam arquivos binários para armazenamento e gerenciam índices para busca rápida.
- **CSVConverter.cpp**: Contém a lógica de transformação dos dados do Kaggle (CSV) para o formato de registro de tamanho variável do projeto.

### 2. Estruturas de Dados
- **Hash.cpp / HashExtensivel.cpp**: Implementação do índice de Hash Extensível. Suporta busca por ID em tempo O(1) e busca de múltiplos registros (1:N) para os relacionamentos.
- **OrdenacaoExterna.cpp**: Implementa o algoritmo de ordenação externa (Intercalação Balanceada) para processar grandes volumes de dados que não cabem na memória.

### 3. Controlador e Servidor
- **GameController.cpp**: Implementa os endpoints da API REST utilizando a biblioteca `httplib`. Ele traduz requisições JSON em operações no banco de dados e vice-versa.

## Como funciona a persistência:
O sistema utiliza **arquivos binários** para máxima performance. Cada registro é salvo com um indicador de tamanho, permitindo registros de tamanho variável. A exclusão é feita de forma **lógica** através de uma "lápide" (char `*`) no início do registro.
