/*******************************************************************************
 *     File Name :                        matrix.c
 *     Abstract Description :             矩阵运算 (based on Marc Pony's MatrixComputation)
 *     Todo :                             (function) matrix_sort, unfulfilled
 *                                        (function) matrix_cut, row and column cut
 *                                        (function) read_matrix_from_csv, Open file only once
 *                                        judge overwrite the original data, function matrix_average_broadcast overwrite the original data
 *                                        Multiple matrix calculations
 *                                        large matrix not create by heap
 *                                        ...
 *******************************************************************************/

/*******************************************************************************
 * (1)Debug Switch Section
 *******************************************************************************/

/*******************************************************************************
 * (2)Include File Section
 *******************************************************************************/
#include "matrix.h"

/*******************************************************************************
 * (3)Macro Define Section
 *******************************************************************************/

/*******************************************************************************
 * (4)Struct(Data Types) Define Section
 *******************************************************************************/

/*******************************************************************************
 * (5)Prototype Declare Section
 *******************************************************************************/

/*******************************************************************************
 * (6)Global Variable Declare Section
 *******************************************************************************/

/*******************************************************************************
 * (7)File Static Variable Define Section
 *******************************************************************************/

/*******************************************************************************
 * (8)Function Define Section
 *******************************************************************************/
FLAG is_in_calculate_tab(CALCULATE_CHOOSE choose)
{
    FLAG flag = false;

    switch (choose)
    {
#define CALCULATE_MACRO(a, b) case a:
        CALCULATE_TAB
#undef CALCULATE_MACRO

        flag = true;
        break;

    default:
        break;
    }

    return flag;
}

/**********************************************************************************************
Function: init_stack
Description: 初始化栈
Input: 无
Output: 无
Input_Output: 栈指针
Return: 无
***********************************************************************************************/
VOID init_stack(_IN_OUT STACKS *S)
{
    if (S == NULL)
    {
        return;
    }

    memset(S, 0, sizeof(STACKS));
}

/**********************************************************************************************
Function: free_stack
Description: 释放栈
Input: 栈指针
Output: 无
Input_Output: 无
Return: 无
***********************************************************************************************/
VOID free_stack(_IN STACKS *S)
{
    MATRIX_NODE *matrixNode = NULL;
    MATRIX_ELEMENT_NODE *matrixElementNode = NULL;

    if (S == NULL)
    {
        return;
    }

    int n = 0;
    // 从后往前依次释放矩阵栈中的矩阵和矩阵数据节点
    while (S->matrixNode != NULL)
    {
        n = n+1;
        matrixNode = S->matrixNode;
        S->matrixNode = matrixNode->next;

        free(matrixNode->ptr);
        matrixNode->ptr = NULL;
        free(matrixNode);
        matrixNode = NULL;
    }

    while (S->matrixElementNode != NULL)
    {
        matrixElementNode = S->matrixElementNode;
        S->matrixElementNode = matrixElementNode->next;

        free(matrixElementNode->ptr);
        matrixElementNode->ptr = NULL;
        free(matrixElementNode);
        matrixElementNode = NULL;
    }

    // ...
    // 释放其他指针
}

/**********************************************************************************************
Function: print_matrix
Description: 输出矩阵数据
Input: 矩阵A，名称name
Output: 无
Input_Output: 无
Return: 无
***********************************************************************************************/
VOID print_matrix(MATRIX *A, NAME name)
{
    INDEX i, j;
    printf("matrix %s:", name);
    printf("\n");
    for (i = 0; i < A->rows; i++)
    {
        for (j = 0; j < A->columns; j++)
        {
            printf("%f  ", A->p[i * A->columns + j]);
        }
        printf("\n");
    }

    printf("\n");
}

/**********************************************************************************************
矩阵创建与剪切
***********************************************************************************************/

/**********************************************************************************************
Function: creat_matrix
Description: 创建矩阵
Input: 矩阵行数rows，列数columns
Output: 错误号指针errorID，栈指针S
Input_Output: 无
Return: 矩阵指针
***********************************************************************************************/
MATRIX *creat_matrix(_IN INTEGER rows, _IN INTEGER columns, _OUT ERROR_ID *errorID, _OUT STACKS *S)
{
    MATRIX *matrix = NULL;
    MATRIX_NODE *matrixNode = NULL;
    MATRIX_ELEMENT_NODE *matrixElementNode = NULL;

    // 输入参数判断
    if (errorID == NULL)
    {
        return NULL;
    }

    *errorID = _ERROR_NO_ERROR;
    if (rows <= 0 || columns <= 0 || S == NULL)
    {
        *errorID = _ERROR_INPUT_PARAMETERS_ERROR;
        return NULL;
    }

    // 分配矩阵、矩阵节点、矩阵数据节点内存
    matrix = (MATRIX *)malloc(sizeof(MATRIX));
    matrixNode = (MATRIX_NODE *)malloc(sizeof(MATRIX_NODE));
    matrixElementNode = (MATRIX_ELEMENT_NODE *)malloc(sizeof(MATRIX_ELEMENT_NODE));
    if (matrix == NULL || matrixNode == NULL || matrixElementNode == NULL)
    {
        free(matrix);
        matrix = NULL;
        free(matrixNode);
        matrixNode = NULL;
        free(matrixElementNode);
        matrixElementNode = NULL;

        *errorID = _ERROR_FAILED_TO_ALLOCATE_HEAP_MEMORY;
        return NULL;
    }

    // 分配矩阵数据内存
    matrix->rows = rows;
    matrix->columns = columns;
    matrix->p = (REAL *)malloc(rows * columns * sizeof(REAL)); //确保matrix非空才能执行指针操作
    if (matrix->p == NULL)
    {
        free(matrix->p);
        matrix->p = NULL;
        free(matrix);
        matrix = NULL;
        free(matrixNode);
        matrixNode = NULL;
        free(matrixElementNode);
        matrixElementNode = NULL;

        *errorID = _ERROR_FAILED_TO_ALLOCATE_HEAP_MEMORY;
        return NULL;
    }

    // 矩阵节点指向矩阵，矩阵数据节点指向矩阵数据，矩阵栈存储指向矩阵和矩阵数据节点的指针
    matrixNode->ptr = matrix;
    matrixNode->next = S->matrixNode;
    S->matrixNode = matrixNode;

    matrixElementNode->ptr = matrix->p;
    matrixElementNode->next = S->matrixElementNode;
    S->matrixElementNode = matrixElementNode;

    return matrix;
}

