#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Формат ввода аргументов: %s <имя файла>\n", argv[0]);
        return 1;
    }

    const char* filename = argv[1]; 
    FILE* file = fopen(filename, "w"); 

    if (file == NULL) { 
        perror("Ошибка открытия файла");
        return 1;
    }

    printf("Введите символы (для выхода используйте Ctrl-F):\n");

    int ch;
    while ((ch = getc(stdin)) != EOF) { 
        if (ch == 6) {  
            break;
        }

        if (putc(ch, file) == EOF) { 
            perror("Ошибка записи в файл");
            fclose(file);
            return 1;
        }
    }

    if (fclose(file) == EOF) {
        perror("Ошибка закрытия файла");
        return 1;
    }

    printf("Данные успешно записаны в файл: %s\n", filename);

    return 0;
}
