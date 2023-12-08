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
    int code = 0;
    while (!isEndCharacter(buffer[0])) {
        buffer++;
        char* end = 0;
        long number = strtol(buffer, &end, 10);
        if (includes(winningNumbers, i, number)) {
            if (code == 0) {
                code = 1;
            } else {
                code *= 2;
            }
        }
        buffer = end;
    }
    return code;
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

    while (!feof(file)) {
        fgets(buffer, BUFF_SIZE, file);
        code += decodeLine(buffer);
        lineCount++;
    }

    printf("code=%d\n", code);
    printf("lines=%d\n", lineCount);
    return 0;
}