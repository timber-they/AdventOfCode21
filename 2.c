#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

typedef struct
{
    int horizontal;
    int depth;
    int aim;
} Pos;

typedef enum
{
    Forward,
    Down,
    Up
} Dir;

int part1(FILE *in);
int part2(FILE *in);
Pos takeCourse(FILE *course, Pos(*movePtr)(Dir, int, Pos));
Pos move(Dir dir, int units, Pos pos);
Pos moveAiming(Dir dir, int units, Pos pos);

int main()
{
    FILE *in = fopen("in2", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    Pos pos = takeCourse(in, &move);
    return pos.horizontal * pos.depth;
}

int part2(FILE *in)
{
    Pos pos = takeCourse(in, &moveAiming);
    return pos.horizontal * pos.depth;
}

Pos takeCourse(FILE *course, Pos(*movePtr)(Dir, int, Pos))
{
    char *line = NULL;
    size_t n = 0;
    Pos pos = (Pos){0};
    while (getline(&line, &n, course) != EOF && *line != '\n' && *line != '\0')
    {
        *strchr(line, '\n') = '\0';
        int units;
        Dir dir;
        if (sscanf(line, "forward %d", &units))
            dir = Forward;
        else if (sscanf(line, "down %d", &units))
            dir = Down;
        else if (sscanf(line, "up %d", &units))
            dir = Up;
        else
        {
            fprintf(stderr, "PANIC\n");
            exit(2);
        }

        pos = (*movePtr)(dir, units, pos);
    }
    free(line);
    return pos;
}

Pos move(Dir dir, int units, Pos pos)
{
    switch(dir)
    {
        case Forward:
            pos.horizontal += units;
            break;
        case Down:
            pos.depth += units;
            break;
        case Up:
            pos.depth -= units;
            break;
        default:
            fprintf(stderr, "PANIC\n");
            exit(1);
            break;
    }
    return pos;
}

Pos moveAiming(Dir dir, int units, Pos pos)
{
    switch(dir)
    {
        case Forward:
            pos.horizontal += units;
            pos.depth += units * pos.aim;
            break;
        case Down:
            pos.aim += units;
            break;
        case Up:
            pos.aim -= units;
            break;
        default:
            fprintf(stderr, "PANIC\n");
            exit(1);
            break;
    }
    return pos;
}
