#ifndef DATA_UTILS_H
#define DATA_UTILS_H

#include <time.h>
#include <string.h>
#include <stdio.h>

// tipo: 0 = completo (YYYY-MM-DD / DD/MM/AAAA)
//       1 = mensal (YYYY-MM / MM/AAAA)
//       2 = anual (YYYY / AAAA)

static inline void obter_data_atual(char* buffer, int tipo) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    if (tipo == 0) {
        sprintf(buffer, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    } else if (tipo == 1) {
        sprintf(buffer, "%04d-%02d", tm.tm_year + 1900, tm.tm_mon + 1);
    } else if (tipo == 2) {
        sprintf(buffer, "%04d", tm.tm_year + 1900);
    }
}

static inline void converter_data_br_para_iso(const char* br, char* iso, int tipo) {
    if (tipo == 0) {
        if (strlen(br) == 10 && br[2] == '/' && br[5] == '/') {
            sprintf(iso, "%c%c%c%c-%c%c-%c%c", br[6], br[7], br[8], br[9], br[3], br[4], br[0], br[1]);
        } else {
            strcpy(iso, br);
        }
    } else if (tipo == 1) {
        if (strlen(br) == 7 && br[2] == '/') {
            sprintf(iso, "%c%c%c%c-%c%c", br[3], br[4], br[5], br[6], br[0], br[1]);
        } else {
            strcpy(iso, br);
        }
    } else {
        strcpy(iso, br);
    }
}

static inline void converter_data_iso_para_br(const char* iso, char* br, int tipo) {
    if (tipo == 0) {
        if (strlen(iso) == 10 && iso[4] == '-' && iso[7] == '-') {
            sprintf(br, "%c%c/%c%c/%c%c%c%c", iso[8], iso[9], iso[5], iso[6], iso[0], iso[1], iso[2], iso[3]);
        } else {
            strcpy(br, iso);
        }
    } else if (tipo == 1) {
        if (strlen(iso) == 7 && iso[4] == '-') {
            sprintf(br, "%c%c/%c%c%c%c", iso[5], iso[6], iso[0], iso[1], iso[2], iso[3]);
        } else {
            strcpy(br, iso);
        }
    } else {
        strcpy(br, iso);
    }
}

#endif
