# Gerenciador de Banco de Dados Steam (AEDs3)

Este projeto é um sistema completo de gerenciamento de banco de dados desenvolvido para a disciplina de **Algoritmos e Estruturas de Dados III (AEDs3)**. Ele demonstra a implementação de conceitos fundamentais de armazenamento persistente, indexação e processamento de dados em larga escala utilizando C++.

## Visão Geral

O sistema permite gerenciar um catálogo de jogos da Steam, realizando operações de CRUD (Criar, Ler, Atualizar e Deletar) diretamente em arquivos binários. Diferente de bancos de dados convencionais, toda a lógica de gerenciamento de arquivos, índices de Hash e ordenação externa foi desenvolvida do zero.

## Como Executar

O projeto inclui scripts automatizados para facilitar a compilação no Windows:

1.  **Compilar**: Execute o arquivo `build_db.bat`. Isso gerará o executável `a.exe`.
2.  **Executar**: Rode o `a.exe`. O servidor iniciará e abrirá automaticamente a porta **8080**.
3.  **Acessar**: Abra seu navegador em `http://localhost:8080`.
4.  **Resetar**: Se precisar limpar o banco e reimportar os dados do CSV original, execute o `reset_db.bat`.

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
O relacionamento entre Jogos e Avaliações foi implementado usando um índice de **Hash Extensível Secundário**. Esse índice mapeia a Chave Estrangeira (`idJogo`) para todos os offsets das avaliações correspondentes no arquivo de dados. Isso permite que, ao visualizar um jogo, o sistema recupere instantaneamente todas as suas avaliações vinculadas sem percorrer o arquivo inteiro.

### g) Como os índices são persistidos em disco?
Os índices são reconstruídos em memória a partir dos arquivos de dados (`.bin`) toda vez que a aplicação é iniciada através do método `reconstruirHash()`. Toda operação de escrita (Inserção/Atualização) sincroniza os dados no disco e atualiza o objeto de Hash, garantindo que o índice reflita exatamente o estado atual dos arquivos.

### h) Como está estruturado o projeto no GitHub?
O projeto segue uma arquitetura modular clara:
- `/include`: Cabeçalhos e definições de interfaces.
- `/src`: Implementação da lógica de dados (DAOs), algoritmos de ordenação e controlador.
- `/public`: Interface do usuário (Frontend Web).
- `/external`: Bibliotecas de terceiros para comunicação em rede.
- **Raiz**: Scripts de automação (`.bat`) e documentação principal.

---
*Desenvolvido como projeto prático para a disciplina de AEDs III.*