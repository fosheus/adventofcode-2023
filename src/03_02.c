#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const unsigned int BUFF_SIZE = 256;

static const unsigned int MIN_MATRIX_HEIGHT = 10;  // for the sake of doing it

struct gear {
    int position;
    int gearRatio;
    int gearRatioSize;
};

struct gear* allocGears(size_t length) {
    struct gear* gears = (struct gear*)malloc(sizeof(struct gear) * length);
    return gears;
}

int getKnownGear(int position, struct gear* gears, size_t length, unsigned int* index) {
    for (int i = 0; i < length; i++) {
        if ((gears + i)->position == position) {
            *index = i;
            return 1;
        }
    }
    return 0;
}

struct gear* createGear(int position, struct gear* gears, size_t* length) {
    struct gear* actualGear = gears + *length;
    actualGear->gearRatio = 1;
    actualGear->position = position;
    actualGear->gearRatioSize = 0;
    *length = *length + 1;
    return actualGear;
}

void addOrUpdateGear(int position, struct gear* gears, size_t* length, int gearFactor) {
    unsigned int indexOfGear = 0;
    struct gear* actualGear = NULL;
    if (getKnownGear(position, gears, *length, &indexOfGear)) {
        actualGear = gears + indexOfGear;
    } else {
        actualGear = createGear(position, gears, length);
    }
    actualGear->gearRatio *= gearFactor;
    actualGear->gearRatioSize++;
}

struct matrix {
    char* container;
    int width;
    int heigth;
};

struct matrix* allocMatrix(int width, int heigth) {
    struct matrix* matrix = (struct matrix*)malloc(sizeof(struct matrix));
    matrix->container = (char*)malloc(width * heigth * sizeof(char*));
    matrix->width = width;
    matrix->heigth = heigth;
    return matrix;
}

void resizeMatrix(struct matrix* matrix, unsigned int newWidth, unsigned int newHeight) {  // for the sake of doing it
    char* newContainer = (char*)malloc(newWidth * newHeight * sizeof(char*));
    memcpy(newContainer, matrix->container, newWidth * newHeight);
    free(matrix->container);
    matrix->container = newContainer;
    matrix->width = newWidth;
    matrix->heigth = newHeight;
}

int isValidCoord(struct matrix* matrix, unsigned int x, unsigned int y) {
    if (x < 0 || y < 0 || x >= matrix->width || y >= matrix->heigth) {
        return 0;
    }
    return 1;
}

void positionToCoord(struct matrix* matrix, unsigned int position, unsigned int* x, unsigned int* y) {
    if (position < 0 || position > matrix->width * matrix->heigth) {
        printf("positionToCoord: Error out of bounds");
        exit(1);
    }

    *x = position % matrix->width;
    *y = (int)(position / matrix->width);
}

int positionFromCoord(struct matrix* matrix, unsigned int x, unsigned int y) {
    return y * matrix->width + x;
}

char charAtCoord(struct matrix* matrix, unsigned int x, unsigned int y) {
    if (matrix->width <= x || matrix->heigth <= y) {
        printf("charAtCoord: Error out of bounds");
        exit(1);
    }
    return matrix->container[y * matrix->width + x];
}

char charAtPosition(struct matrix* matrix, unsigned int position) {
    if (position < 0 || position > matrix->width * matrix->heigth) {
        printf("charAtPosition: Error out of bounds");
        exit(1);
    }
    return matrix->container[position];
}

void insertRowIntoMatrix(struct matrix* matrix, unsigned int x, char* buffer, unsigned int length) {
    if (length > matrix->width) {
        printf("insertRowIntoMatrix: Error out of bounds");
        exit(1);
    }
    memcpy(matrix->container + x * matrix->width, buffer, length);
}

void printMatrix(struct matrix* matrix) {
    for (int i = 0; i < matrix->heigth; i++) {
        printf("%s\n", matrix->container + matrix->width * i);
    }
}

void freeMatrix(struct matrix* matrix) {
    free(matrix->container);
    free(matrix);
}

int isGear(char character) {
    if (character == '*') {
        return 1;
    }
    return 0;
}

int getGearAdjacentToNumber(struct matrix* matrix, unsigned int position, int length) {
    unsigned int x;
    unsigned int y;
    positionToCoord(matrix, position, &x, &y);

    for (int i = -1; i <= length; i++) {
        for (int j = -1; j <= 1; j++) {
            if (j == 0 && i >= 0 && i < length) {
                // we should not check the positions of the number for optimisation
                continue;
            }
            if (!isValidCoord(matrix, x + i, y + j)) {
                continue;
            }

            if (isGear(charAtCoord(matrix, x + i, y + j))) {
                return positionFromCoord(matrix, x + i, y + j);
            }
        }
    }

    return -1;
}

int findNumberAtPosition(int position, struct matrix* matrix, int* number, int* numberLength) {
    if (!isdigit(charAtPosition(matrix, position))) {
        return 0;
    }
    char* end = 0;
    *number = strtol(matrix->container + position, &end, 10);
    *numberLength = end - matrix->container - position;
    return 1;
}

struct matrix* readFileAndBuildMatrix(FILE* file) {
    char* buffer = (char*)malloc(BUFF_SIZE * sizeof(char));
    int lineCount = 0;
    struct matrix* matrix = NULL;
    int length = 0;
    while (!feof(file)) {
        fgets(buffer, BUFF_SIZE, file);
        int pos = strcspn(buffer, "\r\n");
        buffer[pos] = 0;
        length = strlen(buffer) + 1;
        if (matrix == NULL) {
            matrix = allocMatrix(length, MIN_MATRIX_HEIGHT);
        } else if (lineCount == matrix->heigth) {  // for the sake of doing it
            resizeMatrix(matrix, matrix->width, matrix->heigth + MIN_MATRIX_HEIGHT);
        }
        insertRowIntoMatrix(matrix, lineCount, buffer, length);
        lineCount++;
    }
    printf("length=%d\n", length);

    free(buffer);
    return matrix;
}

int calculateCode(struct gear* gearCandidates, size_t length) {
    int code = 0;
    for (int i = 0; i < length; i++) {
        struct gear* actualGearCandidate = gearCandidates + i;
        if (actualGearCandidate->gearRatioSize > 1) {
            code += actualGearCandidate->gearRatio;
        }
    }
    return code;
}

int findCode(struct matrix* matrix) {
    unsigned int i = 0;
    struct gear* gears = allocGears(matrix->width * matrix->heigth);
    size_t gearsSize = 0;
    while (i < matrix->width * matrix->heigth) {
        int numberLength = 0;
        int number = 0;
        if (!findNumberAtPosition(i, matrix, &number, &numberLength)) {
            i++;
            continue;
        }
        int gearPosition = getGearAdjacentToNumber(matrix, i, numberLength);
        if (gearPosition >= 0) {
            addOrUpdateGear(gearPosition, gears, &gearsSize, number);
        }
        i += numberLength;
    }

    int code = calculateCode(gears, gearsSize);
    free(gears);
    return code;
}

int main(int argc, char** argv) {
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Can't open file");
        return 1;
    }

    struct matrix* matrix = readFileAndBuildMatrix(file);
    printMatrix(matrix);
    // printf("charat=%c\n", charAtCoord(matrix, 10, 2));
    int code = findCode(matrix);
    printf("\nthe code is : %d\n", code);
    freeMatrix(matrix);
    fclose(file);
}
