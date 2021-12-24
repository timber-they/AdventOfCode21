#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define COST(map,type,from,to) ((map)[(type) + (from)*TYPES + (to)*TYPES*POSITIONS])
#define REQUIRES(map,type,to,stopped) ((map)[(type) + (to)*TYPES + (stopped)*TYPES*POSITIONS])
#define POD(map,type,pop) ((map)[(type) + pop*TYPES])

#define POSITIONS1 19
#define POSITIONS2 27
#define TYPES 4
#define POP1 2
#define POP2 4
#define HALLWAY 11
#define MEMORY 100000000
#define MAX_MOVES 2

int POP;
int POSITIONS;

int part1(FILE *in);
int part2(FILE *in);
void fillCosts(int *costs);
void fillCosts2(int *costs);
void fillRequires(int *requires);
void fillPods(FILE *in, int *pods);
void fillPods2(FILE *in, int *pods);
long id(int *pods);
long id2(int *pods);
int minCost(int *pods, long *ids, long *ids2, int *memory, int *costs, int *forbiddenStopper, int depth, int currentMin, int *moves);
int hasWon(int *pods);
void print(int *pods);
int isDone(int type, int pop, int *pods);
int getCost(int type, int *pods, int start, int end, int *mem, int *costs);

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
    POP = POP1;
    POSITIONS = POSITIONS1;
    int costs[TYPES*POSITIONS1*POSITIONS1] = {0};
    int pods[TYPES*POP1];
    for (int i = 0; i < TYPES*POP; i++)
        pods[i] = -1;
    long *ids = malloc(MEMORY * sizeof(*ids));
    for (int i = 0; i < MEMORY; i++)
        ids[i] = -1;
    long *ids2 = malloc(MEMORY * sizeof(*ids));
    for (int i = 0; i < MEMORY; i++)
        ids2[i] = -1;
    int *memory = calloc(MEMORY, sizeof(*memory));
    int forbiddenStopper[POSITIONS1] = {0};
    for (int i = 2; i <= 8; i+=2)
        forbiddenStopper[i] = 1;
    int moves[TYPES*POP1] = {0};

    fillCosts(costs);
    fillPods(in, pods);

    int res = minCost(pods, ids, ids2, memory, costs, forbiddenStopper, 0, 100000, moves);
    free(memory);
    free(ids);
    free(ids2);
    return res;
}

int part2(FILE *in)
{
    POP = POP2;
    POSITIONS = POSITIONS2;
    int costs[TYPES*POSITIONS2*POSITIONS2] = {0};
    int pods[TYPES*POP2];
    for (int i = 0; i < TYPES*POP; i++)
        pods[i] = -1;
    long *ids = malloc(MEMORY * sizeof(*ids));
    for (int i = 0; i < MEMORY; i++)
        ids[i] = -1;
    long *ids2 = malloc(MEMORY * sizeof(*ids));
    for (int i = 0; i < MEMORY; i++)
        ids2[i] = -1;
    int *memory = calloc(MEMORY, sizeof(*memory));
    int forbiddenStopper[POSITIONS2] = {0};
    for (int i = 2; i <= 8; i+=2)
        forbiddenStopper[i] = 1;
    int moves[TYPES*POP2] = {0};

    fillCosts2(costs);
    fillPods2(in, pods);

    int res = minCost(pods, ids, ids2, memory, costs, forbiddenStopper, 0, 1000000, moves);
    free(memory);
    free(ids);
    free(ids2);
    return res;
}

void fillCosts(int *costs)
{
    for (int i = 0; i < HALLWAY-1; i++)
    {
        COST(costs,0,i,i+1) = 1;
        COST(costs,0,i+1,i) = 1;
        COST(costs,1,i,i+1) = 10;
        COST(costs,1,i+1,i) = 10;
        COST(costs,2,i,i+1) = 100;
        COST(costs,2,i+1,i) = 100;
        COST(costs,3,i,i+1) = 1000;
        COST(costs,3,i+1,i) = 1000;
    }
    for (int i = 0; i < TYPES; i++)
    {
        int val = 1;
        for (int j = 0; j < i; j++)
            val *= 10;
        COST(costs,i,11,2) = val;
        COST(costs,i,11,15) = val;
        COST(costs,i,15,11) = val;
        COST(costs,i,12,4) = val;
        COST(costs,i,12,16) = val;
        COST(costs,i,16,12) = val;
        COST(costs,i,13,6) = val;
        COST(costs,i,13,17) = val;
        COST(costs,i,17,13) = val;
        COST(costs,i,14,8) = val;
        COST(costs,i,14,18) = val;
        COST(costs,i,18,14) = val;
    }
    COST(costs,0,2,11) = 1;
    COST(costs,1,4,12) = 10;
    COST(costs,2,6,13) = 100;
    COST(costs,3,8,14) = 1000;
}