/**********************************************************************************************
Function: creat_multiple_matrices
Description: 创建多个矩阵
Input: 矩阵行数rows，列数columns，个数count
Output: 错误号指针errorID，栈指针S
Input_Output: 无
Return: 矩阵指针
***********************************************************************************************/
MATRIX *creat_multiple_matrices(_IN INTEGER rows, _IN INTEGER columns, _IN INTEGER count, _OUT ERROR_ID *errorID, _OUT STACKS *S)
{
    INDEX i;
    MATRIX *matrix = NULL, *p = NULL;
    MATRIX_NODE *matrixNode = NULL;

    // 输入参数判断
    if (errorID == NULL)
    {
        return NULL;
    }

    if (rows <= 0 || columns <= 0 || count <= 0 || S == NULL)
    {
        *errorID = _ERROR_INPUT_PARAMETERS_ERROR;
        return NULL;
    }

    // 分配矩阵、矩阵节点节点内存
    matrix = (MATRIX *)malloc(count * sizeof(MATRIX));
    matrixNode = (MATRIX_NODE *)malloc(sizeof(MATRIX_NODE));
    if (matrix == NULL || matrixNode == NULL)
    {
        free(matrix);
        matrix = NULL;
        free(matrixNode);
        matrixNode = NULL;

        *errorID = _ERROR_FAILED_TO_ALLOCATE_HEAP_MEMORY;
        return NULL;
    }

    // 利用creat_matrix函数创建多个矩阵
    for (i = 0; i < count; i++)
    {
        p = creat_matrix(rows, columns, errorID, S);
        if (p == NULL)
        {
            free(matrix);
            matrix = NULL;
            free(matrixNode);
            matrixNode = NULL;

            *errorID = _ERROR_FAILED_TO_ALLOCATE_HEAP_MEMORY;
            return NULL;
        }

        matrix[i] = *p;
    }

    // 将matrix加入到矩阵栈中，方便统一释放（数据已在创建矩阵时加入）
    matrixNode->ptr = matrix;
    matrixNode->next = S->matrixNode;
    S->matrixNode = matrixNode;

    return matrix;
}

/**********************************************************************************************
Function: read_matrix_from_csv
Description: 从CSV文件中读取矩阵（默认第一行为表头)
Input: 文件名称filename
Output: 错误号指针errorID，栈指针S
Input_Output: 无
Return: 矩阵指针
***********************************************************************************************/
MATRIX *read_matrix_from_csv(_IN NAME filename, _OUT ERROR_ID *errorID, _OUT STACKS *S)
{
    MATRIX *matrix = NULL;
    CHAR line[_MAX_SIZE];
    INTEGER rows = 0, columns = 0;

    // 输入参数判断，S == NULL 会在creat_matrix中判断，此处不再判断
    if (errorID == NULL)
    {
        return NULL;
    }

    // 打开文件进行读取
    FILE *stream = fopen(filename, "r");
    if (!stream)
    {
        *errorID = _ERROR_OPEN_FILE_ERROR;
        return NULL;
    }

    // 读取文件列数
    fgets(line, _MAX_SIZE, stream);
    char *token = strtok(line, ",");
    while (token)
    {
        token = strtok(NULL, ",");
        columns++;
    }

    // 读取文件行数
    while (fgets(line, _MAX_SIZE, stream))
    {
        rows++;
    }
    fclose(stream);

    // 判断文件是否为空
    if (rows == 0 || columns == 0)
    {
        *errorID = _ERROR_EMPTY_FILE;
        return NULL;
    }

    matrix = creat_matrix(rows, columns, errorID, S);
    if (*errorID == _ERROR_NO_ERROR)
    {
        // 再次打开文件用于数据读取，TODO:仅打开一次文件，读取行列数和数据
        stream = fopen(filename, "r");
        if (!stream)
        {
            *errorID = _ERROR_OPEN_FILE_ERROR;
            return NULL;
        }

        INTEGER row = 0, col = 0;
        STRING tok;
        STRING ptr;
        //逐行读取，默认首行为描述信息，不进行读取
        fgets(line, _MAX_SIZE, stream);
        while (fgets(line, _MAX_SIZE, stream))
        {
            col = 0;
            // 利用,分隔符读取数据
            for (tok = strtok(line, ","); tok && *tok; col++, tok = strtok(NULL, ",\n"))
            {
//                matrix->p[row * columns + col] = atof(tok); //转换成浮点数
                matrix->p[row * columns + col] = strtod(tok, &ptr); //转换成浮点数
            }
            row++;
        }
        fclose(stream);
    }

    return matrix;
}

/**********************************************************************************************
Function: creat_zero_matrix
Description: 创建零矩阵
Input: 矩阵行数rows，列数columns
Output: 错误号指针errorID，栈指针S
Input_Output: 无
Return: 矩阵指针
***********************************************************************************************/
MATRIX *creat_zero_matrix(_IN INTEGER rows, _IN INTEGER columns, _OUT ERROR_ID *errorID, _OUT STACKS *S)
{
    MATRIX *matrix = NULL;

    // 输入参数判断，rows <= 0 || columns <= 0 || S == NULL
    // 会在creat_matrix中判断，此处不再判断
    if (errorID == NULL)
    {
        return NULL;
    }

    // 创建矩阵并用memset赋值为0
    matrix = creat_matrix(rows, columns, errorID, S);
    if (*errorID == _ERROR_NO_ERROR)
    {
        memset(matrix->p, 0, rows * columns * sizeof(REAL));
    }

    return matrix;
}

/**********************************************************************************************
Function: creat_eye_matrix
Description: 创建单位矩阵
Input: 矩阵行数rows，列数columns
Output: 错误号指针errorID，栈指针S
Input_Output: 无
Return: 矩阵指针
***********************************************************************************************/
MATRIX *creat_eye_matrix(_IN INTEGER n, _OUT ERROR_ID *errorID, _OUT STACKS *S)
{
    INDEX i;
    MATRIX *matrix = NULL;

    // 输入参数判断，n <= 0 || S == NULL 会在creat_matrix中判断，此处不再判断
    if (errorID == NULL)
    {
        return NULL;
    }

    // 创建矩阵并用memset赋值为0
    matrix = creat_matrix(n, n, errorID, S);
    if (*errorID == _ERROR_NO_ERROR)
    {
        memset(matrix->p, 0, n * n * sizeof(REAL));

        // 将对角线数据赋值为1
        for (i = 0; i < n; i++)
        {
            matrix->p[i * n + i] = 1.0;
        }
    }

    return matrix;
}

/**********************************************************************************************
Function: matrix_cut
Description: 矩阵剪切
Input: 矩阵A，选择choose(Row/Column)，初始列st，结束列ed
Output: 错误号指针errorID，栈指针S
Input_Output: 无
Return: 矩阵指针
***********************************************************************************************/
MATRIX *matrix_cut(_IN MATRIX *A, _IN CALCULATE_CHOOSE choose, _IN INTEGER st, _IN INTEGER ed, _OUT ERROR_ID *errorID, _OUT STACKS *S)
{
    INDEX i, j, k;
    MATRIX *matrix = NULL;

    // 输入参数判断，S == NULL 会在creat_matrix中判断，此处不再判断
    if (errorID == NULL)
    {
        return NULL;
    }

    if (A == NULL)
    {
        *errorID = _ERROR_INPUT_PARAMETERS_ERROR;
        return NULL;
    }

    // 允许输入负数，代表倒数第几行
    if (st < 0)
    {
        st = st + A->columns;
    }

    if (ed < 0)
    {
        ed = ed + A->columns;
    }

    switch (choose)
    {
    case Row:
        // 判断是否超出矩阵维度
        if (st > ed || st >= A->rows || ed >= A->rows || st < 0 || ed < 0)
        {
            *errorID = _ERROR_OUT_OF_MATRIX_DIMENSION;
            return NULL;
        }

        // 创建矩阵并赋值
        matrix = creat_matrix(ed - st + 1, A->columns, errorID, S);
        if (*errorID == _ERROR_NO_ERROR)
        {
            for (i = 0, k = st; i < ed - st + 1; i++, k++)
            {
                for (j = 0; j < A->columns; j++)
                {
                    matrix->p[i * A->columns + j] = A->p[k * A->columns + j];
                }
            }
        }
        break;

    case Column:
        // 判断是否超出矩阵维度
        if (st > ed || st >= A->columns || ed >= A->columns || st < 0 || ed < 0)
        {
            *errorID = _ERROR_OUT_OF_MATRIX_DIMENSION;
            return NULL;
        }

        // 创建矩阵并赋值
        matrix = creat_zero_matrix(A->rows, ed - st + 1, errorID, S);
        if (*errorID == _ERROR_NO_ERROR)
        {
            for (i = 0; i < A->rows; i++)
            {
                for (j = 0, k = st; j < ed - st + 1; j++, k++)
                {
                    matrix->p[i * (ed - st + 1) + j] = A->p[i * A->columns + st + j];
                }
            }
        }
    default:
        break;
    }

    return matrix;
}

