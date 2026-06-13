#!/bin/bash
# Copia dados do mock para o banco de dados principal
cp database_mock/*.csv database/

# Remove os metadados antigos para forçar a recriação (fallback O(n)) no próximo uso
rm -f metadata/*.meta

echo "Banco de dados preenchido com dados de mock (seed) com sucesso!"
