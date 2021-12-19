#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define SCANNERS 5//35
// Each scanner sees 25 or 26 beacons
#define BEACONS 26
#define MIN_MATCH 12

typedef struct
{
    int x;
    int y;
    int z;
    int active;
} Beacon;

typedef Beacon * Scanner;

int part1(FILE *in);
int part2(FILE *in);
Scanner *read(FILE *in, Scanner *buff);
// Assumes a and b are properly transformed (except for shifts)
int matches(Scanner a, Scanner b);
// Transforms b until it matches a, if possible, otherwise returns 0
int areMatchable(Scanner a, Scanner b);
Beacon subtract(Beacon a, Beacon b);
Beacon add(Beacon a, Beacon b);
void shift(Scanner s, Beacon diff);
int countIdentical(Scanner a, Scanner b);
void alignAll(Scanner *scanners);
int count(Scanner *scanners);
void print(Scanner scanner, int size);

int main()
{
    FILE *in = fopen("test19", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    Beacon beacons[BEACONS * SCANNERS] = {0};
    Scanner buff[SCANNERS] = {0};
    for (int i = 0; i < SCANNERS; i++)
        buff[i] = &beacons[i * BEACONS];

    Scanner *scanners = read(in, buff);
    for (int i = 0; i < SCANNERS; i++)
    {
        printf("Scanner %d:\n", i);
        print(scanners[i], 9);
    }
    printf("Read\n");
    alignAll(scanners);
    printf("Aligned\n");
    return count(scanners);
}

int part2(FILE *in)
{
    return -2;
}

Scanner *read(FILE *in, Scanner *buff)
{
    char *line = NULL;
    size_t n = 0;
    int scanner = -1;
    int beacon = 0;

    int i = 0;
    while (getline(&line, &n, in) != EOF && *line != '\0')
    {
        i++;
        if (strchr(line, '\n') != NULL)
            *strchr(line, '\n') = '\0';

        if (*line == '\0')
            // Blank between scanners
            continue;
        if (line[1] == '-')
        {
            // Scanner header
            scanner++;
            beacon = 0;
            continue;
        }
        printf("i=%d, scanner=%d, beacon=%d\n", i, scanner, beacon);
        if (sscanf(line, "%d,%d,%d", 
                    &buff[scanner][beacon].x,
                    &buff[scanner][beacon].y,
                    &buff[scanner][beacon].z) != 3)
        {
            fprintf(stderr, "Couldn't parse line '%s' (%d)\n", line, i);
            exit(1);
        }
        buff[scanner][beacon].active = 1;
        beacon++;
    }

    free(line);
    return buff;
}

int matches(Scanner a, Scanner b)
{
    for (int i = 0; i < BEACONS; i++)
    {
        if (!a[i].active)
            continue;
        //int shouldMatch = 0;
        /*if (a[i].x == -618 && a[i].y == -824 && a[i].z == -621)
            shouldMatch = 1;*/
        for (int j = 0; j < BEACONS; j++)
        {
            if (!b[j].active)
                continue;
            /*if (shouldMatch && (b[j].x == 686 || b[j].x == -686 ||
                    b[j].y == 686 || b[j].y == -686 ||
                    b[j].z == 686 || b[j].z == -686))
                printf("This should match! (%d,%d,%d)\n",
                        b[j].x, b[j].y, b[j].z);*/
            Beacon diff = subtract(a[i], b[j]);
            shift(b, diff);
            if (countIdentical(a, b) >= MIN_MATCH)
                return 1;
            diff.x = -diff.x;
            diff.y = -diff.y;
            diff.z = -diff.z;
            shift(b, diff);
        }
    }
    return 0;
}

Beacon subtract(Beacon a, Beacon b)
{
    return (Beacon)
    {
        a.x - b.x,
        a.y - b.y,
        a.z - b.z,
    };
}

Beacon add(Beacon a, Beacon b)
{
    return (Beacon)
    {
        a.x + b.x,
        a.y + b.y,
        a.z + b.z,
    };
}

void shift(Scanner s, Beacon diff)
{
    for (int i = 0; i < BEACONS; i++)
    {
        if (s[i].x == 0 && s[i].y == 0 && s[i].z == 0)
            continue;
        s[i] = add(s[i], diff);
    }
}

int countIdentical(Scanner a, Scanner b)
{
    int res = 0;
    for (int i = 0; i < BEACONS; i++)
    {
        if (!a[i].active)
            continue;
        for (int j = 0; j < BEACONS; j++)
        {
            if (!b[j].active)
                continue;

            if (a[i].x == b[j].x && a[i].y == b[j].y && a[i].z == b[j].z)
                res++;
        }
    }
    return res;
}

static int areMatchableByRotation(Scanner a, Scanner b)
{
    // xyz
    if (matches(a, b))
        return 1;
    // xzy
    for (int i = 0; i < BEACONS; i++)
    {
        /*if (b[i].x == 686 || b[i].x == -686 ||
                b[i].y == 686 || b[i].y == -686 ||
                b[i].z == 686 || b[i].z == -686)
            printf("Are matchable by rotation? (%d,%d,%d) -> (%d,%d,%d)\n",
                    b[i].x, b[i].y, b[i].z,
                    b[i].x + 618, b[i].y + 824, b[i].z + 621);*/
        if (!b[i].active)
            continue;
        int x = b[i].x;
        int y = b[i].y;
        int z = b[i].z;
        b[i].x = x;
        b[i].y = z;
        b[i].z = y;
    }
    if (matches(a, b))
        return 2;
    // yzx
    for (int i = 0; i < BEACONS; i++)
    {
        if (!b[i].active)
            continue;
        int x = b[i].x;
        int z = b[i].y;
        int y = b[i].z;
        b[i].x = y;
        b[i].y = z;
        b[i].z = x;
    }
    if (matches(a, b))
        return 3;
    // yxz
    for (int i = 0; i < BEACONS; i++)
    {
        if (!b[i].active)
            continue;
        int y = b[i].x;
        int z = b[i].y;
        int x = b[i].z;
        b[i].x = y;
        b[i].y = x;
        b[i].z = z;
    }
    if (matches(a, b))
        return 4;
    // zxy
    for (int i = 0; i < BEACONS; i++)
    {
        if (!b[i].active)
            continue;
        int y = b[i].x;
        int x = b[i].y;
        int z = b[i].z;
        b[i].x = z;
        b[i].y = x;
        b[i].z = y;
    }
    if (matches(a, b))
        return 5;
    // zyx
    for (int i = 0; i < BEACONS; i++)
    {
        if (!b[i].active)
            continue;
        int z = b[i].x;
        int x = b[i].y;
        int y = b[i].z;
        b[i].x = z;
        b[i].y = y;
        b[i].z = x;
    }
    if (matches(a, b))
        return 6;
    return 0;
}
int areMatchable(Scanner a, Scanner b)
{
    // That's 48 transformations (instead of 24), so I probably
    //  have every mutation twice, but who cares
    // +++
    if (areMatchableByRotation(a, b))
        return 1;
    // ++-
    for (int i = 0; i < BEACONS; i++)
    {
        if (!b[i].active)
            continue;
        b[i].z = -b[i].z;
    }
    if (areMatchableByRotation(a, b))
        return 2;
    // +-+
    for (int i = 0; i < BEACONS; i++)
    {
        if (!b[i].active)
            continue;
        b[i].y = -b[i].y;
        b[i].z = -b[i].z;
    }
    if (areMatchableByRotation(a, b))
        return 3;
    // +--
    for (int i = 0; i < BEACONS; i++)
    {
        if (!b[i].active)
            continue;
        b[i].z = -b[i].z;
    }
    if (areMatchableByRotation(a, b))
        return 4;
    // -++
    for (int i = 0; i < BEACONS; i++)
    {
        if (!b[i].active)
            continue;
        b[i].x = -b[i].x;
        b[i].y = -b[i].y;
        b[i].z = -b[i].z;
    }
    if (areMatchableByRotation(a, b))
        return 5;
    // -+-
    for (int i = 0; i < BEACONS; i++)
    {
        if (!b[i].active)
            continue;
        b[i].z = -b[i].z;
    }
    if (areMatchableByRotation(a, b))
        return 6;
    // --+
    for (int i = 0; i < BEACONS; i++)
    {
        if (!b[i].active)
            continue;
        b[i].y = -b[i].y;
        b[i].z = -b[i].z;
    }
    if (areMatchableByRotation(a, b))
        return 7;
    // ---
    for (int i = 0; i < BEACONS; i++)
    {
        if (!b[i].active)
            continue;
        b[i].z = -b[i].z;
    }
    if (areMatchableByRotation(a, b))
        return 8;
    return 0;
}

void alignAll(Scanner *scanners)
{
    Scanner aligned[SCANNERS] = {0};
    int alignedCount = 0;
    int alignedIndices[SCANNERS] = {0};
    // Everything will be aligned b the first element
    aligned[alignedCount++] = scanners[0];
    while (alignedCount != SCANNERS)
    {
        for (int i = 0; i < SCANNERS; i++)
        {
            // Don't align aligned scanners
            int j;
            for (j = 0; j < alignedCount; j++)
                if (alignedIndices[j] == i)
                    break;
            if (j != alignedCount)
                // Already aligned
                continue;
            for (j = 0; j < alignedCount; j++)
                if (areMatchable(aligned[j], scanners[i]))
                {
                    printf("%d got aligned with %d\n", i, j);
                    break;
                }
            if (j != alignedCount)
            {
                // Got aligned, yay
                aligned[alignedCount] = scanners[i];
                alignedIndices[alignedCount++] = i;
            }
        }
        //printf("Iteration done\n");
    }
}

int count(Scanner *scanners)
{
    Beacon beacons[BEACONS * SCANNERS] = {0};
    int beaconCount = 0;

    for (int i = 0; i < SCANNERS; i++)
        for (int j = 0; j < BEACONS; j++)
        {
            if (!scanners[i][j].active)
                continue;
            int k;
            for (k = 0; k < beaconCount; k++)
                if (beacons[k].x == scanners[i][j].x &&
                        beacons[k].y == scanners[i][j].y &&
                        beacons[k].z == scanners[i][j].z)
                    break;
            if (k == beaconCount)
            {
                // Found new beacon
                printf("Found new beacon\n");
                beacons[beaconCount++] = scanners[i][j];
            }
        }

    return beaconCount;
}

void print(Scanner scanner, int size)
{
    for (int y = -size; y < size; y++)
    {
        for (int z = -size; z < size; z++)
        {
            for (int x = -size; x < size; x++)
            {
                int i;
                for (i = 0; i < BEACONS; i++)
                {
                    if (!scanner[i].active)
                        continue;
                    if (x == scanner[i].x &&
                            y == scanner[i].y &&
                            z == scanner[i].z)
                        break;
                }
                printf("%c", i == BEACONS ? '.' : '#');
            }
            printf("\t");
        }
        printf("\n");
    }
    printf("\n");
}

