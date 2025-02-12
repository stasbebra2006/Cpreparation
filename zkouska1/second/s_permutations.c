#include <stdio.h>
#include <string.h>

// Функция для обмена символов
void swap(char *a, char *b) {
    char temp = *a;
    *a = *b;
    *b = temp;
}

// Рекурсивная функция для генерации перестановок
void generate_permutations(char *str, int start, int end) {
    if (start == end) {
        printf("%s\n", str);
    } else {
        for (int i = start; i <= end; i++) {
            swap(&str[start], &str[i]);
            generate_permutations(str, start + 1, end);
            swap(&str[start], &str[i]); // Возвращаем исходный порядок
        }
    }
}

int main() {
    char original[] = "ABCDE";
    int len = strlen(original);

    // Перебор всех непустых подмножеств с помощью битовых масок
    for (int mask = 1; mask < (1 << len); mask++) {
        char subset[len + 1]; // Буфер для подмножества символов
        int sub_len = 0;

        // Заполнение подмножества на основе маски
        for (int i = 0; i < len; i++) {
            if (mask & (1 << i)) {
                subset[sub_len++] = original[i];
            }
        }
        subset[sub_len] = '\0'; // Завершаем строку

        // Генерация всех перестановок текущего подмножества
        generate_permutations(subset, 0, sub_len - 1);
    }

    return 0;
}