/**********************************************************************************************
矩阵求均值、方差等
***********************************************************************************************/

/**********************************************************************************************
Function: matrix_sum
Description: 矩阵求和
Input: 矩阵A，选择choose(Row/Column/All)
Output: 错误号指针errorID，栈指针S
Input_Output: 无
Return: 矩阵指针
***********************************************************************************************/
MATRIX *matrix_sum(_IN MATRIX *A, _IN CALCULATE_CHOOSE choose, _OUT ERROR_ID *errorID, _OUT STACKS *S)
{
    INDEX i, j;
    INTEGER rows, columns;
    REAL sum;
    MATRIX *matrix = NULL;

    // 输入参数判断，S == NULL 会在creat_matrix中判断，此处不再判断
    if (errorID == NULL)
    {
        return NULL;
    }

    if (A == NULL || !is_in_calculate_tab(choose))
    {
        *errorID = _ERROR_INPUT_PARAMETERS_ERROR;
        return NULL;
    }

    // 分别按行、列、全部进行计算，输出为矩阵形式
    rows = A->rows;
    columns = A->columns;
    switch (choose)
    {
    case Row:
        matrix = creat_matrix(1, columns, errorID, S);
        if (*errorID == _ERROR_NO_ERROR)
        {
            for (j = 0; j < columns; j++)
            {
                sum = 0;
                for (i = 0; i < rows; i++)
                {
                    sum += A->p[i * columns + j];
                }

                matrix->p[j] = sum;
            }
        }
        break;

    case Column:
        matrix = creat_matrix(rows, 1, errorID, S);
        if (*errorID == _ERROR_NO_ERROR)
        {
            for (i = 0; i < rows; i++)
            {
                sum = 0;
                for (j = 0; j < columns; j++)
                {
                    sum += A->p[i * columns + j];
                }

                matrix->p[i] = sum;
            }
        }
        break;

    case All:
        matrix = creat_matrix(1, 1, errorID, S);
        if (*errorID == _ERROR_NO_ERROR)
        {
            sum = 0;
            for (i = 0; i < rows; i++)
            {
                for (j = 0; j < columns; j++)
                {
                    sum += A->p[i * columns + j];
                }
            }

            matrix->p[0] = sum;
        }
        break;

    default:
        break;
    }

    return matrix;
}

/**********************************************************************************************
Function: matrix_average
Description: 矩阵求均值
Input: 矩阵A，选择choose(Row/Column/All)
Output: 错误号指针errorID，栈指针S
Input_Output: 无
Return: 矩阵指针
***********************************************************************************************/
MATRIX *matrix_average(_IN MATRIX *A, _IN CALCULATE_CHOOSE choose, _OUT ERROR_ID *errorID, _OUT STACKS *S)
{
    INDEX i, j;
    INTEGER rows, columns;
    REAL sum;
    MATRIX *matrix = NULL;

    // 输入参数判断，S == NULL 会在creat_matrix中判断，此处不再判断
    if (errorID == NULL)
    {
        return NULL;
    }

    if (A == NULL || !is_in_calculate_tab(choose))
    {
        *errorID = _ERROR_INPUT_PARAMETERS_ERROR;
        return NULL;
    }

    // 分别按行、列、全部进行计算，输出为矩阵形式
    rows = A->rows;
    columns = A->columns;
    switch (choose)
    {
    case Row:
        matrix = creat_matrix(1, columns, errorID, S);
        if (*errorID == _ERROR_NO_ERROR)
        {
            for (j = 0; j < columns; j++)
            {
                sum = 0;
                for (i = 0; i < rows; i++)
                {
                    sum += A->p[i * columns + j];
                }

                matrix->p[j] = sum / rows;
            }
        }
        break;

    case Column:
        matrix = creat_matrix(rows, 1, errorID, S);
        if (*errorID == _ERROR_NO_ERROR)
        {
            for (i = 0; i < rows; i++)
            {
                sum = 0;
                for (j = 0; j < columns; j++)
                {
                    sum += A->p[i * columns + j];
                }

                matrix->p[i] = sum / columns;
            }
        }
        break;

    case All:
        matrix = creat_matrix(1, 1, errorID, S);
        if (*errorID == _ERROR_NO_ERROR)
        {
            sum = 0;
            for (i = 0; i < rows; i++)
            {
                for (j = 0; j < columns; j++)
                {
                    sum += A->p[i * columns + j];
                }
            }

            matrix->p[0] = sum / rows / columns;
        }
        break;

    default:
        break;
    }

    return matrix;
}

/**********************************************************************************************
Function: matrix_average_broadcast
Description: 矩阵求均值再广播到原矩阵
Input: 矩阵A，选择choose(Row/Column/All)
Output: 无
Input_Output: 无
Return: 矩阵指针
***********************************************************************************************/
ERROR_ID matrix_average_broadcast(_IN_OUT MATRIX *A, _IN CALCULATE_CHOOSE choose)
{
    INDEX i, j;
    INTEGER rows, columns;
    REAL sum;
    ERROR_ID errorID = _ERROR_NO_ERROR;

    // 输入参数判断
    if (A == NULL || !is_in_calculate_tab(choose))
    {
        errorID = _ERROR_INPUT_PARAMETERS_ERROR;
        return errorID;
    }

    // 分别按行、列、全部进行计算
    rows = A->rows;
    columns = A->columns;
    switch (choose)
    {
    case Row:
        for (j = 0; j < columns; j++)
        {
            sum = 0;
            for (i = 0; i < rows; i++)
            {
                sum += A->p[i * columns + j];
            }

            sum = sum / rows;
            for (i = 0; i < rows; i++)
            {
                A->p[i * columns + j] = sum;
            }
        }
        break;

    case Column:
        for (i = 0; i < rows; i++)
        {
            sum = 0;
            for (j = 0; j < columns; j++)
            {
                sum += A->p[i * columns + j];
            }

            sum = sum / columns;
            for (j = 0; j < columns; j++)
            {
                A->p[i * columns + j] = sum;
            }
        }
        break;

    case All:
        sum = 0;
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                sum += A->p[i * columns + j];
            }
        }

        sum = sum / rows / columns;
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                A->p[i * columns + j] = sum;
            }
        }
        break;

    default:
        break;
    }

    return errorID;
}

