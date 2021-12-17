#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

int part1(FILE *in);
int part2(FILE *in);
int reachesTarget(int vx, int vy, int xmin, int xmax, int ymin, int ymax);
// The highest point is above the trench
int getHighestPoint(int vy);

int main()
{
    FILE *in = fopen("in17", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    int xmin, xmax, ymin, ymax;
    if (fscanf(in, "target area: x=%d..%d, y=%d..%d\n", &xmin, &xmax, &ymin, &ymax) != 4)
    {
        fprintf(stderr, "Couldn't read input\n");
        exit(1);
    }
    
    int highestHeight = 0;
    for (int vy = 0; vy <= -ymin; vy++)
    {
        int vx;
        for (vx = 0; vx <= xmax; vx++)
            if (reachesTarget(vx, vy, xmin, xmax, ymin, ymax))
                break;
        if (vx == xmax+1)
            continue;

        int height = getHighestPoint(vy);
        if (height > highestHeight)
            highestHeight = height;
    }

    return highestHeight;
}

int part2(FILE *in)
{
    int xmin, xmax, ymin, ymax;
    if (fscanf(in, "target area: x=%d..%d, y=%d..%d\n", &xmin, &xmax, &ymin, &ymax) != 4)
    {
        fprintf(stderr, "Couldn't read input\n");
        exit(1);
    }
    
    int count = 0;
    for (int vy = ymin; vy <= -ymin; vy++)
    {
        int vx;
        for (vx = 0; vx <= xmax; vx++)
            if (reachesTarget(vx, vy, xmin, xmax, ymin, ymax))
                count++;
    }

    return count;
}

int reachesTarget(int vx, int vy, int xmin, int xmax, int ymin, int ymax)
{
    int x = 0, y = 0;
    while (y >= ymin)
    {
        if (x >= xmin && y >= ymin && x <= xmax && y <= ymax)
            return 1;
        x += vx;
        y += vy;

        if (vx > 0)
            vx--;
        else if (vx < 0)
            vx++;
        
        vy--;
    }
    return 0;
}

int getHighestPoint(int vy)
{
    return vy * (vy + 1) / 2;
}

