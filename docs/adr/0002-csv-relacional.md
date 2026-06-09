# Persistência em CSV com layout relacional

Decidimos usar um arquivo CSV por entidade (clientes.csv, produtos.csv, pedidos.csv, itens_pedido.csv, devolucoes.csv), seguindo o modelo relacional — cada arquivo é análogo a uma tabela de banco de dados, com referências por ID entre eles.

Alternativa considerada: arquivo único desnormalizado. Descartada porque duplica dados e complica atualizações parciais (ex: mudar preço de um Produto afetaria N linhas). O layout relacional mantém consistência e permite que cada módulo do sistema opere sobre um arquivo isolado.

CSV foi escolhido por requisito do projeto. Trade-off aceito: sem suporte nativo a transações, locks ou tipos — a camada de persistência precisa tratar isso manualmente.
