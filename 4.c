#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define BOARD_COUNT 100
#define NUMBER_COUNT 100
#define BOARD_SIZE 5

#define GET(board,x,y) ((board)[(BOARD_SIZE) * (y) + (x)])

typedef struct
{
    int *board;
    int *marked;
    int lastCalled;
    int won;
} Board;

int part1(FILE *in);
int part2(FILE *in);
int *readNumbers(FILE *in, int *numberBuff);
Board *readBoards(FILE *in, Board *boardBuff, int *boardNumberBuff, int *markedBuff);
void printBoard(Board board);
// Returns the index of the won board, if exists, and -1 otherwise
int iterate(Board *boards, int number, int iteration);
int hasWon(Board board);
int getScore(Board board);

int main()
{
    FILE *in = fopen("in4", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    int numberBuff[NUMBER_COUNT];
    Board boardBuff[BOARD_COUNT];
    int boardNumberBuff[BOARD_SIZE * BOARD_SIZE * BOARD_COUNT];
    int markedBuff[BOARD_SIZE * BOARD_SIZE * BOARD_COUNT] = {0};

    int *numbers = readNumbers(in, numberBuff);
    if (fgetc(in) != '\n')
        fprintf(stderr, "Expected line break!\n");
    Board *boards = readBoards(in, boardBuff, boardNumberBuff, markedBuff);

    for (int i = 0; i < NUMBER_COUNT; i++)
    {
        int res = iterate(boards, numbers[i], i);
        if (res >= 0)
            return getScore(boards[res]);
    }

    return -1;
}

int part2(FILE *in)
{
    int numberBuff[NUMBER_COUNT];
    Board boardBuff[BOARD_COUNT];
    int boardNumberBuff[BOARD_SIZE * BOARD_SIZE * BOARD_COUNT];
    int markedBuff[BOARD_SIZE * BOARD_SIZE * BOARD_COUNT] = {0};

    int *numbers = readNumbers(in, numberBuff);
    if (fgetc(in) != '\n')
        fprintf(stderr, "Expected line break!\n");
    Board *boards = readBoards(in, boardBuff, boardNumberBuff, markedBuff);

    for (int i = 0; i < NUMBER_COUNT; i++)
        iterate(boards, numbers[i], i);

    int lastIndex = 0;
    for (int j = 1; j < BOARD_COUNT; j++)
        if (boards[j].won > boards[lastIndex].won)
            lastIndex = j;
    printf("The last board to win was board %d after round %d\n", lastIndex, boards[lastIndex].won);
    return getScore(boards[lastIndex]);;
}

int *readNumbers(FILE *in, int *numberBuff)
{
    int i = 0;
    int num = 0;
    int c;
    while ((c = fgetc(in)) != '\n')
    {
        if (c == ',')
        {
            numberBuff[i++] = num;
            num = 0;
            continue;
        }
        num = num * 10 + (c - '0');
    }
    numberBuff[i++] = num;
    return numberBuff;
}

Board *readBoards(FILE *in, Board *boardBuff, int *boardNumberBuff, int *markedBuff)
{
    int boardIndex = 0;
    int boardLine = 0;
    int boardColumn = 0;
    int c;
    while ((c = fgetc(in)) == ' ') ;
    int num = 0;
    while (1)
    {
        switch (c)
        {
            case ' ':
                GET(boardNumberBuff, boardColumn++, boardLine) = num;
                num = 0;
                break;
            case '\n':
                if (boardColumn == 0)
                {
                    // The end of a board
                    boardBuff[boardIndex++] = (Board)
                    {
                        .board = boardNumberBuff,
                        .marked = markedBuff,
                        .lastCalled = -1
                    };
                    boardNumberBuff += BOARD_SIZE * BOARD_SIZE;
                    markedBuff += BOARD_SIZE * BOARD_SIZE;
                    boardLine = 0;
                    break;
                }
                // Next board line
                GET(boardNumberBuff, boardColumn, boardLine++) = num;
                boardColumn = 0;
                num = 0;
                break;
            case EOF:
                boardBuff[boardIndex++] = (Board)
                {
                    .board = boardNumberBuff,
                    .marked = markedBuff,
                    .lastCalled = -1
                };
                return boardBuff;
            default:
                num = num * 10 + (c - '0');
                c = fgetc(in);
                continue;
        }
        while ((c = fgetc(in)) == ' ') ;
    }
    fprintf(stderr, "Didn't expect to end up here...\n");
    return boardBuff;
}

void printBoard(Board board)
{
    printf("Last: %d\n", board.lastCalled);
    for (int y = 0; y < BOARD_SIZE; y++)
    {
        for (int x = 0; x < BOARD_SIZE; x++)
            printf("%s%2d ", GET(board.marked, x, y) ? "█" : " ", GET(board.board, x, y));
        printf("\n");
    }
}

int iterate(Board *boards, int number, int iteration)
{
    int res = -1;
    for (int i = 0; i < BOARD_COUNT; i++)
    {
        if (boards[i].won)
            continue;
        for (int j = 0; j < BOARD_SIZE * BOARD_SIZE; j++)
            if (boards[i].board[j] == number)
                boards[i].marked[j] = 1;
        boards[i].lastCalled = number;
        if (hasWon(boards[i]))
        {
            boards[i].won = iteration+1;
            res = i;
        }
    }
    return res;
}

int hasWon(Board board)
{
    for (int y = 0; y < BOARD_SIZE; y++)
    {
        int x;
        for (x = 0; x < BOARD_SIZE; x++)
            if (!GET(board.marked, x, y))
                break;
        if (x == BOARD_SIZE)
            return 1;
    }
    for (int x = 0; x < BOARD_SIZE; x++)
    {
        int y;
        for (y = 0; y < BOARD_SIZE; y++)
            if (!GET(board.marked, x, y))
                break;
        if (y == BOARD_SIZE)
            return 2;
    }
    return 0;
}

int getScore(Board board)
{
    int sum = 0;
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
        if (!board.marked[i])
            sum += board.board[i];
    return sum * board.lastCalled;
}

