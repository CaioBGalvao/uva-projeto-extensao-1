#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "input.h"
#include "lookup.h"

static int input_handle_lookup(int count, int key) {
    (void)count;
    (void)key;
    
    // Clear readline state temporarily
    rl_clear_visible_line();
    
    // Restore normal terminal state so scanf works
    rl_deprep_terminal();
    
    // Run the lookup UI inline
    lookup_menu();
    
    // Put terminal back in readline raw mode
    rl_prep_terminal(1);
    
    // Force redraw of the prompt exactly as it was
    limpar_tela();
    rl_on_new_line();
    rl_forced_update_display();
    return 0;
}

void input_configurar_atalhos(void) {
    rl_bind_key('\002', input_handle_lookup); // Ctrl+B
}

void limpar_tela(void) {
#ifdef _WIN32
    system("cls");
#else
    printf("\033[H\033[J");
#endif
}

int ler_string(const char *prompt, char *buffer, int max_len) {
    char *input = readline(prompt);
    if (input == NULL) {
        return 0; // EOF
    }
    
    // Adiciona ao histórico caso não seja vazio
    if (strlen(input) > 0) {
        add_history(input);
    }
    
    // Copia para o buffer limitando o tamanho
    strncpy(buffer, input, max_len - 1);
    buffer[max_len - 1] = '\0';
    
    free(input);
    return 1;
}

int ler_string_com_padrao(const char *prompt, char *buffer, int max_len, const char *padrao) {
    if (!ler_string(prompt, buffer, max_len)) return 0;
    
    if (buffer[0] == '\0' && padrao != NULL) {
        // ANSI escape code: \033[1A move o cursor para a linha anterior
        // \033[<N>C avança o cursor <N> colunas (tamanho do prompt)
        printf("\033[1A\033[%dC%s\n", (int)strlen(prompt), padrao);
        strncpy(buffer, padrao, max_len - 1);
        buffer[max_len - 1] = '\0';
    }
    return 1;
}

int ler_inteiro(const char *prompt, int *valor) {
    char *input = readline(prompt);
    if (input == NULL) return 0;
    
    if (strlen(input) > 0) {
        add_history(input);
    }
    
    int ret = sscanf(input, "%d", valor);
    free(input);
    return (ret == 1) ? 1 : 0;
}

int ler_float(const char *prompt, float *valor) {
    char *input = readline(prompt);
    if (input == NULL) return 0;
    
    if (strlen(input) > 0) {
        add_history(input);
    }
    
    int ret = sscanf(input, "%f", valor);
    free(input);
    return (ret == 1) ? 1 : 0;
}

int ler_char(const char *prompt, char *valor) {
    char *input = readline(prompt);
    if (input == NULL) return 0;
    
    if (strlen(input) > 0) {
        add_history(input);
    }
    
    int ret = sscanf(input, " %c", valor);
    free(input);
    return (ret == 1) ? 1 : 0;
}
