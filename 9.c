#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define SIZE 100

int part1(FILE *in);
int part2(FILE *in);
char *readMap(FILE *in, char *buff);
char get(char *map, int x, int y);
int isLowPoint(char *map, int x, int y);
int getRisk(char *map, int x, int y);
int getBasinSize(char *map, int x, int y);

int main()
{
    FILE *in = fopen("in9", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    int sum = 0;
    char buff[SIZE * SIZE];
    char *map = readMap(in, buff);
    for (int y = 0; y < SIZE; y++)
        for (int x = 0; x < SIZE; x++)
            if (isLowPoint(map, x, y))
                sum += getRisk(map, x, y);
    return sum;
}

int part2(FILE *in)
{
    int largest[3] = {0};
    char buff[SIZE * SIZE];
    char *map = readMap(in, buff);
    for (int y = 0; y < SIZE; y++)
        for (int x = 0; x < SIZE; x++)
            if (isLowPoint(map, x, y))
            {
                int basinSize = getBasinSize(map, x, y);
                if (basinSize > largest[0])
                {
                    largest[2] = largest[1];
                    largest[1] = largest[0];
                    largest[0] = basinSize;
                }
                else if (basinSize > largest[1])
                {
                    largest[2] = largest[1];
                    largest[1] = basinSize;
                }
                else if (basinSize > largest[2])
                    largest[2] = basinSize;
            }
    int prod = 1;
    for (int i = 0; i < 3; i++)
        prod *= largest[i];
    return prod;
}

char *readMap(FILE *in, char *buff)
{
    for (int i = 0; i < SIZE * SIZE; i++)
    {
        int c = fgetc(in);
        if (c == '\n')
        {
            i--;
            continue;
        }
        buff[i] = c - '0';
    }
    return buff;
}

char get(char *map, int x, int y)
{
    return (x >= SIZE || y >= SIZE || x < 0 || y < 0)
        ? 10
        : map[x + SIZE * y];
}

int isLowPoint(char *map, int x, int y)
{
    char curr = get(map, x, y);
    return
        curr < get(map, x+1, y) &&
        curr < get(map, x, y+1) &&
        curr < get(map, x-1, y) &&
        curr < get(map, x, y-1);
}

int getRisk(char *map, int x, int y)
{
    return get(map, x, y) + 1;
}

int getBasinSize(char *map, int x, int y)
{
    if (get(map, x, y) >= 9)
        return 0;
    map[x + SIZE * y] = 11;
    return 1 +
        getBasinSize(map, x+1, y) +
        getBasinSize(map, x, y+1) +
        getBasinSize(map, x-1, y) +
        getBasinSize(map, x, y-1);
}

