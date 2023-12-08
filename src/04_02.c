#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const unsigned int BUFF_SIZE = 256;

int isEndCharacter(char character) {
    return character == '\n' || character == '\r' || character == '\0';
}

int includes(long* array, int length, long element) {
    for (int i = 0; i < length; i++) {
        if (array[i] == element) {
            return 1;
        }
    }
    return 0;
}

char* seekToColon(char* buffer) {
    while (buffer[0] != ':') {
        buffer++;
    }
    return buffer;
}

int decodeLine(char* buffer) {
    buffer = seekToColon(buffer);
    int i = 0;
    long winningNumbers[500];
    while (buffer[0] != '|') {
        char* end = 0;
        winningNumbers[i++] = strtol(buffer, &end, 10);
        buffer = end + 1;
    }
    buffer++;
    int matches = 0;
    while (!isEndCharacter(buffer[0])) {
        buffer++;
        char* end = 0;
        long number = strtol(buffer, &end, 10);
        if (includes(winningNumbers, i, number)) {
            matches++;
        }
        buffer = end;
    }
    return matches;
}

int cardDecodeRecusive(int* cards, int count, int currentIndex, int depth) {
    if (depth == 0) {
        return 1;
    }
    if (currentIndex >= count - 1) {
        return 0;
    }
    if (cards[currentIndex] == 0) {
        return 1;
    }
    int points = 1;
    for (int i = 1; i <= depth; i++) {
        points +=  cardDecodeRecusive(cards, count, currentIndex + i, cards[currentIndex + i]);
    }

    return points;
}

int main(int argc, char** argv) {
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Can't open file");
        return 1;
    }
    char* buffer = (char*)malloc(BUFF_SIZE * sizeof(char));
    int lineCount = 0;

    int code = 0;
    int cards[1000];

    while (!feof(file)) {
        fgets(buffer, BUFF_SIZE, file);
        cards[lineCount] = decodeLine(buffer);
        lineCount++;
    }

    for (int i = 0; i < lineCount; i++) {
        code += cardDecodeRecusive(cards, lineCount, i, cards[i]);
    }

    printf("\ncode=%d\n", code);
    printf("lines=%d\n", lineCount);
    return 0;
}