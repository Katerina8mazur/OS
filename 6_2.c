#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

void printTime() {
    struct timeval timeval;
    gettimeofday(&timeval, NULL);

    printf("%02d:%02d:%02d:%03ld",
           (int)(timeval.tv_sec % 86400) / 3600,
           (int)(timeval.tv_sec % 3600) / 60,
           (int)(timeval.tv_sec % 60),
           timeval.tv_usec / 1000);
}

int main() {
    pid_t pid1, pid2;

    // Создаем первый дочерний процесс
    pid1 = fork();

    if (pid1 == -1) {
        perror("Ошибка fork() для первого процесса");
        return 1;
    } else if (pid1 == 0) {
        // Код для первого дочернего процесса
        printf("Дочерний процесс 1: PID=%d, PPID=%d, Время: ", getpid(), getppid());
        printTime();
        printf("\n");
        return 0;
    }

    // Создаем второй дочерний процесс
    pid2 = fork();

    if (pid2 == -1) {
        perror("Ошибка fork() для второго процесса");
        return 1;
    } else if (pid2 == 0) {
        // Код для второго дочернего процесса
        printf("Дочерний процесс 2: PID=%d, PPID=%d, Время: ", getpid(), getppid());
        printTime();
        printf("\n");
        return 0;
    }

    // Родительский процесс
    // Выводим информацию о родительском процессе
    printf("Родительский процесс: PID=%d, PPID=%d, Время: ", getpid(), getppid());
    printTime();
    printf("\n");

    // Используем system() для выполнения команды ps -x
    system("ps -x");

    // Ждем завершения обоих дочерних процессов
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    return 0;
}