/*
 * ########################
 * #0 1 2 3 4 5 6 7 8 9 10#
 * #####11##12##13##14#####
 *     #15##16##17##18#
 *     #19##20##21##22#
 *     #23##24##25##26#
 *     ################
 */
void fillCosts2(int *costs)
{
    fillCosts(costs);
    for (int i = 0; i < TYPES; i++)
    {
        int val = 1;
        for (int j = 0; j < i; j++)
            val *= 10;
        for (int j = 15; j < 22; j++)
        {
            COST(costs, i, j, j+4) = val;
            COST(costs, i, j+4, j) = val;
        }
    }
}

// Move is required, if the pod moved in the last move
void fillRequires(int *requires)
{
    for (int i = 0; i < TYPES; i++)
    {
        for (int j = 2; j <= 8; j+= 2)
        {
            REQUIRES(requires,i,j,0) = 1;
            REQUIRES(requires,i,j,1) = 1;
        }
        for (int j = 0; j < HALLWAY; j++)
            REQUIRES(requires,i,j,1) = 1;
    }
}

/*
 * ########################
 * #0 1 2 3 4 5 6 7 8 9 10#
 * #####11##12##13##14#####
 *     #15##16##17##18#
 *     #19##20##21##22#
 *     #23##24##25##26#
 *     ################
 */
void fillPods(FILE *in, int *pods)
{
    // First two lines are discarded
    fscanf(in, "%*s\n");
    fscanf(in, "%*s\n");
    char roomPods[POP1*TYPES];
    fscanf(in, "###%c#%c#%c#%c###\n  #%c#%c#%c#%c#\n",
            roomPods+0, roomPods+1, roomPods+2, roomPods+3,
            roomPods+4, roomPods+5, roomPods+6, roomPods+7) == POP1*TYPES ||
        fprintf(stderr, "Couldn't read pods\n");
    // Normalize
    for (int i = 0; i < POP*TYPES; i++)
        roomPods[i] -= 'A';
    for (int i = 0; i < POP*TYPES; i++)
    {
        int j;
        for (j = 0; j < POP; j++)
            if (POD(pods,roomPods[i],j) < 0)
            {
                POD(pods,roomPods[i], j) = HALLWAY+i;
                break;
            }
        if (j == POP)
            fprintf(stderr, "Couldn't fit pod\n");
    }
}
void fillPods2(FILE *in, int *pods)
{
    // First two lines are discarded
    fscanf(in, "%*s\n");
    fscanf(in, "%*s\n");
    char roomPods[POP2*TYPES];
    fscanf(in, "###%c#%c#%c#%c###\n  #%c#%c#%c#%c#\n",
            roomPods+0, roomPods+1, roomPods+2, roomPods+3,
            roomPods+12, roomPods+13, roomPods+14, roomPods+15) == POP1*TYPES ||
        fprintf(stderr, "Couldn't read pods\n");
    roomPods[4] = /*'A';*/'D';
    roomPods[5] = /*'B';*/'C';
    roomPods[6] = /*'C';*/'B';
    roomPods[7] = /*'D';*/'A';
    roomPods[8] = /*'A';*/'D';
    roomPods[9] = /*'B';*/'B';
    roomPods[10] = /*'C';*/'A';
    roomPods[11] = /*'D';*/'C';
    // Normalize
    for (int i = 0; i < POP*TYPES; i++)
        roomPods[i] -= 'A';
    for (int i = 0; i < POP*TYPES; i++)
    {
        int j;
        for (j = 0; j < POP; j++)
            if (POD(pods,roomPods[i],j) < 0)
            {
                POD(pods,roomPods[i], j) = HALLWAY+i;
                break;
            }
        if (j == POP)
            fprintf(stderr, "Couldn't fit pod\n");
    }
}

