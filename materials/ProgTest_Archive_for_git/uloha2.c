#include <stdio.h>
#include <math.h>
#include <stdbool.h>

int if_right(int a, int b, int c)
{
	return (a * a + b * b) == c * c;
}

void printf_Triangles(char op, int lo, int hi)
{
	int celkem = 0;
	switch (op)
	{
	case '?':
		for (int a = lo; a <= hi; a++)
		{
			for (int b = a; b <= hi; b++)
			{
				int c = (int)sqrt(a * a + b * b);
				if (c <= hi && c >= lo && if_right(a, b, c))
				{
					printf("* %d %d %d\n", a, b, c);
					celkem++;
				}
			}
		}
		break;
	case '#':
		for (int a = lo; a <= hi; a++)
		{
			for (int b = a; b <= hi; b++)
			{
				int c = (int)sqrt(a * a + b * b);
				if (c <= hi && c >= lo && if_right(a, b, c))
				{
					celkem++;
				}
			}
		}
		break;
	}

	printf("Celkem: %d\n", celkem);
}

int main()
{
	printf("Problemy:\n");
	char op = 0,
		 bracket = 0;
	int lo = 0,
		hi = 0,
		result = 0;

	while ((result = scanf(" %c < %d ; %d %c", &op, &lo, &hi, &bracket)) != EOF)
	{
		if (result != 4 || lo > hi || (op != '?' && op != '#') || bracket != '>' || lo < 0 || hi < 0)
		{
			printf("Nespravny vstup.\n");
			return 1;
		}
		else
		{
			printf_Triangles(op, lo, hi);
		}
	}
	return 0;
}