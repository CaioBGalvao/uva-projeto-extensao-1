# Limites do sistema como constantes configuráveis

O professor informou que a restrição de 50 itens foi removida, portanto não há mais limites de volume no sistema.

No entanto, o requisito obrigatório do projeto exige o uso de "array" (vetores de tamanho fixo em C) e o escopo acadêmico favorece simplicidade, desencorajando o uso de alocação dinâmica.

Decidimos manter o uso de `#define` para as dimensões, porém com um valor limite seguro e amplo (`9999`), simulando a ausência de limites práticos para as avaliações.
