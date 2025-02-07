#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct
{
	char value;
	int isIncluded;
} letter;

int is_next_char_eof()
{
	int c = fgetc(stdin);
	if (c == EOF)
	{
		return 1;
	}
	ungetc(c, stdin);
	return 0;
}

void free_grid(letter **grid, ssize_t rowsInitial)
{
	for (ssize_t i = 0; i < rowsInitial; i++)
	{
		free(grid[i]);
	}
	free(grid);
}

ssize_t directions[8][2] = {
	{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

bool search(letter **grid, ssize_t rows, ssize_t cols, ssize_t row, ssize_t col, char *word, ssize_t dir[2])
{
	size_t lengthOfWord = strlen(word);
	size_t i = 0;
	for (; i < lengthOfWord && word[i]; i++)
	{
		if (row >= rows || row < 0 || col >= cols || col < 0)
		{
			return false;
		}
		if (toupper((unsigned char)word[i]) != toupper((unsigned char)grid[row][col].value))
			return false;

		///
		row = row + dir[0];
		col = col + dir[1];
	}
	return i == lengthOfWord || word[i] == grid[row][col].value;
}

void deleteSymbols(letter **grid, ssize_t rows, ssize_t cols, ssize_t row, ssize_t col, char *word, ssize_t dir[2])
{
	size_t lengthOfWord = strlen(word);
	size_t i = 0;
	for (; i < lengthOfWord && word[i]; i++)
	{
		grid[row][col].isIncluded = false;
		row = row + dir[0];
		col = col + dir[1];
	}
}

size_t countPrime(letter **grid, ssize_t rows, ssize_t cols, char command, char *word)
{
	size_t counter = 0;
	switch (command)
	{
	case '-':
		for (ssize_t row = 0; row < rows; row++)
		{
			for (ssize_t col = 0; col < cols; col++)
			{
				for (int d = 0; d < 8; d++)
				{
					if (search(grid, rows, cols, row, col, word, directions[d]) == true)
					{
						counter++;
						deleteSymbols(grid, rows, cols, row, col, word, directions[d]);
					}
				}
			}
		}
		break;
	case '#':
		for (ssize_t row = 0; row < rows; row++)
		{
			for (ssize_t col = 0; col < cols; col++)
			{
				for (int d = 0; d < 8; d++)
				{
					if (search(grid, rows, cols, row, col, word, directions[d]) == true)
					{
						counter++;
					}
				}
			}
		}
		break;
	}
	return counter;
}

void remove_char(char *str, size_t index)
{
	size_t len = strlen(str);
	for (size_t i = index; i < len; i++)
		str[i] = str[i + 1];
}

void remove_nl(char *str)
{
	size_t l = strlen(str);
	if (l > 0 && (str[l - 1] == '\n'))
		remove_char(str, l - 1);
}

void normalize(char *str)
{
	size_t out = 0, in = 0;
	bool seen_space = true;

	for (; str[in] != '\0'; in++)
	{
		if (!seen_space)
		{
			str[out++] = str[in];
			if (str[in] == ' ' || str[in] == '\t')
				seen_space = true;
		}
		else if (str[in] != ' ' && str[in] != '\t')
		{
			str[out++] = str[in];
			seen_space = false;
		}
	}

	if (out > 0 && (str[out - 1] == ' ' || str[out - 1] == '\t'))
		out--;

	str[out] = '\0';
}

void printTajenka(letter **grid, ssize_t rows, ssize_t cols)
{
	bool newLineTrue = false;
	printf("Tajenka:");
	size_t counter = 0;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (grid[i][j].isIncluded == 1 && islower(grid[i][j].value))
			{
				newLineTrue = true;
			}
		}
	}
	if (!newLineTrue)
	{
		printf("\n");
	}
	if (newLineTrue)
	{
		printf("\n");
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				if (grid[i][j].isIncluded == 1 && islower(grid[i][j].value))
				{
					printf("%c", grid[i][j].value);
					counter++;
					if (counter % 60 == 0)
					{
						printf("\n");
					}
				}
			}
		}
		printf("\n");
	}
}

