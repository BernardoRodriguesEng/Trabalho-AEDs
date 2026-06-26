# Casamento de Padrões (Pattern Matching)

Este módulo implementa algoritmos de busca de padrões (casamento de padrões) em cadeias de texto, criados para otimizar a velocidade de busca dentro do nosso banco de dados da Steam. Foram implementados dois dos algoritmos mais consagrados na área da ciência da computação.

## 1. Algoritmo KMP (Knuth-Morris-Pratt)

O algoritmo **KMP** encontra todas as ocorrências de um padrão em um texto e tem como principal vantagem evitar voltar (fazer "backtracking") no texto quando ocorre uma falha na comparação (ou seja, quando os caracteres não casam).

### Como funciona
1. **Tabela de Prefixo e Sufixo (LPS - Longest Proper Prefix which is also Suffix):** 
   O algoritmo primeiro analisa o próprio **padrão** que estamos buscando e cria uma tabela com os maiores prefixos que também são sufixos na string. Essa tabela dita quantos caracteres podemos pular com segurança ao detectarmos um não-casamento, já sabendo que o prefixo casou.
2. **Busca:** 
   O algoritmo compara os caracteres da esquerda para a direita. Se ocorrer um caractere incompatível na posição `j` do padrão, ele não recomeça a busca do início, mas usa a tabela LPS para saber onde `j` deve continuar. Dessa forma, nós evitamos re-comparar partes do texto.

**Complexidade:**
- **Tempo:** O(N + M), onde N é o tamanho do texto e M o tamanho do padrão.
- **Vantagem:** Muito eficiente para textos muito grandes.

---

## 2. Algoritmo Boyer-Moore

O algoritmo **Boyer-Moore** é considerado muitas vezes a referência em buscas de substring. Ele introduz uma abordagem contra-intuitiva de comparar os caracteres do **final para o começo** do padrão, combinada com duas heurísticas de salto. 

Neste projeto, implementamos a heurística de **Caractere Ruim** (Bad Character).

### Como funciona (Heurística de Bad Character)
1. **Tabela de Caracteres (Bad Character Rule):** 
   Na fase de pré-processamento, o algoritmo mapeia em qual posição da string do padrão encontra-se a última ocorrência de cada caractere do alfabeto (tamanho 256 na tabela ASCII). Se um caractere não existe no padrão, o valor dele na tabela é -1.
2. **Busca da Direita para a Esquerda:**
   A comparação do padrão com o texto é feita da direita para a esquerda.
3. **Pulos:**
   Se houver um "não-casamento" (o caractere do texto analisado no momento é um "caractere ruim"), o Boyer-Moore utiliza a tabela para deslocar rapidamente todo o padrão para a direita. O objetivo do salto é alinhar essa letra "ruim" encontrada no texto à sua última ocorrência dentro do padrão. Se a letra "ruim" do texto nem sequer existir no padrão, nós simplesmente saltamos **todo** o padrão.

**Complexidade:**
- **Tempo Médio:** Extremamente sub-linear. No melhor caso de texto e padrão com pouca repetição, tem performance na ordem de O(N/M).
- **Vantagem:** É extremamente rápido em linguagem natural, visto que permite dar grandes saltos à frente sem precisar validar os caracteres que ficaram para trás.

---

## Como estão integrados neste projeto?
Tanto a implementação de [KMP](KMP.cpp) quanto a de [Boyer-Moore](BoyerMoore.cpp) foram embutidas na classe `GameDAO` (camada responsável pelo banco de dados). 

Para efetuar a busca:
- O banco binário (`steam.bin`) é percorrido sequencialmente.
- O campo `name` é descompactado no momento da leitura de um jogo e, ao invés de utilizarmos funções built-in como `string::find`, delegamos aos módulos KMP ou Boyer-Moore as validações, testando se a substring (fornecida no terminal pelo usuário) existe dentro do texto principal.
- Todo esse módulo está coberto pelos testes contidos em `tests/TestesCasamentoPadroes.cpp`.
