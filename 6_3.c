#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_COMMAND_LENGTH 100
#define MAX_ARGUMENTS 10

int main() {
    while (1) {
        // Ввод команды с клавиатуры
        char input[MAX_COMMAND_LENGTH];
        printf("Введите команду (или 'exit' для выхода): ");
        fgets(input, sizeof(input), stdin);

        // Удаление символа новой строки, добавленного функцией fgets
        input[strcspn(input, "\n")] = 0;

        // Проверка на выход
        if (strcmp(input, "exit") == 0)
            break;

        // Создание массива аргументов
        char *arguments[MAX_ARGUMENTS];
        int arg_count = 0;

        // Разделение строки на аргументы
        char *token = strtok(input, " ");
        while (token != NULL && arg_count < MAX_ARGUMENTS - 1) {
            arguments[arg_count] = token;
            arg_count++;
            token = strtok(NULL, " ");
        }

        arguments[arg_count] = NULL; // Завершающий указатель NULL для execvp

        // Создание нового процесса
        pid_t pid = fork();

        if (pid == -1) {
            perror("Ошибка fork");
            return 1;
        } else if (pid == 0) {
            // Дочерний процесс

            // Выполнение команды с использованием execvp
            if (execvp(arguments[0], arguments) != 0) {
                perror("Ошибка execvp");
                return 1;
            }
        } else {
            // Родительский процесс

            // Ожидание завершения дочернего процесса
            int status;
            if (waitpid(pid, &status, 0) != 0) {
                perror("Ошибка waitpid");
                return 1;
            }

            // Проверка на ошибки выполнения команды
            if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                printf("Команда завершилась с ошибкой. Код возврата: %d\n", WEXITSTATUS(status));
            }
        }
    }

    printf("Программа завершена.\n");
    return 0;
}