/**********************************************************************************************
Function: matrix_min
Description: 矩阵求最小值
Input: 矩阵A，选择choose(Row/Column/All)
Output: 错误号指针errorID，栈指针S
Input_Output: 无
Return: 矩阵指针
***********************************************************************************************/
MATRIX *matrix_min(_IN MATRIX *A, _IN CALCULATE_CHOOSE choose, _OUT ERROR_ID *errorID, _OUT STACKS *S)
{
    INDEX i, j;
    INTEGER rows, columns;
    REAL min;
    MATRIX *matrix = NULL;

    // 输入参数判断，S == NULL 会在creat_matrix中判断，此处不再判断
    if (errorID == NULL)
    {
        return NULL;
    }

    if (A == NULL || !is_in_calculate_tab(choose))
    {
        *errorID = _ERROR_INPUT_PARAMETERS_ERROR;
        return NULL;
    }

    // 分别按行、列、全部进行计算，输出为矩阵形式
    rows = A->rows;
    columns = A->columns;
    switch (choose)
    {
        case Row:
            matrix = creat_matrix(1, columns, errorID, S);
            if (*errorID == _ERROR_NO_ERROR)
            {
                for (j = 0; j < columns; j++)
                {
                    min = A->p[j];
                    for (i = 1; i < rows; i++)
                    {
                        if (min > A->p[i * columns + j])
                        {
                            min = A->p[i * columns + j];
                        }
                    }

                    matrix->p[j] = min;
                }
            }
            break;

        case Column:
            matrix = creat_matrix(rows, 1, errorID, S);
            if (*errorID == _ERROR_NO_ERROR)
            {
                for (i = 0; i < rows; i++)
                {
                    min = A->p[i * columns];
                    for (j = 1; j < columns; j++)
                    {
                        if (min > A->p[i * columns + j])
                        {
                            min = A->p[i * columns + j];
                        }
                    }

                    matrix->p[i] = min;
                }
            }
            break;

        case All:
            matrix = creat_matrix(1, 1, errorID, S);
            if (*errorID == _ERROR_NO_ERROR)
            {
                min = A->p[0];
                for (i = 0; i < rows; i++)
                {
                    for (j = 0; j < columns; j++)
                    {
                        if (min > A->p[i * columns + j])
                        {
                            min = A->p[i * columns + j];
                        }
                    }
                }

                matrix->p[0] = min;
            }
            break;

        default:
            break;
    }

    return matrix;
}


/**********************************************************************************************
Function: matrix_max
Description: 矩阵求最大值
Input: 矩阵A，选择choose(Row/Column/All)
Output: 错误号指针errorID，栈指针S
Input_Output: 无
Return: 矩阵指针
***********************************************************************************************/
MATRIX *matrix_max(_IN MATRIX *A, _IN CALCULATE_CHOOSE choose, _OUT ERROR_ID *errorID, _OUT STACKS *S)
{
    INDEX i, j;
    INTEGER rows, columns;
    REAL max;
    MATRIX *matrix = NULL;

    // 输入参数判断，S == NULL 会在creat_matrix中判断，此处不再判断
    if (errorID == NULL)
    {
        return NULL;
    }

    if (A == NULL || !is_in_calculate_tab(choose))
    {
        *errorID = _ERROR_INPUT_PARAMETERS_ERROR;
        return NULL;
    }

    // 分别按行、列、全部进行计算，输出为矩阵形式
    rows = A->rows;
    columns = A->columns;
    switch (choose)
    {
        case Row:
            matrix = creat_matrix(1, columns, errorID, S);
            if (*errorID == _ERROR_NO_ERROR)
            {
                for (j = 0; j < columns; j++)
                {
                    max = A->p[j];
                    for (i = 1; i < rows; i++)
                    {
                        if (max < A->p[i * columns + j])
                        {
                            max = A->p[i * columns + j];
                        }
                    }

                    matrix->p[j] = max;
                }
            }
            break;

        case Column:
            matrix = creat_matrix(rows, 1, errorID, S);
            if (*errorID == _ERROR_NO_ERROR)
            {
                for (i = 0; i < rows; i++)
                {
                    max = A->p[i * columns];
                    for (j = 1; j < columns; j++)
                    {
                        if (max < A->p[i * columns + j])
                        {
                            max = A->p[i * columns + j];
                        }
                    }

                    matrix->p[i] = max;
                }
            }
            break;

        case All:
            matrix = creat_matrix(1, 1, errorID, S);
            if (*errorID == _ERROR_NO_ERROR)
            {
                max = A->p[0];
                for (i = 0; i < rows; i++)
                {
                    for (j = 0; j < columns; j++)
                    {
                        if (max < A->p[i * columns + j])
                        {
                            max = A->p[i * columns + j];
                        }
                    }
                }

                matrix->p[0] = max;
            }
            break;

        default:
            break;
    }

    return matrix;
}


/**********************************************************************************************
Function: matrix_var
Description: 矩阵求方差
Input: 矩阵A，选择choose(Row/Column/All)
Output: 错误号指针errorID，栈指针S
Input_Output: 无
Return: 矩阵指针
***********************************************************************************************/
MATRIX *matrix_var(_IN MATRIX *A, _IN CALCULATE_CHOOSE choose, _OUT ERROR_ID *errorID, _OUT STACKS *S)
{
    INDEX i, j;
    INTEGER rows, columns;
    REAL max;
    MATRIX *averageMatrix = NULL, *subMatrix = NULL, *sumMatrix = NULL;
    STACKS tempS;
    init_stack(&tempS);

    // 输入参数判断，S == NULL 会在creat_matrix中判断，此处不再判断
    if (errorID == NULL)
    {
        return NULL;
    }

    if (A == NULL || !is_in_calculate_tab(choose))
    {
        *errorID = _ERROR_INPUT_PARAMETERS_ERROR;
        return NULL;
    }

    // 分别按行、列、全部进行计算，输出为矩阵形式
    rows = A->rows;
    columns = A->columns;
    switch (choose)
    {
        case Row:
        case Column:
            averageMatrix = matrix_average(A, choose, errorID, &tempS);
            subMatrix = creat_matrix(A->rows, A->columns, errorID, &tempS);
            *errorID = matrix_sub(A, averageMatrix, subMatrix);
            *errorID = matrix_pow_num(subMatrix, 2, subMatrix);
            sumMatrix = matrix_sum(subMatrix, choose, errorID, S);
            break;

        case All:
            averageMatrix = matrix_average(A, choose, errorID, &tempS);
            subMatrix = creat_matrix(A->rows, A->columns, errorID, &tempS);
            *errorID = matrix_sub_num(A, averageMatrix->p[0], subMatrix);
            *errorID = matrix_pow_num(subMatrix, 2, subMatrix);
            sumMatrix = matrix_sum(subMatrix, choose, errorID, S);
            break;

        default:
            break;
    }

    free_stack(&tempS);
    return sumMatrix;
}

