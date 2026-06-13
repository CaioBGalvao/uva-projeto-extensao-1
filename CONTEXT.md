# Sistema de Pedidos de Loja

Sistema de gestão de pedidos, produtos e clientes para uma loja, com relatórios de vendas diários, mensais e anuais. Implementado em C com persistência em CSV.

## Language

**Pedido**:
Agrupamento de itens comprados por um Cliente numa data específica. Possui ID único, data, referência ao Cliente e uma lista de Itens.
_Avoid_: Compra, venda, transação, order

**ItemPedido**:
Uma linha dentro de um Pedido: referencia um Produto, quantidade adquirida, preço unitário no momento da compra, e valor total (preço × quantidade).
_Avoid_: Item, linha, detalhe

**Produto**:
Entidade cadastrada com ID único, nome e preço unitário.
_Avoid_: Mercadoria, artigo

**Cliente**:
Pessoa ou organização que realiza Pedidos. Possui ID único, nome e dados de contato.
_Avoid_: Comprador, consumidor, account

**Devolução**:
Troca física de produto, sem reembolso. O valor do Pedido original permanece faturado. Sempre devolve a quantidade total do ItemPedido. A partir da segunda Devolução do mesmo ItemPedido, cobra-se taxa fixa de R$ 20,00 como débito extra ao Cliente (logística reversa). O sistema não rastreia reenvios entre devoluções.
_Avoid_: Retorno, reembolso, estorno

## Regras e Limites

- **Limites de Volume**: Não há limites de itens por pedido ou pedidos por dia definidos pelas regras de negócio. Implementação deve suportar grandes volumes (usamos arrays estáticos com limites arbitrariamente altos, ex: 9999, devido à restrição acadêmica do uso de arrays em C).
