#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

// 7!
#define POSSIBLE_COMBINATIONS 5040
#define IDENTIFIER_COUNT 10
#define OUTPUT_COUNT 4

/*
 *  aaa
 * b   c
 * b   c
 *  ddd
 * e   f
 * e   f
 *  ggg
 * */

#define ZERO (1 + (1<<1) + (1<<2) + (0<<3) + (1<<4) + (1<<5) + (1<<6))
#define ONE (0 + (0<<1) + (1<<2) + (0<<3) + (0<<4) + (1<<5) + (0<<6))
#define TWO (1 + (0<<1) + (1<<2) + (1<<3) + (1<<4) + (0<<5) + (1<<6))
#define THREE (1 + (0<<1) + (1<<2) + (1<<3) + (0<<4) + (1<<5) + (1<<6))
#define FOUR (0 + (1<<1) + (1<<2) + (1<<3) + (0<<4) + (1<<5) + (0<<6))
#define FIVE (1 + (1<<1) + (0<<2) + (1<<3) + (0<<4) + (1<<5) + (1<<6))
#define SIX (1 + (1<<1) + (0<<2) + (1<<3) + (1<<4) + (1<<5) + (1<<6))
#define SEVEN (1 + (0<<1) + (1<<2) + (0<<3) + (0<<4) + (1<<5) + (0<<6))
#define EIGHT (1 + (1<<1) + (1<<2) + (1<<3) + (1<<4) + (1<<5) + (1<<6))
#define NINE (1 + (1<<1) + (1<<2) + (1<<3) + (0<<4) + (1<<5) + (1<<6))

int part1(FILE *in);
int part2(FILE *in);
int countUniqueDigits(FILE *in);
int getMappedPattern(int mapping, int pattern);
int getValue(int pattern);
int getMappedValue(int mapping, int pattern);
int *getPossibleMappings(int *buff);
int readPatterns(FILE *in, int *identifier, int *output);

int segmentCounts[] = 
{
    6, // 0
    2, // 1
    5, // 2
    5, // 3
    4, // 4
    5, // 5
    6, // 6
    3, // 7
    7, // 8
    6  // 9
};

int main()
{
    FILE *in = fopen("in8", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    return countUniqueDigits(in);
}

int part2(FILE *in)
{
    int buff[POSSIBLE_COMBINATIONS];
    int *mappings = getPossibleMappings(buff);
    int identifier[IDENTIFIER_COUNT];
    int output[OUTPUT_COUNT];
    int sum = 0;
    int line = 0;
    while (readPatterns(in, identifier, output))
    {
        int i;
        for (i = 0; i < POSSIBLE_COMBINATIONS; i++)
        {
            int j;
            for (j = 0; j < IDENTIFIER_COUNT; j++)
                if (getMappedValue(mappings[i], identifier[j]) < 0)
                    break;
            if (j == IDENTIFIER_COUNT)
            {
                int val = 0;
                // All values were valid
                for (int k = 0; k < OUTPUT_COUNT; k++)
                    val = (val * 10) + getMappedValue(mappings[i], output[k]);
                sum += val;
                break;
            }
        }
        if (i == POSSIBLE_COMBINATIONS)
            fprintf(stderr, "No mapping matched for %d\n", line);
        line++;
    }
    
    return sum;
}

int countUniqueDigits(FILE *in)
{
    int res = 0;
    int curr = 0;
    int rel = 0;
    int c;
    int unique[10] = {0};
    unique[segmentCounts[1]] = 1;
    unique[segmentCounts[4]] = 1;
    unique[segmentCounts[7]] = 1;
    unique[segmentCounts[8]] = 1;
    while((c = fgetc(in)))
    {
        switch (c)
        {
            case ' ':
                if (!rel)
                    break;
                if (unique[curr])
                    res++;
                curr = 0;
                break;
            case '|':
                rel = 1;
                break;
            case '\n':
                if (!rel)
                    return res;
                if (unique[curr])
                    res++;
                curr = 0;
                rel = 0;
                break;
            case EOF:
                if (unique[curr])
                    res++;
                return res;
            default:
                if (!rel)
                    break;
                curr++;
                break;
        }
    }
    printf("Done!\n");
    return res;
}

int getMappedPattern(int mapping, int pattern)
{
    int res = 0;
    for (int i = 0; i < 7; i++)
        res += (pattern & (1<<i))
            ? 1 << ((mapping >> (3*i)) & 7)
            : 0;
    return res;
}

int getValue(int pattern)
{
    switch(pattern)
    {
        case ZERO:
            return 0;
        case ONE:
            return 1;
        case TWO:
            return 2;
        case THREE:
            return 3;
        case FOUR:
            return 4;
        case FIVE:
            return 5;
        case SIX:
            return 6;
        case SEVEN:
            return 7;
        case EIGHT:
            return 8;
        case NINE:
            return 9;
        default:
            return -1;
    }
}

int getMappedValue(int mapping, int pattern)
{
    return getValue(getMappedPattern(mapping, pattern));
}

int *getPossibleMappings(int *buff)
{
    int i = 0;
    // The most beautiful code ever
    for (int a = 0; a < 7; a++)
    for (int b = 0; b < 7; b++)
    for (int c = 0; c < 7 && a != b; c++)
    for (int d = 0; d < 7 && a != c && b != c; d++)
    for (int e = 0; e < 7 && a != d && b != d && c != d; e++)
    for (int f = 0; f < 7 && a != e && b != e && c != e && d != e; f++)
    for (int g = 0; g < 7 && a != f && b != f && c != f && d != f && e != f; g++)
    {
        if (a == g || b == g || c == g || d == g || e == g || f == g)
            continue;
        buff[i++] = (a << (3*0)) +
            (b << (3*1)) +
            (c << (3*2)) +
            (d << (3*3)) +
            (e << (3*4)) +
            (f << (3*5)) +
            (g << (3*6));
    }
    if (i != POSSIBLE_COMBINATIONS)
        fprintf(stderr, "Unexpected count - %d != 7!\n", i);
    return buff;
}

int readPatterns(FILE *in, int *identifier, int *output)
{
    int c;
    int inOutput = 0;
    int i = 0;

    memset(identifier, 0, IDENTIFIER_COUNT * sizeof(*identifier));
    memset(output, 0, OUTPUT_COUNT * sizeof(*output));

    while ((c = fgetc(in)))
    {
        switch(c)
        {
            case ' ':
                i++;
                break;
            case '|':
                inOutput = 1;
                i = -1;
                break;
            case '\n':
                return 1;
            case EOF:
                return 0;
            default:
                if (inOutput)
                    output[i] += 1<<(c - 'a');
                else
                    identifier[i] += 1<<(c - 'a');
                break;
        }
    }
    return 0;
}

