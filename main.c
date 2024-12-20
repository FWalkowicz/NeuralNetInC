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
            mat.data[i * mat.cols + j] = randFloat() * 0.0001;
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

Matrix copyMatrix(Matrix from) {
    Matrix to = createMatrix(from.rows, from.cols);
    for (int i = 0; i < from.rows; i++) {
        for (int j = 0; j < from.cols; j++) {
            to.data[i * from.cols + j] = from.data[i * from.cols + j];
        }
    }
    return to;
}

void readData(Matrix DatasetMatrix) {
    char line[1000];
    char *token;
    FILE *file = fopen("/home/walkowiczf/CLionProjects/untitled1/Data/gold.csv", "r");
    if (file == NULL) {
        perror("Error opening file");
    }

    fgets(line, sizeof(line), file); // skip header

    for (int i = 0; i < DatasetMatrix.rows; i++) {
        fgets(line, sizeof(line), file);
        token = strtok(line, ",");
        for (int j = 0; j < DatasetMatrix.cols; j++) {
            DatasetMatrix.data[i * DatasetMatrix.cols + j] = atof(token);
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

typedef struct {
    // wejście
    Matrix a0;
    // 1 warstwa
    Matrix w1, b1, a1;
    // 2 warstwa
    Matrix w2, b2, a2;
    // wyjście
    Matrix w3, b3, a3;
} GoldModel;

GoldModel createGoldModel(void) {
    GoldModel model;
    model.a0 = createMatrix(1, 4);
    initMatrix(model.a0);
    // 1 warstwa
    model.w1 = createMatrix(4, 7);
    initMatrix(model.w1);
    model.b1 = createMatrix(1, 7);
    initMatrix(model.b1);
    model.a1 = createMatrix(1, 7);
    // 2 warstwa
    model.w2 = createMatrix(7, 7);
    initMatrix(model.w2);
    model.b2 = createMatrix(1, 7);
    initMatrix(model.w2);
    model.a2 = createMatrix(1, 7);
    initMatrix(model.a2);
    // wyjście 1 neuron
    model.w3 = createMatrix(7, 1);
    initMatrix(model.w3);
    model.b3 = createMatrix(1, 1);
    initMatrix(model.w3);
    model.a3 = createMatrix(1, 1);
    initMatrix(model.a3);

    return model;
}

float MSE(Matrix actual, Matrix predicted) {
    float mse = 0;
    for (int i = 0; i < actual.rows; i++) {
        for (int j = 0; j < actual.cols; j++) {
            float error = actual.data[i * actual.cols + j] - predicted.data[i * predicted.cols + j];
            mse += error * error;
        }
    }
    return mse / (float) actual.rows;
}

float MeanValue(Matrix mat) {
    float value = 0;
    for (int i = 0; i < mat.rows; i++) {
        for (int j = 0; j < mat.cols; j++) {
            value += mat.data[i * mat.cols + j];
        };
    }
    return value / (float)mat.rows;
}

float R_squared(Matrix actual, Matrix predicted) {
    float rss = 0;
    float tss = 0;
    float mean = MeanValue(predicted);
    for (int i = 0; i < actual.rows; i++) {
        for (int j = 0; j < actual.cols; j++) {
            rss += pow((actual.data[i * actual.cols + j] - predicted.data[i * actual.cols + j]), 2);
            tss += pow((actual.data[i * actual.cols + j] - mean),2);
        }
    }
    return 1- (rss / tss);
}

void ReLU(Matrix mat) {
    for (int i = 0; i < mat.rows; i++) {
        for (int j = 0; j < mat.cols; j++) {
            if (mat.data[i * mat.cols + j] < 0) {
                mat.data[i * mat.cols + j] = 0;
            }
        }
    }
}

void derivativeReLU(Matrix mat) {
    for (int i = 0; i < mat.rows; i++) {
        for (int j = 0; j < mat.cols; j++) {
            if (mat.data[i * mat.cols + j] < 0) {
                mat.data[i * mat.cols + j] = 0;
            }
            else {
                mat.data[i * mat.cols + j] = 1;
            }
        }
    }
}

float forward(GoldModel model) {
    dotProduct(model.a0, model.w1, model.a1);
    addMatrix(model.a1, model.b1);
    ReLU(model.a1);

    dotProduct(model.a1, model.w2, model.a2);
    addMatrix(model.a2, model.b2);
    ReLU(model.a2);

    dotProduct(model.a2, model.w3, model.a3);
    addMatrix(model.a3, model.b3);

    return *model.a3.data;
}

void backward(GoldModel model, Matrix predictions, DatasetSplit dataset_split, float learningRate) {
    // gradient dMSE/da3
    // gradient MSE względem A3(wyjścia sieci)
    float dMSEda3 = 0;
    for (int i = 0; i < predictions.rows; i++) {
        for (int j = 0; j < predictions.cols; j++) {
            dMSEda3 += (-2.0 / (float) dataset_split.trainDatasetY.rows) * (
                dataset_split.trainDatasetY.data[i] - predictions.data[i]);
        }
    }

    // dMSE/db3 = dMSE/da3 * da3/db3 = dMSE/da3 * 1
    float dMSEdb3 = dMSEda3;

    // dMSE/dw3 = dMSE/da3 * da3/da2 * da2/dw3= dMSEda3 * a2.T
    Matrix dMSEdw3 = createMatrix(model.w3.rows, model.w3.cols);
    Matrix transposedA2 = transposeMatrix(model.a2);
    for (int i = 0; i < dMSEdw3.rows; i++) {
        for (int j = 0; j < dMSEdw3.cols; j++) {
            dMSEdw3.data[i * dMSEdw3.cols + j] = transposedA2.data[i * dMSEdw3.cols + j];
        }
    }
    printf("dMSEdw3: %d x %d, transposedA2: %d x %d\n",
    dMSEdw3.rows, dMSEdw3.cols, transposedA2.rows, transposedA2.cols);
    // dMSE/da2 = dMSE/a3 * w3.T
    // Gradient propagowany do A2 - > dMSE/da2
    Matrix dMSEda2 = copyMatrix(model.w3);
    Matrix dMSEda2T = transposeMatrix(dMSEda2);
    multipleMatrixByValue(dMSEda2T, dMSEda3);

    // Gradient po ReLU dla a2
    Matrix da2 = copyMatrix(model.a2);
    derivativeReLU(da2);
    Matrix dReLUda2 = createMatrix(model.a2.rows, model.a2.cols);
    dotProduct(dMSEda2T, da2, dReLUda2);


    // Gradient względem W2
    Matrix dMSEdw2 = createMatrix(model.a1.cols, dReLUda2.cols);
    Matrix a1Copy = copyMatrix(model.a1);
    Matrix TransposedA1 = transposeMatrix(a1Copy);
    dotProduct(TransposedA1, dReLUda2, dMSEdw2);

    // dMSE/db2
    Matrix dMSEdb2 = copyMatrix(dReLUda2);

    // dMSE/da2 = dMSE/a3 * w3.T
    // Gradient propagowany do A1 - > dMSE/da1
    // dReLu/da2 @ w2.T
    Matrix w2Copy = copyMatrix(model.w2);
    Matrix TransposedW2 = transposeMatrix(w2Copy);
    Matrix dReLuda1 = createMatrix(dReLUda2.rows, TransposedW2.cols);
    dotProduct(dReLUda2, TransposedW2, dReLuda1);

    // Gradient po ReLU dla a1
    Matrix da1 = copyMatrix(model.a1);
    derivativeReLU(da1);
    Matrix dMSEda1 = createMatrix(dReLuda1.rows, da1.cols);
    dotProduct(dReLuda1, da1, dMSEda1);

    // dMSE/dw1
    Matrix a0Copy = copyMatrix(model.a0);
    Matrix TransposedA0 = transposeMatrix(a0Copy);
    Matrix dMSEdw1 = createMatrix(TransposedA0.rows, dMSEda1.cols);
    dotProduct(TransposedA0, dMSEda1, dMSEdw1);

    // dMSE/db1
    Matrix dMSEdb1 = copyMatrix(dMSEda1);

    // printf("======= b3 gradient ======\n");
    float step_size_b3 = learningRate * dMSEdb3;
    model.b3.data[0] = model.b3.data[0] - step_size_b3;

    // printf("======= w3 gradient ======\n");
    // displayMatrix(dMSEdw3);
    multipleMatrixByValue(dMSEdw3, learningRate);
    subtractMatrix(model.w3, dMSEdw3);

    // printf("======= b2 gradient ======\n");
    multipleMatrixByValue(dMSEdb2, learningRate);
    subtractMatrix(model.b2, dMSEdb2);

    // printf("======= w2 gradiet ======\n");
    multipleMatrixByValue(dMSEdw2, learningRate);
    subtractMatrix(model.w2, dMSEdw2);

    // printf("======= b1 gradiet ======\n");
    multipleMatrixByValue(dMSEdb1, learningRate);
    subtractMatrix(model.b1, dMSEdb1);

    // printf("======= w1 gradiet ======\n");
    multipleMatrixByValue(dMSEdw1, learningRate);
    subtractMatrix(model.w1, dMSEdw1);

}

typedef struct {
    Matrix TrainPredictions;
    Matrix ValidPredictions;
    Matrix TestPredictions;
} Predictions;

int main(void) {
    Matrix DatasetCSV = createMatrix(515, 5);
    readData(DatasetCSV);
    MinMaxNormalize(DatasetCSV);
    DatasetSplit Dataset = CutDataset(DatasetCSV, 0.8);
    free(DatasetCSV.data);

    GoldModel model = createGoldModel();

    Predictions predictions;
    predictions.TrainPredictions = createMatrix(Dataset.trainDatasetX.rows, 1);
    predictions.ValidPredictions = createMatrix(Dataset.validDatasetX.rows, 1);
    predictions.TestPredictions = createMatrix(Dataset.testDatasetX.rows, 1);

    printf("Valid rows: %d", predictions.ValidPredictions.rows);

    const int epochs = 10;
    for (int epoch = 0; epoch < epochs; epoch++) {
        // Training phase
        for (int i = 0; i < Dataset.trainDatasetX.rows; i++) {
            memset(model.a0.data, 0, sizeof(float) * model.a0.cols);
            for (int j = 0; j < Dataset.trainDatasetX.cols; j++) {
                model.a0.data[j] = Dataset.trainDatasetX.data[i * Dataset.trainDatasetX.cols + j];
            }
            predictions.TrainPredictions.data[i] = forward(model);
        }
        backward(model, predictions.TrainPredictions, Dataset, 0.00000001);
        // Validation phase
        for (int i = 0; i < Dataset.validDatasetX.rows; i++) {
            memset(model.a0.data, 0, sizeof(float) * model.a0.cols);
            for (int j = 0; j < Dataset.validDatasetX.cols; j++) {
                model.a0.data[j] = Dataset.validDatasetX.data[i * Dataset.validDatasetX.cols + j];
            }
            predictions.ValidPredictions.data[i] = forward(model);
        }
        float TrainMse = MSE(Dataset.trainDatasetY, predictions.TrainPredictions);
        float ValidMse = MSE(Dataset.validDatasetY, predictions.ValidPredictions);
        float RSquaredTrain = R_squared(Dataset.trainDatasetY, predictions.TrainPredictions);
        float RSquaredValid = R_squared(Dataset.validDatasetY, predictions.ValidPredictions);
        printf("Epoch %d/%d %d/%d [===========================] - ms/step - MSE loss: %.03f, R^2: %f  val_loss : %.02f\n",
            epoch + 1, epochs, Dataset.trainDatasetX.rows,
            predictions.TrainPredictions.rows, TrainMse,
            RSquaredTrain);


    }

    return 0;
}