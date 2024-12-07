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

void dotProduct(Matrix mat_a, Matrix mat_b, Matrix result) {
    result.rows = mat_a.rows;
    result.cols = mat_b.cols;
    for (int i = 0; i < result.rows; i++) {
        for (int j = 0; j < result.cols; j++) {
            for (int k = 0; k < mat_a.cols; k++) {
                result.data[i * result.cols + j] += mat_a.data[i * result.cols + k] * mat_b.data[k * result.cols + j];
            }
        }
    }
}

void addMatrix(Matrix result, Matrix mat) {
    for (int i = 0; i < result.rows; i++) {
        for (int j = 0; j < result.cols; j++) {
            result.data[i * result.cols + j] += mat.data[i * result.cols + j];
        }
    }
}

void subtractMatrix(Matrix result, Matrix mat) {
    for (int i = 0; i < result.rows; i++) {
        for (int j = 0; j < result.cols; j++) {
            result.data[i * result.cols + j] -= mat.data[i * result.cols + j];
        }
    }
}

Matrix transposeMatrix(Matrix mat) {
    Matrix result = createMatrix(mat.cols, mat.rows);
    for (int i = 0; i < result.rows; i++) {
        for (int j = 0; j < result.cols; j++) {
            result.data[i * result.cols + j] = mat.data[j * mat.cols + i];
        }
    }
    return result;
}

void multipleMatrixByValue(Matrix result, float value) {
    for (int i = 0; i < result.rows; i++) {
        for (int j = 0; j < result.cols; j++) {
            result.data[i * result.cols + j] *= value;
        }
    }
}

void readData(Matrix DatasetMatix) {
    char line[1000];
    char *token;
    FILE *file = fopen("./Data/gold.csv", "r");
    if (file == NULL) {
        perror("Error opening file");
    }

    fgets(line, sizeof(line), file); // skip header

    for (int i = 0; i < DatasetMatix.rows; i++) {
        fgets(line, sizeof(line), file);
        token = strtok(line, ",");
        for (int j = 0; j < DatasetMatix.cols; j++) {
            DatasetMatix.data[i * DatasetMatix.cols + j] = atof(token); // przypisanie wartości z CSV do macierzy i zamiana str na float
            token = strtok(NULL, ",");
            if (token == NULL) {
                break;
            }
        }
    }
    fclose(file);
}

int main(void) {
    Matrix mat = createMatrix(3, 3);
    initMatrix(mat);
    displayMatrix(mat);
    free(mat.data);
    return 0;
}