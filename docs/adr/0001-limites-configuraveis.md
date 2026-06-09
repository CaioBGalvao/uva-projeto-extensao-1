# Limites do sistema como constantes configuráveis

O requisito menciona "50 vendas por dia" sem definir claramente a qual dimensão se aplica. A coordenação ainda não esclareceu. Existem três interpretações possíveis:

1. 50 pedidos por dia por loja
2. 50 itens por pedido
3. 50 unidades por item por pedido

Decidimos usar `#define` para todas as três dimensões, com valor padrão 50. Quando a coordenação definir o significado exato, o programador ajusta a constante correspondente sem alterar structs ou lógica.

Alternativa considerada: alocação dinâmica com `malloc`. Descartada porque o requisito obrigatório exige "array" (implica tamanho fixo) e o escopo acadêmico favorece simplicidade.
