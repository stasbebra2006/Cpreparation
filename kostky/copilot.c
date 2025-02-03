#include <stdlib.h>
#include <assert.h>

typedef struct TDice {
    struct TDice * m_Next;
    int m_Sides;
} TDICE;

size_t countCombinations(TDICE * dice, int from, int to) {
    // Pokud není žádná kostka, jediný možný součet je 0.
    if (!dice)
        return (from <= 0 && 0 <= to) ? 1 : 0;

    // Inicializace dynamického pole pro DP.
    // Na začátku máme pouze jeden způsob, jak získat součet 0.
    size_t * dp = (size_t*)calloc(1, sizeof(size_t)); 
    dp[0] = 1;
    int currentMax = 0;  // maximální dosažitelný součet dosud

    // Projdeme všechny kostky v seznamu.
    TDICE * current = dice;
    while(current) {
        int sides = current->m_Sides;
        int newMax = currentMax + sides;
        // Alokujeme nové pole pro rozšířenou distribuci součtů.
        size_t * newdp = (size_t*)calloc(newMax + 1, sizeof(size_t));
        // Pro každý dosud dosažitelný součet, přičteme hodnotu každé strany aktuální kostky.
        for (int sum = 0; sum <= currentMax; sum++) {
            if (dp[sum] != 0) {
                for (int face = 1; face <= sides; face++) {
                    newdp[sum + face] += dp[sum];
                }
            }
        }
        free(dp);
        dp = newdp;
        currentMax = newMax;
        current = current->m_Next;
    }

    // Sečteme počty kombinací pro součty v intervalu [from, to].
    size_t result = 0;
    // Součet nelze získat, pokud je nižší než počet kostek (protože každá kostka dává nejméně 1)
    // ale protože dp bylo vybudováno od 0, jednoduše procházíme od from do to.
    for (int s = from; s <= to && s <= currentMax; s++) {
        if (s >= 0)
            result += dp[s];
    }
    free(dp);
    return result;
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
