#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define STACK_SIZE 200
#define LINE_COUNT 98

int part1(FILE *in);
long part2(FILE *in);
int getScore(FILE *in, char *stackBuff);
long getScore2(FILE *in, char *stackBuff);

int main()
{
    FILE *in = fopen("in10", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %ld\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    char stackBuff[STACK_SIZE];
    return getScore(in, stackBuff);
}

long part2(FILE *in)
{
    char stackBuff[STACK_SIZE];
    return getScore2(in, stackBuff);
}

int getScore(FILE *in, char *stackBuff)
{
    char *stack = stackBuff;
    int c;
    int sum = 0;
    int lineInvalid = 0;

    int scores[256] = {0};
    scores[')'] = 3;
    scores[']'] = 57;
    scores['}'] = 1197;
    scores['>'] = 25137;

    int *closing = scores;
    closing['('] = ')';
    closing['['] = ']';
    closing['{'] = '}';
    closing['<'] = '>';

    while ((c = fgetc(in)))
    {
        if (lineInvalid && c != '\n')
            continue;
        switch(c)
        {
            case '\n':
                stack = stackBuff;
                lineInvalid = 0;
                break;
            case EOF:
                return sum;
            case '(':
            case '[':
            case '{':
            case '<':
                *(stack++) = c;
                break;
            case ')':
            case ']':
            case '}':
            case '>':
                if (closing[(int) *(--stack)] != c)
                {
                    lineInvalid = 1;
                    sum += scores[c];
                }
        }
    }
    return sum;
}

static int cmpScores(const void *p1, const void *p2)
{
    long a = *(long *) p1;
    long b = *(long *) p2;
    if (a < b)
        return -1;
    return a > b;
}

long getScore2(FILE *in, char *stackBuff)
{
    char *stack = stackBuff;
    int c;
    int lineInvalid = 0;
    long allScores[LINE_COUNT] = {0};
    int scoreCount = 0;

    int scores[256] = {0};
    scores[')'] = 1;
    scores[']'] = 2;
    scores['}'] = 3;
    scores['>'] = 4;

    int *closing = scores;
    closing['('] = ')';
    closing['['] = ']';
    closing['{'] = '}';
    closing['<'] = '>';

    while ((c = fgetc(in)))
    {
        if (lineInvalid && c != '\n')
            continue;
        switch(c)
        {
            case '\n':
                if (!lineInvalid)
                {
                    while (stack != stackBuff)
                        allScores[scoreCount] = allScores[scoreCount] * 5 + scores[closing[(int) *(--stack)]];
                    scoreCount++;
                }
                else
                    stack = stackBuff;
                lineInvalid = 0;
                break;
            case EOF:
                goto end;
            case '(':
            case '[':
            case '{':
            case '<':
                *(stack++) = c;
                break;
            case ')':
            case ']':
            case '}':
            case '>':
                if (closing[(int) *(--stack)] != c)
                    lineInvalid = 1;
                break;
        }
    }
end:
    qsort(allScores, scoreCount, sizeof(*allScores), cmpScores);
    return allScores[scoreCount / 2];
}

