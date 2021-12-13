#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define MAX 2000

#define GET(paper,x,y) ((paper)[(x)+(MAX)*(y)])

int part1(FILE *in);
int part2(FILE *in);
int *read(FILE *in, int *buff, int *sizeX, int *sizeY);
void foldLeft(int *paper, int foldLine, int *sizeX, int *sizeY);
void foldUp(int *paper, int foldLine, int *sizeX, int *sizeY);
int countDots(int *paper, int sizeX, int sizeY);
int applyFold(FILE *in, int *paper, int *sizeX, int *sizeY);
void print(int *paper, int sizeX, int sizeY);

int main()
{
    FILE *in = fopen("in13", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    int *buff = calloc(MAX * MAX, sizeof(*buff));
    int sizeX = 0, sizeY = 0;

    int *paper = read(in, buff, &sizeX, &sizeY);

    applyFold(in, paper, &sizeX, &sizeY);
    int res = countDots(paper, sizeX, sizeY);

    free(buff);
    return res;
}

int part2(FILE *in)
{
    int *buff = calloc(MAX * MAX, sizeof(*buff));
    int sizeX = 0, sizeY = 0;

    int *paper = read(in, buff, &sizeX, &sizeY);

    while (applyFold(in, paper, &sizeX, &sizeY)) ;
    print(paper, sizeX, sizeY);
    int res = countDots(paper, sizeX, sizeY);

    free(buff);
    return res;
}

int *read(FILE *in, int *buff, int *sizeX, int *sizeY)
{
    int x, y;
    while (fscanf(in, "%d,%d\n", &x, &y) == 2)
    {
        if (x > *sizeX)
            *sizeX = x;
        if (y > *sizeY)
            *sizeY = y;
        GET(buff,x,y) = 1;
    }
    // 0-based indices
    (*sizeX)++;
    (*sizeY)++;
    return buff;
}

void foldLeft(int *paper, int foldLine, int *sizeX, int *sizeY)
{
    for (int y = 0; y < *sizeY; y++)
        for (int x = 0; x < *sizeX - foldLine; x++)
            GET(paper, foldLine-x, y) += GET(paper, foldLine+x, y);
    *sizeX = foldLine;
}

void foldUp(int *paper, int foldLine, int *sizeX, int *sizeY)
{
    for (int y = 0; y < *sizeY - foldLine; y++)
        for (int x = 0; x < *sizeX; x++)
            GET(paper, x, foldLine-y) += GET(paper, x, foldLine+y);
    *sizeY = foldLine;
}

int countDots(int *paper, int sizeX, int sizeY)
{
    int res = 0;
    for (int y = 0; y < sizeY; y++)
        for (int x = 0; x < sizeX; x++)
            if (GET(paper, x, y))
                res++;
    return res;
}

int applyFold(FILE *in, int *paper, int *sizeX, int *sizeY)
{
    int foldLine;
    char dir;
    if (fscanf(in, "fold along %c=%d\n", &dir, &foldLine) != 2)
        return 0;
    if (dir == 'x')
        foldLeft(paper, foldLine, sizeX, sizeY);
    else if (dir == 'y')
        foldUp(paper, foldLine, sizeX, sizeY);
    else
        fprintf(stderr, "Unknown fold direction: '%c'\n", dir);
    return 1;
}

void print(int *paper, int sizeX, int sizeY)
{
    for (int y = 0; y < sizeY; y++)
    {
        for (int x = 0; x < sizeX; x++)
            printf("%s", GET(paper, x, y) ? "█" : ".");
        printf("\n");
    }
}

