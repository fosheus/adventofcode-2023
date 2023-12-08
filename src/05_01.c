#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const size_t BUFFER_SIZE = 256;
static const char *SEEDS_PREFIX = "seeds:";
static const size_t MAX_SEEDS = 50;

int isEndCharacter(char character)
{
    return character == '\n' || character == '\r' || character == '\0';
}

long long *initializeForNextStep(long long *seedProduction, size_t seedCount, size_t currentProductionSize)
{

    long long *newSeedProduction = (long long *)malloc(sizeof(long long) * (seedCount * currentProductionSize));
    memcpy(newSeedProduction, seedProduction, currentProductionSize);
    free(seedProduction);
    // initialize remaining memory with -1
    for (int i = currentProductionSize; i < currentProductionSize + seedCount; i++)
    {
        newSeedProduction[i] = -1;
    }

    return newSeedProduction;
}

char *readLine(FILE *file, char *buffer, size_t bufferSize, size_t *line)
{
    if (feof(file))
    {
        return NULL;
    }
    fgets(buffer, bufferSize, file);
    *line += 1;
    return buffer;
}

long long *readSeads(char *buffer, int bufferSize, size_t *seedFound)
{

    long long *seeds = (long long *)malloc(sizeof(long long) * MAX_SEEDS);

    buffer = buffer + strlen(SEEDS_PREFIX);
    *seedFound = 0;
    while (!isEndCharacter(buffer[0]))
    {
        if (*seedFound >= MAX_SEEDS)
        {
            puts("Too much seeds, can't process");
            exit(1);
        }
        buffer++;
        char *end = 0;
        long long number = strtoll(buffer, &end, 10);
        seeds[*seedFound] = number;
        (*seedFound) += 1;
        buffer = end;
    }
    return seeds;
}

int main(int argc, char **argv)
{
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        puts("Can't open file");
        return 1;
    }
    char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));
    size_t lineCount = 0;
    buffer = readLine(file, buffer, BUFFER_SIZE, &lineCount);
    size_t seedCount = 0;
    long long *seeds = readSeads(buffer, BUFFER_SIZE, &seedCount);
    long long *seedProduction = seeds;

    int currentStep = 0;
    while ((buffer = readLine(file, buffer, BUFFER_SIZE, &lineCount)))
    {
        printf("line=%s", buffer);
        if (isEndCharacter(buffer[0]))
        {
            // consumme also next line which is just text
            readLine(file, buffer, BUFFER_SIZE, &lineCount);
            printf("CONSUMED line=%s", buffer);
            continue;
        }
        seedProduction = initializeForNextStep(seedProduction, seedCount, seedCount);
        currentStep++;

        // read the three operands of the line
        long long operands[3];
        for (int i = 0; i < 3; i++)
        {
            char *end = 0;
            operands[i] = strtoll(buffer, &end, 10);
            buffer = end;
        }

        // check if previous step in range of current line for each seed
        for (int i = 0; i < seedCount; i++)
        {
            long long previousSeedProductionStepValue = seedProduction[seedCount * (currentStep - 1) + i];
            long long currentSeedProductionStepValue = seedProduction[seedCount * (currentStep) + i];
            printf(" prev=%lld,curr=%lld ", previousSeedProductionStepValue, currentSeedProductionStepValue);
            if (currentSeedProductionStepValue != -1)
            {
                continue;
            }
            if (previousSeedProductionStepValue >= operands[0] &&
                previousSeedProductionStepValue < operands[0] + operands[2])
            {
                long long delta = previousSeedProductionStepValue - operands[0];
                seedProduction[seedCount * (currentStep) + i] = operands[1] + delta;
            }
        }
    }

    long long minFinalValue = seedProduction[seedCount * currentStep];

    for (int i = currentStep * seedCount; i < currentStep * seedCount + seedCount; i++)
    {
        if (minFinalValue < seedProduction[seedCount * currentStep + i])
        {
            minFinalValue = seedProduction[seedCount * currentStep + i];
        }
    }
    printf("\nmin=%lld\n", minFinalValue);
    printf("lines=%ld\n", lineCount);
    return 0;
}