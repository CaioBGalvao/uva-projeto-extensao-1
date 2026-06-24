#ifndef INPUT_H
#define INPUT_H

/**
 * Limpa o console para simular uma janela fixa.
 */
void limpar_tela(void);

/**
 * Configura atalhos de teclado (Ctrl+B) para busca.
 */
void input_configurar_atalhos(void);

/**
 * Lê uma string do teclado usando o readline para suportar edição avançada (setas, etc).
 * 
 * @param prompt Mensagem exibida antes da leitura.
 * @param buffer Buffer onde a string será armazenada.
 * @param max_len Tamanho máximo do buffer.
 * @return 1 se leu com sucesso, 0 se EOF ou erro.
 */
int ler_string(const char *prompt, char *buffer, int max_len);

/**
 * Lê uma string do teclado. Se o usuário der enter direto (vazio), o buffer é preenchido com o padrao
 * e o padrão é impresso na tela ao lado do prompt.
 */
int ler_string_com_padrao(const char *prompt, char *buffer, int max_len, const char *padrao);

/**
 * Lê um número inteiro do teclado.
 * 
 * @param prompt Mensagem exibida antes da leitura.
 * @param valor Ponteiro para armazenar o valor lido.
 * @return 1 se leu com sucesso, 0 se entrada inválida ou erro.
 */
int ler_inteiro(const char *prompt, int *valor);

/**
 * Lê um número de ponto flutuante do teclado.
 * 
 * @param prompt Mensagem exibida antes da leitura.
 * @param valor Ponteiro para armazenar o valor lido.
 * @return 1 se leu com sucesso, 0 se entrada inválida ou erro.
 */
int ler_float(const char *prompt, float *valor);

/**
 * Lê um caractere do teclado ignorando espaços em branco no início.
 * 
 * @param prompt Mensagem exibida antes da leitura.
 * @param valor Ponteiro para armazenar o caractere.
 * @return 1 se leu com sucesso, 0 se entrada inválida ou erro.
 */
int ler_char(const char *prompt, char *valor);

#endif // INPUT_H
