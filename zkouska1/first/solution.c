#include <stdio.h>
#include <stdlib.h>

typedef struct position
{
	int x;
	int y;
} position;

int find_min(int W, int H)
{
	return (W < H) ? W : H;
}

void read_WH(int *W, int *H, int *error)
{
	printf("Napiste mapu\n");
	int test = scanf(" %d %d", W, H);
	if (test != 2)
	{
		*error = 1;
		return;
	}
}

int **initialize_grid(int W, int H)
{
	int **grid = (int **)calloc(H, sizeof(int *));
	for (int i = 0; i < H; i++)
	{
		grid[i] = (int *)calloc(W, sizeof(int));
	}
	return grid;
}

void free_grid(int **grid, int H)
{
	for (int i = 0; i < H; i++)
	{
		free(grid[i]);
	}
	free(grid);
}

void read_grid(int **grid, int W, int H, int *error)
{
	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
		{
			int DEI = 0;
			int test = scanf(" %d", &DEI);
			if (test != 1)
			{
				*error = 1;
				return;
			}
			grid[i][j] = DEI;
		}
	}
}

void print_grid(int **grid, int W, int H)
{
	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
		{
			printf("%d	", grid[i][j]);
		}
		printf("\n");
	}
}

void find_biggest_space(int **grid, int W, int H, int DEI)
{
	int **bin = initialize_grid(W, H);
	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
		{
			bin[i][j] = (grid[i][j] >= DEI) ? 1 : 0;
		}
	}
	int **dp = initialize_grid(W, H);
	int maxSquare = 0;
	int countPositions = 0;
	position *positions = NULL;
	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
		{
			if (i == 0 || j == 0 || bin[i][j] == 0)
			{
				dp[i][j] = bin[i][j];
			}
			else
			{
				int up = dp[i - 1][j];
				int left = dp[i][j - 1];
				int diagonal = dp[i - 1][j - 1];
				dp[i][j] = 1 + find_min(find_min(up, left), diagonal);
			}
			if (dp[i][j] > maxSquare)
			{
				maxSquare = dp[i][j];
				free(positions);
				positions = NULL;
				countPositions = 0;
			}
			if (dp[i][j] == maxSquare)
			{
				int currentX = i - dp[i][j] + 1;
				int currentY = j - dp[i][j] + 1;
				countPositions++;
				positions = (position *)realloc(positions, countPositions * sizeof(position));
				positions[countPositions - 1].x = currentX;
				positions[countPositions - 1].y = currentY;
			}
		}
	}
	//
	// printf("DEI is %d\n", DEI);
	// printf("bin:\n");
	// print_grid(bin,W,H);
	// printf("dp:\n");
	// print_grid(dp,W,H);
	//
	printf("The biggest square is %dx%d\n", maxSquare, maxSquare);
	for (int i = 0; i < countPositions; i++)
	{
		printf("* (%d,%d)\n", positions[i].x, positions[i].y);
	}
	free_grid(bin, H);
	free_grid(dp, H);
	if (positions != NULL)
	{
		free(positions);
	}
}

void read_DEI(int **grid, int W, int H, int *error)
{
	printf("Napiste minimalni DEI\n");
	while (1)
	{
		int DEI = 0;
		int test = scanf(" %d", &DEI);
		if (test == EOF)
		{
			return;
		}
		if (test != 1)
		{
			*error = 1;
			return;
		}
		find_biggest_space(grid, W, H, DEI);
	}
}

int main()
{
	int error = 0;
	int W, H;
	read_WH(&W, &H, &error);
	if (error == 1)
	{
		printf("Nespravny vstup\n");
		return 1;
	}
	int **grid = initialize_grid(W, H);
	read_grid(grid, W, H, &error);
	if (error == 1)
	{
		printf("Nespravny vstup\n");
		free_grid(grid, H);
		return 1;
	}
	read_DEI(grid, W, H, &error);
	if (error == 1)
	{
		printf("Nespravny vstup\n");
		free_grid(grid, H);
		return 1;
	}
	free_grid(grid, H);
	return 0;
}