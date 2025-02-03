#include <stdlib.h>
#include <assert.h>

typedef struct TDice {
    struct TDice * m_Next;
    int m_Sides;
} TDICE;

size_t countCombinations(TDICE * dice, int from, int to) {
    if (from > to) return 0;

    int DicesCounter = 0;
    int initialCountOfDices = 10;
    int * set_of_dices = (int *)malloc(initialCountOfDices * sizeof(int));
    TDICE * current = dice;

    while (current != NULL) {
        if (DicesCounter >= initialCountOfDices) {
            initialCountOfDices *= 2;
            set_of_dices = (int *)realloc(set_of_dices, initialCountOfDices * sizeof(int));
        }
        set_of_dices[DicesCounter++] = current->m_Sides;
        current = current->m_Next;
    }

    int maxSum = 0;
    for (int i = 0; i < DicesCounter; i++) {
        maxSum += set_of_dices[i];
    }

    int * ways = (int *)calloc(maxSum + 1, sizeof(int));
    ways[0] = 1;

    for (int i = 0; i < DicesCounter; i++) {
        for (int j = maxSum; j >= set_of_dices[i]; j--) {
            for (int k = 1; k <= set_of_dices[i]; k++) {
                if (j >= k) {
                    ways[j] += ways[j - k];
                }
            }
        }
    }

    size_t combinations = 0;
    for (int i = from; i <= to; i++) {
        if (i <= maxSum) {
            combinations += ways[i];
        }
    }

    free(set_of_dices);
    free(ways);

    return combinations;
}

#ifndef __TRAINER__
void freeDice(TDICE * dice) {
    while(dice) {
        TDICE * tmp = dice;
        dice = dice->m_Next;
        free(tmp);
    }
}

TDICE * newDice(int sides, TDICE * next) {
    TDICE * result = (TDICE *)malloc(sizeof(TDICE));
    result->m_Sides = sides;
    result->m_Next = next;
    return result;
}

int main () {
    TDICE * a = newDice(6, newDice(6, NULL));
    assert(countCombinations(a, 6, 6) == 5);
    assert(countCombinations(a, 3, 3) == 2);
    assert(countCombinations(a, 11, 12) == 3);
    assert(countCombinations(a, 2, 3) == 3);
    assert(countCombinations(a, 1, 7) == 21);
    assert(countCombinations(a, 0, 12) == 36);
    assert(countCombinations(a, 0, 0) == 0);
    assert(countCombinations(a, 13, 13) == 0);
    freeDice(a);

    TDICE * b = newDice(6, newDice(4, newDice(4, NULL)));
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