long id(int *pods)
{
    long res = 0;
    for (int i = 0; i < TYPES; i++)
    {
        if (POD(pods,i,0) > POD(pods,i,1))
        {
            res = res * POSITIONS + (long) POD(pods,i,1);
            res = res * POSITIONS + (long) POD(pods,i,0);
        }
        else
        {
            res = res * POSITIONS + (long) POD(pods,i,0);
            res = res * POSITIONS + (long) POD(pods,i,1);
        }
    }
    return res;
}

long id2(int *pods)
{
    if (POP == 2)
        return 0;
    long res = 0;
    for (int i = 0; i < TYPES; i++)
    {
        if (POD(pods,i,2) > POD(pods,i,3))
        {
            res = res * POSITIONS + (long) POD(pods,i,3);
            res = res * POSITIONS + (long) POD(pods,i,2);
        }
        else
        {
            res = res * POSITIONS + (long) POD(pods,i,2);
            res = res * POSITIONS + (long) POD(pods,i,3);
        }
    }
    return res;
}

/*
 * ########################
 * #0 1 2 3 4 5 6 7 8 9 10#
 * #####11##12##13##14#####
 *     #15##16##17##18#
 *     #19##20##21##22#
 *     #23##24##25##26#
 *     ################
 */
int minCost(int *pods, long *ids, long *ids2, int *memory, int *costs, int *forbiddenStopper, int depth, int currentMin, int *moves)
{
    if (depth > 240)
        return -1;
    //print(pods);
    //getchar();
    if (hasWon(pods))
        return 0;
    long currentId = id(pods);
    long currentId2 = id2(pods);
    int index;
    for (index = 0; ids[index] >= 0; index++)
        if (ids[index] == currentId && ids2[index] == currentId2)
            break;
    if (ids[index] >= 0)
        return memory[index];
    // Prevent loops
    ids[index] = currentId;
    ids2[index] = currentId2;
    memory[index] = -1;
    // Infinity
    int complete = 1;
    int min = 1<<30;

    // Move in
    for (int type = 0; type < TYPES; type++)
        for (int pop = 0; pop < POP; pop++)
        {
            // Can't move in what's not in the hallway
            if (POD(pods, type, pop) >= HALLWAY)
                continue;
            // Already moved twice
            if (POD(moves, type, pop) >= MAX_MOVES)
            {
                complete = -1;
                continue;
            }
            int start = POD(pods, type, pop);
            for (int dest = HALLWAY; dest < POSITIONS; dest++)
            {
                // We actually do want to move
                if (start == dest)
                    continue;
                // We can't jump into ourselves
                int i;
                for (i = 0; i < TYPES*POP; i++)
                    if (pods[i] == dest)
                        break;
                if (i != TYPES*POP)
                    continue;
                POD(pods, type, pop) = dest;
                int *mem = calloc(POSITIONS, sizeof(*mem));
                int cost = getCost(type, pods, start, dest, mem, costs);
                free(mem);
                if (cost >= currentMin)
                {
                    complete = 0;
                    continue;
                }
                if (cost >= 0 && cost < min)
                {
                    POD(moves, type, pop)++;
                    int new = minCost(pods, ids, ids2, memory, costs, forbiddenStopper, depth+1, currentMin, moves);
                    POD(moves, type, pop)--;
                    int total = cost + new;
                    if (new >= 0 && total <= min)
                    {
                        //printf("cost from %d to %d for type %d is: %d, yielding a total %d\n", start, dest, type, cost, total);
                        min = total;
                        if (isDone(type, pop, pods))
                        {
                            // If we completed a room, that's the best we can do
                            POD(pods, type, pop) = start;
                            memory[index] = min;
                            return min;
                        }
                    }
                }
                POD(pods, type, pop) = start;
            }
        }

    // Move out
    for (int type = TYPES-1; type >= 0; type--)
        for (int pop = POP-1; pop >= 0; pop--)
        {
            // Can't move out what's not in its room
            if (POD(pods,type,pop) < HALLWAY)
                continue;
            // Won't move out what's done
            if (isDone(type, pop, pods))
                continue;
            // Already moved twice
            if (POD(moves, type, pop) >= MAX_MOVES)
            {
                complete = -1;
                continue;
            }
            int start = POD(pods, type, pop);
            for (int dest = 0; dest < HALLWAY; dest++)
            {
                // We actually do want to move
                if (dest == start)
                    continue;
                // We can't stop here
                if (forbiddenStopper[dest])
                    continue;
                // We can't jump into ourselves
                int i;
                for (i = 0; i < TYPES*POP; i++)
                    if (pods[i] == dest)
                        break;
                if (i != TYPES*POP)
                    continue;
                //if (start == 26)
                //{
                    //printf("ACTION\n");
                    //print(pods);
                //}
                POD(pods, type, pop) = dest;
                int *mem = calloc(POSITIONS, sizeof(*mem));
                int cost = getCost(type, pods, start, dest, mem, costs);
                //if (start == 26)
                    //printf("Cost to %d is %d\n", dest, cost);
                free(mem);
                if (cost >= currentMin)
                {
                    complete = 0;
                    continue;
                }
                if (cost >= 0 && cost < min)
                {
                    POD(moves, type, pop)++;
                    int new = minCost(pods, ids, ids2, memory, costs, forbiddenStopper, depth+1, currentMin, moves);
                    POD(moves, type, pop)--;
                    int total = cost + new;
                    if (new >= 0 && total < min)
                    {
                        //printf("cost from %d to %d for type %d is: %d, yielding a total %d\n", start, dest, type, cost, total);
                        min = total;
                    }
                }
                POD(pods, type, pop) = start;
            }
        }

    if (complete > 0 || (!complete && min < 1<<29))
        memory[index] = min;
    else
    {
        ids[index] = -1;
        ids2[index] = -1;
        memory[index] = -1;
    }
    return min;
}

