# Requisitos do Projeto

## 1. Escopo e Funcionalidades

O sistema deverá contemplar o registro e processamento dos seguintes itens:

- **Produto:** Registro do produto e seu respectivo valor (preço).
- **Cliente:** Registro dos dados do cliente.
- **Pedido:**
  - Registro da quantidade adquirida de produtos.
  - Cálculo do valor total de cada item do pedido.

## 2. Regras de Negócio

O programa deverá considerar as seguintes regras em seu processamento:

- **Taxa de Devolução:** Caso algum item do pedido seja devolvido pela **segunda vez**, deverá ser cobrada uma taxa fixa de **R$ 20,00**, referente às despesas da transportadora.

## 3. Relatórios Obrigatórios

O programa deverá ser capaz de gerar os seguintes relatórios:

### 3.1. Relatório Diário

Uma listagem contendo:

- Os pedidos realizados no dia;
- Os respectivos valores de cada pedido;
- O valor total consolidado das vendas diárias.

### 3.2. Relatório Mensal

Uma listagem mensal contendo:

- O total vendido em cada mês;
- O valor total consolidado das vendas mensais.

### 3.3. Relatório Anual

O programa deverá:

- Calcular o total das vendas realizadas ao longo do ano;
- Apresentar os meses em **ordem decrescente de faturamento**, iniciando pelo mês com maior valor vendido até o de menor valor.

## 5. Requisitos Técnicos (Obrigatório)

O desenvolvimento da solução deverá seguir rigorosamente as especificações abaixo:

- **Linguagem:** Implementar o pseudocódigo (solução) na linguagem **C**.
- **Estruturas Técnicas:** É obrigatório utilizar:
  - `struct` (registros);
  - `array` (vetores);
  - Algoritmo de ordenação;
  - Funções.
- **Testes:** Apresentar os testes realizados, ou seja:
  - Inserir e documentar os valores de entrada utilizados;
  - Apresentar os respectivos resultados processados (saída).
