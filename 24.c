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
int part2(FILE *in);
Config *readConfigs(FILE *in, Config *buff);
long getMax(int *memory, int *digits, long *zis, int *ws, Config *configs);
long getZ(Config *configs, int digit, long zi, int w);
int isValid(Config *configs, int *memory, int *digits, long *zis, int *ws,
        int digit, long zi, int w);
long getModulo(int digit);

int main()
{
    FILE *in = fopen("in24", "r");

    printf("Part1: %ld\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

long part1(FILE *in)
{
    Config buff[DIGITS] = {0};
    Config *configs = readConfigs(in, buff);
    int *memory = malloc(MEMORY * sizeof(*memory));
    for (int i = 0; i < MEMORY; i++)
        memory[i] = -1;
    int *digits = malloc(MEMORY * sizeof(*digits));
    long *zis = malloc(MEMORY * sizeof(*digits));
    int *ws = malloc(MEMORY * sizeof(*ws));
    long res = -1;
    res = getMax(memory, digits, zis, ws, configs);
    int digit = DIGITS-1;
    printf("Config for digit %d is: %d,%d,%d\n", digit, configs[digit].div, configs[digit].add1, configs[digit].add2);
    /*for (long z = -100000000; z < 100000000; z++)
        for (int w = 1; w <= 9; w++)
        {
            if (labs(getZ(configs, digit, z, w)) < 8)
                printf("%ld,%d yields %ld\n", z, w, getZ(configs, digit, z, w));
            //if (isValid(configs, memory, digits, zis, ws, digit, z, w))
                //printf("%d,%ld,%d is valid!\n", digit, z, w);
        }*/
    //1 -> -3 5,-2 5,-1 8
    /*for (int w = 1; w <= 9; w++)
    {
        int z = getZ(configs, 0, 0, w);
        printf("%d yields %d\n", w, z);
    }*/

    free(memory);
    free(digits);
    free(zis);
    free(ws);
    return res;
}

int part2(FILE *in)
{
    return -2;
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
    /*// 10 <= add1 <= 15
    if (zi % 26 == w - configs[digit].add1)
        if (configs[digit].div == 26)
            return zi / 26 +
                // 5 <= add2 <= 15
                w + configs[digit].add2;
        else
            return zi +
                // 5 <= add2 <= 15
                w + configs[digit].add2;
    else
        if (configs[digit].div == 26)
            return zi +
                // 5 <= add2 <= 15
                w + configs[digit].add2 + 1;
        else
            return zi * 26 +
                // 5 <= add2 <= 15
                w + configs[digit].add2 + 1;*/
    long x = 0, y = 0, z = 0;
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

long getMax(int *memory, int *digits, long *zis, int *ws, Config *configs)
{
    long res = 0;
    long z = 0;
    for (int digit = 0; digit < DIGITS; digit++)
    {
        printf("digit=%d (< %d)\n", digit, DIGITS);
        for (int w = MAX_W; w >= 1; w--)
        {
            if (isValid(configs, memory, digits, zis, ws, digit, z, w))
            {
                z = getZ(configs, digit, z, w);
                res = res * 10 + w;
                printf("Found w=%d for digit=%d\n", w, digit);
                break;
            }
        }
    }
    return res;
}

int isValid(Config *configs, int *memory, int *digits, long *zis, int *ws,
        int digit, long zi, int w)
{
    if (digit == DIGITS)
        return zi == 0;
    /*if (labs(zi) > 100000 && digit >= DIGITS-5)
        return 0;
    else*/
    /*{
        if (digit == DIGITS-1)
            return zi == -17 + (9-w);
        else if (digit == DIGITS-2)
            return zi <= -19 && zi >= -35 &&
                w-1 <= (-19 - zi) &&
                w-1 >= (-27 - zi);
        else if (digit == DIGITS-3)
            return zi <= -29 && zi >= -53 &&
                w-1 <= (-29 - zi) &&
                w-1 >= (-45 - zi);
        else if (digit == DIGITS-4)
            return zi <= -41 && zi >= -73 &&
                w-1 <= (-41 - zi) &&
                w-1 >= (-65 - zi);
        else if (digit == DIGITS-5)
            return zi <= -55 && zi >= -95 &&
                w-1 <= (-55 - zi) &&
                w-1 >= (-87 - zi);
    }*/
    //printf("digit=%d\n", digit);
    int i = 0;
    /*for (i = 0; memory[i] >= 0; i++)
    {
        if (i >= MEMORY-1)
            fprintf(stderr, "Overflow\n");
        if (digits[i] == digit && zis[i] == zi && ws[i] == w)
            return memory[i];
    }*/
    //printf("New: %d,%ld,%d\n", digit, zi, w);
    //getchar();
    long z = getZ(configs, digit, zi, w);
    if (digit == DIGITS-1)
    {
        memory[i] = z == 0;
    }
    else
    {
        int w2;
        for (w2 = 1; w2 <= MAX_W; w2++)
            if (isValid(configs, memory, digits, zis, ws, digit+1, z, w2))
            {
                //printf("Valid for %d: %d\n", digit, w2);
                break;
            }
        memory[i] = w2 <= MAX_W;
    }
    //printf("Now digit=%d\n", digit);
    digits[i] = digit;
    zis[i] = zi;
    ws[i] = w;
    return memory[i];
}