/*
 * ########################
 * #0 1 2 3 4 5 6 7 8 9 10#
 * #####11##12##13##14#####
 *     #15##16##17##18#
 *     ################
 */
int hasWon(int *pods)
{
    for (int i = 0; i < TYPES; i++)
        for (int j = 0; j < POP; j++)
        {
            if (POD(pods, i, j) < HALLWAY)
                return 0;
            int room = (POD(pods, i, j) - HALLWAY) % TYPES;
            if (room != i)
                return 0;
        }
    //printf("WINNER:\n");
    //print(pods);
    return 1;
}

void print(int *pods)
{
    char map[POSITIONS];
    for (int i = 0; i < POSITIONS; i++)
        map[i] = '.';
    for (int i = 0; i < TYPES; i++)
        for (int j = 0; j < POP; j++)
            map[POD(pods, i, j)] = 'A'+i;

    printf("#############\n");
    printf("#");
    for (int i = 0; i < HALLWAY; i++)
        printf("%c", map[i]);
    printf("#\n##");
    for (int i = 0; i < TYPES; i++)
        printf("#%c", map[i+HALLWAY]);
    printf("##");
    for (int j = 1; j < POP; j++)
    {
        printf("#\n  ");
        for (int i = 0; i < TYPES; i++)
            printf("#%c", map[i+j*TYPES+HALLWAY]);
    }
    printf("#\n  #########\n");
}

int isDone(int type, int pop, int *pods)
{
    int this = POD(pods, type, pop);
    int room = (this - HALLWAY) % TYPES;
    if (room != type)
        return 0;
    for (int i = this+TYPES; i < POSITIONS; i+=TYPES)
    {
        int j;
        for (j = 0; j < POP; j++)
        {
            if (j == pop)
                continue;
            if (POD(pods, type, j) == i)
                break;
        }
        if (j == POP)
            return 0;
    }
    //printf("%d is done, from\n", this);
    //print(pods);
    //printf("\n");
    return 1;
}

int getCost(int type, int *pods, int start, int end, int *mem, int *costs)
{
    if (start == end)
        return 0;
    if (mem[start])
        return mem[start];
    for (int i = 0; i < POP*TYPES; i++)
        if (pods[i] == start)
            return -1;
    mem[start] = -1;
    int min = 1<<30;
    for (int i = 0; i < POSITIONS; i++)
    {
        int cost = COST(costs, type, start, i);
        if (cost && cost < min)
        {
            int new = getCost(type, pods, i, end, mem, costs);
            if (new >= 0)
            {
                int total = new + cost;
                if (total < min)
                    min = total;
            }
        }
    }
    mem[start] = min;
    return min != 1<<30 ? min : -1;
}

