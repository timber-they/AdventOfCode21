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
    long res = getMax(memory, digits, zis, ws, configs);
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
    long z = zi *
        // 10 <= add1 <= 15
        (25 * ((zi % 26 + configs[digit].add1) != w) + 1) /
        // 26 or 1
        configs[digit].div +
        // 5 <= add2 <= 15
        w + configs[digit].add2 + ((zi % 26 + configs[digit].add1) != w);
    return z;
}

long getMax(int *memory, int *digits, long *zis, int *ws, Config *configs)
{
    long res = 0;
    long z = 0;
    for (int digit = 0; digit < DIGITS; digit++)
    {
        printf("digit=%d (< %d)\n", digit, DIGITS);
        for (int w = 9; w >= 1; w--)
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
    //printf("digit=%d\n", digit);
    int res;
    /*int i;
    for (i = 0; memory[i] >= 0; i++)
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
        //memory[i] = z == 0;
        res = z == 0;
        if (z == 0)
            printf("%d,%ld,%d is valid!\n", digit, zi, w);
    }
    else
    {
        int w2;
        for (w2 = 1; w2 <= MAX_W; w2++)
            if (isValid(configs, memory, digits, zis, ws, digit+1, z, w2))
                break;
        res = w2 <= MAX_W;
        //memory[i] = w2 <= MAX_W;
    }
    //printf("Now digit=%d\n", digit);
    //digits[i] = digit;
    //zis[i] = zi;
    //ws[i] = w;
    //return memory[i];
    return res;
}

