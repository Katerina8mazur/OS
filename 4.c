#include <stdio.h>
#include <stdlib.h>

void printUsage(const char* programName) {
    fprintf(stderr, "Формат ввода аргументов: %s <имя файла> <N>\n", programName);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printUsage(argv[0]);
        return 1;
    }

    const char* filename = argv[1]; 
    int N = atoi(argv[2]); 

    FILE* file = fopen(filename, "r"); 

    if (file == NULL) {
        perror("Ошибка открытия файла");
        return 1;
    }

    int ch;
    int lineCount = 0;

    while ((ch = fgetc(file)) != EOF) {
        putchar(ch); 

        if (ch == '\n') { 
            lineCount++;

            if (N > 0 && lineCount % N == 0) {
                printf("Нажмите любую клавишу для продолжения...");
                getchar(); 
                printf("\r"); 
            }
        }
    }

    if (fclose(file) == EOF) {
        perror("Ошибка закрытия файла");
        return 1;
    }

    printf("\n"); 

    return 0;
}
