/**
 * @file input.c
 * @author Caio Galvao (Dev 5)
 * @brief Implementacao de funcoes seguras de UX no terminal.
 * 
 * Este arquivo foi documentado conforme o padrao CDoc/Doxygen.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "isocline.h"
#include "input.h"
#include "lookup.h"

/**
 * @brief Executa a operacao de input_configurar_atalhos.
 *
 */
void input_configurar_atalhos(void) {
    // Para manter 100% de compatibilidade entre Windows e Linux sem depender do ncurses,
    // o hook global de atalho foi removido. Agora usamos '?' via texto.
}

/**
 * @brief Executa a operacao de limpar_tela.
 *
 */
void limpar_tela(void) {
#ifdef _WIN32
    system("cls");
#else
    printf("\033[H\033[J");
#endif
}

/**
 * @brief Executa a operacao de ler_string.
 *
 * @param prompt Parametro de entrada.
 * @param buffer Parametro de entrada.
 * @param max_len Parametro de entrada.
 * @return int Retorno da operacao.
 */
int ler_string(const char *prompt, char *buffer, int max_len) {
    while (1) {
        char *input = ic_readline(prompt);
        if (input == NULL) {
            return 0; // EOF
        }
        
        if (strcmp(input, "?") == 0) {
            printf("\033[?1049h\033[H");
            fflush(stdout);
            lookup_menu();
            printf("\033[?1049l");
            fflush(stdout);
            ic_free(input);
            continue;
        }
        
        // Copia para o buffer limitando o tamanho
        strncpy(buffer, input, max_len - 1);
        buffer[max_len - 1] = '\0';
        
        ic_free(input);
        return 1;
    }
}

/**
 * @brief Executa a operacao de ler_string_com_padrao.
 *
 * @param prompt Parametro de entrada.
 * @param buffer Parametro de entrada.
 * @param max_len Parametro de entrada.
 * @param padrao Parametro de entrada.
 * @return int Retorno da operacao.
 */
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

/**
 * @brief Executa a operacao de ler_inteiro.
 *
 * @param prompt Parametro de entrada.
 * @param valor Parametro de entrada.
 * @return int Retorno da operacao.
 */
int ler_inteiro(const char *prompt, int *valor) {
    while (1) {
        char *input = ic_readline(prompt);
        if (input == NULL) return 0;
        
        if (strcmp(input, "?") == 0) {
            printf("\033[?1049h\033[H");
            fflush(stdout);
            lookup_menu();
            printf("\033[?1049l");
            fflush(stdout);
            ic_free(input);
            continue;
        }
        
        int ret = sscanf(input, "%d", valor);
        ic_free(input);
        return (ret == 1) ? 1 : 0;
    }
}

/**
 * @brief Executa a operacao de ler_float.
 *
 * @param prompt Parametro de entrada.
 * @param valor Parametro de entrada.
 * @return int Retorno da operacao.
 */
int ler_float(const char *prompt, float *valor) {
    while (1) {
        char *input = ic_readline(prompt);
        if (input == NULL) return 0;
        
        if (strcmp(input, "?") == 0) {
            printf("\033[?1049h\033[H");
            fflush(stdout);
            lookup_menu();
            printf("\033[?1049l");
            fflush(stdout);
            ic_free(input);
            continue;
        }
        // Permite o uso de vírgula como separador decimal
        for (int i = 0; input[i] != '\0'; i++) {
            if (input[i] == ',') {
                input[i] = '.';
            }
        }
        
        int ret = sscanf(input, "%f", valor);
        ic_free(input);
        return (ret == 1) ? 1 : 0;
    }
}

/**
 * @brief Executa a operacao de ler_char.
 *
 * @param prompt Parametro de entrada.
 * @param valor Parametro de entrada.
 * @return int Retorno da operacao.
 */
int ler_char(const char *prompt, char *valor) {
    while (1) {
        char *input = ic_readline(prompt);
        if (input == NULL) return 0;
        
        if (strcmp(input, "?") == 0) {
            printf("\033[?1049h\033[H");
            fflush(stdout);
            lookup_menu();
            printf("\033[?1049l");
            fflush(stdout);
            ic_free(input);
            continue;
        }
        
        int ret = sscanf(input, " %c", valor);
        ic_free(input);
        return (ret == 1) ? 1 : 0;
    }
}
