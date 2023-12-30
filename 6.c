#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>

void printDirectoryContents(const char *directoryPath) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(directoryPath);
    if (dir == NULL) {
        perror("Ошибка открытия каталога");
        return;
    }

    printf("Содержимое каталога '%s':\n", directoryPath);
    while (1)
    {
        entry = readdir(dir);
        if (entry == NULL)
            break;
        printf("%s\n", entry->d_name);
    }

    if (closedir(dir) == -1) {
        perror("Ошибка закрытия каталога");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Формат ввода аргументов: %s <путь к каталогу>\n", argv[0]);
        return 1;
    }

    const char *directoryPath = argv[1];

    printf("Содержимое текущего каталога:\n");
    printDirectoryContents(".");

    printDirectoryContents(directoryPath);

    return 0;
}
