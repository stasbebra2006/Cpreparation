#ifndef __TRAINER__
#include <stdlib.h>
#include <assert.h>

typedef struct TDice
{
	struct TDice *m_Next;
	int m_Sides;
} TDICE;
#endif

size_t countCombinations(TDICE *dice, int from, int to)
{
	if (!dice)
	{
		// return (from = 0 && to >= 0) ? 1 : 0;
		return (from <= 0 && to >= 0) ? 1 : 0;
	}

	int *dp = (int *)calloc(1, sizeof(int));
	int maxSum = 0;
	dp[0] = 1;

	while (dice)
	{
		int sides = dice->m_Sides;
		int newMaxSum = maxSum + sides;
		int *newdp = (int *)calloc(newMaxSum + 1, sizeof(int));
		for (int sum = 0; sum <= maxSum; sum++)
		{
			if (dp[sum] != 0)
			{
				for (int side = 1; side <= sides; side++)
				{
					newdp[sum + side] += dp[sum];
				}
			}
		}
		maxSum = newMaxSum;
		free(dp);
		dp = newdp;
		dice = dice->m_Next;
	}
	size_t countCombo = 0;
	for (int sum = from; sum <= to && sum <= maxSum; sum++)
	{
		countCombo += dp[sum];
	}
	free(dp);
	return countCombo;
}

#ifndef __TRAINER__
void freeDice(TDICE *dice)
{
	while (dice)
	{
		TDICE *tmp = dice;
		dice = dice->m_Next;
		free(tmp);
	}
}

TDICE *newDice(int sides, TDICE *next)
{
	TDICE *result = (TDICE *)malloc(sizeof(TDICE));
	result->m_Sides = sides;
	result->m_Next = next;
	return result;
}

int main()
{
	TDICE *a = newDice(6, newDice(6, NULL));
	assert(countCombinations(a, 6, 6) == 5);
	assert(countCombinations(a, 3, 3) == 2);
	assert(countCombinations(a, 11, 12) == 3);
	assert(countCombinations(a, 2, 3) == 3);
	assert(countCombinations(a, 1, 7) == 21);
	assert(countCombinations(a, 0, 12) == 36);
	assert(countCombinations(a, 0, 0) == 0);
	assert(countCombinations(a, 13, 13) == 0);
	freeDice(a);

	TDICE *b = newDice(6, newDice(4, newDice(4, NULL)));
	assert(countCombinations(b, 6, 6) == 10);
	assert(countCombinations(b, 3, 3) == 1);
	assert(countCombinations(b, 11, 12) == 16);
	assert(countCombinations(b, 2, 3) == 1);
	assert(countCombinations(b, 1, 7) == 33);
	assert(countCombinations(b, 0, 12) == 92);
	assert(countCombinations(b, 0, 0) == 0);
	assert(countCombinations(b, 13, 13) == 3);
	freeDice(b);

	return 0;
}
#endif
