#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <float.h>

bool isEqual(double a, double b)
{
    if (a == 0.0 && b == 0.0) {
        return true;
    }
    return fabs(a - b) < 1000 * DBL_EPSILON * (fabs(a) + fabs(b));
}

int readPoint(double point[2], char *bracket)
{
	return scanf(" [ %lf , %lf %c", &point[0], &point[1], bracket);
}

bool areCollinear(double A[2], double B[2], double C[2])
{
	return isEqual(fabs((B[0] - A[0]) * (C[1] - A[1])), fabs((B[1] - A[1]) * (C[0] - A[0])));
} //(216.754-466.659)*(371.927-) ; ()*()
void calculateFourthPoint(double A[2], double B[2], double C[2], double D[2])
{
	D[0] = A[0] + C[0] - B[0];
	D[1] = A[1] + C[1] - B[1];
}

const char *determineParallelogramType(double A[2], double B[2], double C[2], double D[2])
{
	double AB = hypot(B[0] - A[0], B[1] - A[1]);
	double BC = hypot(C[0] - B[0], C[1] - B[1]);
	double CD = hypot(D[0] - C[0], D[1] - C[1]);
	double DA = hypot(A[0] - D[0], A[1] - D[1]);
	double AC = hypot(C[0] - A[0], C[1] - A[1]);
	double BD = hypot(D[0] - B[0], D[1] - B[1]);

	if (isEqual(AB, CD) && isEqual(BC, DA))
	{
		if (isEqual(AB, BC))
		{
			if (isEqual(AC, BD))
			{
				return "ctverec";
			}
			return "kosoctverec";
		}
		if (isEqual(AC, BD))
		{
			return "obdelnik";
		}
	}
	return "rovnobeznik";
}

int main()
{
	char bracket = 0;
	double A[2] = {0, 0}, B[2] = {0, 0}, C[2] = {0, 0};
	double *points[] = {A, B, C};
	const char *pointNames[] = {"A", "B", "C"};

	for (int i = 0; i < 3; i++)
	{
		printf("Bod %s:\n", pointNames[i]);
		if (readPoint(points[i], &bracket) != 3 || bracket != ']')
		{
			printf("Nespravny vstup.\n");
			return 1;
		}
	}

	if (areCollinear(A, B, C))
	{
		printf("Rovnobezniky nelze sestrojit.\n");
		return 1;
	}
	else
	{
		double D1[2] = {0, 0}, D2[2] = {0, 0}, D3[2] = {0, 0};

		calculateFourthPoint(C, A, B, D1);
		calculateFourthPoint(A, B, C, D2);
		calculateFourthPoint(B, C, A, D3);

		printf("A': [%g,%g], %s\n", D1[0], D1[1], determineParallelogramType(C, A, B, D1));
		printf("B': [%g,%g], %s\n", D2[0], D2[1], determineParallelogramType(A, B, C, D2));
		printf("C': [%g,%g], %s\n", D3[0], D3[1], determineParallelogramType(B, C, A, D3));
	}
	return 0;
}