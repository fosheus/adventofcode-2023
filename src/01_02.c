#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct number {
    char name[10];
    int value;
};

const struct number numbers[] = {
    {"zero", 0},
    {"one", 1},
    {"two", 2},
    {"three", 3},
    {"four", 4},
    {"five", 5},
    {"six", 6},
    {"seven", 7},
    {"eight", 8},
    {"nine", 9},
    {"0", 0},
    {"1", 1},
    {"2", 2},
    {"3", 3},
    {"4", 4},
    {"5", 5},
    {"6", 6},
    {"7", 7},
    {"8", 8},
    {"9", 9}};

static int LENGTH_OF_ARRAY = sizeof(numbers) / sizeof(struct number);

int findNumber(char buffer[256], size_t length) {
    int encountered[100];
    int encounteredSize = 0;
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < LENGTH_OF_ARRAY; j++) {
            size_t lenghtOfCurrentNumber = strlen(numbers[j].name);
            if (i + lenghtOfCurrentNumber <= length) {
                if (strncmp(buffer + i, numbers[j].name, lenghtOfCurrentNumber) == 0) {
                    if (encounteredSize == 100) {
                        printf("ERROR encountered size exceeded");
                        exit(1);
                    }
                    encountered[encounteredSize++] = numbers[j].value;
                    break;
                }
            }
        }
    }
    printf("encountered=");
    for (int i = 0; i < encounteredSize; i++) {
        printf("%d,", encountered[i]);
    }
    printf(" => ");
    if (encounteredSize == 0) {
        return 0;
    }
    if (encounteredSize == 1) {
        return encountered[0] * 10 + encountered[0];
    }
    return encountered[0] * 10 + encountered[encounteredSize - 1];
}

int main(int argc, char** argv) {
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Can't open file");
        return 1;
    }
    char* buffer = 0;
    size_t length = 0;
    int sum = 0;
    int lineCount = 0;
    int read = 0;

    while ((read = getline(&buffer, &length, file)) != -1) {
        lineCount++;
        printf("%d %d ", lineCount, read);
        int number = findNumber(buffer, read);
        printf("%d\n", number);
        sum += number;
    }
    fclose(file);

    printf("%d\n", sum);

    return 0;
}