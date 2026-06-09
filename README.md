# Sistema de Gestão de Pedidos - Guia de Projeto e Implementação da Equipe

Bem-vindos ao projeto! Este documento detalha a arquitetura, as regras de negócio e a divisão de tarefas da nossa equipe para o desenvolvimento do Sistema de Pedidos em C.

Ele foi desenhado para ser o seu **guia definitivo**. Leia a parte que lhe corresponde com atenção antes de começar a codificar.

---

## 1. Visão Geral do Projeto

O objetivo deste projeto é construir um sistema robusto de gerenciamento de vendas e devoluções para uma loja, rodando inteiramente no terminal. Os dados não serão perdidos ao fechar o programa; eles serão salvos de forma permanente em arquivos de texto (formato CSV), simulando as tabelas de um banco de dados relacional.

A nossa equipe é composta por 5 desenvolvedores. Para que todos possam trabalhar em paralelo sem que o código de um atrapalhe o do outro, adotamos uma **arquitetura estritamente modular**. O esqueleto principal (o menu) já está pronto. O seu trabalho será dar vida à funcionalidade ("inteligência") da opção de menu que lhe foi designada.

## 2. Regras de Negócio e Arquitetura

* **Sem Banco de Dados Nativo:** Cada entidade do sistema (Cliente, Produto, Pedido, etc.) tem seu próprio arquivo CSV. **Vocês não precisarão e não devem manipular os arquivos de texto diretamente no código de vocês**. O Caio disponibilizou uma "API de Persistência" pronta, basta chamar a função certa.
* **Limites Fixos de Memória:** Como parte dos requisitos acadêmicos da disciplina, não lidaremos com alocação dinâmica avançada para coleções. Usaremos constantes globais previamente definidas para travar o limite máximo de arrays temporários.

* **Regra da Devolução Logística:** A devolução no nosso sistema não gera reembolso (é uma troca de item físico).
  * Sempre devolve-se a quantidade *total* daquele item do pedido.
  * A primeira ocorrência de devolução para aquele item não tem custo.
  * A partir da segunda devolução do *mesmo item do mesmo pedido*, dispara a regra da taxa logística e o sistema deve gerar um débito extra de **R$ 20,00** para o cliente.

---

## 3. Divisão de Tarefas da Equipe

A arquitetura do código foi fatiada. Cada um cuida apenas do seu escopo:

* **Caio (Líder / Infraestrutura):** Responsável pelas estruturas de base, loop de sistema, toda a manipulação complexa de CSV (Módulo de Persistência) e pelos Relatórios Analíticos que cruzam tabelas.
* **Dev 1:** Módulo de Clientes
* **Dev 2:** Módulo de Produtos
* **Dev 3:** Módulo de Pedidos (Venda)
* **Dev 4:** Módulo de Devoluções (Logística Reversa)

---

## 4. Guia Didático: O que cada módulo deve fazer?

Abaixo, explicamos o algoritmo lógico que cada um de vocês deve implementar dentro da sua função designada. **Nenhum código C será dado aqui** — o objetivo é que você compreenda a lógica de negócio e traduza isso para a linguagem.

Seu objetivo geral em todos os módulos de negócio é:

1. Fazer perguntas ao usuário via terminal.
2. Validar se a resposta faz sentido.
3. Chamar a persistência para buscar ou salvar.

### Dev 1: Módulo de Cliente

* **Objetivo:** Cadastrar uma nova pessoa autorizada a comprar na loja.
* **Fluxo sugerido:**
    1. Peça ao usuário que digite o nome do cliente.
    2. Solicite à persistência qual será o próximo ID disponível na "tabela" de clientes.
    3. Construa o objeto (sua struct) juntando esse ID e o Nome.
    4. Envie o objeto para a persistência gravar no disco.
    5. Informe na tela, com uma mensagem amigável, que o cliente de ID `X` foi cadastrado com sucesso.

### Dev 2: Módulo de Produto

* **Objetivo:** Cadastrar as mercadorias disponíveis para venda na loja.
* **Fluxo sugerido:**
    1. Solicite o nome do produto e o preço unitário de venda.
    2. Como no cliente, adquira o próximo ID disponível no sistema.
    3. Monte o objeto (sua struct) do Produto.
    4. Envie para persistência salvar.
    5. Informe o sucesso da operação.

### Dev 3: Módulo de Pedido (O Carrinho de Compras)

