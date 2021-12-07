#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

int part;

#define CRAB_COUNT 1000

int part1(FILE *in);
int part2(FILE *in);
int *readCrabs(FILE *in, int *buff);
int max(int *crabs);
int getCost(int *crabs, int pos);
int getBestPosition(int *crabs);
int getCorrectCost(int *crabs, int pos);

int main()
{
    FILE *in = fopen("in7", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    part = 1;
    int crabBuff[CRAB_COUNT] = {0};
    int *crabs = readCrabs(in, crabBuff);
    int bestPosition = getBestPosition(crabs);
    return getCost(crabs, bestPosition);
}

int part2(FILE *in)
{
    part = 2;
    int crabBuff[CRAB_COUNT] = {0};
    int *crabs = readCrabs(in, crabBuff);
    int bestPosition = getBestPosition(crabs);
    return getCost(crabs, bestPosition);
}

int *readCrabs(FILE *in, int *buff)
{
    int *res = buff;
    int c;
    while ((c = fgetc(in)))
    {
        if (c == EOF || c == '\n')
            return res;
        if (c == ',')
            buff++;
        else
            *buff = *buff * 10 + (c-'0');
    }
    return res;
}

int max(int *crabs)
{
    int res = crabs[0];
    for (int i = 1; i < CRAB_COUNT; i++)
        if (crabs[i] > res)
            res = crabs[i];
    return res;
}

int getCost(int *crabs, int pos)
{
    int res = 0;
    if (part == 1)
        for (int i = 0; i < CRAB_COUNT; i++)
            res += abs(crabs[i] - pos);
    else
        for (int i = 0; i < CRAB_COUNT; i++)
        {
            int diff = abs(crabs[i] - pos);
            res += diff * (diff + 1) / 2;
        }
    return res;
}

int getBestPosition(int *crabs)
{
    int bestPosition = 0;
    int bestCost = getCost(crabs, 0);
    for (int pos = 1; pos <= max(crabs); pos++)
    {
        int newCost = getCost(crabs, pos);
        if (newCost < bestCost)
        {
            bestCost = newCost;
            bestPosition = pos;
        }
    }
    return bestPosition;
}

