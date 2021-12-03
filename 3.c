#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

int part1(FILE *in);
int part2(FILE *in);
int getPowerConsumption(FILE *diagnostic);
int *getOnes(FILE *diagnostic, int *scrubbed);
int getLifeSupportRating(FILE *diagnostic);
int convert(char *binary);

int main()
{
    FILE *in = fopen("in3", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    return getPowerConsumption(in);
}

int part2(FILE *in)
{
    return getLifeSupportRating(in);
}

int lines = 0;
int lineLength = 0;
int getPowerConsumption(FILE *diagnostic)
{
    int *ones = getOnes(diagnostic, NULL);
    int gamma = 0;
    int epsilon = 0;
    for (int i = 0; i < lineLength; i++)
    {
        gamma = (gamma << 1) + (ones[i] > lines / 2);
        epsilon = (epsilon << 1) + (ones[i] <= lines / 2);
    }

    free(ones);
    return gamma * epsilon;
}

int *getOnes(FILE *diagnostic, int *scrubbed)
{
    lines = 0;
    lineLength = 0;
    char *line = NULL;
    size_t n = 0;
    int *ones = NULL;
    int i = 0;

    while (getline(&line, &n, diagnostic) != EOF && *line != '\n')
    {
        *strchr(line, '\n') = '\0';
        if (ones == NULL)
        {
            lineLength = strlen(line);
            ones = calloc(lineLength, sizeof(*ones));
        }
        if (scrubbed == NULL || !scrubbed[i])
        {
            for (int i = 0; i < lineLength; i++)
                ones[i] += line[i] == '1';
            lines++;
        }
        i++;
    }
    free(line);
    return ones;
}

int getLifeSupportRating(FILE *diagnostic)
{
    int *scrubbedOxygen = calloc(lines, sizeof(*scrubbedOxygen));
    int *scrubbedCo2 = calloc(lines, sizeof(*scrubbedCo2));
    int *oxygenOnes = getOnes(diagnostic, scrubbedOxygen);
    int oxygenLines = lines;
    rewind(diagnostic);
    int *co2Ones = getOnes(diagnostic, scrubbedCo2);
    int co2Lines = lines;
    int foundOxygen = lines;
    int foundCo2 = lines;

    int oxygenRating = 0;
    int co2Rating = 0;

    char *line = NULL;
    size_t n = 0;

    int i = 0;
    while (foundOxygen > 1 || foundCo2 > 1)
    {
        int lookingForOxygen = (foundOxygen > 1);
        int lookingForCo2 = (foundCo2 > 1);
        if (lookingForOxygen)
        {
            foundOxygen = 0;
            free(oxygenOnes);
            rewind(diagnostic);
            oxygenOnes = getOnes(diagnostic, scrubbedOxygen);
            oxygenLines = lines;
        }
        if (lookingForCo2)
        {
            foundCo2 = 0;
            free(co2Ones);
            rewind(diagnostic);
            co2Ones = getOnes(diagnostic, scrubbedCo2);
            co2Lines = lines;
        }
        int j = 0;
        rewind(diagnostic);
        while (getline(&line, &n, diagnostic) != EOF && *line != '\n')
        {
            *strchr(line, '\n') = '\0';
            if (lookingForOxygen && !scrubbedOxygen[j])
            {
                scrubbedOxygen[j] = 
                    line[i] != (oxygenOnes[i] * 2 >= oxygenLines ? '1' : '0');
                if (!scrubbedOxygen[j])
                {
                    foundOxygen++;
                    if (foundOxygen == 1)
                        // This might be it!
                        oxygenRating = convert(line);
                }
            }
            if (lookingForCo2 && !scrubbedCo2[j])
            {
                scrubbedCo2[j] = 
                    line[i] != (co2Ones[i] * 2 >= co2Lines ? '0' : '1');
                if (!scrubbedCo2[j])
                {
                    foundCo2++;
                    if (foundCo2 == 1)
                        // This might be it!
                        co2Rating = convert(line);
                }
            }
            j++;
        }
        i++;
    }
    free(scrubbedOxygen);
    free(scrubbedCo2);
    free(line);
    free(oxygenOnes);
    free(co2Ones);
    return co2Rating * oxygenRating;
}

int convert(char *binary)
{
    int res = 0;
    for (int i = 0; i < lineLength; i++)
        res = (res << 1) + (binary[i] == '1');
    return res;
}

