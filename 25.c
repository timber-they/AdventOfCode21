#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define X 139
#define Y 137

#define GET(map,x,y) ((map)[((x)%(X)) + ((y)%(Y))*X])

int part1(FILE *in);
int part2(FILE *in);
// 0 -> nothing, 1 -> right, 2 -> down
char *read(FILE *in, char *buff);
int iterate(char *map);
void print(char *map);

int main()
{
    FILE *in = fopen("in25", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    int res = 1;
    char buff[X*Y] = {0};
    char *map = read(in, buff);
    while (iterate(map))
        res++;
    return res;
}

int part2(FILE *in)
{
    return 42;
}

char *read(FILE *in, char *buff)
{
    int x = 0, y = 0;
    int c;
    while ((c = fgetc(in)) != EOF)
    {
        switch(c)
        {
            case '\n':
                y++;
                x = 0;
                continue;
            case '.':
                GET(buff,x,y) = 0;
                break;
            case '>':
                GET(buff,x,y) = 1;
                break;
            case 'v':
                GET(buff,x,y) = 2;
                break;
        }
        x++;
    }
    return buff;
}

int iterate(char *map)
{
    int moved = 0;
    char new[X*Y] = {0};
    // Right
    for (int y = 0; y < Y; y++)
        for (int x = 0; x < X; x++)
            if (GET(map,x,y) == 1 && !GET(map,x+1,y))
            {
                GET(new,x+1,y) = GET(map,x,y);
                moved = 1;
            }
            else if (GET(map,x,y))
                GET(new,x,y) = GET(map,x,y);
    memcpy(map, new, X*Y*sizeof(*new));
    memset(new, 0, X*Y*sizeof(*new));
    // Down
    for (int y = 0; y < Y; y++)
        for (int x = 0; x < X; x++)
            if (GET(map,x,y) == 2 && !GET(map,x,y+1))
            {
                GET(new,x,y+1) = GET(map,x,y);
                moved = 1;
            }
            else if (GET(map,x,y))
                GET(new,x,y) = GET(map,x,y);
    memcpy(map, new, X*Y*sizeof(*new));
    
    return moved;
}

void print(char *map)
{
    for (int y = 0; y < Y; y++)
    {
        for (int x = 0; x < X; x++)
            printf("%c", GET(map,x,y) == 1 ? '>' : GET(map,x,y) == 2 ? 'v' : '.');
        printf("\n");
    }
}

