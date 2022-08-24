#ifndef MAIN_NETWORK_H
#define MAIN_NETWORK_H

/*******************************************************************************
 * Include File Section
 *******************************************************************************/
#include "activation.h"
#include "init_weights.h"
#include "matrix.h"

/*******************************************************************************
 * Macro Define Section
 *******************************************************************************/

/*******************************************************************************
 * Enum Define Section
 *******************************************************************************/
/**********************************************************************************************
Description: 网络层的名称
Value:  HeadLayer    初始层，作为循环链表的头节点，不存储数据
        DenseLayer   全连接层
***********************************************************************************************/
#define LAYER_TAB               \
    LAYER_MACRO(HeadLayer, 0)   \
    LAYER_MACRO(DenseLayer, 1)

typedef enum
{
#define LAYER_MACRO(a, b) a = b,
    LAYER_TAB
#undef LAYER_MACRO
} LAYER_NAME;

/**********************************************************************************************
Description: 网络优化方式
***********************************************************************************************/
#define OPTIMIZER_TAB            \
    OPTIMIZER_MACRO(Sgd, 0)      \
    OPTIMIZER_MACRO(Momentum, 1) \
    OPTIMIZER_MACRO(Rmsprop, 2)  \
    OPTIMIZER_MACRO(Adam, 3)     \
    OPTIMIZER_MACRO(NullOptimizer, -1)

typedef enum
{
#define OPTIMIZER_MACRO(a, b) a = b,
    OPTIMIZER_TAB
#undef OPTIMIZER_MACRO
} OPTIMIZER;

/**********************************************************************************************
Description: 网络优化损失函数
***********************************************************************************************/
#define LOSS_TAB                    \
    LOSS_MACRO(MeanSquaredError, 0) \
    LOSS_MACRO(NullLoss, 1)

typedef enum
{
#define LOSS_MACRO(a, b) a = b,
    LOSS_TAB
#undef LOSS_MACRO
} LOSS;

/*******************************************************************************
 * Struct(Data Types) Define Section
 *******************************************************************************/
/**********************************************************************************************
Description: 全连接层结构体，X_NI为上一层输入，X_NJ为下一层输入
        Y_NJ = X_NI * W_IJ + E_NJ
        X_NJ = f(Y_NJ)
***********************************************************************************************/
typedef struct dense_layer
{
    // 参数
    INTEGER units;              // 隐层层单元数headLayer
    ACTIVATION activation;      // 激活函数
    REAL alpha;                 // 部分激活函数需要
    INIT_WEIGHTS initWeights;   // 初始化权重W的方式
    // 前向计算矩阵
    MATRIX *pXni;
    MATRIX *pWij;
    MATRIX *pEnj;
    MATRIX *pYnj;
    MATRIX *pXnj;
    // 反向计算需要的转置矩阵
    MATRIX *pXni_T;
    MATRIX *pWij_T;
    // 权重与偏置更新矩阵
    MATRIX *pDEnj;
    MATRIX *pDWij;
    // momentum, rmsprop, adam计算所需的中间矩阵
    MATRIX *pmDEnj;
    MATRIX *pmDWij;
    MATRIX *pvDEnj;
    MATRIX *pvDWij;
    MATRIX *pTempDEnj;
    MATRIX *pTempDWij;
} DENSE_LAYER;

/**********************************************************************************************
Description: 层节点
Value:  name        该层名称
        nodeID      第几层
        pDenseLayer 指向全连接层
        prev        指向上一个节点
        next        指向下一个节点
Author: J.L. (2764371020@qq.com)
***********************************************************************************************/
typedef struct layer_node
{
    LAYER_NAME name;
    INTEGER nodeID;
    MATRIX *Xnext;
    DENSE_LAYER *pDenseLayer;

    // 其他种类的层指针
    // ...
    struct layer_node *prev;
    struct layer_node *next;
} LAYER_NODE;

/**********************************************************************************************
Description: 优化器所需参数
***********************************************************************************************/
typedef struct optimizer_default_parameters
{
    REAL sgdAlpha;
    REAL momentumAlpha;
    REAL momentumBeta;
    REAL rmspropAlpha;
    REAL rmspropBeta;
    REAL rmspropEpsilon;
    REAL adamAlpha;
    REAL adamBeta1;
    REAL adamBeta2;
    REAL adamEpsilon;
} OPTIMIZER_DEFAULT_PARAMETERS;

/**********************************************************************************************
Description: 网络表头
Value:  optimizer       优化器
        optParameters   优化器参数
        loss            损失函数
        next            指向第一个层节点（循环链表头节点）
***********************************************************************************************/
typedef struct layer_list
{
    OPTIMIZER optimizer;
    OPTIMIZER_DEFAULT_PARAMETERS *optParameters;
    LOSS loss;
    struct layer_node *next;
} LAYER_LIST;

/*******************************************************************************
 * Prototype Declare Section
 *******************************************************************************/
/**********************************************************************************************
Function: network_init
Description: 网络初始化
Input: 指向网络的指针layerList（网络表）
Output: 无
Input_Output: 无
Return: 错误号errorID
***********************************************************************************************/
ERROR_ID network_init(_IN LAYER_LIST *layerList);

/**********************************************************************************************
Function: add_dense_layer
Description: 增加全连接层
Input: 隐层单元数units，选取的激活函数activation，实数a
(看选取的激活函数是否需要)，初始化权重方式initWeights，指向网络的指针layerList
Output: 无
Input_Output: 无
Return: 错误号errorID
***********************************************************************************************/
ERROR_ID add_dense_layer(_IN INTEGER units, _IN ACTIVATION activation, _IN REAL alpha, _IN INIT_WEIGHTS initWeights, _IN LAYER_LIST *layerList);

/**********************************************************************************************
Function: network_compile
Description: 完成网络添加
Input: 优化器optimizer，损失函数loss，指向网络的指针layerList
Output: 无
Input_Output: 无
Return: 错误号errorID
***********************************************************************************************/
ERROR_ID network_compile(_IN OPTIMIZER optimizer, LOSS loss, _IN LAYER_LIST *layerList);

/**********************************************************************************************
Function: network_fit
Description: 网络拟合
Input: 训练数据xTrain，训练值yTrain，一次训练的数据量batchSize，训练次数epoches，损失函数loss，指向网络的指针layerList，矩阵栈S
Output: 无
Input_Output: 无
Return: 错误号errorID
***********************************************************************************************/
ERROR_ID network_fit(_IN MATRIX *xTrain, _IN MATRIX *yTrain, _IN INTEGER batchSize, _IN INTEGER epoches, _IN LAYER_LIST *layerList, _IN STACKS *S);

/**********************************************************************************************
Function: network_predict
Description: 网络预测
Input: 测试数据xTest，指向网络的指针layerList，错误号errorID，矩阵栈S
Output: 无
Input_Output: 无
Return: 预测值
***********************************************************************************************/
MATRIX *network_predict(_IN MATRIX *xTest, _IN LAYER_LIST *layerList, _IN ERROR_ID *errorID, _IN STACKS *S);

/**********************************************************************************************
Function: free_network
Description: 网络释放
Input: 指向网络的指针layerList
Output: 无
Input_Output: 无
Return: 无
***********************************************************************************************/
VOID free_network(_IN LAYER_LIST *layerList);
#endif // MAIN_NETWORK_H