int main()
{
	printf("Osmismerka:\n");
	ssize_t rowsInitial = 100;
	ssize_t colsInitial = 0;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	read = getline(&line, &len, stdin);
	if (read == -1 || read < 2)
	{
		printf("Nespravny vstup.\n");
		free(line);
		return 1;
	}
	else
	{
		colsInitial = read - 1;
		for (ssize_t i = 0; i < colsInitial; i++)
		{
			if (line[i] != '.' && !islower(line[i]))
			{
				printf("Nespravny vstup.\n");
				free(line);
				return 1;
			}
		}
	}
	letter **grid = (letter **)malloc(rowsInitial * sizeof(letter *));
	for (ssize_t i = 0; i < rowsInitial; i++)
	{
		grid[i] = (letter *)malloc(colsInitial * sizeof(letter));
	}
	for (ssize_t i = 0; i < colsInitial; i++)
	{
		grid[0][i].value = line[i];
		grid[0][i].isIncluded = 1;
	}
	ssize_t rowsActual = 1;
	// čtění gridu
	while (1)
	{
		read = getline(&line, &len, stdin);
		if (read == 1)
		{
			break;
		}
		ssize_t lineTest = read - 1;
		if (lineTest != colsInitial)
		{
			printf("Nespravny vstup.\n");
			free_grid(grid, rowsInitial);
			free(line);
			return 1;
		}
		for (ssize_t i = 0; i < colsInitial; i++)
		{
			if (line[i] != '.' && !islower(line[i]))
			{
				printf("Nespravny vstup.\n");
				free_grid(grid, rowsInitial);
				free(line);
				return 1;
			}
		}
		rowsActual = rowsActual + 1;
		if (rowsActual >= rowsInitial)
		{
			rowsInitial *= 3;
			grid = (letter **)realloc(grid, rowsInitial * sizeof(letter *));
			for (ssize_t i = 0; i < rowsInitial; i++)
			{
				grid[i] = (letter *)realloc(grid[i], colsInitial * sizeof(letter));
			}
		}
		for (ssize_t i = 0; i < colsInitial; i++)
		{
			grid[rowsActual - 1][i].value = line[i];
			grid[rowsActual - 1][i].isIncluded = 1;
		}
	}

	if (rowsActual < 1 || colsInitial < 1)
	{
		printf("Nespravny vstup.\n");
		free_grid(grid, rowsInitial);
		free(line);
		return 1;
	}

	// čtění slov
	char *word = NULL;
	char *actualWordPointer = NULL;
	size_t lenWord = 0;
	ssize_t read2;
	char command;
	while (1)
	{

		int commandReturn = scanf("%c", &command);
		if (commandReturn == EOF)
		{
			break;
		}
		if (command != '#' && command != '?' && command != '-')
		{
			printf("Nespravny vstup.\n");
			free_grid(grid, rowsInitial);
			free(word);
			free(line);
			return 1;
		}
		if (command == '?')
		{
			read2 = getline(&word, &lenWord, stdin);
			if (read2 > 1)
			{
				printf("Nespravny vstup.\n");
				free_grid(grid, rowsInitial);
				free(word);
				free(line);
				return 1;
			}
		}
		// if (command == '#' || command == '-')
		// {
		// 	read2 = getline(&word, &lenWord, stdin);
		// 	if (read2 == -1)
		// 	{
		// 		printf("Nespravny vstup.\n");
		// 		free_grid(grid, rowsInitial);
		// 		free(word);
		// 		free(line);
		// 		return 1;
		// 	}
		// 	remove_nl(word);
		// 	normalize(word);
		// 	if (strlen(word) <= 1)
		// 	{
		// 		printf("Nespravny vstup.\n");
		// 		free_grid(grid, rowsInitial);
		// 		free(word);
		// 		free(line);
		// 		return 1;
		// 	}
		// }
		if (command == '#' || command == '-')
		{
			read2 = getline(&word, &lenWord, stdin);
			actualWordPointer = word;
			while (*actualWordPointer == ' ' || *actualWordPointer == '\t')
			{
				actualWordPointer++;
			}
			remove_nl(actualWordPointer);
			size_t wordsize = strlen(actualWordPointer);
			if (wordsize <= 1)
			{
				printf("Nespravny vstup.\n");
				free_grid(grid, rowsInitial);
				free(line);
				free(word);
				return 1;
			}
			for (size_t i = 0; i < wordsize; i++)
			{
				if (islower(actualWordPointer[i]) == false)
				{
					printf("Nespravny vstup.\n");
					free_grid(grid, rowsInitial);
					free(line);
					free(word);
					return 1;
				}
			}
		}
		size_t resultCount;
		switch (command)
		{
		case '-':
		case '#':
			resultCount = countPrime(grid, rowsActual, colsInitial, command, actualWordPointer);
			printf("%.*s: %zux\n", (int)strlen(actualWordPointer), actualWordPointer, resultCount);
			break;
		case '?':
			printTajenka(grid, rowsActual, colsInitial);
			break;
		}
	}
	free_grid(grid, rowsInitial);
	free(word);
	free(line);
	return 0;
}