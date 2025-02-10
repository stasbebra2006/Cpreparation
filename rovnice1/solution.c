#include <stdio.h>
#include <stdlib.h>

void read_numbers(int *numbers, int *numbersCount, int *error)
{
	printf("write numbers:\n");
	while (1)
	{
		int number = -1;
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

int evaluate(int *numbers, char opsArray[], int indexOfStart, int indexOfEnd)
{
	int result = numbers[indexOfStart];
	for (int opsIndex = indexOfStart; opsIndex < indexOfEnd; opsIndex++)
	{
		if (opsArray[opsIndex] == '+')
		{
			result += numbers[opsIndex + 1];
		}

		if (opsArray[opsIndex] == '-')
		{
			result -= numbers[opsIndex + 1];
		}
	}
	return result;
}

void show_string(int *numbers, int numbersCount, char opsArray[], size_t * comboCount)
{
	//find_index_of_start
	int indexOfStart = -1;
	for (int i = 0; i < numbersCount - 1; i++)
	{
		if (opsArray[i] == '=')
		{
			indexOfStart = i;
			break;
		}
	}
	//

	int left = evaluate(numbers, opsArray, 0, indexOfStart);
	int right = evaluate(numbers, opsArray, indexOfStart + 1, numbersCount - 1);
	if (left == right)
	{
		(*comboCount)++;
		for (int opsIndex = 0; opsIndex < numbersCount; opsIndex++)
		{
			printf("%d", numbers[opsIndex]);
			if (opsIndex < numbersCount - 1)
			{
				printf(" %c ", opsArray[opsIndex]);
			}
		}
		printf("\n");
	}
}

void find_strings(int *numbers, int numbersCount, char opsArray[], int index, size_t * comboCount)
{
	int checkOnlyOne = 0;
	for (int i = 0; i < numbersCount - 1; i++)
	{
		if (opsArray[i] == '=')
			checkOnlyOne++;
	}
	if (checkOnlyOne > 1)
	{
		return;
	}

	if (index == numbersCount - 1)
	{
		show_string(numbers, numbersCount, opsArray, comboCount);
		return;
	}

	opsArray[index] = '+';
	find_strings(numbers, numbersCount, opsArray, index + 1, comboCount);
	opsArray[index] = '-';
	find_strings(numbers, numbersCount, opsArray, index + 1, comboCount);
	opsArray[index] = '=';
	find_strings(numbers, numbersCount, opsArray, index + 1, comboCount);
}

void strings_main(int *numbers, int numbersCount)
{
	size_t comboCount = 0;
	char opsArray[numbersCount - 1];
	find_strings(numbers, numbersCount, opsArray, 0, &comboCount);
	printf("CombosCount: %zu\n", comboCount);
}

int main()
{
	int error = 0;
	int numbersCount = 0;

	int *numbers = (int *)calloc(51, sizeof(int));

	read_numbers(numbers, &numbersCount, &error);
	if (error == 1)
	{
		printf("Nespravny vstup\n");
		free(numbers);
		return 1;
	}
	print_array(numbers, numbersCount);
	strings_main(numbers, numbersCount);
	free(numbers);
	return 0;
}