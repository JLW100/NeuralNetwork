/*******************************************************************************
 *       File Name :                         matrix.h
 *       Library/Module Name :               MatrixComputation
 *       Author :                            Marc Pony(marc_pony@163.com)
 *       Change by :                         J.L. (2764371020@qq.com)
 *       Create Date :                       2021/6/28
 *       Abstract Description :              共用的类型
 *******************************************************************************/

#ifndef MAIN_BASE_H
#define MAIN_BASE_H

/*******************************************************************************
 * Include File Section
 *******************************************************************************/
#include <malloc.h>
#include <math.h>
#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

/*******************************************************************************
 * Base Define Section
 *******************************************************************************/
#define _IN
#define _OUT
#define _IN_OUT
#define _CRT_SECURE_NO_WARNINGS
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

#define _MAX_SIZE 2048
#define PI 3.14159265358979323846
#define POSITIVE_INFINITY 999999999
#define NEGATIVE_INFINITY -999999999
/*******************************************************************************
 * Error Define Section
 *******************************************************************************/
// 使用断言测试
#ifdef DEBUG
    void Assert(char * filename, unsigned int lineno);          // 处理函数原型
    #define ASSERT(condition)               \
    do{                                     \
        if(condition)                       \
            NULL;                           \
        else                                \
            Assert(__FILE__ , __LINE__);    \
    } while(0)
#else
    // 不使用断言测试
    #define ASSERT(condition) NULL
#endif

#define _ERROR_NO_ERROR 0X00000000                          // 无错误
// 01-0f 内存错误
#define _ERROR_FAILED_TO_ALLOCATE_HEAP_MEMORY 0X00000001    // 分配堆内存失败
// 11-2f 文件错误
#define _ERROR_OPEN_FILE_ERROR 0X00000011                   // 文件打开失败
#define _ERROR_EMPTY_FILE 0X00000012                        // 空文件
// 31-4f 矩阵计算错误
#define _ERROR_OUT_OF_MATRIX_DIMENSION 0X00000031           // 超出矩阵维度
#define _ERROR_MATRIX_ROWS_OR_COLUMNS_NOT_EQUAL 0X00000032  // 矩阵行数或列数不相等
#define _ERROR_SVD_EXCEED_MAX_ITERATIONS 0X00000033         // svd超过最大迭代次数
#define _ERROR_MATRIX_MULTIPLICATION 0X00000034             // 矩阵乘法错误(第一个矩阵的列数不等于第二个矩阵行数)
#define _ERROR_MATRIX_MUST_BE_SQUARE 0X00000035             // 矩阵必须为方阵
#define _ERROR_MATRIX_NORM_TYPE_INVALID 0X00000036          // 矩阵模类型无效
#define _ERROR_MATRIX_EQUATION_HAS_NO_SOLUTIONS 0X00000037  // 矩阵方程无解
#define _ERROR_MATRIX_EQUATION_HAS_INFINITY_MANNY_SOLUTIONS 0X00000038      // 矩阵方程有无穷多解
#define _ERROR_QR_DECOMPOSITION_FAILED 0X00000039           // QR分解失败
#define _ERROR_CHOLESKY_DECOMPOSITION_FAILED 0X0000003a     // cholesky分解失败
#define _ERROR_IMPROVED_CHOLESKY_DECOMPOSITION_FAILED 0X0000003b            // improved cholesky分解失败
#define _ERROR_LU_DECOMPOSITION_FAILED 0X0000003c           // LU分解失败
#define _ERROR_CREATE_MATRIX_FAILED 0X0000003d              // 创建矩阵失败
#define _ERROR_MATRIX_TRANSPOSE_FAILED 0X0000003e           // 矩阵转置失败
#define _ERROR_CREATE_VECTOR_FAILED 0X0000003f              // 创建向量失败
#define _ERROR_VECTOR_DIMENSION_NOT_EQUAL 0X00000040        // 向量维数不相同
#define _ERROR_VECTOR_NORM_TYPE_INVALID 0X00000041          // 向量模类型无效
#define _ERROR_VECTOR_CROSS_FAILED 0X00000042               // 向量叉乘失败
// 51- 9f 输入错误
#define _ERROR_INPUT_PARAMETERS_ERROR 0X00000051            // 输入参数错误
#define _ERROR_INPUT_ACTIVATION_ERROR 0X00000052            // 激活函数输入错误
#define _ERROR_INPUT_INIT_WEIGHTS_ERROR 0X00000053          // 激活函数输入错误
#define _ERROR_INPUT_OPTIMIZER_ERROR 0X00000054             // 激活函数输入错误
#define _ERROR_INPUT_LOSS_ERROR 0X00000055                  // 激活函数输入错误
/*******************************************************************************
 * Data Types Define Section
 *******************************************************************************/
typedef unsigned int ERROR_ID;
typedef unsigned int SEED;
typedef int INDEX;
typedef int INTEGER;
typedef long int LONG_INTEGER;
typedef bool FLAG;
typedef double REAL;
typedef char *STRING;
typedef char *NAME;
typedef char CHAR;
typedef void VOID;

#endif // MAIN_BASE_H