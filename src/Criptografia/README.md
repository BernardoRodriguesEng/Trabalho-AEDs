# Módulo de Criptografia

Este diretório contém a implementação dos algoritmos de segurança utilizados para proteger os arquivos binários do banco de dados da Steam.

## One-Time Pad (XOR)

A classe `XOR` (`XOR.h` / `XOR.cpp`) implementa uma abordagem prática baseada na cifra de Vernam (One-Time Pad) utilizando a porta lógica Ou Exclusivo (XOR).

### Características
* **Simetria:** A mesma operação matemática é aplicada tanto para criptografar quanto para descriptografar. `Criptografado = Original ^ Chave` e `Original = Criptografado ^ Chave`.
* **Segurança Prática:** Durante o processo de criptografia, uma chave randômica do mesmo tamanho exato do banco de dados original é gerada. Essa chave é salva no arquivo `chave_xor.key`.
* **Desempenho:** O algoritmo foi projetado em "estilo C" raiz, utilizando leitura em blocos (buffers de 8KB) com arrays manuais, minimizando alocações na heap (dispensando `std::vector`) para que operações em grandes massas de dados (vários MBs) ocorram de forma instantânea.

### Como funciona no projeto
1. A interface web (API `/api/encrypt`) chama o método de criptografia.
2. O `steam.bin` é lido, aplicando-se o XOR byte a byte com a geração randômica (`rand()`).
3. O resultado ininteligível é salvo no arquivo `steam_criptografado.bin`.
4. Para reaver os dados, o usuário chama a rotina de descriptografia (API `/api/decrypt`), que usa o arquivo criptografado e a `chave_xor.key` para gerar um arquivo restaurado identico ao original.
