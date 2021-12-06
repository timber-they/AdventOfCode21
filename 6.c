#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define REPRODUCTION_RATE 7
#define NEWBORN_LAG 2
#define DAYS 80
#define DAYS2 256

long part1(FILE *in);
long part2(FILE *in);
long *readStateCounts(FILE *in, long *buff);
long *iterate(long *stateCounts);
long count(long *stateCounts);

int main()
{
    FILE *in = fopen("in6", "r");

    printf("Part1: %ld\n", part1(in));
    rewind(in);
    printf("Part2: %ld\n", part2(in));

    fclose(in);
    return 0;
}

long part1(FILE *in)
{
    long buff[REPRODUCTION_RATE + NEWBORN_LAG] = {0};
    long *stateCounts = readStateCounts(in, buff);
    for (int i = 0; i < DAYS; i++)
        stateCounts = iterate(stateCounts);
    return count(stateCounts);
}

long part2(FILE *in)
{
    long buff[REPRODUCTION_RATE + NEWBORN_LAG] = {0};
    long *stateCounts = readStateCounts(in, buff);
    for (int i = 0; i < DAYS2; i++)
        stateCounts = iterate(stateCounts);
    return count(stateCounts);
}

long *readStateCounts(FILE *in, long *buff)
{
    int c;
    while ((c = fgetc(in)) != EOF)
    {
        buff[c-'0']++;
        if (fgetc(in) != ',')
            break;
    }
    return buff;
}

long *iterate(long *stateCounts)
{
    long buff[REPRODUCTION_RATE + NEWBORN_LAG];
    for (long i = 1; i < REPRODUCTION_RATE + NEWBORN_LAG; i++)
        buff[i-1] = stateCounts[i];
    buff[REPRODUCTION_RATE-1] += stateCounts[0];
    buff[REPRODUCTION_RATE+NEWBORN_LAG-1] = stateCounts[0];
    return memcpy(stateCounts, buff, (REPRODUCTION_RATE + NEWBORN_LAG) * sizeof(*stateCounts));
}

long count(long *stateCounts)
{
    long res = 0;
    for (int i = 0; i < REPRODUCTION_RATE + NEWBORN_LAG; i++)
        res += stateCounts[i];
    return res;
}

