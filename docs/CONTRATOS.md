# Guia de Contratos do Sistema de Pedidos

Este documento especifica o comportamento exato de todas as funções do sistema. Todos os desenvolvedores devem aderir rigorosamente a estas assinaturas e regras de negócio para garantir a coesão do projeto.

---

## 1. Módulo de Persistência (`persistencia.h`) - Desenvolvido por Caio

Este módulo encapsula as leituras e escritas nos arquivos CSV. **Os outros desenvolvedores não devem interagir diretamente com arquivos.** Todas as leituras e gravações devem passar por esta API.

Todas as funções de inserção e busca retornam `0` em caso de sucesso e um valor negativo (como `-1`) em caso de erro (ex: arquivo corrompido, ID inexistente, limites excedidos).

### `int csv_obter_proximo_id(const char* arquivo)`
- **Objetivo**: Lê o arquivo CSV informado e retorna o maior ID cadastrado incrementado em 1. Se o arquivo não existir ou estiver vazio, retorna `1`.
- **Exemplo de uso**: `int novo_id = csv_obter_proximo_id("dados/clientes.csv");`

### `int csv_inserir_cliente(const Cliente* c)`
- **Objetivo**: Abre `dados/clientes.csv` em modo *append* (`a`) e grava uma nova linha: `id,nome`.

### `int csv_buscar_cliente_por_id(int id, Cliente* resultado)`
- **Objetivo**: Abre `dados/clientes.csv` para leitura, localiza a linha com o `id` fornecido, preenche a struct apontada por `resultado` e retorna `0`. Se não encontrar, retorna `-1`.

### `int csv_inserir_produto(const Produto* p)`
- **Objetivo**: Grava no arquivo `dados/produtos.csv` a linha: `id,nome,preco`.

### `int csv_buscar_produto_por_id(int id, Produto* resultado)`
- **Objetivo**: Busca em `dados/produtos.csv` pelo `id`, preenchendo a struct apontada por `resultado`. Retorna `-1` se o produto não existir.

### `int csv_inserir_pedido(const Pedido* p)`
- **Objetivo**: Grava no arquivo `dados/pedidos.csv` a linha: `id,id_cliente,data`.

### `int csv_inserir_item_pedido(const ItemPedido* ip)`
- **Objetivo**: Grava no arquivo `dados/itens_pedido.csv` a linha: `id,id_pedido,id_produto,quantidade,preco_unitario,valor_total`.

### `int csv_listar_itens_por_pedido(int id_pedido, ItemPedido* resultados, int max_resultados, int* qtd_encontrada)`
- **Objetivo**: Busca no arquivo `dados/itens_pedido.csv` todas as linhas que possuam `id_pedido` igual ao fornecido. Preenche o array `resultados` (respeitando o limite `max_resultados`) e armazena a quantidade de itens encontrados em `qtd_encontrada`.

### `int csv_ler_todos_pedidos(Pedido* resultados, int max_resultados, int* qtd_encontrada)`
- **Objetivo**: Lê todo o arquivo `dados/pedidos.csv` e carrega no array `resultados` para permitir filtragem posterior pelos relatórios.

### `int csv_inserir_devolucao(const Devolucao* d)`
- **Objetivo**: Grava no arquivo `dados/devolucoes.csv` a linha: `id,id_item_pedido,data,contagem,taxa_cobrada`.

### `int csv_contar_devolucoes_item(int id_item_pedido)`
- **Objetivo**: Percorre `dados/devolucoes.csv` contando quantas ocorrências de devolução existem para o `id_item_pedido` fornecido. Retorna esse número inteiro (ex: se já existem 0, retorna 0; se já existem 1, retorna 1).

---

## 2. Módulos de Negócio (Desenvolvidos pela Equipe)

Estes módulos são responsáveis pela interface de texto com o usuário (I/O) e por aplicar validações básicas de negócio antes de chamar a persistência.

### Módulo de Cliente (`cliente.h` - Dev 1)

#### `void cliente_cadastrar(void)`
1. Pergunta o nome do cliente ao usuário via `scanf` (respeitando o limite `MAX_NOME`).
2. Obtém o próximo ID chamando `csv_obter_proximo_id("dados/clientes.csv")`.
3. Preenche a struct `Cliente`.
4. Invoca `csv_inserir_cliente`. Em caso de sucesso, exibe mensagem confirmando o ID do cliente criado.

