# Gerenciador de Banco de Dados Steam (AEDs3)

Este projeto é um sistema completo de gerenciamento de banco de dados desenvolvido para a disciplina de **Algoritmos e Estruturas de Dados III (AEDs3)**. Ele demonstra a implementação de conceitos fundamentais de armazenamento persistente, indexação e processamento de dados em larga escala utilizando C++.

## Visão Geral

O sistema permite gerenciar um catálogo de jogos da Steam, realizando operações de CRUD (Criar, Ler, Atualizar e Deletar) diretamente em arquivos binários. Diferente de bancos de dados convencionais, toda a lógica de gerenciamento de arquivos, índices de Hash e ordenação externa foi desenvolvida do zero.

## Requisitos do Sistema

Para compilar e executar este projeto corretamente, seu ambiente deve atender aos seguintes requisitos:

-   **Compilador**: GCC com suporte a **C++17** ou superior.
-   **Compatibilidade**: 
    - **Linux**: O comando de build utiliza `-lpthread`.
    - **Windows**: Linkar com `-lws2_32`.

## Como Executar

O projeto foi otimizado para ser "plug and play". Não é necessário rodar scripts de conversão manuais.

1.  **Compilar**: 
    - **Linux**: `g++ -std=c++17 main.cpp -o app -lpthread`
    - **Windows**: `g++ -std=c++17 main.cpp -o app.exe -lws2_32`
2.  **Executar**: Rode o executável gerado (`./app` ou `app.exe`).
3.  **Inicialização Automática**: Na primeira execução, o programa detectará a ausência dos arquivos `.bin` e realizará automaticamente a conversão do CSV, a geração de índices e a ordenação externa.
4.  **Acessar**: Abra seu navegador em `http://localhost:8080`.

## Formulário Técnico

### a) Qual a estrutura usada para representar os registros?
Os registros são de **tamanho variável**. Cada registro no arquivo binário é precedido por um número inteiro que indica seu tamanho total em bytes. Os campos são serializados sequencialmente, e as strings são armazenadas com um prefixo (`unsigned short`) indicando seu comprimento.

### b) Como atributos multivalorados do tipo string foram tratados?
Atributos como categorias e gêneros foram tratados como vetores de strings (`std::vector<std::string>`). Na serialização, gravamos primeiro o número de elementos do vetor e, em seguida, cada string individualmente (comprimento + conteúdo).

### c) Como foi implementada a exclusão lógica?
Utilizamos o conceito de **lápide** (`lapide`). O primeiro byte de cada registro é um caractere que indica seu estado: um espaço (`' '`) significa que o registro está ativo, enquanto um asterisco (`'*'`) indica que ele foi excluído. O sistema ignora registros marcados com o asterisco durante buscas e listagens.

### d) Além das PKs, quais outras chaves foram utilizadas nesta etapa?
Além das Chaves Primárias (`appid` para jogos e `idReview` para avaliações), utilizamos o `idJogo` como **Chave Estrangeira (FK)** para vincular avaliações a jogos específicos. Implementamos busca por **Nome** (chave secundária) com suporte a múltiplos resultados.

### e) Como a estrutura (hash) foi implementada para cada chave de pesquisa?
Implementamos um **Hash Extensível** dinâmico. Ele utiliza um diretório de ponteiros para buckets que residem em memória e são reconstruídos a partir do disco. Quando um bucket atinge sua capacidade máxima, ele sofre um *split* (divisão), e a profundidade local/global é atualizada, garantindo acesso O(1).

### f) Como foi implementado o relacionamento 1:N?
O relacionamento entre Jogos e Avaliações foi implementado usando um índice de **Hash Extensível Secundário**. Esse índice mapeia a Chave Estrangeira (`idJogo`) para todos os offsets das avaliações correspondentes no arquivo de dados. Isso permite que, ao visualizar um jogo, o sistema recupere e exiba instantaneamente todas as suas avaliações vinculadas através da interface Web.

### g) Como os índices são persistidos em disco?
Os índices são reconstruídos em memória a partir dos arquivos de dados (`.bin`) toda vez que a aplicação é iniciada através do método `reconstruirHash()`. Toda operação de escrita (Inserção/Atualização) sincroniza os dados no disco e atualiza o objeto de Hash, garantindo que o índice reflita exatamente o estado atual dos arquivos.

### h) Como está estruturado o projeto no GitHub?
O projeto segue uma arquitetura modular clara:
- `/include`: Cabeçalhos e definições de interfaces.
- `/src`: Implementação da lógica de dados (DAOs), algoritmos de ordenação e controlador.
- `/public`: Interface do usuário (Frontend Web com suporte a visualização e criação de reviews).
- `/external`: Biblioteca header-only `httplib.h` para comunicação em rede.
- **Raiz**: Arquivo `main.cpp` e documentação do projeto.

---
*Desenvolvido como projeto prático para a disciplina de AEDs III.*