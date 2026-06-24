# Relatório Final - Projeto de Extensão 1 (Sistema de Pedidos)

Este relatório compila o contexto, as contribuições individuais e as decisões arquiteturais da equipe responsável pelo desenvolvimento do **Sistema de Pedidos**, implementado integralmente em C para ambiente de terminal.

## 1. Contexto do Projeto

Baseado no guia fundamental de requisitos (`requisitos_de _projeto.md`), o sistema foi concebido para atender às demandas de gestão de vendas e produtos em ambiente local. As principais frentes de atuação definidas foram:

- **Gestão de Produtos:** Registro com atributos como ID, nome e preço.
- **Gestão de Clientes:** Controle de cadastro e emissão de número identificador.
- **Registro de Pedidos e Devoluções:** Compra de itens quantificados e a validação de regras de devolução.
  > [!IMPORTANT]
  > Regra de Negócio Crítica: Todo item devolvido pela **segunda vez** sofre uma taxa adicional e fixa de **R$ 20,00**.
- **Módulo Analítico e Relatórios:** Agrupamento de informações diárias, mensais (com detalhamento das taxas de devolução) e relatórios anuais que apresentam os meses em *ordem decrescente de faturamento*.

Toda a solução técnica foi exigida na linguagem **C**, utilizando structs, vetores genéricos, algoritmos de ordenação e modularização baseada em funções.

---

## 2. Contribuições da Equipe

A equipe de 5 desenvolvedores adotou uma segmentação de responsabilidade, a fim de mitigar conflitos e garantir especialização nos subcomponentes do software.

### Dev 1: Gabriel Bittencourt (Módulo Cliente)
- **Atuação:** Responsável pelo gerenciamento do cadastro dos clientes (`cliente.c`, `cliente.h`).
- **Principais Contribuições:** 
  - Criação da interface interativa de cadastro.
  - Validação de regras e persistência de dados do cliente, isolando a regra de negócio num contrato previsível e limpo para o front-end de terminal.

### Dev 2: Wallace Calisto (Módulo Produto)
- **Atuação:** Responsável pelo core de Produtos (`produto.c`, `produto.h`).
- **Principais Contribuições:** 
  - Desenvolvimento da lógica interativa de cadastro de novos produtos no estoque.
  - Implementação de validações lógicas (preços positivos obrigatórios e prevenção contra inserções nulas).
  - Encaminhamento da struct preenchida para a camada de armazenamento.

### Dev 3: Ana Silva (Módulo Pedido e Banco de Dados)
- **Atuação:** Estruturação dos fluxos de compras e das operações do File System (banco de dados manual em CSV) através dos arquivos `pedido.c`, `persistencia.c` e `tipos.h`.
- **Principais Contribuições:**
  - Desenvolvimento do motor de carrinho de compras em memória usando vetores estáticos antes de realizar a gravação definitiva, garantindo a integridade dos dados (`ACID` simplificado).
  - Criação dos CRUDs primitivos para arquivos CSV simulando tabelas em um RDBMS (`csv_inserir_*`, `csv_buscar_*`, `csv_obter_proximo_id`).

### Dev 4: Guilherme Brazil (Módulo de Devoluções e Regras)
- **Atuação:** Responsável pelas regras tributárias estipuladas no escopo, implementadas em `devolucao.c`.
- **Principais Contribuições:**
  - Lógica para calcular a incidência da taxa de R$ 20,00 a partir da segunda devolução do mesmo item de um pedido.
  - Interface no terminal para registrar a devolução com busca de metadados do pedido e auditoria da quantidade em devolução.

### Dev 5: Caio Galvão (Lead - Core, Relatórios e UX)
- **Atuação:** Relatórios, Input avançado e Utilitários (`relatorio.c`, `input.c`, `logger.c`, `lookup.c`, `main.c`).
- **Principais Contribuições:**
  - Criação dos algoritmos para relatórios (Diário, Mensal, Anual) incluindo o uso de `qsort()` para listar faturamentos decrescentes e mesclar taxas de devolução aos saldos contábeis.
  - Sistema avançado de *Input* no terminal, adicionando cancelamento por sinais de teclado (`Ctrl+D` ou Enter vazio) e menus de busca/lookup dinâmico interrupendo a navegação ativa (`Ctrl+B`).
  - Criação do `logger.c`, estabelecendo rastro e trilha de auditoria para todas as operações sensíveis do sistema (salvas em `acoes_usuario.log`).

---

## 3. Decisões Arquiteturais Tomadas

> [!NOTE]
> A arquitetura da aplicação adota o modelo Procedural com fortes influências de **MVC (Model-View-Controller)** dentro do limite que C fornece sem O.O.

1. **Separação de Preocupações (SoC):** Cada entidade (Cliente, Produto, Pedido) possui o seu arquivo isolado. Isso permitiu que a equipe escalasse o trabalho simultaneamente. As regras de negócio ficam contidas na implementação de `_salvar()` enquanto a exibição reside nas subrotinas `_menu()`.
2. **Sistema de Persistência Baseado em Arquivos Textuais (CSV):** Optamos pelo uso de arquivos de texto de forma a simular um sistema RDBMS sem bibliotecas externas para não infringir o peso do executável local. Funções como `csv_obter_proximo_id` funcionam análogas ao `AUTO_INCREMENT`.
3. **Auditoria Centralizada (Logger):** Implementado no final do projeto, permite que não precisemos depurar `printfs` perdidos na tela. Funções registram fluxos de sucesso ou falhas no arquivo físico `acoes_usuario.log`.
4. **Resiliência de Interface (UX):** Entradas em sistemas C tendem a causar Buffer Overflows ou _loops infinitos_ usando `scanf()`. Implementamos em `input.c` heurísticas em volta de `fgets()` e purgações de lixo no `stdin` (via macro/limpeza por caracter) que fornecem um comportamento semelhante a inputs assíncronos modernos.

---
**Status Final:** Todos os requisitos documentados foram implementados, testados (com base em mocks incluídos no projeto) e o código fonte se encontra formatado dentro da convenção Doxygen/CDoc.
