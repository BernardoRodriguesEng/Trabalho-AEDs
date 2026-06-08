# Gerenciador de Banco de Dados Steam (AEDs3)

Este projeto é um sistema completo de gerenciamento de banco de dados desenvolvido para a disciplina de **Algoritmos e Estruturas de Dados III (AEDs3)**. Ele demonstra a implementação de conceitos fundamentais de armazenamento persistente, indexação estruturada, relacionamento entre entidades e processamento de dados em larga escala utilizando C++.

## 🚀 Visão Geral

O sistema permite gerenciar um catálogo de jogos da Steam, interagindo através de uma **Interface Web (HTML/JS/CSS)** que se comunica via **API REST** com o back-end em C++. Toda a lógica de gerenciamento de arquivos, índices de Hash, Árvores B+, relacionamentos e compressão foi desenvolvida do zero, sem o uso de SGBDs externos (como MySQL ou SQLite).

### ✨ Principais Funcionalidades Implementadas
- **CRUD Completo:** Criação, leitura, atualização e exclusão (lógica com lápide) de registros de Jogos, Usuários e Reviews.
- **Estruturas de Dados Avançadas:**
  - **Hash Extensível:** Usado para busca rápida de registros (O(1)) por ID.
  - **Árvore B+:** Usada para consultas em faixa de valores (ex: buscar jogos dentro de uma faixa de preço).
- **Relacionamentos:**
  - **1:N (Um para Muitos):** Um Jogo pode ter várias Avaliações (Reviews).
  - **N:N (Muitos para Muitos):** Usuários possuem uma Biblioteca de Jogos (tabela associativa).
- **Ordenação Externa:** Capacidade de ordenar arquivos gigantescos que não cabem na memória RAM.
- **Compressão de Dados:** Algoritmos **LZW** e **Huffman** aplicados para reduzir o espaço em disco do banco de dados quando o servidor é desligado.

---

## ⚙️ Requisitos do Sistema

Para compilar e executar este projeto corretamente, seu ambiente deve atender aos seguintes requisitos:

- **Compilador**: GCC (MinGW no Windows) com suporte a **C++17** ou superior.
- **Arquivos Iniciais**: O arquivo base `steam.csv` deve estar presente na pasta raiz do projeto.

---

## 🏃 Como Compilar e Executar

O projeto foi construído para facilitar a execução. Ele engloba seus módulos diretamente na `main.cpp`, então a compilação requer apenas a indicação do arquivo principal.

### 1. Compilar

**No Windows (PowerShell/CMD):**
```bash
g++ main.cpp -o app.exe -lws2_32
```

**No Linux/macOS:**
```bash
g++ -std=c++17 main.cpp -o app -lpthread
```

### 2. Executar

**No Windows:**
```bash
.\app.exe
```

**No Linux/macOS:**
```bash
./app
```

### 3. Acessar a Interface
Após executar o servidor, abra o seu navegador e acesse:
👉 **[http://localhost:8080](http://localhost:8080)**

---

## 🏗 Estrutura do Projeto

A arquitetura do repositório segue a separação de responsabilidades no padrão MVC (Model-View-Controller) Modificado:

```text
/
├── include/                 # Declarações e cabeçalhos das classes (.h)
│   ├── Compressao/          # Cabeçalhos referentes ao LZW e Huffman
│   ├── Game.h, User.h...    # Modelos de dados
│   ├── GameDAO.h...         # Interfaces de acesso físico
│   └── GameController.h     # Orquestração das rotas HTTP REST
├── src/                     # Implementação da lógica em C++ (.cpp)
│   ├── Compressao/          # Lógica do LZW, Trie, Huffman
│   ├── BPlusTree.cpp        # Lógica de indexação B+
│   ├── Hash.cpp             # Lógica do Hash Extensível
│   └── ...                  # DAOs e Models
├── public/                  # Interface do Usuário (Frontend Web)
│   ├── index.html           # Tela principal (Página Única)
│   ├── style.css            # Estilização
│   └── script.js            # Lógica de integração via Fetch API
├── external/                # Bibliotecas externas de terceiros
│   └── httplib.h            # Biblioteca header-only para o Servidor HTTP
├── tests/                   # Motor de testes unitários da aplicação
└── main.cpp                 # Ponto de entrada (Entrypoint)
```

---

## 📚 Documentação das Fases do Projeto

Este projeto evoluiu em quatro fases avaliativas ao longo do semestre:

- **Fase 1 (Estrutura Base):** Construção da leitura de CSV, conversão para `.bin`, implementação de operações CRUD e indexação por ID usando Hash Extensível.
- **Fase 2 (Busca e Relacionamento 1:N):** Adição de buscas secundárias, Árvore B+ e a entidade de "Avaliações" (Relacionamento Jogo 1 : N Avaliações). Integrado à Interface Web.
- **Fase 3 (Relacionamento N:N e Deleção em Cascata):** Inclusão da entidade "Usuário" e a tabela "Biblioteca" relacionando N Usuários com N Jogos. Implementada a validação e exclusão em cascata (remover um jogo deleta suas reviews e bibliotecas).
- **Fase 4 (Compressão):** Aplicação das técnicas de Lempel-Ziv-Welch (LZW) e Huffman. Ao fechar o servidor de forma limpa, o sistema comprime os arquivos binários. Ao reabrir, o banco é perfeitamente restaurado de forma transparente.

---

## 🛠 Motor de Testes

O projeto contém uma suíte de testes unitários própria para validar se alterações estruturais não quebram o código.
Para rodar os testes:

1. Execute o arquivo de lote `compilar_testes.bat` para compilar tudo de uma vez.
2. Em seguida, execute o `./run_tests.exe`.

---
*Desenvolvido pelo Grupo 04 para a disciplina de AEDs III - PUC Minas.*