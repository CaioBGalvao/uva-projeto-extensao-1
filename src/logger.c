#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include "logger.h"

#define ARQ_LOG "storage/acoes_usuario.log"

void registrar_log(const char* formato, ...) {
    FILE* f = fopen(ARQ_LOG, "a+");
    if (!f) return;

    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    char buffer_data[32];
    strftime(buffer_data, 32, "%Y-%m-%d %H:%M:%S", tm_info);

    fprintf(f, "[%s] ", buffer_data);

    va_list args;
    va_start(args, formato);
    vfprintf(f, formato, args);
    va_end(args);

    fprintf(f, "\n");
    fclose(f);
}
