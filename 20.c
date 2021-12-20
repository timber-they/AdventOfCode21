#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define ALGORITHM 512
#define SIZE 100
#define FINAL_SIZE ((SIZE) + 2*(OVERHEAD))
#define ITERATIONS1 2
#define OVERHEAD (2*ITERATIONS)
#define GET(image,x,y) ((image)[(x) + (y)*(FINAL_SIZE)])

int ITERATIONS;

int part1(FILE *in);
int part2(FILE *in);
void skipLineBreak(FILE *in);
int *readAlgorithm(FILE *in, int *buff);
int *readImage(FILE *in, int *buff);
int iterate(int *algorithm, int *image, int index);
int evaluateSurrounding(int *image, int x, int y, int index);
void print(int *image);

int main()
{
    FILE *in = fopen("in20", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    ITERATIONS = 2;
    int *algorithmBuff = malloc(ALGORITHM * sizeof(*algorithmBuff));
    int *imageBuff = calloc(FINAL_SIZE * FINAL_SIZE, sizeof(*imageBuff));

    int *algorithm = readAlgorithm(in, algorithmBuff);
    skipLineBreak(in);
    int *image = readImage(in, imageBuff);

    int res = 0;
    for (int i = 0; i < ITERATIONS; i++)
        res = iterate(algorithm, image, i);
    free(algorithmBuff);
    free(imageBuff);
    return res;
}

int part2(FILE *in)
{
    ITERATIONS = 50;
    int *algorithmBuff = malloc(ALGORITHM * sizeof(*algorithmBuff));
    int *imageBuff = calloc(FINAL_SIZE * FINAL_SIZE, sizeof(*imageBuff));

    int *algorithm = readAlgorithm(in, algorithmBuff);
    skipLineBreak(in);
    int *image = readImage(in, imageBuff);

    int res = 0;
    for (int i = 0; i < ITERATIONS; i++)
        res = iterate(algorithm, image, i);
    free(algorithmBuff);
    free(imageBuff);
    return res;
}

void skipLineBreak(FILE *in)
{
    if (fgetc(in) != '\n')
    {
        fprintf(stderr, "Expected line break!\n");
        exit(1);
    }
}

int *readAlgorithm(FILE *in, int *buff)
{
    for (int i = 0; i < ALGORITHM; i++)
        buff[i] = fgetc(in) == '#';
    skipLineBreak(in);
    return buff;
}

int *readImage(FILE *in, int *buff)
{
    for (int y = 0; y < SIZE; y++)
    {
        for (int x = 0; x < SIZE; x++)
            GET(buff,x+OVERHEAD,y+OVERHEAD) = fgetc(in) == '#';

        skipLineBreak(in);
    }
    return buff;
}

int iterate(int *algorithm, int *image, int index)
{
    for (int x = 0; x < FINAL_SIZE; x++)
        for (int y = 0; y < FINAL_SIZE; y++)
            GET(image, x, y) += algorithm[evaluateSurrounding(image, x, y, index)] 
                ? 2 : 0;

    int res = 0;
    for (int x = 0; x < FINAL_SIZE; x++)
        for (int y = 0; y < FINAL_SIZE; y++)
        {
            GET(image, x, y) = GET(image, x, y) >> 1;
            if (GET(image, x, y))
                res++;
        }
    return res;
}

int evaluateSurrounding(int *image, int x, int y, int index)
{
    int res = 0;
    for (int yi = y-1; yi <= y+1; yi++)
        for (int xi = x-1; xi <= x+1; xi++)
        {
            int on = 0;
            if (xi < 0 || yi < 0 || xi >= FINAL_SIZE || yi >= FINAL_SIZE)
                on = index & 1;
            else
                on = GET(image, xi, yi) & 1;
            res += on << (8 - (3*(yi-y+1) + xi-x+1));
        }
    return res;
}

void print(int *image)
{
    for (int y = 0; y < FINAL_SIZE; y++)
    {
        for (int x = 0; x < FINAL_SIZE; x++)
            printf("%c", GET(image, x, y) & 1 ? '#' : '.');
        printf("\n");
    }
}

