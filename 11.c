#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define SIZE 10
#define ITERATIONS 100

#define GET(octupuses,x,y) ((octupuses)[(x) + (y) * (SIZE)])

int part1(FILE *in);
int part2(FILE *in);
int *read(FILE *in, int *buff);
int iterate(int *octupuses);
int isDue(int *octupuses);
void inc (int x, int y, int *octupuses);
void print(int *octupuses);

int main()
{
    FILE *in = fopen("in11", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    int buff[SIZE * SIZE];
    int *octupuses = read(in, buff);
    int sum = 0;
    for (int i = 0; i < ITERATIONS; i++)
        sum += iterate(octupuses);
    return sum;
}

int part2(FILE *in)
{
    int buff[SIZE * SIZE];
    int *octupuses = read(in, buff);
    int i;
    for (i = 0; iterate(octupuses) != SIZE * SIZE; i++) ;
    return i+1;
}

int *read(FILE *in, int *buff)
{
    int *res = buff;
    for (int y = 0; y < SIZE; y++)
    {
        for (int x = 0; x < SIZE; x++)
            *(buff++) = fgetc(in) - '0';
        int c = fgetc(in);
        if (c == '\n')
            continue;
        if (c == '\0')
            return res;
        fprintf(stderr, "Unexpected character: '%c'\n", c);
    }
    return res;
}

int iterate(int *octupuses)
{
    for (int i = 0; i < SIZE * SIZE; i++)
        octupuses[i]++;
    int flashes = 0;
    int i;
    while ((i = isDue(octupuses)))
    {
        i--;
        flashes++;
        int x = i % SIZE;
        int y = i / SIZE;
        inc(x+1, y, octupuses);
        inc(x+1, y+1, octupuses);
        inc(x+1, y-1, octupuses);
        inc(x-1, y, octupuses);
        inc(x-1, y+1, octupuses);
        inc(x-1, y-1, octupuses);
        inc(x, y+1, octupuses);
        inc(x, y-1, octupuses);
        octupuses[i] = -1;
    }
    for (int i = 0; i < SIZE * SIZE; i++)
        if (octupuses[i] < 0)
            octupuses[i] = 0;
    return flashes;
}

int isDue(int *octupuses)
{
    for (int i = 0; i < SIZE * SIZE; i++)
        if (octupuses[i] >= 10)
            return i+1;
    return 0;
}

void inc (int x, int y, int *octupuses)
{
    if (x < 0 || y < 0 || x >= SIZE || y >= SIZE
            || GET(octupuses, x, y) < 0)
        return;
    GET(octupuses, x, y)++;
}

void print(int *octupuses)
{
    for (int y = 0; y < SIZE; y++)
    {
        for (int x = 0; x < SIZE; x++)
            if (GET(octupuses, x, y) >= 10)
                printf("█");
            else if (GET(octupuses, x, y) < 0)
                printf(" ");
            else
                printf("%d", GET(octupuses, x, y));
        printf("\n");
    }
}