/**********************************************************************************************
Function: matrix_std
Description: 矩阵求标准差
Input: 矩阵A，选择choose(Row/Column/All)
Output: 错误号指针errorID，栈指针S
Input_Output: 无
Return: 矩阵指针
***********************************************************************************************/
MATRIX *matrix_std(_IN MATRIX *A, _IN CALCULATE_CHOOSE choose, _OUT ERROR_ID *errorID, _OUT STACKS *S)
{
    INDEX i, j;
    INTEGER rows, columns;
    REAL max;
    MATRIX *averageMatrix = NULL, *subMatrix = NULL, *sumMatrix = NULL;
    STACKS tempS;
    init_stack(&tempS);

    // 输入参数判断，S == NULL 会在creat_matrix中判断，此处不再判断
    if (errorID == NULL)
    {
        return NULL;
    }

    if (A == NULL || !is_in_calculate_tab(choose))
    {
        *errorID = _ERROR_INPUT_PARAMETERS_ERROR;
        return NULL;
    }

    // 分别按行、列、全部进行计算，输出为矩阵形式
    rows = A->rows;
    columns = A->columns;
    switch (choose)
    {
        case Row:
        case Column:
            averageMatrix = matrix_average(A, choose, errorID, &tempS);
            subMatrix = creat_matrix(A->rows, A->columns, errorID, &tempS);
            *errorID = matrix_sub(A, averageMatrix, subMatrix);
            *errorID = matrix_pow_num(subMatrix, 2, subMatrix);
            sumMatrix = matrix_sum(subMatrix, choose, errorID, S);
            *errorID = matrix_pow_num(sumMatrix, 0.5, sumMatrix);
            break;

        case All:
            averageMatrix = matrix_average(A, choose, errorID, &tempS);
            subMatrix = creat_matrix(A->rows, A->columns, errorID, &tempS);
            *errorID = matrix_sub_num(A, averageMatrix->p[0], subMatrix);
            *errorID = matrix_pow_num(subMatrix, 2, subMatrix);
            sumMatrix = matrix_sum(subMatrix, choose, errorID, S);
            *errorID = matrix_pow_num(sumMatrix, 0.5, sumMatrix);
            break;

        default:
            break;
    }

    free_stack(&tempS);
    return sumMatrix;
}


///**********************************************************************************************
//Function: matrix_std
//Description: 矩阵求标准差
//Input: 矩阵A，选择choose(Row/Column/All)
//Output: 错误号指针errorID，栈指针S
//Input_Output: 无
//Return: 矩阵指针
//***********************************************************************************************/
//MATRIX *matrix_sort(_IN MATRIX *A, _IN INTEGER whichRowOrCol, _IN CALCULATE_CHOOSE choose, _OUT ERROR_ID *errorID, _OUT STACKS *S)
//{
//    INDEX i, j;
//    INTEGER rows, columns;
//    MATRIX *matrix;
//
//    // 输入参数判断，S == NULL 会在creat_matrix中判断，此处不再判断
//    if (errorID == NULL)
//    {
//        return NULL;
//    }
//
//    if (A == NULL || !is_in_calculate_tab(choose) || choose == All)
//    {
//        *errorID = _ERROR_INPUT_PARAMETERS_ERROR;
//        return NULL;
//    }
//
//    // 允许输入负数，代表倒数第几行或第几列
//    if (choose == Row && whichRowOrCol < 0)
//    {
//        whichRowOrCol = whichRowOrCol + A->rows;
//    }
//    else if (choose == Column && whichRowOrCol < 0)
//    {
//        whichRowOrCol = whichRowOrCol + A->columns;
//    }
//
//    // 判断是否超出维度
//    if ((choose == Row && whichRowOrCol > A->rows) || (choose == Column && whichRowOrCol > A->columns))
//    {
//        *errorID = _ERROR_OUT_OF_MATRIX_DIMENSION;
//        return NULL;
//    }
//
//    // 分别按行、列进行计算，输出为矩阵形式
//    rows = A->rows;
//    columns = A->columns;
//    switch (choose)
//    {
//        case Row:
//
//
//        case Column:
//
//            break;
//
//        default:
//            break;
//    }
//
//    return matrix;
//}
/**********************************************************************************************
矩阵计算
***********************************************************************************************/

/**********************************************************************************************
Function: matrix_add
Description: 矩阵A + 矩阵B = 矩阵C
Input: 矩阵A,矩阵B
Output: 矩阵C
Input_Output: 无
Return: 错误号
***********************************************************************************************/
ERROR_ID matrix_add(_IN MATRIX *A, _IN MATRIX *B, _OUT MATRIX *C)
{
    INDEX i, j;
    INTEGER rows, columns;
    ERROR_ID errorID = _ERROR_NO_ERROR;

    // 输入参数判断
    if (A == NULL || B == NULL || C == NULL)
    {
        errorID = _ERROR_INPUT_PARAMETERS_ERROR;
        return errorID;
    }

    if (A->rows != C->rows || A->columns != C->columns)
    {
        errorID = _ERROR_MATRIX_ROWS_OR_COLUMNS_NOT_EQUAL;
        return errorID;
    }

    rows = A->rows;
    columns = A->columns;
    // A、B矩阵对应位置相加
    if (A->rows == B->rows && A->columns == B->columns)
    {
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                C->p[i * columns + j] = A->p[i * columns + j] + B->p[i * columns + j];
            }
        }
    }
    else if (A->rows == 1 && A->columns == B->columns)
    {
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                C->p[j] = A->p[j] + B->p[i * columns + j];
            }
        }
    }
    else if (B->rows == 1 && A->columns == B->columns)
    {
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                C->p[i * columns + j] = A->p[i * columns + j] + B->p[j];
            }
        }
    }
    else if (A->columns == 1 && A->rows == B->rows)
    {
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                C->p[i] = A->p[i] + B->p[i * columns + j];
            }
        }
    }
    else if (B->columns == 1 && A->rows == B->rows)
    {
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                C->p[i * columns + j] = A->p[i * columns + j] + B->p[i];
            }
        }
    }
    else
    {
        errorID = _ERROR_INPUT_PARAMETERS_ERROR;
        return errorID;
    }

    return errorID;
}

/**********************************************************************************************
Function: matrix_sub
Description: 矩阵A - 矩阵B = 矩阵C
Input: 矩阵A,矩阵B
Output: 矩阵C
Input_Output: 无
Return: 错误号
***********************************************************************************************/
ERROR_ID matrix_sub(_IN MATRIX *A, _IN MATRIX *B, _OUT MATRIX *C)
{
    INDEX i, j;
    INTEGER rows, columns;
    ERROR_ID errorID = _ERROR_NO_ERROR;

    // 输入参数判断
    if (A == NULL || B == NULL || C == NULL)
    {
        errorID = _ERROR_INPUT_PARAMETERS_ERROR;
        return errorID;
    }

    if (A->rows != C->rows || A->columns != B->columns || A->columns != C->columns || B->columns != C->columns)
    {
        errorID = _ERROR_MATRIX_ROWS_OR_COLUMNS_NOT_EQUAL;
        return errorID;
    }

    rows = A->rows;
    columns = A->columns;
    // A、B矩阵对应位置相减
    if (A->rows == B->rows && A->columns == B->columns)
    {
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                C->p[i * columns + j] = A->p[i * columns + j] - B->p[i * columns + j];
            }
        }
    }
    else if (A->rows == 1 && A->columns == B->columns)
    {
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                C->p[j] = A->p[j] - B->p[i * columns + j];
            }
        }
    }
    else if (B->rows == 1 && A->columns == B->columns)
    {
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                C->p[i * columns + j] = A->p[i * columns + j] - B->p[j];
            }
        }
    }
    else if (A->columns == 1 && A->rows == B->rows)
    {
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                C->p[i] = A->p[i] - B->p[i * columns + j];
            }
        }
    }
    else if (B->columns == 1 && A->rows == B->rows)
    {
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                C->p[i * columns + j] = A->p[i * columns + j] - B->p[i];
            }
        }
    }
    else
    {
        errorID = _ERROR_INPUT_PARAMETERS_ERROR;
        return errorID;
    }

    return errorID;
}

