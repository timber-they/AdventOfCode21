#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define POSITIONS 15
/*
 * #################
 * #01. 2. 3. 4. 56#
 * ###7 #9 #11#13###
 *   #8 #10#12#14#
 *   #############
 */
#define ID(a1,a2,b1,b2,c1,c2,d1,d2) ((a1) + (a2) * POSITIONS + \
        (b1) * POSITIONS * POSITIONS + (b2) * POSITIONS * POSITIONS * POSITIONS + \
        (c1) * POSITIONS * POSITIONS * POSITIONS * POSITIONS + \
        (c2) * POSITIONS * POSITIONS * POSITIONS * POSITIONS * POSITIONS + \
        (d1) * POSITIONS * POSITIONS * POSITIONS * POSITIONS * POSITIONS * POSITIONS + \
        (long) (d2) * POSITIONS * POSITIONS * POSITIONS * \
        POSITIONS * POSITIONS * POSITIONS * POSITIONS)
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

int part1(FILE *in);
int part2(FILE *in);
long read(FILE *in);
int minCost(long position, int *memory);
int hasWon(long position);
long template(long id, int index);
long normalize(long id);
int isBlocked(int start, int end, int *others);
int isMyRoom(int pos, int name);
int getSteps(int start, int end);

int main()
{
    FILE *in = fopen("in23", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    long pos = read(in);
    int *memory = calloc((long) POSITIONS * POSITIONS * POSITIONS * POSITIONS *
            POSITIONS * POSITIONS * POSITIONS * POSITIONS, sizeof(*memory));
    int res = minCost(pos, memory);
    free(memory);
    return res;
}

int part2(FILE *in)
{
    return -2;
}

static void fill (char p, long val, long *a1, long *a2, long *b1, long *b2,
        long *c1, long *c2, long *d1, long *d2)
{
    switch(p)
    {
        case 'A':
            if (*a1 < 0)
                *a1 = val;
            else
                *a2 = val;
            break;
        case 'B':
            if (*b1 < 0)
                *b1 = val;
            else
                *b2 = val;
            break;
        case 'C':
            if (*c1 < 0)
                *c1 = val;
            else
                *c2 = val;
            break;
        case 'D':
            if (*d1 < 0)
                *d1 = val;
            else
                *d2 = val;
            break;
    }
}
long read(FILE *in)
{
    // The first two lines are unimportant
    fscanf(in, "%*s\n");
    fscanf(in, "%*s\n");
    char p1, p2, p3, p4, p5, p6, p7, p8;
    fscanf(in, "###%c#%c#%c#%c###\n", &p1, &p2, &p3, &p4) == 4 ||
        fprintf(stderr, "Couldn't read line\n");
    fscanf(in, "  #%c#%c#%c#%c#  \n", &p5, &p6, &p7, &p8) == 4 ||
        fprintf(stderr, "Couldn't read line\n");
    long a1 = -1, a2 = -1, b1 = -1, b2 = -1, c1 = -1, c2 = -1, d1 = -1, d2 = -1;
    fill(p1, 7, &a1, &a2, &b1, &b2, &c1, &c2, &d1, &d2);
    fill(p2, 8, &a1, &a2, &b1, &b2, &c1, &c2, &d1, &d2);
    fill(p3, 9, &a1, &a2, &b1, &b2, &c1, &c2, &d1, &d2);
    fill(p4, 10, &a1, &a2, &b1, &b2, &c1, &c2, &d1, &d2);
    fill(p5, 11, &a1, &a2, &b1, &b2, &c1, &c2, &d1, &d2);
    fill(p6, 12, &a1, &a2, &b1, &b2, &c1, &c2, &d1, &d2);
    fill(p7, 13, &a1, &a2, &b1, &b2, &c1, &c2, &d1, &d2);
    fill(p8, 14, &a1, &a2, &b1, &b2, &c1, &c2, &d1, &d2);
    return ID(a1, a2, b1, b2, c1, c2, d1, d2);
}

int minCost(long position, int *memory);
{
    position = normalize(position);
    if (hasWon(position))
        return 0;
    else if (memory[position] > 0)
        return memory[position];
    else if (memory[position] < 0)
        return -1;
    memory[position] = -1;
    int minCost = 1<<30;
    long a1 = template(position, 0),
        a2 = template(position, 1),
        b1 = template(position, 2),
        b2 = template(position, 3),
        c1 = template(position, 4),
        c2 = template(position, 5),
        d1 = template(position, 6),
        d2 = template(position, 7);
    int positions[] = {a1, a2, b1, b2, c1, c2, d1, d2};

    for (int i = 0; i < POSITIONS; i++)
    {
        int cost;
        if (i != a1 && !isBlocked(a1, i) && (i < 7 || isMyRoom(i, 0)) && (a1 >= 7 || i >= 7))
        {
            cost = minCost(ID(i, a2, b1, b2, c1, c2, d1, d2), memory);
            if (cost >= 0)
            {
                cost += getSteps(a1, i);
                if (cost < minCost)
                    minCost = cost;
            }
        }
        if (i != a2 && !isBlocked(a2, i) && (i < 7 || isMyRoom(i, 0)) && (a2 >= 7 || i >= 7))
        {
            cost = minCost(ID(a1, i, b1, b2, c1, c2, d1, d2), memory);
            if (cost >= 0)
            {
                cost += getSteps(a2, i);
                if (cost < minCost)
                    minCost = cost;
            }
        }
        if (i != b1 && !isBlocked(b1, i) && (i < 7 || isMyRoom(i, 1)) && (b1 >= 7 || i >= 7))
        {
            cost = minCost(ID(a1, a2, i, b2, c1, c2, d1, d2), memory);
            if (cost >= 0)
            {
                cost += getSteps(b1, i);
                if (cost < minCost)
                    minCost = cost;
            }
        }
        if (i != b2 && !isBlocked(b2, i) && (i < 7 || isMyRoom(i, 1)) && (b2 >= 7 || i >= 7))
        {
            cost = minCost(ID(a1, a2, b1, i, c1, c2, d1, d2), memory);
            if (cost >= 0)
            {
                cost += getSteps(b2, i);
                if (cost < minCost)
                    minCost = cost;
            }
        }
        if (i != c1 && !isBlocked(c1, i) && (i < 7 || isMyRoom(i, 2)) && (c1 >= 7 || i >= 7))
        {
            cost = minCost(ID(a1, a2, b1, b2, i, c2, d1, d2), memory);
            if (cost >= 0)
            {
                cost += getSteps(c1, i);
                if (cost < minCost)
                    minCost = cost;
            }
        }
        // TODO: Continue
        if (i != a1 && !isBlocked(a1, i) && (i < 7 || isMyRoom(i, 0)) && (a1 >= 7 || i >= 7))
        {
            cost = minCost(ID(i, a2, b1, b2, c1, c2, d1, d2), memory);
            if (cost >= 0)
            {
                cost += getSteps(a1, i);
                if (cost < minCost)
                    minCost = cost;
            }
        }
        if (i != a1 && !isBlocked(a1, i) && (i < 7 || isMyRoom(i, 0)) && (a1 >= 7 || i >= 7))
        {
            cost = minCost(ID(i, a2, b1, b2, c1, c2, d1, d2), memory);
            if (cost >= 0)
            {
                cost += getSteps(a1, i);
                if (cost < minCost)
                    minCost = cost;
            }
        }
        if (i != a1 && !isBlocked(a1, i) && (i < 7 || isMyRoom(i, 0)) && (a1 >= 7 || i >= 7))
        {
            cost = minCost(ID(i, a2, b1, b2, c1, c2, d1, d2), memory);
            if (cost >= 0)
            {
                cost += getSteps(a1, i);
                if (cost < minCost)
                    minCost = cost;
            }
        }

    }

    memory[position] = minCost;
    return minCost;
}

int hasWon(long position)
{
    long a1 = template(position, 0),
        a2 = template(position, 1),
        b1 = template(position, 2),
        b2 = template(position, 3),
        c1 = template(position, 4),
        c2 = template(position, 5),
        d1 = template(position, 6),
        d2 = template(position, 7);
    return a1 != a2 &&
        b1 != b2 &&
        c1 != c2 &&
        d1 != d2 &&
        a1 == 7 &&
        a2 == 8 &&
        b1 == 9 &&
        b2 == 10 &&
        c1 == 11 &&
        c2 == 12 &&
        d1 == 13 &&
        d2 == 14;
}

long template(long id, int index)
{
    for (int i = 0; i < index; i++)
        id /= POSITIONS;
    id -= (id / POSITIONS) * POSITIONS;
    return id;
}

long normalize(long id)
{
    long a1 = template(position, 0),
        a2 = template(position, 1),
        b1 = template(position, 2),
        b2 = template(position, 3),
        c1 = template(position, 4),
        c2 = template(position, 5),
        d1 = template(position, 6),
        d2 = template(position, 7);
    return ID(MIN(a1, a2), MAX(a1, a2),
            MIN(b1, b2), MAX(b1, b2),
            MIN(c1, c2), MAX(c1, c2),
            MIN(d1, d2), MAX(d1, d2));
}


/*
 * #################
 * #01. 2. 3. 4. 56#
 * ###7 #9 #11#13###
 *   #8 #10#12#14#
 *   #############
 */
static int fillBlockingPoints(int start, int end, int *blockingPoints, int blockingCount)
{
    switch(start)
    {
        case 0:
            switch(end)
            {
                case 1:
                    break;
                default:
                    blockingPoints[blockingCount++] = 1;
                    blockingCount = fillBlockingPoints(1, end, blockingPoints, blockingCount);
                    break;
            }
            break;
        case 1:
            switch(end)
            {
                case 0:
                    break;
                case 7:
                    break;
                case 8:
                    blockingPoints[blockingCount++] = 7;
                    break;
                case 2:
                    break;
                default:
                    blockingPoints[blockingCount++] = 2;
                    blockingCount = fillBlockingPoints(2, end, blockingPoints, blockingCount);
                    break;
            }
            break;
        case 2:
            switch(end)
            {
                case 7:
                    break;
                case 8:
                    blockingPoints[blockingCount++] = 7;
                    break;
                case 1:
                    break;
                case 0:
                    blockingPoints[blockingCount++] = 1;
                    blockingCount = fillBlockingPoints(1, end, blockingPoints, blockingCount);
                    break;
                case 9:
                    break;
                case 10:
                    blockingPoints[blockingCount++] = 9;
                    break;
                case 3:
                    break;
                default:
                    blockingPoints[blockingCount++] = 3;
                    blockingCount = fillBlockingPoints(3, end, blockingPoints, blockingCount);
                    break;
            }
            break;
        case 3:
            switch(end)
            {
                case 9:
                    break;
                case 10:
                    blockingPoints[blockingCount++] = 9;
                    break;
                case 2:
                    break;
                case 0:
                case 1:
                case 7:
                case 8:
                    blockingPoints[blockingCount++] = 2;
                    blockingCount = fillBlockingPoints(2, end, blockingPoints, blockingCount);
                    break;
                case 11:
                    break;
                case 12:
                    blockingPoints[blockingCount++] = 11;
                    break;
                case 4:
                    break;
                default:
                    blockingPoints[blockingCount++] = 4;
                    blockingCount = fillBlockingPoints(4, end, blockingPoints, blockingCount);
                    break;
            }
            break;
        case 4:
            switch(end)
            {
                case 13:
                    break;
                case 14:
                    blockingPoints[blockingCount++] = 13;
                    break;
                case 5:
                    break;
                case 6:
                    blockingPoints[blockingCount++] = 5;
                    blockingCount = fillBlockingPoints(5, end, blockingPoints, blockingCount);
                    break;
                case 11:
                    break;
                case 12:
                    blockingPoints[blockingCount++] = 11;
                    break;
                case 3:
                    break;
                default:
                    blockingPoints[blockingCount++] = 3;
                    blockingCount = fillBlockingPoints(3, end, blockingPoints, blockingCount);
                    break;
            }
            break;
        case 5:
            switch(end)
            {
                case 6:
                    break;
                case 13:
                    break;
                case 14:
                    blockingPoints[blockingCount++] = 13;
                    break;
                case 4:
                    break;
                default:
                    blockingPoints[blockingCount++] = 4;
                    blockingCount = fillBlockingPoints(4, end, blockingPoints, blockingCount);
                    break;
            }
            break;
        case 6:
            switch(end)
            {
                case 5:
                    break;
                default:
                    blockingPoints[blockingCount++] = 5;
                    blockingCount = fillBlockingPoints(5, end, blockingPoints, blockingCount);
                    break;
            }
            break;
        case 7:
            switch(end)
            {
                case 1:
                    break;
                case 0:
                    blockingPoints[blockingCount++] = 1;
                    blockingCount = fillBlockingPoints(1, end, blockingPoints, blockingCount);
                    break;
                case 8:
                    break;
                case 2:
                    break;
                default:
                    blockingPoints[blockingCount++] = 2;
                    blockingCount = fillBlockingPoints(2, end, blockingPoints, blockingCount);
                    break;
            }
            break;
        case 8:
            switch(end)
            {
                case 7:
                    break;
                default:
                    blockingPoints[blockingCount++] = 7;
                    blockingCount = fillBlockingPoints(7, end, blockingPoints, blockingCount);
                    break;
            }
            break;
        case 9:
            switch(end)
            {
                case 2:
                    break;
                case 0:
                case 1:
                case 7:
                case 8:
                    blockingPoints[blockingCount++] = 2;
                    blockingCount = fillBlockingPoints(2, end, blockingPoints, blockingCount);
                    break;
                case 10:
                    break;
                case 3:
                    break;
                default:
                    blockingPoints[blockingCount++] = 3;
                    blockingCount = fillBlockingPoints(3, end, blockingPoints, blockingCount);
                    break;
            }
            break;
        case 10:
            switch(end)
            {
                case 9:
                    break;
                default:
                    blockingPoints[blockingCount++] = 9;
                    blockingCount = fillBlockingPoints(9, end, blockingPoints, blockingCount);
                    break;
            }
            break;
        case 11:
            switch(end)
            {
                case 4:
                    break;
                case 13:
                case 14:
                case 5:
                case 6:
                    blockingPoints[blockingCount++] = 4;
                    blockingCount = fillBlockingPoints(4, end, blockingPoints, blockingCount);
                    break;
                case 12:
                    break;
                case 3:
                    break;
                default:
                    blockingPoints[blockingCount++] = 3;
                    blockingCount = fillBlockingPoints(3, end, blockingPoints, blockingCount);
                    break;
            }
            break;
        case 12:
            switch(end)
            {
                case 11:
                    break;
                default:
                    blockingPoints[blockingCount++] = 11;
                    blockingCount = fillBlockingPoints(11, end, blockingPoints, blockingCount);
                    break;
            }
            break;
        case 13:
            switch(end)
            {
                case 5:
                    break;
                case 6:
                    blockingPoints[blockingCount++] = 5;
                    blockingCount = fillBlockingPoints(5, end, blockingPoints, blockingCount);
                    break;
                case 14:
                    break;
                case 4:
                    break;
                default:
                    blockingPoints[blockingCount++] = 4;
                    blockingCount = fillBlockingPoints(4, end, blockingPoints, blockingCount);
                    break;
            }
            break;
        case 14:
            switch(end)
            {
                case 13:
                    break;
                default:
                    blockingPoints[blockingCount++] = 13;
                    blockingCount = fillBlockingPoints(13, end, blockingPoints, blockingCount);
                    break;
            }
            break;
    }
    return blockingCount;
}
int isBlocked(int start, int end, int *others)
{
    int blockingPoints[POSITIONS] = {0};
    int blockingCount = 0;
    if (end == start)
        return 0;
    blockingPoints[blockingCount++] = end;
    blockingCount = fillBlockingPoints(start, end, blockingPoints, blockingCount);
    for (int i = 0; i < POSITIONS; i++)
    {
        if (i == start)
            // This must be myself
            continue;
        for (int j = 0; j < blockingCount; j++)
            if (blockingPoints == others[i])
                return 1;
    }
    return 0;
}

int isMyRoom(int pos, int name)
{
    return ((pos == 7 || pos == 8) && name == 0) ||
        ((pos == 9 || pos == 10) && name == 1) ||
        ((pos == 11 || pos == 12) && name == 2) ||
        ((pos == 13 || pos == 14) && name == 3);
}

int getSteps(int start, int end)
{
    if (start == end)
        return 0;
    switch(start)
    {
        case 0:
            switch(end)
            {
                case 1:
                    return 1;
                default:
                    return 1 + getSteps(1, end);
            }
            break;
        case 1:
            switch(end)
            {
                case 0:
                    return 1;
                case 7:
                    return 2;
                case 8:
                    return 3;
                case 2:
                    return 2;
                default:
                    return 2 + getSteps(2, end);
            }
            break;
        case 2:
            switch(end)
            {
                case 7:
                    return 2;
                case 8:
                    return 3;
                case 1:
                    return 2;
                case 0:
                    return 2 + getSteps(1, end);
                case 9:
                    return 2;
                case 10:
                    return 3;
                case 3:
                    return 2;
                default:
                    return 2 + getSteps(2, end);
            }
            break;
        case 3:
            switch(end)
            {
                case 9:
                    return 2;
                case 10:
                    return 3;
                case 2:
                    2;
                case 0:
                case 1:
                case 7:
                case 8:
                    return 2 + getSteps(2, end);
                case 11:
                    return 2;
                case 12:
                    return 3;
                case 4:
                    return 2;
                default:
                    return 2 + getSteps(4, end);
            }
            break;
        case 4:
            switch(end)
            {
                case 13:
                    return 2;
                case 14:
                    return 3;
                case 5:
                    return 2;
                case 6:
                    return 2 + getSteps(5, end);
                case 11:
                    return 2;
                case 12:
                    return 3;
                case 3:
                    return 2;
                default:
                    return 2 + getSteps(3, end);
            }
            break;
        case 5:
            switch(end)
            {
                case 6:
                    return 1;
                case 13:
                    return 2;
                case 14:
                    return 3;
                case 4:
                    return 2;
                default:
                    return 2 + getSteps(2, end);
            }
            break;
        case 6:
            switch(end)
            {
                case 5:
                    return 1;
                default:
                    return 1 + getSteps(5, end);
            }
            break;
        case 7:
            switch(end)
            {
                case 1:
                    return 2;
                case 0:
                    return 2 + getSteps(1, end);
                case 8:
                    return 1;
                case 2:
                    return 2;
                default:
                    return 2 + getSteps(2, end);
            }
            break;
        case 8:
            switch(end)
            {
                case 7:
                    return 1;
                default:
                    return 1 + getSteps(7, end);
            }
            break;
        case 9:
            switch(end)
            {
                case 2:
                    return 2;
                case 0:
                case 1:
                case 7:
                case 8:
                    return 2 + getSteps(2, end);
                case 10:
                    return 1;
                case 3:
                    return 2;
                default:
                    return 2 + getSteps(3, end);
            }
            break;
        case 10:
            switch(end)
            {
                case 9:
                    return 1;
                default:
                    return 1 + getSteps(9, end);
            }
            break;
        case 11:
            switch(end)
            {
                case 4:
                    return 2;
                case 13:
                case 14:
                case 5:
                case 6:
                    return 2 + getSteps(4, end);
                case 12:
                    return 1;
                case 3:
                    return 2;
                default:
                    return 2 + getSteps(3, end);
            }
            break;
        case 12:
            switch(end)
            {
                case 11:
                    return 1;
                default:
                    return 1 + getSteps(11, end);
            }
            break;
        case 13:
            switch(end)
            {
                case 5:
                    return 2;
                case 6:
                    return 2 + getSteps(5, end);
                case 14:
                    return 1;
                case 4:
                    return 2;
                default:
                    return 2 + getSteps(4, end);
            }
            break;
        case 14:
            switch(end)
            {
                case 13:
                    return 1;
                default:
                    return 1 + getSteps(13, end);
            }
            break;
    }
    return blockingCount;
}

