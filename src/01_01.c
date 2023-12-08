#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const int RADIX = 10;

int findNumber(char buffer[256], size_t length) {
    int i = 0;
    int leftOperand = -1;
    int rightOperand = -1;

    while (i < length) {
        char currentChar = *(buffer + i);
        if (isdigit(currentChar)) {  // we have a digit
            int digit = currentChar - 0x30;
            if (leftOperand == -1) {
                leftOperand = digit;
            }
            rightOperand = digit;
        }
        i++;
    }
    printf("%d%d -> %d \n", leftOperand, rightOperand, leftOperand * RADIX + rightOperand);
    return leftOperand * RADIX + rightOperand;
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
        printf("%d %d :", lineCount, read);
        int number = findNumber(buffer, read);
        printf("%d\n", number);
        sum += number;
    }
    fclose(file);
    printf("%d\n", sum);

    return 0;
}