### Módulo de Produto (`produto.h` - Dev 2)

#### `void produto_cadastrar(void)`
1. Pergunta o nome do produto e o preço unitário.
2. Obtém o próximo ID chamando `csv_obter_proximo_id("dados/produtos.csv")`.
3. Preenche a struct `Produto`.
4. Invoca `csv_inserir_produto`. Em caso de sucesso, exibe mensagem confirmando o ID do produto criado.

### Módulo de Pedido (`pedido.h` - Dev 3)

#### `void pedido_registrar(void)`
1. Solicita a data do pedido (formato: `AAAA-MM-DD`).
2. Solicita o ID do cliente.
   - **Validação**: Deve chamar `csv_buscar_cliente_por_id`. Se retornar erro, informa que o cliente não existe e aborta a operação.
3. Inicia um loop para adicionar itens ao pedido (máximo de `MAX_ITENS_PEDIDO` itens):
   - Solicita o ID do produto.
     - **Validação**: Chama `csv_buscar_produto_por_id`. Se não existir, avisa o usuário e pede outro ID.
   - Solicita a quantidade.
     - **Validação**: Deve ser maior que 0 e menor ou igual a `MAX_QTD_ITEM`.
   - Preenche os campos do `ItemPedido` (calculando `valor_total = quantidade * preco_unitario`).
   - Pergunta se o usuário deseja adicionar mais itens.
4. Obtém o próximo ID do pedido chamando `csv_obter_proximo_id("dados/pedidos.csv")`.
5. Salva o cabeçalho do pedido chamando `csv_inserir_pedido`.
6. Grava cada item do pedido no arquivo chamando `csv_inserir_item_pedido`.
7. Imprime na tela o resumo do pedido registrado com seu respectivo valor total consolidado.

### Módulo de Devolução (`devolucao.h` - Dev 4)

#### `void devolucao_registrar(void)`
1. Solicita a data da devolução (`AAAA-MM-DD`).
2. Solicita o ID do pedido que gerou a devolução.
3. Carrega os itens deste pedido usando `csv_listar_itens_por_pedido`.
   - Se o pedido não tiver itens ou não existir, avisa o usuário e aborta.
4. Exibe a lista de itens do pedido na tela (ID do ItemPedido, Nome do Produto, Quantidade).
5. Solicita que o usuário selecione o ID do ItemPedido a ser devolvido.
   - **Validação**: O ID escolhido deve constar na lista de itens daquele pedido.
6. Chama `csv_contar_devolucoes_item` para verificar o histórico deste item específico:
   - Se retornar `0`, significa que é a **primeira devolução**: `contagem = 1` e `taxa_cobrada = 0.00`.
   - Se retornar `1` ou mais, significa que é a **segunda devolução** ou superior: `contagem = contagem_anterior + 1` e `taxa_cobrada = 20.00` (débito extra).
7. Obtém o próximo ID da devolução chamando `csv_obter_proximo_id("dados/devolucoes.csv")`.
8. Preenche a struct `Devolucao` e a grava chamando `csv_inserir_devolucao`.
9. Exibe mensagem de confirmação. Se houver taxa de R$ 20.00 cobrada, emite um alerta explícito na tela para cobrança do cliente.

---

## 3. Módulo de Relatórios (`relatorio.h`) - Desenvolvido por Caio

Este módulo lê os arquivos correspondentes e gera a saída em tela mais a gravação de arquivos texto de relatório na pasta `dados/` (ex: `dados/relatorio_diario.txt`).

- **Diário**: Recebe uma data (ou assume hoje) -> lista pedidos daquela data (ID, Cliente, Valor Total do Pedido) -> Exibe total acumulado no dia.
- **Mensal**: Recebe mês/ano -> lista total vendido por mês e total acumulado.
- **Anual**: Recebe o ano -> agrega faturamento de todos os 12 meses -> ordena os meses por faturamento decrescente (utilizando `qsort` nativo do C) e exibe a listagem ordenada.
