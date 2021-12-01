#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

int part1(FILE *in);
int part2(FILE *in);
int countIncs(FILE *in);
int countSlidingIncs(FILE *in);

int main()
{
    FILE *in = fopen("in1", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    return countIncs(in);
}

int part2(FILE *in)
{
    return countSlidingIncs(in);
}

int countIncs(FILE *in)
{
    int a = 0;
    fscanf(in, "%d", &a) || fprintf(stderr, "PANIC\n");
    int b = 0;
    int incs = 0;
    while (fscanf(in, "%d", &b) == 1)
    {
        incs += b > a;
        a = b;
    }
    return incs;
}

int countSlidingIncs(FILE *in)
{
    int a = 0;
    int b = 0;
    int c = 0;
    fscanf(in, "%d", &a) || fprintf(stderr, "PANIC\n");
    fscanf(in, "%d", &b) || fprintf(stderr, "PANIC\n");
    fscanf(in, "%d", &c) || fprintf(stderr, "PANIC\n");
    int prev = a + b + c;
    a = b;
    b = c;
    int incs = 0;

    while (fscanf(in, "%d", &c) == 1)
    {
        int new = a + b + c;
        incs += new > prev;
        a = b;
        b = c;
        prev = new;
    }
    return incs;
}

