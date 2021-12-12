#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define LETTERS 26
#define TUNNELS 25

#define GET(matrix,x,y) ((matrix)[(x) + (y)*(LETTERS)*(LETTERS)])

int allCaves[TUNNELS] = {0};
int caveCount = 0;

int part1(FILE *in);
int part2(FILE *in);
int *readPathMatrix(FILE *in, int *buff, int *caves);
int getKey(char *cave);
int getAlphIndex(char c);
int countPaths(int currentCave, int *caves, int *pathMatrix);
int countPaths2(int currentCave, int *caves, int *pathMatrix, int visittedTwice);

int main()
{
    FILE *in = fopen("in12", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    int matrixBuff[LETTERS * LETTERS * LETTERS * LETTERS] = {0};
    int caves[LETTERS * LETTERS] = {0};

    int *matrix = readPathMatrix(in, matrixBuff, caves);
    return countPaths(getKey("start"), caves, matrix);
}

int part2(FILE *in)
{
    int matrixBuff[LETTERS * LETTERS * LETTERS * LETTERS] = {0};
    int caves[LETTERS * LETTERS] = {0};

    int *matrix = readPathMatrix(in, matrixBuff, caves);
    return countPaths2(getKey("start"), caves, matrix, 0);
}

int *readPathMatrix(FILE *in, int *buff, int *caves)
{
    char a[7];
    char b[7];
    int caveIndex = 0;
    while (fscanf(in, "%[^-]-%s\n", a, b) == 2)
    {
        int keyA = getKey(a);
        int keyB = getKey(b);
        GET(buff, keyA, keyB) = 1;
        GET(buff, keyB, keyA) = 1;

        caves[keyA] = a[0] >= 'A' && a[0] <= 'Z' ? 3 : 2;
        caves[keyB] = b[0] >= 'A' && b[0] <= 'Z' ? 3 : 2;

        int i;
        for (i = 0; i < caveIndex; i++)
            if (allCaves[i] == keyA)
                break;
        if (i == caveIndex)
            allCaves[caveIndex++] = keyA;
        for (i = 0; i < caveIndex; i++)
            if (allCaves[i] == keyB)
                break;
        if (i == caveIndex)
            allCaves[caveIndex++] = keyB;
    }
    caveCount = caveIndex;
    return buff;
}

int getKey(char *cave)
{
    return getAlphIndex(cave[0]) * 26
        + getAlphIndex(cave[1]);
}

int getAlphIndex(char c)
{
    return c >= 'A' && c <= 'Z'
        ? c - 'A'
        : c - 'a';
}

int countPaths(int currentCave, int *caves, int *pathMatrix)
{
    if (currentCave == getKey("end"))
        return 1;
    if (!caves[currentCave])
        return 0;
    caves[currentCave] = caves[currentCave] == 3 ? 3 : 0;
    int res = 0;
    for (int i = 0; i < caveCount; i++)
    {
        int cave = allCaves[i];
        if (caves[cave] && cave != currentCave && GET(pathMatrix, currentCave, cave))
            res += countPaths(cave, caves, pathMatrix);
    }
    caves[currentCave] = caves[currentCave] ? 3 : 2;
    return res;
}

int countPaths2(int currentCave, int *caves, int *pathMatrix, int visittedTwice)
{
    if (currentCave == getKey("end"))
        return 1;
    if (!caves[currentCave] || (caves[currentCave] == 1 && visittedTwice))
        return 0;

    int prev = caves[currentCave];
    if (currentCave == getKey("start"))
        caves[currentCave] = caves[currentCave] == 2 ? 0 : 3;
    else if (caves[currentCave] == 1)
        visittedTwice++;
    else
        caves[currentCave] = caves[currentCave] == 2 ? 1 : 3;

    int res = 0;
    for (int i = 0; i < caveCount; i++)
    {
        int cave = allCaves[i];
        if (cave != currentCave && GET(pathMatrix, currentCave, cave))
            res += countPaths2(cave, caves, pathMatrix, visittedTwice);
    }
    caves[currentCave] = prev;
    return res;
}

