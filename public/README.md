# Diretório Public (Frontend)

Este diretório contém os ativos estáticos que compõem a interface web do sistema.

## Componentes:

- **index.html**: A estrutura principal da aplicação. É uma aplicação de página única (SPA) que alterna entre as seções de Busca, Adição e Gerenciamento sem recarregar a página.
- **style.css**: Define o design moderno e minimalista do sistema, utilizando um tema escuro e painéis com efeito de vidro (glassmorphism).
- **script.js**: Contém toda a lógica do cliente.
    - Realiza chamadas `fetch` para a API REST do servidor C++.
    - Gerencia o estado da aplicação (como os resultados da busca atual).
    - Implementa o sistema de "Cycling" (navegação entre múltiplos resultados).
    - Lida com o fechamento automático do servidor ao fechar a aba do navegador.

## Comunicação:
O frontend se comunica com o backend via JSON através do endereço `http://localhost:8080`.
