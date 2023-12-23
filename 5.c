#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

void printUsage(const char* programName) {
    fprintf(stderr, "Формат ввода аргументов: %s <исходный файл> <файл назначения>\n", programName);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printUsage(argv[0]);
        return 1;
    }

    const char* sourceFilename = argv[1];
    const char* destinationFilename = argv[2];

    FILE* sourceFile = fopen(sourceFilename, "r");
    if (sourceFile == NULL) {
        perror("Ошибка открытия исходного файла");
        return 1;
    }

    FILE* destinationFile = fopen(destinationFilename, "w");
    if (destinationFile == NULL) {
        perror("Ошибка открытия файла назначения");
        fclose(sourceFile);
        return 1;
    }

    struct stat sourceFileStat;
    if (stat(sourceFilename, &sourceFileStat) == -1) {
        perror("Ошибка получения информации о файле");
        fclose(sourceFile);
        fclose(destinationFile);
        return 1;
    }

    if (chmod(destinationFilename, sourceFileStat.st_mode) == -1) {
        perror("Ошибка установки прав доступа для файла назначения");
        fclose(sourceFile);
        fclose(destinationFile);
        return 1;
    }

    int ch;
    while ((ch = fgetc(sourceFile)) != EOF) {
        if (fputc(ch, destinationFile) == EOF) {
            perror("Ошибка записи в файл назначения");
            fclose(sourceFile);
            fclose(destinationFile);
            return 1;
        }
    }

    if (fclose(sourceFile) == EOF || fclose(destinationFile) == EOF) {
        perror("Ошибка закрытия файлов");
        return 1;
    }

    printf("Копирование успешно завершено.\n");

    return 0;
}
