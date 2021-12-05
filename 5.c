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
    {
        fscanf(in, "%d,%d -> %d,%d", &buff[i].x1, &buff[i].y1, &buff[i].x2, &buff[i].y2) == 4 ||
            fprintf(stderr, "Couldn't parse line\n");
        if (buff[i].x1 >= GRID_SIZE ||
                buff[i].x2 >= GRID_SIZE ||
                buff[i].y1 >= GRID_SIZE ||
                buff[i].y2 >= GRID_SIZE)
            fprintf(stderr, "Out of bounds!\n");
        if (buff[i].x1 == buff[i].x2 && buff[i].y1 > buff[i].y2)
        {
            int swp = buff[i].y1;
            buff[i].y1 = buff[i].y2;
            buff[i].y2 = swp;
        }
        else if (buff[i].y1 == buff[i].y2 && buff[i].x1 > buff[i].x2)
        {
            int swp = buff[i].x1;
            buff[i].x1 = buff[i].x2;
            buff[i].x2 = swp;
        }
        else if (buff[i].x1 > buff[i].x2)
        {
            // Lines should always be left to right
            int swp = buff[i].x1;
            buff[i].x1 = buff[i].x2;
            buff[i].x2 = swp;

            swp = buff[i].y1;
            buff[i].y1 = buff[i].y2;
            buff[i].y2 = swp;
        }
    }
    return buff;
}

void draw(Line line, int *grid, int includingDiagonal)
{
    // Vertical
    if (line.x1 == line.x2)
        for (int y = line.y1; y <= line.y2; y++)
            GET(grid, line.x1, y)++;
    // Horizontal
    else if (line.y1 == line.y2)
        for (int x = line.x1; x <= line.x2; x++)
            GET(grid, x, line.y1)++;
    // Diagonal
    else if (includingDiagonal)
        for (int d = 0; line.x1 + d <= line.x2; d++)
            GET(grid, line.x1+d, line.y2 > line.y1 ? line.y1 + d : line.y1 - d)++;
}

void printGrid(int *grid)
{
    for (int y = 0; y < GRID_SIZE; y++)
    {
        for (int x = 0; x < GRID_SIZE; x++)
            printf("%d", GET(grid, x, y));
        printf("\n");
    }
}

