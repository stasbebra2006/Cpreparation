#include <stdio.h>
#include <stdlib.h>

void read_WH(int *W, int *H, int *error)
{
	printf("Napiste mapu\n");
	int test = scanf(" %d %d", W, H);
	if (test != 2)
	{
		*error = 1;
		return;
	}
	printf("W = %d, H = %d\n", *W, *H);
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

int **read_grid(int **grid, int W, int H, int *error)
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
				return NULL;
			}
			grid[i][j] = DEI;
		}
	}
	return grid;
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
	grid = read_grid(grid, W, H, &error);
	if (error == 1)
	{
		printf("Nespravny vstup\n");
		return 1;
	}
	print_grid(grid, W, H);
	return 0;
}