/**********************************************************************************************
Function: matrix_mul
Description: 矩阵乘法C = A * B
Input: 矩阵A,矩阵B
Output: 矩阵C
Input_Output: 无
Return: 错误号
***********************************************************************************************/
ERROR_ID matrix_mul(_IN MATRIX *A, _IN MATRIX *B, _OUT MATRIX *C)
{
    INDEX i, j, k;
    REAL sum;
    ERROR_ID errorID = _ERROR_NO_ERROR;

    // 输入参数判断
    if (A == NULL || B == NULL || C == NULL)
    {
        errorID = _ERROR_INPUT_PARAMETERS_ERROR;
        return errorID;
    }

    if (A->columns != B->rows || A->rows != C->rows ||
        B->columns != C->columns)
    {
        errorID = _ERROR_MATRIX_MULTIPLICATION;
        return errorID;
    }

    // 矩阵A的行与矩阵B的列相乘
    for (i = 0; i < A->rows; i++)
    {
        for (j = 0; j < B->columns; j++)
        {
            sum = 0.0;
            for (k = 0; k < A->columns; k++)
            {
                sum += A->p[i * A->columns + k] * B->p[k * B->columns + j];
            }
            C->p[i * B->columns + j] = sum;
        }
    }
    return errorID;
}

/**********************************************************************************************
Function: matrix_dot
Description: 矩阵对应位置相乘C = A dot B
Input: 矩阵A,矩阵B
Output: 矩阵C
Input_Output: 无
Return: 错误号
***********************************************************************************************/
ERROR_ID matrix_dot(_IN MATRIX *A, _IN MATRIX *B, _OUT MATRIX *C)
{
    INDEX i, j, k;
    INTEGER rows, columns;
    ERROR_ID errorID = _ERROR_NO_ERROR;

    // 输入参数判断
    if (A == NULL || B == NULL || C == NULL)
    {
        errorID = _ERROR_INPUT_PARAMETERS_ERROR;
        return errorID;
    }

    if (A->rows != C->rows || A->columns != B->columns || A->columns != C->columns || B->columns != C->columns)
    {
        errorID = _ERROR_MATRIX_ROWS_OR_COLUMNS_NOT_EQUAL;
        return errorID;
    }

    rows = A->rows;
    columns = A->columns;
    // A、B矩阵对应位置相乘
    if (A->rows == B->rows && A->columns == B->columns)
    {
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                C->p[i * columns + j] = A->p[i * columns + j] * B->p[i * columns + j];
            }
        }
    }
    else if (A->rows == 1 && A->columns == B->columns)
    {
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                C->p[j] = A->p[j] * B->p[i * columns + j];
            }
        }
    }
    else if (B->rows == 1 && A->columns == B->columns)
    {
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                C->p[i * columns + j] = A->p[i * columns + j] * B->p[j];
            }
        }
    }
    else if (A->columns == 1 && A->rows == B->rows)
    {
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                C->p[i] = A->p[i] * B->p[i * columns + j];
            }
        }
    }
    else if (B->columns == 1 && A->rows == B->rows)
    {
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                C->p[i * columns + j] = A->p[i * columns + j] * B->p[i];
            }
        }
    }
    else
    {
        errorID = _ERROR_INPUT_PARAMETERS_ERROR;
        return errorID;
    }

    return errorID;
}

/**********************************************************************************************
Function: matrix_divide
Description: 矩阵对应位置（！！！）相除C = A / B
Input: 矩阵A,矩阵B
Output: 矩阵C
Input_Output: 无
Return: 错误号
***********************************************************************************************/
ERROR_ID matrix_divide(_IN MATRIX *A, _IN MATRIX *B, _OUT MATRIX *C)
{
    INDEX i, j, k;
    INTEGER rows, columns;
    ERROR_ID errorID = _ERROR_NO_ERROR;

    // 输入参数判断
    if (A == NULL || B == NULL || C == NULL)
    {
        errorID = _ERROR_INPUT_PARAMETERS_ERROR;
        return errorID;
    }

    if (A->rows != C->rows || A->columns != B->columns || A->columns != C->columns || B->columns != C->columns)
    {
        errorID = _ERROR_MATRIX_ROWS_OR_COLUMNS_NOT_EQUAL;
        return errorID;
    }

    rows = A->rows;
    columns = A->columns;
    // A、B矩阵对应位置相除
    if (A->rows == B->rows && A->columns == B->columns)
    {
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                C->p[i * columns + j] = A->p[i * columns + j] / B->p[i * columns + j];
            }
        }
    }
    else if (A->rows == 1 && A->columns == B->columns)
    {
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                C->p[j] = A->p[j] / B->p[i * columns + j];
            }
        }
    }
    else if (B->rows == 1 && A->columns == B->columns)
    {
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                C->p[i * columns + j] = A->p[i * columns + j] / B->p[j];
            }
        }
    }
    else if (A->columns == 1 && A->rows == B->rows)
    {
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                C->p[i] = A->p[i] / B->p[i * columns + j];
            }
        }
    }
    else if (B->columns == 1 && A->rows == B->rows)
    {
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                C->p[i * columns + j] = A->p[i * columns + j] / B->p[i];
            }
        }
    }
    else
    {
        errorID = _ERROR_INPUT_PARAMETERS_ERROR;
        return errorID;
    }

    return errorID;
}
/**********************************************************************************************
Function: matrix_matrix_calculate
Description: 矩阵计算C = A + B or A - B or A * B
Input: 矩阵A,函数指针fp,矩阵B
Output: 矩阵C
Input_Output: 无
Return: 错误号
***********************************************************************************************/
ERROR_ID matrix_matrix_calculate(_IN MATRIX *A, _IN PTR_TO_MATRIX_MATRIX_CALCULATE fp, _IN MATRIX *B, _OUT MATRIX *C)
{
    return (*fp)(A, B, C);
}

/**********************************************************************************************
Function: matrix_add_num
Description: 矩阵乘法B = A + a
Input: 矩阵A,实数a
Output: 矩阵B
Input_Output: 无
Return: 错误号
***********************************************************************************************/
ERROR_ID matrix_add_num(_IN MATRIX *A, _IN REAL a, _OUT MATRIX *B)
{
    INDEX i, j;
    INTEGER rows, columns;
    ERROR_ID errorID = _ERROR_NO_ERROR;

    // 输入参数判断
    if (A == NULL || B == NULL)
    {
        errorID = _ERROR_INPUT_PARAMETERS_ERROR;
        return errorID;
    }

    if (A->columns != B->columns || A->rows != B->rows)
    {
        errorID = _ERROR_MATRIX_MULTIPLICATION;
        return errorID;
    }

    // A矩阵各个数均加上a
    rows = A->rows;
    columns = A->columns;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < columns; j++)
        {
            B->p[i * columns + j] = a + A->p[i * columns + j];
        }
    }
    return errorID;
}

