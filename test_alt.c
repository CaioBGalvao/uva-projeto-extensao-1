#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    printf("Line 1\n");
    printf("Line 2\n");
    printf("Press Enter to enter lookup...");
    getchar();

    printf("\033[?1049h\033[H");
    fflush(stdout);

    printf("--- LOOKUP MENU ---\n");
    printf("Result 1\nResult 2\n");
    printf("Press Enter to exit lookup...");
    getchar();

    printf("\033[?1049l");
    fflush(stdout);
    
    printf("Back in main. Enter to quit.\n");
    getchar();
    return 0;
}
