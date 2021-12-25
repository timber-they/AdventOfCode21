#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define DIGITS 14
#define MAX_W 9
#define MEMORY 1000000

typedef struct
{
    int div;
    int add1;
    int add2;
} Config;

long part1(FILE *in);
long part2(FILE *in);
Config *readConfigs(FILE *in, Config *buff);
long getZ(Config *configs, int digit, long zi, int w);
long invert(long l);
long getLong();
long getFinalZ(Config *configs, long modelNumber);
long getBestLong(Config *configs);
long getLowestLong(Config *configs);
void reduce(int *last, Config *configs);
void increase(int *last, Config *configs);

/*
 * 0 -> w0+14                           w0+14
 *   -> w1+26*w0+370                    w1+26*d0+6
 *   -> w2+26*w1+26^2*w0+9626           w2+26*d1+6
 *   -> w3+26*w2+26^2*w1+26^3*w0+250289 w3+26*d2+13
 *   ->                                 w4+d3-5
 */

int main()
{
    FILE *in = fopen("in24", "r");

    printf("Part1: %ld\n", part1(in));
    rewind(in);
    printf("Part2: %ld\n", part2(in));

    fclose(in);
    return 0;
}

long part1(FILE *in)
{
    Config buff[DIGITS] = {0};
    Config *configs = readConfigs(in, buff);
    return getBestLong(configs);
}

long part2(FILE *in)
{
    Config buff[DIGITS] = {0};
    Config *configs = readConfigs(in, buff);
    return getLowestLong(configs);
}

Config *readConfigs(FILE *in, Config *buff)
{
    for (int i = 0; i < DIGITS; i++)
    {
        !fscanf(in, "inp w\n") || fprintf(stderr, "PANIC!\n");
        !fscanf(in, "mul x 0\n") || fprintf(stderr, "PANIC!\n");
        !fscanf(in, "add x z\n") || fprintf(stderr, "PANIC!\n");
        !fscanf(in, "mod x 26\n") || fprintf(stderr, "PANIC!\n");
        fscanf(in, "div z %d\n", &buff[i].div) == 1 || fprintf(stderr, "PANIC!\n");
        fscanf(in, "add x %d\n", &buff[i].add1) == 1 || fprintf(stderr, "PANIC!\n");
        !fscanf(in, "eql x w\n") || fprintf(stderr, "PANIC!\n");
        !fscanf(in, "eql x 0\n") || fprintf(stderr, "PANIC!\n");
        !fscanf(in, "mul y 0\n") || fprintf(stderr, "PANIC!\n");
        !fscanf(in, "add y 25\n") || fprintf(stderr, "PANIC!\n");
        !fscanf(in, "mul y x\n") || fprintf(stderr, "PANIC!\n");
        !fscanf(in, "add y 1\n") || fprintf(stderr, "PANIC!\n");
        !fscanf(in, "mul z y\n") || fprintf(stderr, "PANIC!\n");
        !fscanf(in, "mul y 0\n") || fprintf(stderr, "PANIC!\n");
        !fscanf(in, "add y w\n") || fprintf(stderr, "PANIC!\n");
        fscanf(in, "add y %d\n", &buff[i].add2) == 1 || fprintf(stderr, "PANIC!\n");
        !fscanf(in, "mul y x\n") || fprintf(stderr, "PANIC!\n");
        !fscanf(in, "add z y\n") || fprintf(stderr, "PANIC!\n");
    }
    return buff;
}

long getZ(Config *configs, int digit, long zi, int w)
{
    long x = 0, y = 0, z = zi;
    x *= 0;
    x += z;
    x %= 26;
    z /= configs[digit].div;
    x += configs[digit].add1;
    x = x == w;
    x = x == 0;
    y *= 0;
    y += 25;
    y *= x;
    y += 1;
    z *= y;
    y *= 0;
    y += w;
    y += configs[digit].add2;
    y *= x;
    z += y;
    return z;
}

long invert(long l)
{
    long res = 0;
    while (l != 0)
    {
        res = res * 10 + l % 10;
        l /= 10;
    }
    return res;
}

long getLong()
{
    long res;
    printf("Scanning...\n");
    scanf("%ld", &res) == 1 ||
        fprintf(stderr, "Couldn't read input\n");
    printf("Read %ld\n", res);
    res = invert(res);
    printf("Converted %ld\n", res);
    return res;
}

long getFinalZ(Config *configs, long modelNumber)
{
    long z = 0;
    //printf("%ld", z);
    for (int digit = 0; digit < DIGITS; digit++)
    {
        //printf("Digit %d is %ld\n", digit, modelNumber % 10);
        z = getZ(configs, digit, z, modelNumber % 10);
        if (modelNumber == 0)
            fprintf(stderr, "Model number too low\n");
        modelNumber /= 10;
        //printf(" -> %ld (%ld)", z, z%26);
    }
    if (modelNumber > 0)
        fprintf(stderr, "Model number too high\n");
    return z;
}

long getBestLong(Config *configs)
{
    int last[DIGITS];
    for (int i = 0; i < DIGITS; i++)
        last[i] = 9;
    while (1)
    {
        long z = 0;
        long l = 0;
        reduce(last, configs);
        for (int i = 0; i < DIGITS; i++)
        {
            if (configs[i].div == 26)
            {
                last[i] = z % 26 + configs[i].add1;
                if (last[i] <= 0 || last[i] > 9)
                    goto naah;
            }
            if (last[i] <= 0 || last[i] > 9)
                fprintf(stderr, "%d is %d!\n", i, last[i]);
            l = l * 10 + last[i];
            z = getZ(configs, i, z, last[i]);
        }
        if (z == 0)
            return l;
naah:;
    }
}

long getLowestLong(Config *configs)
{
    int last[DIGITS];
    for (int i = 0; i < DIGITS; i++)
        last[i] = 1;
    while (1)
    {
        long z = 0;
        long l = 0;
        increase(last, configs);
        for (int i = 0; i < DIGITS; i++)
        {
            if (configs[i].div == 26)
            {
                last[i] = z % 26 + configs[i].add1;
                if (last[i] <= 0 || last[i] > 9)
                    goto naah;
            }
            if (last[i] <= 0 || last[i] > 9)
                fprintf(stderr, "%d is %d!\n", i, last[i]);
            l = l * 10 + last[i];
            z = getZ(configs, i, z, last[i]);
        }
        if (z == 0)
            return l;
naah:;
    }
}

void reduce(int *last, Config *configs)
{
    for (int i = DIGITS-1; i >= 0; i--)
    {
        if (configs[i].div == 26)
            continue;
        if (last[i] == 1)
        {
            last[i] = 9;
            continue;
        }
        last[i]--;
        return;
    }
    fprintf(stderr, "Can't reduce anymore!\n");
}

void increase(int *last, Config *configs)
{
    for (int i = DIGITS-1; i >= 0; i--)
    {
        if (configs[i].div == 26)
            continue;
        if (last[i] == 9)
        {
            last[i] = 1;
            continue;
        }
        last[i]++;
        return;
    }
    fprintf(stderr, "Can't reduce anymore!\n");
}

