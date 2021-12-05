#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define LINE_COUNT 500
#define GRID_SIZE 1000

#define GET(buff,x,y) ((buff)[(x) + (y) * (GRID_SIZE)])

typedef struct
{
    int x1;
    int y1;
    int x2;
    int y2;
} Line;

int part1(FILE *in);
int part2(FILE *in);
Line *getLines(FILE *in, Line *buff);
void draw(Line line, int *grid, int includingDiagonal);
void printGrid(int *grid);

int main()
{
    FILE *in = fopen("in5", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    Line buff[LINE_COUNT];
    Line *lines = getLines(in, buff);

    int grid[GRID_SIZE * GRID_SIZE] = {0};
    for (int i = 0; i < LINE_COUNT; i++)
        draw(lines[i], grid, 0);

    int res = 0;
    for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++)
        res += grid[i] > 1;
    return res;
}

int part2(FILE *in)
{
    Line buff[LINE_COUNT];
    Line *lines = getLines(in, buff);

    int grid[GRID_SIZE * GRID_SIZE] = {0};
    for (int i = 0; i < LINE_COUNT; i++)
        draw(lines[i], grid, 1);

    int res = 0;
    for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++)
        res += grid[i] > 1;
    return res;
}

Line *getLines(FILE *in, Line *buff)
{
    for (int i = 0; i < LINE_COUNT; i++)
        fscanf(in, "%d,%d -> %d,%d", &buff[i].x1, &buff[i].y1, &buff[i].x2, &buff[i].y2) == 4 ||
            fprintf(stderr, "Couldn't parse line\n");
    return buff;
}

static int max(int a, int b)
{
    return a > b ? a : b;
}
void draw(Line line, int *grid, int includingDiagonal)
{
    if (!includingDiagonal && line.x1 != line.x2 && line.y1 != line.y2)
        return;
    int diff = max(abs(line.x2-line.x1), abs(line.y2-line.y1));
    int xInc = (line.x2 - line.x1) / diff;
    int yInc = (line.y2 - line.y1) / diff;
    for (int d = 0; d <= diff; d++)
        GET(grid, line.x1 + d * xInc, line.y1 + d * yInc)++;
}

