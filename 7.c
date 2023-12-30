#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_PATH_LENGTH 4096

// Структура для хранения информации о файле
struct FileInfo
{
    char path[MAX_PATH_LENGTH];
    char name[MAX_PATH_LENGTH];
    off_t size;
};

// Сравнение функции для qsort для сортировки по размеру
int compareBySize(const void *a, const void *b)
{
    return ((struct FileInfo *)a)->size - ((struct FileInfo *)b)->size;
}

// Сравнение функции для qsort для сортировки по имени
int compareByName(const void *a, const void *b)
{
    return strcmp(((struct FileInfo *)a)->name, ((struct FileInfo *)b)->name);
}

// Функция для получения полного пути к файлу
void getFullPath(const char *basePath, const char *fileName, char *fullPath)
{
    snprintf(fullPath, MAX_PATH_LENGTH, "%s/%s", basePath, fileName);
}

// Функция для обработки каталога и сохранения файлов в список
void processDirectory(const char *basePath, struct FileInfo **fileList, size_t *countFiles)
{
    DIR *dir;
    struct dirent *entry;

    // Открываем каталог
    dir = opendir(basePath);
    if (dir == NULL)
    {
        perror("Ошибка открытия каталога");
        return;
    }

    while (1)
    {
        entry = readdir(dir);
        if (entry == NULL)
            break;
        
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char fullPath[MAX_PATH_LENGTH];
        getFullPath(basePath, entry->d_name, fullPath);

        struct stat fileStat;
        if (stat(fullPath, &fileStat) != 0)
            continue;

        if (S_ISREG(fileStat.st_mode))
        {
            // Если entry - это файл, добавляем его в список
            *fileList = realloc(*fileList, (*countFiles + 1) * sizeof(struct FileInfo));
            if (fileList == NULL)
            {
                // Обработка ошибки realloc
                perror("Ошибка выделения памяти");
                closedir(dir);
                return;
            }
            strncpy((*fileList)[*countFiles].path, basePath, MAX_PATH_LENGTH);
            strncpy((*fileList)[*countFiles].name, entry->d_name, MAX_PATH_LENGTH);
            (*fileList)[*countFiles].size = fileStat.st_size;
            (*countFiles)++;
        }
        else if (S_ISDIR(fileStat.st_mode))
        {
            // Если entry - это директория, рекурсивно обрабатываем её
            processDirectory(fullPath, fileList, countFiles);
        }
    }

    // Закрываем каталог
    closedir(dir);
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Использование: %s <путь к каталогу> <1 (по размеру) или 2 (по имени)> <имя файла для записи>\n", argv[0]);
        return 1;
    }

    const char *basePath = argv[1];
    int sortBy = atoi(argv[2]);
    const char *outputFileName = argv[3];

    // Проверка валидности входного параметра
    if (sortBy != 1 && sortBy != 2)
    {
        fprintf(stderr, "Неверное значение параметра сортировки. Используйте 1 (по размеру) или 2 (по имени).\n");
        return 1;
    }

    // Создаем список файлов
    struct FileInfo *fileList = NULL;
    size_t countFiles = 0;

    // Обработка каталога
    processDirectory(basePath, &fileList, &countFiles);

    // Сортируем список файлов
    if (sortBy == 1)
        qsort(fileList, countFiles, sizeof(struct FileInfo), compareBySize);
    else if (sortBy == 2)
        qsort(fileList, countFiles, sizeof(struct FileInfo), compareByName);

    // Создаем новый файл для записи
    FILE *outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL)
    {
        perror("Ошибка открытия файла для записи");
        free(fileList);
        return 1;
    }

    // Записываем отсортированные файлы в новый файл и выводим информацию
    for (int i = 0; i < countFiles; i++)
    {
        char fullPath[MAX_PATH_LENGTH];
        getFullPath(fileList[i].path, fileList[i].name, fullPath);
        fprintf(outputFile, "Файл: %s, Размер файла: %ld байт\n", fullPath, (long)fileList[i].size);
        printf("Файл: %s, Размер файла: %ld байт\n", fullPath, (long)fileList[i].size);
    }

    // Закрываем файл
    fclose(outputFile);

    // Освобождаем память
    free(fileList);

    return 0;
}
