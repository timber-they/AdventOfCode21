#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

int part1(FILE *in);
int part2(FILE *in);
void read(FILE *in, int *p1, int *p2);

int main()
{
    FILE *in = fopen("in21", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

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

int part2(FILE *in)
{
    return -2;
}

void read(FILE *in, int *p1, int *p2)
{
    if (!fscanf(in, "Player 1 starting position: %d\n", p1) ||
            !fscanf(in, "Player 2 starting position: %d\n", p2))
        fprintf(stderr, "Couldn't read input\n");
    (*p1)--;
    (*p2)--;
}

