#include "matrix.h"
#include "network.h"

int main()
{
//    char *filename = "/home/jlwang/Desktop/matrix_calculate/boston.csv";
    char *filename = "D:\\J.L. Wang\\Documents\\CLionProjects\\matrix_calculate\\boston.csv";

    MATRIX *DATA = NULL;
    MATRIX *xTrain = NULL, *yTrain = NULL, *yPred = NULL;
    MATRIX *dataMax = NULL, *dataMin = NULL, *dataAverage = NULL, *temp = NULL;

    ERROR_ID errorID = _ERROR_NO_ERROR;
    STACKS S;
    LAYER_LIST layerList;
    INTEGER rows, columns;

    init_stack(&S);

    DATA = read_matrix_from_csv(filename, &errorID, &S);

    dataMin = matrix_min(DATA, Row, &errorID, &S);
    dataMax = matrix_max(DATA, Row, &errorID, &S);
    dataAverage = matrix_average(DATA, Row, &errorID, &S);
    temp = creat_matrix(dataMax->rows, dataMax->columns, &errorID, &S);
    errorID = matrix_sub(dataMax, dataMin, temp);
    errorID = matrix_sub(DATA, dataAverage, DATA);
    errorID = matrix_divide(DATA, temp, DATA);

    xTrain = matrix_cut(DATA, Column, 0, -2, &errorID, &S);
    yTrain = matrix_cut(DATA, Column, -1, -1, &errorID, &S);
//    print_matrix(yTrain, "YTrain");

    errorID = network_init(&layerList);
    errorID = add_dense_layer(20, Tanh, 0, XavierNormal, &layerList);
    errorID = add_dense_layer(1, Tanh, 0, XavierNormal, &layerList);
    errorID = network_compile(Adam, MeanSquaredError, &layerList);
    errorID = network_fit(xTrain, yTrain, 20, 50000, &layerList, &S);
    yPred = network_predict(xTrain, &layerList, &errorID, &S);
//    print_matrix(yPred, "YPRED");

    MATRIX *errY = creat_matrix(yTrain->rows, yTrain->columns, &errorID, &S);
    errorID = matrix_sub(yTrain, yPred, errY);
    errorID = matrix_pow_num(errY, 2, errY);
    MATRIX *sumerr = matrix_sum(errY, All, &errorID, &S);
    print_matrix(sumerr, "w");

    free_stack(&S);
    free_network(&layerList);
    return 0;
}
