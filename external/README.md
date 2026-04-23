# Diretório External

Este diretório contém bibliotecas de terceiros e dependências externas utilizadas no projeto.

## Bibliotecas:

- **httplib.h**: Uma biblioteca *header-only* em C++ para criação de servidores HTTP. Ela é responsável por gerenciar as rotas da API e servir os arquivos estáticos da pasta `public`.

## Por que usar o httplib?

A escolha desta biblioteca foi estratégica para atender aos requisitos da disciplina da forma mais moderna e eficiente:

1. **Necessidade de Interface Gráfica**: Como o projeto exige uma interface amigável (front-end), o `httplib` permite transformar o código C++ em um servidor local. Isso possibilita o uso de tecnologias web (HTML/CSS/JS) para a interface, que são muito mais flexíveis e fáceis de customizar do que bibliotecas gráficas nativas de C++ (como Qt ou GTK).
2. **Separação de Camadas**: O uso de um servidor HTTP cria uma separação clara entre a **lógica do banco de dados** (backend) e a **apresentação** (frontend). Isso simula o funcionamento de sistemas reais de grande escala (arquitetura cliente-servidor).
3. **Simplicidade (Header-only)**: Por ser uma biblioteca de arquivo único, ela não requer processos complexos de instalação ou compilação, mantendo o projeto leve e focado nos algoritmos de AEDs3.
4. **É necessário?** Tecnicamente, poderíamos usar outras formas de interface (como uma aplicação de terminal), mas o `httplib` é a solução que oferece o melhor equilíbrio entre atender à exigência de "interface gráfica" e manter o foco total no desenvolvimento das estruturas de dados e arquivos binários.

## Por que external?
Mantemos essas bibliotecas separadas do código fonte principal (`src`) para facilitar a manutenção e garantir que o foco do desenvolvimento permaneça na lógica do banco de dados desenvolvida para a disciplina de AEDs3.

> [!IMPORTANT]
> No Windows, a compilação requer o link com a biblioteca de sockets (`-lws2_32`) para que a `httplib` funcione corretamente.
