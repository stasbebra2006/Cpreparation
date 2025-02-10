#include <stdio.h>
#include <stdlib.h>

void read_numbers(int *numbers, int *numbersCount, int *error)
{
	printf("write numbers:\n");
	while (1)
	{
		int number = 0;
		int check = scanf(" %d", &number);
		if (check == EOF)
		{
			return;
		}
		if (check != 1)
		{
			*error = 1;
			return;
		}
		numbers[*numbersCount] = number;
		(*numbersCount)++;
	}
}

void print_array(int *numbers, int numbersCount)
{
	printf("array: \n");
	for (int i = 0; i < numbersCount; i++)
	{
		printf("%d ", numbers[i]);
	}
	printf("\npocet:\n%d\n", numbersCount);
}

int main()
{
	int error = 0;
	int numbersCount = 0;

	int *numbers = (int *)calloc(50, sizeof(int));

	read_numbers(numbers, &numbersCount, &error);
	if (error == 1)
	{
		printf("Nespravny vstup\n");
		free(numbers);
		return 1;
	}
	print_array(numbers, numbersCount);
	free(numbers);
	// find_strings(numbers);
	return 0;
}