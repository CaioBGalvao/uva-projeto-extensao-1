# Tamanho máximo para campos de nome

Decidimos usar o limite `MAX_NOME 255` em vez de alocação dinâmica infinita para strings como o nome de um Cliente ou Produto. 

Isso simula o comportamento de um banco de dados relacional clássico (como `VARCHAR(255)`) e simplifica o armazenamento nas `structs` e na persistência em CSV.

Alternativa considerada: Ponteiros dinâmicos para strings de tamanho variável. Descartada pois exige mais processamento para serialização/deserialização e alocação de memória (malloc/free), o que foge do escopo do projeto acadêmico atual.
