#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define HEX_SIZE 1398
#define SIZE (4*(HEX_SIZE))

int part1(FILE *in);
int part2(FILE *in);
int *readBits(FILE *in, int *buff);
int readNumber(int *bits, int size);
int sumVersions(int *bits, int *versionSum);
int evaluate(int *bits, int *value);

int main()
{
    FILE *in = fopen("in16", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    int buff[SIZE] = {0};
    int *bits = readBits(in, buff);
    int sum = 0;
    sumVersions(bits, &sum);
    return sum;
}

int part2(FILE *in)
{
    int buff[SIZE] = {0};
    int *bits = readBits(in, buff);
    int res = 0;
    evaluate(bits, &res);
    return res;
}

int *readBits(FILE *in, int *buff)
{
    for (int i = 0; i < HEX_SIZE; i++)
    {
        int c = fgetc(in);
        if (c == EOF)
        {
            fprintf(stderr, "Got EOF\n");
            exit(1);
        }

        if (c >= '0' && c <= '9')
            c -= '0';
        else
            c += 10 - 'A';
        
        for (int j = 3; j >= 0; j--)
            buff[4*i+3-j] = c & (1<<j);
    }
    return buff;
}

int readNumber(int *bits, int size)
{
    int res = 0;
    for (int i = size-1; i >= 0; i--)
        res += (bits[i] != 0) << (size-i-1);
    return res;
}

int sumVersions(int *bits, int *versionSum)
{
    int size = 0;
    int version = readNumber(bits+size, 3);
    size += 3;
    *versionSum += version;
    int id = readNumber(bits+size, 3);
    size += 3;
    int lengthTypeId, subPackets, subLength, totalLength;
    switch (id)
    {
        case 4:
            // Literal
            while (readNumber(bits+size, 5) & (1<<4))
                size += 5;
            // The last block
            size += 5;
            break;
        default:
            // Operator
            lengthTypeId = readNumber(bits+size, 1);
            size += 1;
            if (lengthTypeId == 0)
            {
                totalLength = readNumber(bits+size, 15);
                size += 15;
                subLength = 0;
                while (subLength < totalLength)
                    subLength += sumVersions(bits+size+subLength, versionSum);
                size += totalLength;
            }
            else
            {
                subPackets = readNumber(bits+size, 11);
                size += 11;
                for (int i = 0; i < subPackets; i++)
                    size += sumVersions(bits+size, versionSum);
            }
            break;
    }
    return size;
}

int evaluate(int *bits, int *value)
{
    int size = 0;
    readNumber(bits+size, 3);
    size += 3;
    int id = readNumber(bits+size, 3);
    size += 3;
    int lengthTypeId, subPackets, subLength, totalLength, number;
    switch (id)
    {
        case 4:
            // Literal
            while ((number = readNumber(bits+size, 5)) & (1<<4))
            {
                *value = (*value << 4) + number & (0xF);
                size += 5;
            }
            // The last block
            size += 5;
            break;
        default:
            // Operator
            lengthTypeId = readNumber(bits+size, 1);
            size += 1;
            if (lengthTypeId == 0)
            {
                totalLength = readNumber(bits+size, 15);
                size += 15;
                subLength = 0;
                if (id == 1)
                    *value = 1;
                else if (id == 2)
                    *value = 1<<30;
                int i = 0;
                while (subLength < totalLength)
                {
                    number = 0;
                    subLength += evaluate(bits+size+subLength, &number);
                    switch(id)
                    {
                        case 0:
                            *value += number;
                            break;
                        case 1:
                            if (i == 0)
                                *value = number;
                            else
                                *value *= number;
                            break;
                        case 2:
                            if (i == 0 || number < *value)
                                *value = number;
                            break;
                        case 3:
                            if (i == 0 || number > *value)
                                *value = number;
                            break;
                        case 5:
                            if (i == 0)
                                *value = number;
                            else
                                *value = *value > number;
                            break;
                        case 6:
                            if (i == 0)
                                *value = number;
                            else
                                *value = *value < number;
                            break;
                        case 7:
                            if (i == 0)
                                *value = number;
                            else
                                *value = *value == number;
                            break;
                    }
                    i++;
                }
                size += totalLength;
            }
            else
            {
                subPackets = readNumber(bits+size, 11);
                size += 11;
                for (int i = 0; i < subPackets; i++)
                {
                    size += evaluate(bits+size, &number);
                    switch(id)
                    {
                        case 0:
                            *value += number;
                            break;
                        case 1:
                            if (i == 0)
                                *value = number;
                            else
                                *value *= number;
                            break;
                        case 2:
                            if (i == 0 || number < *value)
                                *value = number;
                            break;
                        case 3:
                            if (i == 0 || number > *value)
                                *value = number;
                            break;
                        case 5:
                            if (i == 0)
                                *value = number;
                            else
                                *value = *value > number;
                            break;
                        case 6:
                            if (i == 0)
                                *value = number;
                            else
                                *value = *value < number;
                            break;
                        case 7:
                            if (i == 0)
                                *value = number;
                            else
                                *value = *value == number;
                            break;
                    }
                }
            }
            break;
    }
    return size;
}

