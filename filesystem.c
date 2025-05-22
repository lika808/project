#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Функция 1.1: открыть (или создать и открыть) файл файловой системы
FILE* fs_open_or_create(const char *fs_path) {
    FILE *f = fopen(fs_path, "r+");
    if (!f) {
        // Если файл не существует, создаём новый
        f = fopen(fs_path, "w+");
        if (!f) {
            perror("Ошибка при создании файла файловой системы");
            return NULL;
        }
    }
    return f;
}

// Функция 1.2: получить содержимое файла из файловой системы по имени
// fs_content - строка с содержимым всей файловой системы
// filename - имя файла, который нужно найти
// Возвращает динамически выделенную строку с содержимым файла или NULL, если файл не найден
char* fs_get_file_content(const char *fs_content, const char *filename) {
    if (!fs_content || !filename) return NULL;

    // Разбиваем fs_content на строки
    char *fs_copy = strdup(fs_content);
    if (!fs_copy) return NULL;

    char *line = NULL;
    char *saveptr = NULL;
    char *result = NULL;
    int found = 0;

    line = strtok_r(fs_copy, "\n", &saveptr);
    while (line) {
        if (found) {
            // Если встретили следующую строку с именем файла (начинается с '/'), значит файл закончился
            if (line[0] == '/') break;

            // Добавляем строку к результату с переносом строки
            size_t old_len = result ? strlen(result) : 0;
            size_t line_len = strlen(line);
            char *new_res = realloc(result, old_len + line_len + 2); // +1 для '\n' +1 для '\0'
            if (!new_res) {
                free(result);
                free(fs_copy);
                return NULL;
            }
            result = new_res;
            memcpy(result + old_len, line, line_len);
            result[old_len + line_len] = '\n';
            result[old_len + line_len + 1] = '\0';
        } else if (strcmp(line, filename) == 0) {
            // Нашли имя файла
            found = 1;
            result = strdup("");
            if (!result) {
                free(fs_copy);
                return NULL;
            }
        }
        line = strtok_r(NULL, "\n", &saveptr);
    }

    free(fs_copy);

    if (!found) {
        free(result);
        return NULL;
    }

    return result;
}

// Функция 1.3: удалить файл из файловой системы
// fs_content - строка с содержимым всей файловой системы
// filename - имя файла, который нужно удалить
// Возвращает динамически выделенную строку с обновлённым содержимым файловой системы
// или NULL при ошибке
char* fs_delete_file(const char *fs_content, const char *filename) {
    if (!fs_content || !filename) return NULL;

    char *fs_copy = strdup(fs_content);
    if (!fs_copy) return NULL;

    char *line = NULL;
    char *saveptr = NULL;
    char *new_fs = NULL;
    size_t new_fs_len = 0;

    int skipping = 0; // флаг, что мы пропускаем строки файла для удаления

    line = strtok_r(fs_copy, "\n", &saveptr);
    while (line) {
        if (skipping) {
            // Если встретили новую строку с именем файла — перестаём пропускать
            if (line[0] == '/') {
                skipping = 0;
            } else {
                line = strtok_r(NULL, "\n", &saveptr);
                continue; // пропускаем эту строку
            }
        }

        if (!skipping && strcmp(line, filename) == 0) {
            skipping = 1;
            line = strtok_r(NULL, "\n", &saveptr);
            continue; // пропускаем имя файла для удаления
        }

        // Добавляем строку к new_fs
        size_t line_len = strlen(line);
        char *tmp = realloc(new_fs, new_fs_len + line_len + 2); // +1 для '\n' +1 для '\0'
        if (!tmp) {
            free(new_fs);
            free(fs_copy);