/**********************************************************************************************
Function: matrix_sub_num
Description: 矩阵乘法B = A - a
Input: 矩阵A,实数a
Output: 矩阵B
Input_Output: 无
Return: 错误号
***********************************************************************************************/
ERROR_ID matrix_sub_num(_IN MATRIX *A, _IN REAL a, _OUT MATRIX *B)
{
    INDEX i, j;
    INTEGER rows, columns;
    ERROR_ID errorID = _ERROR_NO_ERROR;

    // 输入参数判断
    if (A == NULL || B == NULL)
    {
        errorID = _ERROR_INPUT_PARAMETERS_ERROR;
        return errorID;
    }

    if (A->columns != B->columns || A->rows != B->rows)
    {
        errorID = _ERROR_MATRIX_MULTIPLICATION;
        return errorID;
    }

    // A矩阵各个数均减去a
    rows = A->rows;
    columns = A->columns;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < columns; j++)
        {
            B->p[i * columns + j] = A->p[i * columns + j] - a;
        }
    }
    return errorID;
}

/**********************************************************************************************
Function: matrix_mul_num
Description: 矩阵乘法B = A * a
Input: 矩阵A,实数a
Output: 矩阵B
Input_Output: 无
Return: 错误号
***********************************************************************************************/
ERROR_ID matrix_mul_num(_IN MATRIX *A, _IN REAL a, _OUT MATRIX *B)
{
    INDEX i, j;
    INTEGER rows, columns;
    ERROR_ID errorID = _ERROR_NO_ERROR;

    // 输入参数判断
    if (A == NULL || B == NULL)
    {
        errorID = _ERROR_INPUT_PARAMETERS_ERROR;
        return errorID;
    }

    if (A->columns != B->columns || A->rows != B->rows)
    {
        errorID = _ERROR_MATRIX_MULTIPLICATION;
        return errorID;
    }

    // A矩阵各个数均乘以a
    rows = A->rows;
    columns = A->columns;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < columns; j++)
        {
            B->p[i * columns + j] = a * A->p[i * columns + j];
        }
    }
    return errorID;
}

/**********************************************************************************************
Function: matrix_pow_num
Description: 矩阵的幂B = A ^ a
Input: 矩阵A,实数a
Output: 矩阵B
Input_Output: 无
Return: 错误号
***********************************************************************************************/
ERROR_ID matrix_pow_num(_IN MATRIX *A, _IN REAL a, _OUT MATRIX *B)
{
    INDEX i, j, k;
    INTEGER rows, columns;
    ERROR_ID errorID = _ERROR_NO_ERROR;

    // 输入参数判断
    if (A == NULL || B == NULL)
    {
        errorID = _ERROR_INPUT_PARAMETERS_ERROR;
        return errorID;
    }

    if (A->columns != B->columns || A->rows != B->rows)
    {
        errorID = _ERROR_MATRIX_MULTIPLICATION;
        return errorID;
    }

    // A矩阵各个数的a次幂
    rows = A->rows;
    columns = A->columns;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < columns; j++)
        {
            B->p[i * columns + j] = pow(A->p[i * columns + j], a);
        }
    }

    return errorID;
}


/**********************************************************************************************
Function: num_divide_matrix
Description: 数除矩阵B = a / A
Input: 矩阵A,实数a
Output: 矩阵B
Input_Output: 无
Return: 错误号
***********************************************************************************************/
ERROR_ID num_divide_matrix(_IN REAL a, _IN MATRIX *A, _OUT MATRIX *B)
{
    INDEX i, j, k;
    INTEGER rows, columns;
    ERROR_ID errorID = _ERROR_NO_ERROR;

    // 输入参数判断
    if (A == NULL || B == NULL)
    {
        errorID = _ERROR_INPUT_PARAMETERS_ERROR;
        return errorID;
    }

    if (A->columns != B->columns || A->rows != B->rows)
    {
        errorID = _ERROR_MATRIX_MULTIPLICATION;
        return errorID;
    }

    // a除以A矩阵各个数
    rows = A->rows;
    columns = A->columns;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < columns; j++)
        {
            B->p[i * columns + j] = a / A->p[i * columns + j];
        }
    }

    return errorID;
}

/**********************************************************************************************
Function: matrix_num_calculate
Description: 矩阵与数计算C = A + a or A - a or A * a
Input: 矩阵A,函数指针fp,实数a
Output: 矩阵B
Input_Output: 无
Return: 错误号
***********************************************************************************************/
ERROR_ID matrix_num_calculate(_IN MATRIX *A, _IN PTR_TO_MATRIX_NUM_CALCULATE fp, _IN REAL a, _OUT MATRIX *B)
{
    return (*fp)(A, a, B);
}
/**********************************************************************************************
Function: matrix_inverse
Description: 矩阵求逆
Input: 矩阵A
Output: 矩阵A的逆矩阵
Input_Output: 无
Return: 错误号
***********************************************************************************************/
ERROR_ID matrix_inverse(_IN MATRIX *A, _OUT MATRIX *invA)
{
    INDEX i;
    INTEGER n;
    MATRIX *ATemp = NULL;
    ERROR_ID errorID = _ERROR_NO_ERROR;
    STACKS S;

    if (A == NULL || invA == NULL)
    {
        errorID = _ERROR_INPUT_PARAMETERS_ERROR;
        return errorID;
    }

    if (A->rows != A->columns)
    {
        errorID = _ERROR_MATRIX_MUST_BE_SQUARE;
        return errorID;
    }

    init_stack(&S);

    n = A->rows;
    ATemp = creat_matrix(n, n, &errorID, &S);
    if (errorID != _ERROR_NO_ERROR)
        goto EXIT;

    memcpy(ATemp->p, A->p, n * n * sizeof(REAL));
    memset(invA->p, 0, n * n * sizeof(REAL));
    for (i = 0; i < n; i++)
    {
        invA->p[i * n + i] = 1.0;
    }

    errorID = solve_matrix_equation_by_lup_decomposition(ATemp, invA);

EXIT:
    free_stack(&S);
    return errorID;
}

/**********************************************************************************************
Function: matrix_transpose
Description: 矩阵转置
Input: 矩阵A
Output: 矩阵A的转置
Input_Output: 无
Return: 错误号
***********************************************************************************************/
ERROR_ID matrix_transpose(_IN MATRIX *A, _OUT MATRIX *transposeA)
{
    INDEX i, j;
    ERROR_ID errorID = _ERROR_NO_ERROR;

    if (A == NULL || transposeA == NULL)
    {
        errorID = _ERROR_INPUT_PARAMETERS_ERROR;
        return errorID;
    }

    if (A->rows != transposeA->columns || A->columns != transposeA->rows)
    {
        errorID = _ERROR_MATRIX_TRANSPOSE_FAILED;
        return errorID;
    }

    for (i = 0; i < A->rows; i++)
    {
        for (j = 0; j < A->columns; j++)
        {
            transposeA->p[j * A->rows + i] = A->p[i * A->columns + j];
        }
    }
    return errorID;
}

/**********************************************************************************************
Function: matrix_trace
Description: 矩阵的迹
Input: 矩阵A
Output: 矩阵A的迹
Input_Output: 无
Return: 错误号
***********************************************************************************************/
ERROR_ID matrix_trace(_IN MATRIX *A, _OUT REAL *trace)
{
    INDEX i;
    ERROR_ID errorID = _ERROR_NO_ERROR;

    if (A == NULL || trace == NULL)
    {
        errorID = _ERROR_INPUT_PARAMETERS_ERROR;
        return errorID;
    }

    if (A->rows != A->columns)
    {
        errorID = _ERROR_MATRIX_MUST_BE_SQUARE;
        return errorID;
    }

    *trace = 0.0;
    for (i = 0; i < A->rows; i++)
    {
        *trace += A->p[i * A->columns + i];
    }
    return errorID;
}

