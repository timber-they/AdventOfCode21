#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

typedef struct
{
    int x1;
    int x2;
    int y1;
    int y2;
    int z1;
    int z2;
    int action;
} Range;

#define INIT 50
#define SIZE ((INIT)*2+1)
// LOL
#define INSTRUCTIONS 420
// Arbitrary
#define APPLIED 1000000

#define GET(cube,x,y,z) ((cube)[(x)+(INIT) + ((y)+(INIT))*SIZE + ((z)+(INIT))*SIZE*SIZE])

int part1(FILE *in);
long part2(FILE *in);
void iterate(char *cube, FILE *in);
int count(char *cube);
Range *getRanges(FILE *in, Range *buff);
long countSmart(Range *ranges);
Range getOverlap(Range a, Range b);
long size(Range range);
int max(int a, int b);
int min(int a, int b);
int overlaps(Range a, Range b);
void print(Range r);

int main()
{
    FILE *in = fopen("in22", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %ld\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    char cube[SIZE * SIZE * SIZE] = {0};
    for (int i = 0; i < INSTRUCTIONS; i++)
        iterate(cube, in);
    return count(cube);
}

long part2(FILE *in)
{
    Range buff[INSTRUCTIONS];
    Range *ranges = getRanges(in, buff);
    return countSmart(ranges);
}

void iterate(char *cube, FILE *in)
{
    char action[4] = {0};
    int x1, x2, y1, y2, z1, z2;
    if (fscanf(in, "%[^ ] x=%d..%d,y=%d..%d,z=%d..%d\n",
                action, &x1, &x2, &y1, &y2, &z1, &z2) != 7)
    {
        fprintf(stderr, "Couldn't read input\n");
        exit(1);
    }
    if (x2 < -INIT || x1 > INIT ||
            y2 < -INIT || y1 > INIT ||
            z2 < -INIT || y1 > INIT)
        return;
    char val;
    if (!strcmp(action, "on"))
        val = 1;
    else if (!strcmp(action, "off"))
        val = 0;
    else
    {
        fprintf(stderr, "Unknown action: '%s'\n", action);
        exit(2);
    }

    for (int z = z1; z <= z2; z++)
        for (int y = y1; y <= y2; y++)
            memset(&GET(cube, x1, y, z), val, (x2-x1+1)*sizeof(*cube));
}

int count(char *cube)
{
    int res = 0;
    for (int i = 0; i < SIZE*SIZE*SIZE; i++)
        res += cube[i] != 0;
    return res;
}

Range *getRanges(FILE *in, Range *buff)
{
    for (int i = 0; i < INSTRUCTIONS; i++)
    {
        char action[4] = {0};
        if (fscanf(in, "%[^ ] x=%d..%d,y=%d..%d,z=%d..%d\n", action,
                    &buff[i].x1, &buff[i].x2,
                    &buff[i].y1, &buff[i].y2,
                    &buff[i].z1, &buff[i].z2) != 7)
        {
            fprintf(stderr, "Couldn't read input\n");
            exit(1);
        }
        if (!strcmp(action, "on"))
            buff[i].action = 1;
        else if (!strcmp(action, "off"))
            buff[i].action = 0;
        else
        {
            fprintf(stderr, "Unknown action: '%s'\n", action);
            exit(2);
        }
    }
    return buff;
}

long countSmart(Range *ranges)
{
    Range *appliedRanges = calloc(APPLIED, sizeof(*appliedRanges));
    int applied = 0;
    for (int i = 0; i < INSTRUCTIONS; i++)
    {
        Range a = ranges[i];
        int newApplied = applied;
        if (a.action)
            appliedRanges[newApplied++] = a;

        for (int j = 0; j < applied; j++)
        {
            Range b = appliedRanges[j];
            if (!size(b))
                // Instead of removing empty ones, we just ignore them
                continue;
            if (!overlaps(a, b))
                continue;
            // To the left
            appliedRanges[j] = (Range)
            {
                .x1 = b.x1,
                    .x2 = a.x1-1,
                    .y1 = b.y1,
                    .y2 = b.y2,
                    .z1 = b.z1,
                    .z2 = b.z2
            };
            // To the right
            appliedRanges[newApplied++] = (Range)
            {
                .x1 = a.x2+1,
                    .x2 = b.x2,
                    .y1 = b.y1,
                    .y2 = b.y2,
                    .z1 = b.z1,
                    .z2 = b.z2
            };
            // To the back
            appliedRanges[newApplied++] = (Range)
            {
                .x1 = max(b.x1, a.x1),
                    .x2 = min(b.x2, a.x2),
                    .y1 = b.y1,
                    .y2 = a.y1-1,
                    .z1 = b.z1,
                    .z2 = b.z2
            };
            // To the front
            appliedRanges[newApplied++] = (Range)
            {
                .x1 = max(b.x1, a.x1),
                    .x2 = min(b.x2, a.x2),
                    .y1 = a.y2+1,
                    .y2 = b.y2,
                    .z1 = b.z1,
                    .z2 = b.z2
            };
            // To the top
            appliedRanges[newApplied++] = (Range)
            {
                .x1 = max(b.x1, a.x1),
                    .x2 = min(b.x2, a.x2),
                    .y1 = max(b.y1, a.y1),
                    .y2 = min(b.y2, a.y2),
                    .z1 = b.z1,
                    .z2 = a.z1-1
            };
            // To the bottom
            appliedRanges[newApplied++] = (Range)
            {
                .x1 = max(b.x1, a.x1),
                    .x2 = min(b.x2, a.x2),
                    .y1 = max(b.y1, a.y1),
                    .y2 = min(b.y2, a.y2),
                    .z1 = a.z2+1,
                    .z2 = b.z2
            };
        }
        applied = newApplied;
    }
    long res = 0;
    for (int i = 0; i < applied; i++)
    {
        long s = size(appliedRanges[i]);
        if (s > 0)
        {
            res += s;
            //printf("Adding %ld\n", s);
        }
        if (res > 2758514936282235l)
            fprintf(stderr, "Result is getting out of hand\n");
    }
    free(appliedRanges);
    return res;
}

Range getOverlap(Range a, Range b)
{
    return (Range)
    {
        .x1 = max(a.x1, b.x1),
            .x2 = min(a.x2, b.x2),
            .y1 = max(a.y1, b.y1),
            .y2 = min(a.y2, b.y2),
            .z1 = max(a.z1, b.z1),
            .z2 = min(a.z2, b.z2)
    };
}

int overlaps(Range a, Range b)
{
    if (size(a) <= 0 || size(b) <= 0)
        return 0;
    return a.x1 <= b.x2 &&
        b.x1 <= a.x2 &&
        a.y1 <= b.y2 &&
        b.y1 <= a.y2 &&
        a.z1 <= b.z2 &&
        b.z1 <= a.z2;
}

long size(Range range)
{
    if (range.x2 < range.x1 ||
            range.y2 < range.y1 ||
            range.z2 < range.z1)
        return 0;
    return ((long) range.x2 - range.x1 + 1) *
        ((long) range.y2 - range.y1 + 1) *
        ((long) range.z2 - range.z1 + 1);
}

int max(int a, int b)
{
    return a > b ? a : b;
}

int min(int a, int b)
{
    return a < b ? a : b;
}

void print(Range r)
{
    long s = size(r);
    if (s > 0)
        printf("%d..%dx%d..%dx%d..%d (%ld)", r.x1, r.x2, r.y1, r.y2, r.z1, r.z2, s);
    else
        printf("-");
}

