Status: ready-for-agent

## Problem Statement

Estudantes de Ciência da Computação precisam desenvolver um sistema acadêmico de gerenciamento de loja em C para avaliação. O projeto exige a criação de uma estrutura robusta capaz de gerenciar entidades como Clientes, Produtos e Pedidos. Ele precisa suportar relatórios diários, mensais e anuais. Além disso, o sistema deve registrar Devoluções de forma realista (como escambo de produtos, gerando multas apenas a partir da segunda devolução), usar persistência de dados puramente em arquivos CSV para simular um banco de dados relacional e estar perfeitamente adequado para ser avaliado através do prompt do Windows sem que a janela feche abruptamente.

## Solution

A solução é um sistema de console ("CLI") em C, modularizado em camadas claras (como Interface, Regras de Negócio e Persistência). O sistema faz uso do modelo relacional entre vários arquivos `.csv` para cada entidade (Cliente, Produto, Pedido, ItemPedido, Devolucao). O sistema é à prova de falhas comuns em aplicações de console através da proteção de loops, checagem de limites de *arrays* com base em `#define` (como o limite de vendas diárias) e imposição de tamanhos de string como `MAX_NOME 255`. Interações para Windows (`system("pause")` ou equivalentes) são ativadas em pontos de saída da tela para permitir que o avaliador inspecione os resultados. 

## User Stories

1. As a usuário do sistema, I want to cadastrar um Cliente com um ID único, nome de até 255 caracteres e dados de contato, so that o sistema guarde quem está realizando as compras.
2. As a usuário do sistema, I want to cadastrar Produtos com IDs únicos, nomes (até 255 chars) e preços, so that eles possam ser adicionados a pedidos futuros.
3. As a usuário do sistema, I want to registrar um Pedido associando um Cliente a múltiplos Itens (Produtos e quantidades), so that eu possa faturar e registrar a transação comercial.
4. As a usuário do sistema, I want to definir a quantidade de produtos vendidos em cada ItemPedido, so that o sistema calcule o valor total multiplicando pela quantidade e registre o preço unitário congelado no momento da compra.
5. As a usuário do sistema, I want to solicitar uma Devolução, devolvendo integralmente os produtos de um ItemPedido específico, so that o Cliente realize um escambo (sem reembolso monetário da transação original).
6. As a administrador, I want que o sistema cobre automaticamente uma taxa extra de logística reversa de R$ 20,00 a partir da segunda Devolução do mesmo ItemPedido pelo mesmo Cliente, so that eu cubra custos excessivos de transporte.
7. As a professor avaliador, I want to visualizar Relatórios de Vendas Diários, Mensais e Anuais detalhados na tela, so that eu verifique o funcionamento da agregação de dados.
8. As a professor avaliador, I want que a janela do terminal no Windows faça uma pausa após executar uma rotina ou apresentar um relatório, so that eu tenha tempo de ler os outputs sem a tela fechar sozinha.
9. As a desenvolvedor, I want to usar constantes configuráveis (`#define`) para os limites máximos (como 50 unidades de itens ou vendas por dia), so that a coordenação possa mudar esses requisitos facilmente no código se o escopo da disciplina for alterado.
10. As a desenvolvedor, I want to salvar todos os dados usando um formato CSV relacional (arquivos separados com IDs referenciando uns aos outros), so that eu garanta a persistência entre reinícios e mantenha os dados normalizados sem duplicatas.

## Implementation Decisions

- **Persistência Relacional**: Uso de arquivos CSV isolados (`clientes.csv`, `produtos.csv`, `pedidos.csv`, `itens_pedido.csv`, `devolucoes.csv`) para simular o modelo relacional de bancos de dados [ADR-0002].
- **Limites Físicos de Strings**: Variáveis de nome usarão arrays de tamanho estático com `MAX_NOME 255` em vez de ponteiros flexíveis alocados via `malloc`. Isso reduz a sobrecarga computacional de lidar com alocação em disco para strings de tamanhos aleatórios [ADR-0003].
- **Configurações Globais Fixas**: Uso de `#define` (ex: `#define LIMITE_VENDAS_DIA 50`) para cobrir regras de tamanho ainda não 100% clarificadas pela faculdade, em vez de arriscar reprovação ou falta de flexibilidade [ADR-0001].
- **Camada de Compatibilidade Windows**: Implementação de retenções no terminal (como esperar tecla do teclado antes de voltar ao menu principal ou antes do `return 0` na main) [ADR-0004].
- **Regras do Domínio de Devoluções**: Devolução gera "escambo", não gera estorno. A lógica deve buscar na base de dados (ou base CSV) se já existe uma devolução vinculada àquele ItemPedido. Se encontrar registro prévio, a função aplica uma taxa de 20 reais.

## Testing Decisions

Os testes neste ambiente acadêmico se darão por verificação externa do comportamento em vez de rotinas rígidas automatizadas.
- A validação de sucesso ("Good Test") consistirá em realizar operações completas via CLI e cruzar com os dados preenchidos nos arquivos CSV. 
- Módulos Testados: 
  - Crud Base (Cliente e Produto).
  - Gestão de Pedidos (Geração de `pedido` referenciando `itens_pedido`).
  - Fluxo de Devolução (testando a ocorrência da primeira Devolução limpa e a segunda com aplicação de 20 reais de taxa).
  - Geração de Relatórios (Validação de cálculos de lucro em relatórios).
- Táticas de Fronteira: Serão inseridos strings de tamanho acima de 255 chars, bem como inserções numéricas que extrapolem os `#define` de limites de venda, para garantir que o sistema proteja esses *boundaries* e informe o usuário através da camada de Terminal.

## Out of Scope

- Interface Gráfica de Usuário (GUI) e Web (Focado 100% em Terminal CLI C).
- Bancos de dados SQL nativos (SQLite, MySQL, Postgres). Deve-se usar arquivo texto (CSV) sob as mesmas amarras limitadoras de IO.
- Gestão paralela concorrente e *Thread Safeness*. Como se trata de console single player para projeto acadêmico, não haverá mitigação de *race conditions* de concorrência com o mesmo arquivo CSV.
- Logística real de reenvio entre as devoluções (rastreio de pacote, status de entrega - irrelevante para este escopo).

## Further Notes

A aplicação usa o `CONTEXT.md` na raiz para o Glossário e `docs/adr/` para *Architectural Decision Records*. Sempre consultar tais diretórios antes de expandir classes ou *structs*.
