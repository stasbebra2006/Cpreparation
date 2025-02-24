#include <stdio.h>

#define DEFAULT_CAPACITY 10

int main () {
    printf("Zadejte cisla:\n");
    int res, number, count = 0;
	int capacity = DEFAULT_CAPACITY;
    int * numbers = new int[capacity];
    while((res = scanf("%d", &number)) == 1) {
        if(count >= capacity)
		{
			capacity *= 2;
			int * tmp = new int[capacity];
			for(int i = 0; i < count; i++)
			{
				tmp[i] = numbers[i];
			}
			delete[] numbers;
			numbers = tmp;
		}
		numbers[count++] = number;
    }
    if(res != EOF) {
        printf("Nespravny vstup.\n");
		delete[] numbers;
		return 1;
    } else {
        printf("Zadana cisla:\n");
        for(int i = count - 1; i >= 0; i--)
		{
            printf("%d%c", numbers[i], i > 0 ? ' ' : '\n');
		}
    }
	delete[] numbers;
    return 0;
}
