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
            DatasetMatix.data[i * DatasetMatix.cols + j] = atof(token);
            token = strtok(NULL, ",");
            if (token == NULL) {
                break;
            }
        }
    }
    fclose(file);
}

float findMax(Matrix mat, int column) {
    float max = 0;
    for (int i = 0; i < mat.rows; i++) {
        if (mat.data[i * mat.cols + column] > max) {
            max = mat.data[i * mat.cols + column];
        }
    }

    return max;
}

float findMin(Matrix mat, int column) {
    float min = mat.data[mat.cols + column];
    for (int i = 0; i < mat.rows; i++) {
        if (mat.data[i * mat.cols + column] < min) {
            min = mat.data[i * mat.cols + column];
        }
    }

    return min;
}

void MinMaxNormalize(Matrix mat) {
    for (int i = 1; i < mat.cols; i++) {
        float min = findMin(mat, i);
        float max = findMax(mat, i);
        for (int j = 0; j < mat.rows; j++) {
            mat.data[j * mat.cols + i] = (mat.data[j * mat.cols + i] - min) / (max - min);
        }
    }
}

typedef struct {
    Matrix trainDatasetX;
    Matrix trainDatasetY;
    Matrix testDatasetX;
    Matrix testDatasetY;
    Matrix validDatasetX;
    Matrix validDatasetY;
} DatasetSplit;

DatasetSplit CutDataset(Matrix mat, float trainPercent) {
    int trainDatasetSize = (int)(mat.rows * trainPercent);
    int testDatasetSize = (mat.rows - trainDatasetSize) / 2 + (mat.rows % 2);;
    int validDatasetSize = mat.rows - trainDatasetSize - testDatasetSize;;

    printf("train-%d | test-%d | valid-%d \n", trainDatasetSize, testDatasetSize, validDatasetSize);

    DatasetSplit result;

    result.trainDatasetX = createMatrix(trainDatasetSize, mat.cols - 1);
    result.trainDatasetY = createMatrix(trainDatasetSize, 1);
    result.testDatasetX = createMatrix(testDatasetSize, mat.cols - 1);
    result.testDatasetY = createMatrix(testDatasetSize, 1);
    result.validDatasetX = createMatrix(validDatasetSize, mat.cols - 1);
    result.validDatasetY = createMatrix(validDatasetSize, 1);

    for (int i = 0; i < trainDatasetSize; i++) {
        for (int j = 0; j < mat.cols; j++) {
            if (j == 0) {
                result.trainDatasetY.data[i] = mat.data[i * mat.cols + j];
            }else{
                result.trainDatasetX.data[i * result.trainDatasetX.cols + j - 1] = mat.data[i * mat.cols + j];
            }
        }
    }
    for (int i = trainDatasetSize; i < trainDatasetSize + testDatasetSize; i++) {
        for (int j = 0; j < mat.cols; j++) {
            if (j == 0) {
                result.testDatasetY.data[(i - trainDatasetSize) * result.testDatasetY.cols + j] = mat.data[i * mat.cols + j];
            }else {
                result.testDatasetX.data[(i - trainDatasetSize) * result.testDatasetX.cols + j - 1] = mat.data[i * mat.cols + j];
            }
        }
    }

    for (int i = trainDatasetSize + testDatasetSize; i < mat.rows; i++) {
        for (int j = 0; j < mat.cols; j++) {
            if (j == 0) {
                result.validDatasetY.data[(i - trainDatasetSize - testDatasetSize) * result.testDatasetY.cols + j] = mat.data[i * mat.cols + j];
            }else {
                result.validDatasetX.data[(i - trainDatasetSize - testDatasetSize) * result.validDatasetX.cols + j - 1] = mat.data[i * mat.cols + j];
            }
        }
    }

    return result;
}

int main(void) {
    Matrix mat = createMatrix(3, 3);
    initMatrix(mat);
    displayMatrix(mat);
    free(mat.data);
    return 0;
}