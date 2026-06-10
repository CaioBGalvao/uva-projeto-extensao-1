#ifndef RELATORIO_H
#define RELATORIO_H

// Interface do módulo de Relatórios (Caio)

// Responsáveis por agregar dados e renderizar em tela (usam arquivo temporário ou padrão)
void relatorio_diario(void);
void relatorio_mensal(void);
void relatorio_anual(void);

// Regras de negócio testáveis: agregam dados e geram relatórios nos caminhos especificados.
// Retornam 0 em caso de sucesso ou código de erro negativo.
int relatorio_diario_gerar(const char* arquivo_saida);
int relatorio_mensal_gerar(const char* arquivo_saida);
int relatorio_anual_gerar(const char* arquivo_saida);

#endif
