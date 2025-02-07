#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int compareLargeNumbers(char *num1, char *num2)
{
	while (*num1 == '0')
		num1++;
	while (*num2 == '0')
		num2++;

	int len1 = strlen(num1);
	int len2 = strlen(num2);

	if (len1 > len2)
		return 0;
	if (len1 < len2)
		return 1;

	for (int i = 0; i < len1; i++)
	{
		if (num1[i] > num2[i])
			return 0;
		if (num1[i] < num2[i])
			return 1;
	}

	return 1;
}

size_t get_lastcomma_index(char *current, size_t current_index)
{
	while (current[current_index] != ',' && current_index != 0)
	{
		current_index--;
	}
	return current_index;
}

char *get_numbernow(char *current, size_t current_index)
{
	size_t lastcomma = get_lastcomma_index(current, current_index);
	char *numbernow = NULL;
	size_t size_of_numbernow = 0;
	size_t from = 0;
	if (lastcomma == 0)
	{
		size_of_numbernow = current_index + 1;
		from = 0;
	}
	else
	{
		size_of_numbernow = current_index - lastcomma;
		from = lastcomma + 1;
	}
	numbernow = (char *)calloc((size_of_numbernow + 1), sizeof(char));
	strncpy(numbernow, current + from, size_of_numbernow);
	numbernow[size_of_numbernow] = '\0';
	return numbernow;
}

bool is_even(char *number)
{
	int len = strlen(number);
	int digit = number[len - 1] - '0';
	return digit % 2 == 0;
}

void split_recursive(char *input, size_t input_size, char *current, size_t index, size_t current_index, char *previous_number, size_t *counter, char command)
{
	if (index == input_size)
	{
		(*counter)++;
		current[current_index] = '\0';
		if (command == '?')
		{
			printf("* %s\n", current);
		}
		return;
	}

	current[current_index] = input[index];
	char *number_now = get_numbernow(current, current_index);

	if (index < input_size - 1)
	{
		if ((is_even(previous_number) && compareLargeNumbers(previous_number, number_now)) || !is_even(previous_number))
		{
			current[current_index + 1] = ',';
			split_recursive(input, input_size, current, index + 1, current_index + 2, number_now, counter, command);
		}
	}
	if (index == input_size - 1 && is_even(previous_number) && !compareLargeNumbers(previous_number, number_now))
	{
		free(number_now);
		return;
	}
	split_recursive(input, input_size, current, index + 1, current_index + 1, previous_number, counter, command);
	free(number_now);
}

void split_combinations(char *input, char command)
{
	size_t counter = 0;
	int input_size = strlen(input);
	char *current = (char *)calloc(2 * input_size + 1, sizeof(char));
	current[2 * input_size] = '\0';
	size_t index = 0;
	size_t current_index = 0;
	char *previous_number = strdup("0");
	split_recursive(input, input_size, current, index, current_index, previous_number, &counter, command);
	printf("Celkem: %zu\n", counter);
	free(current);
	free(previous_number);
}

char read_command()
{
	char command = 0;
	int commandReturn = scanf("%c", &command);
	if (commandReturn == -1)
	{
		exit(0);
	}
	if (command != '?' && command != '#')
	{
		printf("Nespravny vstup.\n");
		exit(1);
	}
	return command;
}

void read_sequence(char **sequence, char **digits)
{
	size_t bsize = 0;
	getline(sequence, &bsize, stdin);
	*digits = *sequence;
	while (**digits == ' ' || **digits == '\t')
	{
		(*digits)++;
	}

	size_t lengthOfdigits = strlen(*digits);
	if (lengthOfdigits > 0 && (*digits)[lengthOfdigits - 1] == '\n')
	{
		(*digits)[lengthOfdigits - 1] = '\0';
		lengthOfdigits--;
	}

	if (lengthOfdigits == 0)
	{
		printf("Nespravny vstup.\n");
		free(*sequence);
		exit(1);
	}

	for (size_t i = 0; i < lengthOfdigits; i++)
	{
		if (!isdigit((*digits)[i]))
		{
			printf("Nespravny vstup.\n");
			free(*sequence);
			exit(1);
		}
	}
}

int main()
{
	printf("Posloupnost:\n");
	char command = 0;
	char *digits = NULL;
	char *sequence = NULL;

	while (1)
	{
		command = read_command();
		read_sequence(&sequence, &digits);
		split_combinations(digits, command);
		free(sequence);
		sequence = NULL;
		digits = NULL;
	}

	return 0;
}