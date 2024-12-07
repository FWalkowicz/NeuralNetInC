#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

typedef struct {
    int rows;
    int cols;
    float *data;
} Matrix;

Matrix createMatrix(const int rows, const int cols) {
    Matrix mat;
    mat.rows = rows;
    mat.cols = cols;
    mat.data = malloc(rows * cols * sizeof(float)) ;
    assert(mat.data != NULL);
    return mat;
}

float randFloat() {
    return (float) rand() / (float) RAND_MAX;
}

void initMatrix(Matrix mat) {
    for (int i = 0; i < mat.rows; i++) {
        for (int j = 0; j < mat.cols; j++) {
            mat.data[i * mat.cols + j] = randFloat() * 0.001;
        }
    }
}

void displayMatrix(Matrix mat) {
    printf("[\n");
    for (int i = 0; i < mat.rows; i++) {
        for (int j = 0; j < mat.cols; j++) {
            printf("  %f", mat.data[i * mat.cols + j]);
        }
        printf("\n");
    }
    printf("]\n");
}

int main(void) {
    Matrix mat = createMatrix(3, 3);
    initMatrix(mat);
    displayMatrix(mat);
    free(mat.data);
    return 0;
}