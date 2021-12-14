#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define INITIAL_LENGTH 20
#define ELEMENTS 26
#define INSERTION_RULES 100
#define ITERATIONS 10
#define ITERATIONS2 40
// It _should_ not increase faster than by duplication
#define MAX_LENGTH (INITIAL_LENGTH * (1<<10))

typedef struct Polymer
{
    char element;
    struct Polymer *next;
} Polymer;

int part1(FILE *in);
long part2(FILE *in);
Polymer *readTemplate(FILE *in, Polymer *buff);
int *readInsertionRules(FILE *in, int *buff);
int iterate(Polymer *polymer, int polymerLength, int *insertionRules);
int getResult(Polymer *polymer);
void print(Polymer *polymer);
long *getPairCounts(Polymer *polymer, long *buff, long *counts);
void iterate2(long *pairCounts, int *insertionRules, long *counts);
void print2(long *pairCounts);

int main()
{
    FILE *in = fopen("in14", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %ld\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    Polymer buff[MAX_LENGTH] = {0};
    int insertionBuff[ELEMENTS * ELEMENTS] = {0};
    
    Polymer *polymer = readTemplate(in, buff);
    if (fgetc(in) != '\n' || fgetc(in) != '\n')
        fprintf(stderr, "Expected line break!\n");
    int *insertionRules = readInsertionRules(in, insertionBuff);

    int length = INITIAL_LENGTH;
    for (int i = 0; i < ITERATIONS; i++)
        length = iterate(polymer, length, insertionRules);

    return getResult(polymer);
}

long part2(FILE *in)
{
    Polymer buff[MAX_LENGTH] = {0};
    int insertionBuff[ELEMENTS * ELEMENTS] = {0};
    
    Polymer *polymer = readTemplate(in, buff);
    if (fgetc(in) != '\n' || fgetc(in) != '\n')
        fprintf(stderr, "Expected line break!\n");
    int *insertionRules = readInsertionRules(in, insertionBuff);

    long pairBuff[ELEMENTS * ELEMENTS] = {0};
    long counts[ELEMENTS] = {0};
    
    long *pairCounts = getPairCounts(polymer, pairBuff, counts);

    for (int i = 0; i < ITERATIONS2; i++)
        iterate2(pairCounts, insertionRules, counts);

    long max = counts[0];
    long min = counts[0];
    for (int i = 1; i < ELEMENTS; i++)
        if (counts[i] > max)
            max = counts[i];
        else if (counts[i] != 0 && (counts[i] < min || min == 0))
            min = counts[i];
    return max - min;
}

Polymer *readTemplate(FILE *in, Polymer *buff)
{
    for (int i = 0; i < INITIAL_LENGTH; i++)
    {
        int c = fgetc(in);
        if (c < 'A' || c > 'Z')
        {
            fprintf(stderr, "'%c' is not an element\n", c);
            exit(1);
        }
        buff[i].element = c - 'A';
        if (i > 0)
            buff[i-1].next = buff+i;
    }
    return buff;
}

int *readInsertionRules(FILE *in, int *buff)
{
    for (int i = 0; i < INSERTION_RULES; i++)
    {
        char a, b, c;
        if (fscanf(in, "%c%c -> %c\n", &a, &b, &c) != 3)
        {
            fprintf(stderr, "Couldn't read line\n");
            continue;
            exit(2);
        }

        // There is no element called A
        a -= 'A';
        b -= 'A';
        c -= 'A';
        buff[a * ELEMENTS + b] = c;
    }
    return buff;
}

int iterate(Polymer *polymer, int polymerLength, int *insertionRules)
{
    for (Polymer *iter = polymer; iter->next != NULL; iter = iter->next)
    {
        int index = iter->element * ELEMENTS + iter->next->element;
        if (insertionRules[index])
        {
            polymer[polymerLength].element = insertionRules[index];
            Polymer *buff = iter->next;
            iter->next = polymer+polymerLength;
            polymer[polymerLength].next = buff;

            iter = iter->next;
            polymerLength++;
        }
    }
    return polymerLength;
}

int getResult(Polymer *polymer)
{
    int counts[ELEMENTS] = {0};
    for (Polymer *iter = polymer; iter != NULL; iter = iter->next)
        counts[(int)iter->element]++;
    int max = counts[0];
    int min = counts[0];
    for (int i = 1; i < ELEMENTS; i++)
        if (counts[i] > max)
            max = counts[i];
        else if (counts[i] != 0 && (counts[i] < min || min == 0))
            min = counts[i];
    return max - min;
}

void print(Polymer *polymer)
{
    for (Polymer *iter = polymer; iter != NULL; iter = iter->next)
        printf("%c", iter->element+'A');
    printf("\n");
}

long *getPairCounts(Polymer *polymer, long *buff, long *counts)
{
    counts[(int)polymer->element]++;
    for (Polymer *iter = polymer; iter->next != NULL; iter = iter->next)
    {
        buff[iter->element * ELEMENTS + iter->next->element]++;
        counts[(int)iter->next->element]++;
    }
    return buff;
}

void iterate2(long *pairCounts, int *insertionRules, long *counts)
{
    long pairBuff[ELEMENTS * ELEMENTS] = {0};
    memcpy(pairBuff, pairCounts, ELEMENTS * ELEMENTS * sizeof(*pairBuff));
    for (int i = 0; i < ELEMENTS * ELEMENTS; i++)
        if (pairCounts[i] && insertionRules[i])
        {
            int a = i / ELEMENTS;
            int b = i % ELEMENTS;
            int c = insertionRules[i];
            pairBuff[a * ELEMENTS + c] += pairCounts[i];
            pairBuff[c * ELEMENTS + b] += pairCounts[i];
            counts[c] += pairCounts[i];
            pairBuff[i] -= pairCounts[i];
        }
    memcpy(pairCounts, pairBuff, ELEMENTS * ELEMENTS * sizeof(*pairBuff));
}

void print2(long *pairCounts)
{
    for (int i = 0; i < ELEMENTS * ELEMENTS; i++)
        if (pairCounts[i])
        {
            int a = i / ELEMENTS;
            int b = i % ELEMENTS;
            printf("%c%c: %ld\n", a+'A', b+'A', pairCounts[i]);
        }
    printf("\n");
}

