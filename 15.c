#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define SIZE1 100
#define SIZE2 500
#define GET(cave,x,y) ((cave)[(x) + (y) * (SIZE)])

int SIZE;

int part1(FILE *in);
int part2(FILE *in);
int *read(FILE *in, int *buff);
int dijkstra(int *cave);
void expand(int *cave);

int main()
{
    FILE *in = fopen("in15", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    SIZE = SIZE1;
    int buff[SIZE1*SIZE1];
    int *cave = read(in, buff);
    return dijkstra(cave);
}

int part2(FILE *in)
{
    SIZE = SIZE2;
    int buff[SIZE2*SIZE2];
    int *cave = read(in, buff);
    expand(cave);
    return dijkstra(cave);
}

int *read(FILE *in, int *buff)
{
    int c;
    int x = 0, y = 0;
    while ((c = fgetc(in)) != EOF)
    {
        if (c == '\n')
        {
            y++;
            x = 0;
            continue;
        }
        if (c < '0' || c > '9')
            fprintf(stderr, "Invalid character: %c\n", c);
        GET(buff, x, y) = c - '0';
        x++;
    }
    return buff;
}

int isVisitted(int *visitted, int x, int y)
{
    if (x < 0 || y < 0 || x >= SIZE || y >= SIZE)
        return 2;
    return GET(visitted, x, y);
}

int dijkstra(int *cave)
{
    GET(cave, 0, 0) = 0;
    int x = 0, y = 0;
    int *distanceMap = malloc(SIZE * SIZE * sizeof(*distanceMap));
    for (int i = 0; i < SIZE * SIZE; i++)
        distanceMap[i] = 1<<30;
    distanceMap[0] = 0;
    int *visitted = calloc(SIZE * SIZE, sizeof(*visitted));
    int furthestVisittedX = 0, furthestVisittedY = 0;
    while(1)
    {
        if (x < 0 || y < 0)
            break;
        GET(visitted, x, y) = 1;
        if (x > furthestVisittedX)
            furthestVisittedX = x;
        if (y > furthestVisittedY)
            furthestVisittedY = y;
        int distance = GET(distanceMap, x, y);
        if (!isVisitted(visitted, x+1, y) && distance + GET(cave, x+1, y) < GET(distanceMap, x+1, y))
            GET(distanceMap, x+1, y) = distance + GET(cave, x+1, y);
        if (!isVisitted(visitted, x-1, y) && distance + GET(cave, x-1, y) < GET(distanceMap, x-1, y))
            GET(distanceMap, x-1, y) = distance + GET(cave, x-1, y);
        if (!isVisitted(visitted, x, y+1) && distance + GET(cave, x, y+1) < GET(distanceMap, x, y+1))
            GET(distanceMap, x, y+1) = distance + GET(cave, x, y+1);
        if (!isVisitted(visitted, x, y-1) && distance + GET(cave, x, y-1) < GET(distanceMap, x, y-1))
            GET(distanceMap, x, y-1) = distance + GET(cave, x, y-1);

        // Find next node
        int min = 1<<30;
        int minX = -1, minY = -1;
        for (y = 0; y <= furthestVisittedY+1 && y < SIZE; y++)
            for (x = 0; x <= furthestVisittedX+1 && x < SIZE; x++)
                if (y < 0 || x < 0)
                    continue;
                else if (!GET(visitted, x, y) && GET(distanceMap, x, y) < min)
                {
                    min = GET(distanceMap, x, y);
                    minX = x;
                    minY = y;
                }
        x = minX;
        y = minY;
    }
    int res =  GET(distanceMap, SIZE-1, SIZE-1);
    free(distanceMap);
    free(visitted);
    return res;
}

void expand(int *cave)
{
    for (int yo = 0; yo < 5; yo++)
        for (int xo = 0; xo < 5; xo++)
            for (int y = 0; y < SIZE1; y++)
                for (int x = 0; x < SIZE1; x++)
                {
                    int orig = GET(cave, x, y);
                    int new = orig + yo + xo;
                    while (new > 9)
                        new = new - 9;
                    GET(cave, xo * SIZE1 + x, yo * SIZE1 + y) = new;
                }
}

