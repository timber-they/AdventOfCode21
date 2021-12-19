#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define SCANNERS 35
// Each scanner sees 25 or 26 beacons
#define BEACONS 27
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
int manhattan(Scanner *scanners);
int dist(Beacon a, Beacon b);
void print(Scanner scanner, int size);

int main()
{
    FILE *in = fopen("in19", "r");

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
    alignAll(scanners);
    return count(scanners);
}

int part2(FILE *in)
{
    Beacon beacons[BEACONS * SCANNERS] = {0};
    Scanner buff[SCANNERS] = {0};
    for (int i = 0; i < SCANNERS; i++)
        buff[i] = &beacons[i * BEACONS];

    Scanner *scanners = read(in, buff);
    alignAll(scanners);
    return manhattan(scanners);
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
        for (int j = 0; j < BEACONS; j++)
        {
            if (!b[j].active)
                continue;
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
        a.active,
    };
}

Beacon add(Beacon a, Beacon b)
{
    return (Beacon)
    {
        a.x + b.x,
        a.y + b.y,
        a.z + b.z,
        a.active,
    };
}

void shift(Scanner s, Beacon diff)
{
    for (int i = 0; i < BEACONS; i++)
        s[i] = add(s[i], diff);
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
    // yzx
    for (int i = 0; i < BEACONS; i++)
    {
        int x = b[i].x;
        int y = b[i].y;
        int z = b[i].z;
        b[i].x = y;
        b[i].y = z;
        b[i].z = x;
    }
    if (matches(a, b))
        return 2;
    // zxy
    for (int i = 0; i < BEACONS; i++)
    {
        int y = b[i].x;
        int z = b[i].y;
        int x = b[i].z;
        b[i].x = z;
        b[i].y = x;
        b[i].z = y;
    }
    if (matches(a, b))
        return 3;
    for (int i = 0; i < BEACONS; i++)
    {
        int z = b[i].x;
        int x = b[i].y;
        int y = b[i].z;
        b[i].x = x;
        b[i].y = y;
        b[i].z = z;
    }
    return 0;
}
int areMatchable(Scanner a, Scanner b)
{
    // zxy
    for (int i = 0; i < BEACONS; i++)
    {
        int x = b[i].x;
        int y = b[i].y;
        int z = b[i].z;
        b[i].x = z;
        b[i].y = x;
        b[i].z = y;
    }
    if (areMatchableByRotation(a, b))
        return 1;
    // z-yx
    for (int i = 0; i < BEACONS; i++)
    {
        int z = b[i].x;
        int x = b[i].y;
        int y = b[i].z;
        b[i].x = z;
        b[i].y = -y;
        b[i].z = x;
    }
    if (areMatchableByRotation(a, b))
        return 2;
    // z-x-y
    for (int i = 0; i < BEACONS; i++)
    {
        int z = b[i].x;
        int y = -b[i].y;
        int x = b[i].z;
        b[i].x = z;
        b[i].y = -x;
        b[i].z = -y;
    }
    if (areMatchableByRotation(a, b))
        return 3;
    // zy-x
    for (int i = 0; i < BEACONS; i++)
    {
        int z = b[i].x;
        int x = -b[i].y;
        int y = -b[i].z;
        b[i].x = z;
        b[i].y = y;
        b[i].z = -x;
    }
    if (areMatchableByRotation(a, b))
        return 4;
    // -z-xy
    for (int i = 0; i < BEACONS; i++)
    {
        int z = b[i].x;
        int y = b[i].y;
        int x = -b[i].z;
        b[i].x = -z;
        b[i].y = -x;
        b[i].z = y;
    }
    if (areMatchableByRotation(a, b))
        return 5;
    // -z-y-x
    for (int i = 0; i < BEACONS; i++)
    {
        int z = -b[i].x;
        int x = -b[i].y;
        int y = b[i].z;
        b[i].x = -z;
        b[i].y = -y;
        b[i].z = -x;
    }
    if (areMatchableByRotation(a, b))
        return 6;
    // -zx-y
    for (int i = 0; i < BEACONS; i++)
    {
        int z = -b[i].x;
        int y = -b[i].y;
        int x = -b[i].z;
        b[i].x = -z;
        b[i].y = x;
        b[i].z = -y;
    }
    if (areMatchableByRotation(a, b))
        return 7;
    // -zyx
    for (int i = 0; i < BEACONS; i++)
    {
        int z = -b[i].x;
        int x = b[i].y;
        int y = -b[i].z;
        b[i].x = -z;
        b[i].y = y;
        b[i].z = x;
    }
    if (areMatchableByRotation(a, b))
        return 8;
    for (int i = 0; i < BEACONS; i++)
    {
        int z = -b[i].x;
        int y = b[i].y;
        int x = b[i].z;
        b[i].x = x;
        b[i].y = y;
        b[i].z = z;
    }
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
                    break;
            if (j != alignedCount)
            {
                // Got aligned, yay
                aligned[alignedCount] = scanners[i];
                alignedIndices[alignedCount++] = i;
            }
        }
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
                beacons[beaconCount++] = scanners[i][j];
            }
        }

    return beaconCount;
}

int manhattan(Scanner *scanners)
{
    Beacon beacons[BEACONS * SCANNERS] = {0};
    int beaconCount = 0;

    for (int i = 0; i < SCANNERS; i++)
    {
        int j = BEACONS - 1;
        int k;
        for (k = 0; k < beaconCount; k++)
            if (beacons[k].x == scanners[i][j].x &&
                    beacons[k].y == scanners[i][j].y &&
                    beacons[k].z == scanners[i][j].z)
                break;
        if (k == beaconCount)
        {
            // Found new beacon
            beacons[beaconCount++] = scanners[i][j];
        }
    }

    int maxDistance = 0;
    for (int i = 0; i < beaconCount; i++)
        for (int j = 0; j < beaconCount; j++)
        {
            int distance = dist(beacons[i], beacons[j]);
            if (distance > maxDistance)
                maxDistance = distance;
        }

    return maxDistance;
}

int dist(Beacon a, Beacon b)
{
    int x = a.x - b.x;
    if (x < 0)
        x = -x;
    int y = a.y - b.y;
    if (y < 0)
        y = -y;
    int z = a.z - b.z;
    if (z < 0)
        z = -z;
    return x + y + z;
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