/**********************************************************************************************
Function: lup_decomposition
Description: n行n列矩阵LUP分解PA = L * U
Input: n行n列矩阵A
Output: n行n列下三角矩阵L，n行n列上三角矩阵U，n行n列置换矩阵P
Input_Output: 无
Return: 错误号
***********************************************************************************************/
ERROR_ID lup_decomposition(_IN MATRIX *A, _OUT MATRIX *L, _OUT MATRIX *U,
                           _OUT MATRIX *P)
{
    INDEX i, j, k, index, s, t;
    INTEGER n;
    REAL maxValue, temp;
    ERROR_ID errorID = _ERROR_NO_ERROR;

    if (A == NULL || L == NULL || U == NULL || P == NULL)
    {
        errorID = _ERROR_INPUT_PARAMETERS_ERROR;
        return errorID;
    }

    if (A->rows != A->columns)
    {
        errorID = _ERROR_MATRIX_MUST_BE_SQUARE;
        return errorID;
    }

    n = A->rows;
    memcpy(U->p, A->p, n * n * sizeof(REAL));
    memset(L->p, 0, n * n * sizeof(REAL));
    memset(P->p, 0, n * n * sizeof(REAL));
    for (i = 0; i < n; i++)
    {
        L->p[i * n + i] = 1.0;
        P->p[i * n + i] = 1.0;
    }

    for (j = 0; j < n - 1; j++)
    {
        // Select i(>= j) that maximizes | U(i, j) |
        index = -1;
        maxValue = 0.0;
        for (i = j; i < n; i++)
        {
            temp = fabs(U->p[i * n + j]);
            if (temp > maxValue)
            {
                maxValue = temp;
                index = i;
            }
        }

        if (index == -1)
        {
            continue;
        }

        // Interchange rows of U : U(j, j : n) < ->U(i, j : n)
        for (k = j; k < n; k++)
        {
            s = j * n + k;
            t = index * n + k;
            temp = U->p[s];
            U->p[s] = U->p[t];
            U->p[t] = temp;
        }

        // Interchange rows of L : L(j, 1 : j - 1) < ->L(i, 1 : j - 1)
        for (k = 0; k < j; k++)
        {
            s = j * n + k;
            t = index * n + k;
            temp = L->p[s];
            L->p[s] = L->p[t];
            L->p[t] = temp;
        }

        // Interchange rows of P : P(j, 1 : n) < ->P(i, 1 : n)
        for (k = 0; k < n; k++)
        {
            s = j * n + k;
            t = index * n + k;
            temp = P->p[s];
            P->p[s] = P->p[t];
            P->p[t] = temp;
        }

        for (i = j + 1; i < n; i++)
        {
            s = i * n + j;
            L->p[s] = U->p[s] / U->p[j * n + j];
            for (k = j; k < n; k++)
            {
                U->p[i * n + k] -= L->p[s] * U->p[j * n + k];
            }
        }
    }
    return errorID;
}

/**********************************************************************************************
Function: solve_matrix_equation_by_lup_decomposition
Description:
LUP分解解矩阵方程AX=B,其中A为n行n列矩阵，B为n行m列矩阵，X为n行m列待求矩阵(写到矩阵B)
Input: n行n列矩阵A
Output: 无
Input_Output: n行m列矩阵B(即n行m列待求矩阵X)
Return: 错误号
***********************************************************************************************/
ERROR_ID solve_matrix_equation_by_lup_decomposition(_IN MATRIX *A, _IN_OUT MATRIX *B)
{
    INDEX i, j, k, index, s, t;
    INTEGER n, m;
    REAL sum, maxValue, temp;
    MATRIX *L = NULL, *U = NULL, *y = NULL;
    ERROR_ID errorID = _ERROR_NO_ERROR;
    STACKS S;

    if (A == NULL || B == NULL)
    {
        errorID = _ERROR_INPUT_PARAMETERS_ERROR;
        return errorID;
    }

    if (A->rows != A->columns)
    {
        errorID = _ERROR_MATRIX_MUST_BE_SQUARE;
        return errorID;
    }

    init_stack(&S);

    n = A->rows;
    m = B->columns;

    L = creat_matrix(n, n, &errorID, &S);
    if (errorID != _ERROR_NO_ERROR)
        goto EXIT;

    U = creat_matrix(n, n, &errorID, &S);
    if (errorID != _ERROR_NO_ERROR)
        goto EXIT;

    y = creat_matrix(n, m, &errorID, &S);
    if (errorID != _ERROR_NO_ERROR)
        goto EXIT;

    memcpy(U->p, A->p, n * n * sizeof(REAL));
    memset(L->p, 0, n * n * sizeof(REAL));
    for (i = 0; i < n; i++)
    {
        L->p[i * n + i] = 1.0;
    }

    for (j = 0; j < n - 1; j++)
    {
        // Select i(>= j) that maximizes | U(i, j) |
        index = -1;
        maxValue = 0.0;
        for (i = j; i < n; i++)
        {
            temp = fabs(U->p[i * n + j]);
            if (temp > maxValue)
            {
                maxValue = temp;
                index = i;
            }
        }

        if (index == -1)
        {
            continue;
        }

        // Interchange rows of U : U(j, j : n) < ->U(i, j : n)
        for (k = j; k < n; k++)
        {
            s = j * n + k;
            t = index * n + k;
            temp = U->p[s];
            U->p[s] = U->p[t];
            U->p[t] = temp;
        }

        // Interchange rows of L : L(j, 1 : j - 1) < ->L(i, 1 : j - 1)
        for (k = 0; k < j; k++)
        {
            s = j * n + k;
            t = index * n + k;
            temp = L->p[s];
            L->p[s] = L->p[t];
            L->p[t] = temp;
        }

        // Interchange rows of P : P(j, 1 : n) < ->P(i, 1 : n), C = P *
        // B，等价于对B交换行
        for (k = 0; k < m; k++)
        {
            s = j * m + k;
            t = index * m + k;
            temp = B->p[s];
            B->p[s] = B->p[t];
            B->p[t] = temp;
        }

        for (i = j + 1; i < n; i++)
        {
            s = i * n + j;
            L->p[s] = U->p[s] / U->p[j * n + j];
            for (k = j; k < n; k++)
            {
                U->p[i * n + k] -= L->p[s] * U->p[j * n + k];
            }
        }
    }

    for (i = 0; i < n; i++)
    {
        if (fabs(U->p[i * n + i]) < 1.0e-20)
        {
            errorID = _ERROR_MATRIX_EQUATION_HAS_NO_SOLUTIONS;
            goto EXIT;
        }
    }

    // L * y = C
    for (j = 0; j < m; j++)
    {
        for (i = 0; i < n; i++)
        {
            sum = 0.0;
            for (k = 0; k < i; k++)
            {
                sum = sum + L->p[i * n + k] * y->p[k * m + j];
            }
            y->p[i * m + j] = B->p[i * m + j] - sum;
        }
    }

    // U * x = y
    for (j = 0; j < m; j++)
    {
        for (i = n - 1; i >= 0; i--)
        {
            sum = 0.0;
            for (k = i + 1; k < n; k++)
            {
                sum += U->p[i * n + k] * B->p[k * m + j];
            }
            B->p[i * m + j] = (y->p[i * m + j] - sum) / U->p[i * n + i];
        }
    }

EXIT:
    free_stack(&S);
    return errorID;
}