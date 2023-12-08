#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct decoded_line {
    int id;
    int maxBlue;
    int maxRed;
    int maxGreen;
};

enum color {
    BLUE,
    RED,
    GREEN
};

char* readOutGame(char* line) {
    return line + 5;
}

char* readGameId(char* line, int* id) {
    char* end;
    *id = (int)strtol(line, &end, 10);
    return end + 1;  // +1 removes ":"
}

char* readNumber(char* line, int* number) {
    char* end;
    *number = (int)strtol(line, &end, 10);
    return end + 1;  // +1 removes " "
}

char* readColor(char* line, enum color* c) {
    if (strncmp(line, "blue", strlen("blue")) == 0) {
        *c = BLUE;
        return line + strlen("blue");
    }

    if (strncmp(line, "red", strlen("red")) == 0) {
        *c = RED;
        return line + strlen("red");
    }

    if (strncmp(line, "green", strlen("green")) == 0) {
        *c = GREEN;
        return line + strlen("green");
    }
    return line;
}

char* readGroup(char* line, struct decoded_line* out) {
    char lastChar = 1;  // ¯\_(ツ)_/¯
    while (lastChar != 0 && lastChar != '\n' && lastChar != '\r') {
        int number = 0;
        enum color c;
        line = readNumber(line, &number);
        line = readColor(line, &c);
        char* ptr = out;
        if (*(ptr + sizeof(int) + sizeof(int) * ((int)c)) < number) {  // magic polymorphism
            *(ptr + sizeof(int) + sizeof(int) * ((int)c)) = number;
        }
        lastChar = line[0];
        line = line + 1;  // + 1 removes ',' or ';' but can be '\0', '\n' or '\r'
    }
    return line;
}

int isPossible(struct decoded_line* line) {
    if (line->maxBlue <= 14 && line->maxRed <= 12 && line->maxGreen <= 13) {
        return 1;
    }
    return 0;
}

void readLine(char* line, int length, struct decoded_line* out) {
    int gameId = 0;
    line = readOutGame(line);
    line = readGameId(line, &gameId);
    out->id = gameId;

    line = readGroup(line, out);
}

int main(int argc, char** argv) {
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Can't open file");
        return 1;
    }

    char* buffer = 0;
    size_t length = 0;
    int lineCount = 0;
    int read = 0;
    struct decoded_line decoded[1000];
    while ((read = getline(&buffer, &length, file)) != -1) {
        readLine(buffer, read, decoded + lineCount);
        lineCount++;
    }

    int sum = 0;
    for (int i = 0; i < lineCount; i++) {
        if (isPossible(decoded + i)) {
            printf("%d ", decoded[i].id);
            sum += decoded[i].id;
        }
    }
    printf("sum=%d\n", sum);

    printf("%d\n", lineCount);

    return 0;
}