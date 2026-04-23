# Gerenciador de Banco de Dados Steam (AEDs3)

Este projeto é um sistema completo de gerenciamento de banco de dados desenvolvido para a disciplina de **Algoritmos e Estruturas de Dados III (AEDs3)**. Ele demonstra a implementação de conceitos fundamentais de armazenamento persistente, indexação e processamento de dados em larga escala utilizando C++.

## 🚀 Visão Geral

O sistema permite gerenciar um catálogo de jogos da Steam, realizando operações de CRUD (Criar, Ler, Atualizar e Deletar) diretamente em arquivos binários. Diferente de bancos de dados convencionais, toda a lógica de gerenciamento de arquivos, índices de Hash e ordenação externa foi desenvolvida do zero.

## 🏗️ Arquitetura do Sistema

O projeto segue uma arquitetura moderna dividida em três camadas principais:

1.  **Camada de Dados (Persistência)**:
    - Armazenamento em arquivos binários com registros de tamanho variável.
    - Implementação de **Hash Extensível** para indexação primária (acesso O(1)).
    - Gerenciamento de relacionamentos 1:N (Jogos e Avaliações).
    - Suporte a **Exclusão Lógica** via lápide.

2.  **Camada de Lógica e Servidor (Backend)**:
    - Desenvolvida em C++.
    - Implementa uma **API REST** para comunicação com a interface.
    - Algoritmo de **Ordenação Externa** (Intercalação Balanceada de vários caminhos).

3.  **Camada de Apresentação (Frontend)**:
    - Interface web moderna e responsiva (HTML5, CSS3, JavaScript).
    - Comunicação assíncrona via JSON.

## 📂 Estrutura de Pastas

- `/include`: Cabeçalhos (`.h`) com as definições de classes e interfaces.
- `/src`: Implementações (`.cpp`) da lógica de negócios e estruturas de dados.
- `/public`: Arquivos da interface web (frontend).
- `/external`: Bibliotecas de terceiros (servidor HTTP).

## 🛠️ Como Executar

O projeto inclui scripts automatizados para facilitar a compilação no Windows:

1.  **Compilar**: Execute o arquivo `build_db.bat`. Isso gerará o executável `a.exe`.
2.  **Executar**: Rode o `a.exe`. O servidor iniciará e abrirá automaticamente a porta **8080**.
3.  **Acessar**: Abra seu navegador em `http://localhost:8080`.
4.  **Resetar**: Se precisar limpar o banco e reimportar os dados do CSV original, execute o `reset_db.bat`.

---
*Desenvolvido como projeto prático para a disciplina de AEDs III.*