* **Objetivo:** Orquestrar o ato da venda, vinculando um cliente a uma lista de produtos comprados.
* **Fluxo sugerido:**
    1. Pergunte a data em que a compra está sendo registrada.
    2. Pergunte o ID do Cliente.
        * *Atenção (Validação):* Você deve obrigatoriamente chamar a função de busca da persistência para checar se esse cliente existe. Se não existir, avise o usuário e aborte a operação (ou deixe-o tentar de novo).
    3. Inicie um "carrinho de compras" em memória (um array na struct temporária). Crie um laço de repetição para o usuário adicionar os itens da compra:
        * Solicite o ID do Produto e valide na persistência se a mercadoria existe.
        * Solicite a quantidade. Valide para não aceitar números negativos ou zerados.
        * Calcule o valor matemático total daquela linha específica (Quantidade × Preço Unitário).
        * Adicione o item no seu array de carrinho.
        * Pergunte se o usuário quer adicionar outro produto ou fechar a compra.
    4. Quando a compra fechar, adquira o próximo ID de Pedido do sistema.
    5. Envie a "capa" do pedido (o registro principal contendo o ID e o cliente) para salvar.
    6. Faça um laço `for` percorrendo o seu carrinho em memória e envie cada item, individualmente, para a persistência, garantindo que o `id_pedido` associado a eles seja o mesmo gerado no passo 4.
    7. Ao final, some tudo e imprima na tela um "Recibo de Venda" consolidado.

### Dev 4: Módulo de Devolução (Logística Reversa)

* **Objetivo:** Processar o retorno de um item comprado anteriormente, decidindo quando aplicar a taxa financeira.
* **Fluxo sugerido:**
    1. Pergunte a data da devolução e o ID do Pedido problemático.
    2. Peça à persistência para carregar todos os itens que foram comprados dentro deste pedido.
    3. Apresente essa lista de itens na tela (mostrando o ID do item e a quantidade) para que o usuário possa escolher o que está devolvendo.
    4. Pergunte qual o ID do item ele deseja devolver.
        * *Validação:* Certifique-se que o usuário não digitou um ID maluco; a escolha tem que bater com um item da lista apresentada.
    5. **A Regra de Negócio (O grande desafio da sua tarefa):** Você precisa solicitar à persistência quantas devoluções aquele item específico já sofreu na história da loja.
        * **Lógica base:** Se o número retornado do histórico for zero, significa que é a primeira vez que esse item volta pra loja. Logo, o campo `contagem` vira 1, e a `taxa_cobrada` será R$ 0,00.
        * **Reincidência:** Se o número retornado for 1 ou maior, significa que esta é a *segunda vez (ou mais)* que lidamos com o retorno desse exato pedido. Logo, o campo `contagem` deverá ser o valor histórico + 1, e a `taxa_cobrada` deverá receber o rigoroso valor de R$ 20,00.
    6. Adquira o próximo ID de devolução do sistema.
    7. Monte a estrutura e mande a persistência salvar o registro de devolução.
    8. Imprima o "Comprovante de Devolução" no terminal. Se a taxa de R$ 20,00 foi ativada, imprima um **ALERTA** claro avisando que o valor deve ser debitado do cliente.

### Caio: Módulo de Relatórios (Análise de Dados)

* **Objetivo:** Ler toneladas de dados brutos salvos e compilar relatórios que serão mostrados em tela e gravados de forma permanente em arquivos `.txt`.
* **Relatório Diário:** Pergunta a data, varre o CSV de pedidos, filtra apenas os realizados no dia alvo e totaliza o dinheiro faturado.
* **Relatório Mensal:** Mesma lógica, mas filtrando pelo mês corrente de um dado ano.
* **Relatório Anual (O mais denso):** Pergunta o ano alvo, e agrega todo o faturamento dos 12 meses correspondentes de forma isolada. Por fim, utiliza algoritmos de ordenação nativos do C (como `qsort`) para reordenar o array dos 12 meses e exibi-los no formato de *ranking* do maior para o menor faturamento.

---

## 5. Dicas de Ouro para a Equipe

* **Confiem nos Contratos:** Olhem os arquivos `.h` (cabeçalhos). Lá estão todas as ferramentas que vocês precisam. Se o cabeçalho exige um ponteiro, lembre-se de passar o endereço de memória (`&`).
* **I/O Focado no Usuário:** Nosso sistema roda no terminal, a interface gráfica do projeto é o texto que você imprime. Lembrem-se de formatar bem, usar quebras de linha (`\n`) adequadas e emitir mensagens claras em caso de erros de validação ("Cliente não encontrado. Digite o ID novamente:").
* **Teste sua parte:** Você pode compilar e rodar o projeto desde já escolhendo a sua opção no menu. Se tiver dúvidas, olhe novamente a explicação do seu fluxo neste guia.

**Bom trabalho a todos!**
