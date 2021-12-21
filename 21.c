#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define WIN 21
#define GET(memory,s1,s2,p1,p2) ((memory)[(s1) + WIN*(s2) + WIN*WIN*(p1) + WIN*WIN*10*(p2)])

int part1(FILE *in);
long part2(FILE *in);
void read(FILE *in, int *p1, int *p2);
// Assumes it's player 1s turn
long countWinning(int s1, int s2, int p1, int p2, long *memory);

int main()
{
    FILE *in = fopen("in21", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %ld\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    int p1, p2;
    read(in, &p1, &p2);
    int s1 = 0, s2 = 0;
    int rolls = 0;
    for (int d = 1;; d+=3)
    {
        if (d & 1)
        {
            // Player 1
            p1 = (p1 + 3 * d + 3) % 10;
            s1 += p1+1;
        }
        else
        {
            // Player 2
            p2 = (p2 + 3 * d + 3) % 10;
            s2 += p2+1;
        }
        rolls += 3;

        if (s1 >= 1000)
            return s2 * rolls;
        else if (s2 >= 1000)
            return s1 * rolls;
    }
    return -1;
}

long part2(FILE *in)
{
    int p1, p2;
    read(in, &p1, &p2);
    int s1 = 0, s2 = 0;
    long memory[WIN*WIN*10*10];
    for (int i = 0; i < WIN*WIN*10*10; i++)
        memory[i] = -1;
    return countWinning(s1, s2, p1, p2, memory);
}

void read(FILE *in, int *p1, int *p2)
{
    if (!fscanf(in, "Player 1 starting position: %d\n", p1) ||
            !fscanf(in, "Player 2 starting position: %d\n", p2))
        fprintf(stderr, "Couldn't read input\n");
    (*p1)--;
    (*p2)--;
}

long countWinning(int s1, int s2, int p1, int p2, long *memory)
{
    if (s1 >= WIN)
        return 1;
    if (s2 >= WIN)
        return 0;
    if (GET(memory, s1, s2, p1, p2) >= 0)
        return GET(memory, s1, s2, p1, p2);
    long res = 0;
    for (int i1 = 1; i1 <= 3; i1++)
    for (int i2 = 1; i2 <= 3; i2++)
    for (int i3 = 1; i3 <= 3; i3++)
    {
        int pos1 = (p1 + i1 + i2 + i3) % 10;
        int score1 = s1 + pos1 + 1;
        if (score1 >= WIN)
        {
            res++;
            continue;
        }
        for (int j1 = 1; j1 <= 3; j1++)
        for (int j2 = 1; j2 <= 3; j2++)
        for (int j3 = 1; j3 <= 3; j3++)
        {
            int pos2 = (p2 + j1 + j2 + j3) % 10;
            int score2 = s2 + pos2 + 1;
            if (score2 >= WIN)
                continue;
            res += countWinning(score1, score2, pos1, pos2, memory);
        }
    }
    GET(memory, s1, s2, p1, p2) = res;
    return res;
}

