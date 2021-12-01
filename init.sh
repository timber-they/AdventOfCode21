#!/bin/bash

echo "#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

int part1(FILE *in);
int part2(FILE *in);

int main()
{
    FILE *in = fopen(\"in$1\", \"r\");

    printf(\"Part1: %d\\n\", part1(in));
    rewind(in);
    printf(\"Part2: %d\\n\", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    return -1;
}

int part2(FILE *in)
{
    return -2;
}
" >> $1.c

nano in$1

vim $1.c

