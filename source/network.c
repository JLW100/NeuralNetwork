/*******************************************************************************
 *     File Name :                        network.c
 *     Abstract Description :             神经网络计算
 *     todo :                             use base layer struct
 *                                        add comments
 *                                        simplified matrix calculation
 *                                        add rnn, cnn
 *                                        ...
 *******************************************************************************/

#include "network.h"

/**********************************************************************************************
 * 判断相应的输入参数是否在 enum 表中
 ***********************************************************************************************/
FLAG is_in_optimizer_tab(OPTIMIZER optimizer)
{
    FLAG flag = false;

    switch (optimizer)
    {
#define OPTIMIZER_MACRO(a, b) case a:
        OPTIMIZER_TAB
#undef OPTIMIZER_MACRO

        flag = true;
        break;

    default:
        break;
    }

    return flag;
}

FLAG is_in_loss_tab(LOSS loss)
{
    FLAG flag = false;

    switch (loss)
    {
#define LOSS_MACRO(a, b) case a:
        LOSS_TAB
#undef LOSS_MACRO

        flag = true;
        break;

    default:
        break;
    }

    return flag;
}

/**********************************************************************************************
权重初始化
***********************************************************************************************/
/**********************************************************************************************
Function: init_weights
Description: 初始化权重
Input: 初始化权重方式initWeights
Output: 无
Input_Output: 矩阵A
Return: 无
***********************************************************************************************/
VOID init_weights(_IN_OUT MATRIX *A, _IN INIT_WEIGHTS initWeights)
{
    ASSERT(A != NULL);
    ASSERT(is_in_init_weights_tab(initWeights) && initWeights != NullInitWeight);

    INDEX i, j;
    INTEGER rows, columns;

    rows = A->rows;
    columns = A->columns;

    switch (initWeights)
    {
    case Uniform01:
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                A->p[i * columns + j] = uniform_distribution(0, 1); //产生[0, 1]的随机数
            }
        }
        break;

    case Uniform11:
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                A->p[i * columns + j] = uniform_distribution(-1, 1); //产生[-1, 1]的随机数
            }
        }
        break;

    case Normal:
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                A->p[i * columns + j] = normal_distribution(0, 1); //产生均值为0，方差为1的随机数
            }
        }
        break;

    case XavierUniform:
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                A->p[i * columns + j] = xavier_uniform_distribution(rows, columns);
            }
        }
        break;

    case XavierNormal:
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                A->p[i * columns + j] = xavier_normal_distribution(rows, columns);
            }
        }
        break;

    case HeUniform:
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                A->p[i * columns + j] = he_uniform_distribution(rows, columns);
            }
        }
        break;

    case HeNormal:
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                A->p[i * columns + j] = he_normal_distribution(rows, columns);
            }
        }
        break;

    default:
        break;
    }
}

/**********************************************************************************************
激活函数计算
***********************************************************************************************/
/**********************************************************************************************
Function: matrix_activation
Description: f(x)
Input: 矩阵A，选择的激活函数choose，实数a
Output: 矩阵B
Input_Output: 无
Return: 无
***********************************************************************************************/
VOID network_activation(_IN MATRIX *A, _IN ACTIVATION choose, _IN REAL a, _OUT MATRIX *B)
{
    ASSERT(A != NULL);
    ASSERT(is_in_activation_tab(choose) && choose != NullActivation);
    ASSERT(a >= 0 && a <= 1);
    ASSERT(B != NULL);

    INDEX i, j;
    INTEGER rows, columns;

    rows = A->rows;
    columns = A->columns;

    switch (choose)
    {
    case Identity:
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                B->p[i * columns + j] = identity(A->p[i * columns + j]);
            }
        }
        break;

    case Sigmoid:
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                B->p[i * columns + j] = sigmoid(A->p[i * columns + j]);
            }
        }
        break;

    case Tanh:
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                B->p[i * columns + j] = tanh(A->p[i * columns + j]);
            }
        }
        break;

    case Relu:
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                B->p[i * columns + j] = relu(A->p[i * columns + j]);
            }
        }
        break;

    case PRelu:
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                B->p[i * columns + j] = prelu(a, A->p[i * columns + j]);
            }
        }
        break;

    case Elu:
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                B->p[i * columns + j] = elu(a, A->p[i * columns + j]);
            }
        }
        break;

    default:
        break;
    }
}

/**********************************************************************************************
Function: matrix_dactivation
Description: f'(x)
Input: 矩阵A，选择的激活函数choose，实数a
Output: 矩阵B
Input_Output: 无
Return: 无
***********************************************************************************************/
VOID network_dactivation(_IN MATRIX *A, _IN ACTIVATION choose, _IN REAL a, _OUT MATRIX *B)
{
    ASSERT(A != NULL);
    ASSERT(is_in_activation_tab(choose) && choose != NullActivation);
    ASSERT(a >= 0 && a <= 1);
    ASSERT(B != NULL);

    INDEX i, j;
    INTEGER rows, columns;

    rows = A->rows;
    columns = A->columns;

    switch (choose)
    {
    case Identity:
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                B->p[i * columns + j] = d_identity();
            }
        }
        break;

    case Sigmoid:
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                B->p[i * columns + j] = d_sigmoid(A->p[i * columns + j]);
            }
        }
        break;

    case Tanh:
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                B->p[i * columns + j] = d_tanh(A->p[i * columns + j]);
            }
        }
        break;

    case Relu:
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                B->p[i * columns + j] = d_relu(A->p[i * columns + j]);
            }
        }
        break;

    case PRelu:
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                B->p[i * columns + j] = d_prelu(a, A->p[i * columns + j]);
            }
        }
        break;

    case Elu:
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                B->p[i * columns + j] = d_elu(a, A->p[i * columns + j]);
            }
        }
        break;

    default:
        break;
    }
}

/**********************************************************************************************
dense网络计算
***********************************************************************************************/
/**********************************************************************************************
Function: dense_layer_forward_calculate
Description: dense网络前向计算
Input: 当前层currentLayer
Output: 无
Input_Output: 无
Return: 无
***********************************************************************************************/
VOID dense_layer_forward_calculate(_IN LAYER_NODE *currentLayer)
{
    ASSERT(currentLayer != NULL && currentLayer->name == DenseLayer);

    INDEX i, j, k;
    REAL sum;
    MATRIX *Xni = currentLayer->pDenseLayer->pXni;
    MATRIX *Wij = currentLayer->pDenseLayer->pWij;
    MATRIX *Enj = currentLayer->pDenseLayer->pEnj;
    MATRIX *Ynj = currentLayer->pDenseLayer->pYnj;
    MATRIX *Xnj = currentLayer->pDenseLayer->pXnj;

    // Ynj = Xni * Wij + Enj
    for (i = 0; i < Xni->rows; i++)
    {
        for (j = 0; j < Wij->columns; j++)
        {
            sum = 0.0;
            for (k = 0; k < Xni->columns; k++)
            {
                sum += Xni->p[i * Xni->columns + k] * Wij->p[k * Wij->columns + j];
            }
            Ynj->p[i * Wij->columns + j] = sum + Enj->p[j];
        }
    }

    // Xnj = f(Ynj)
    network_activation(Ynj, currentLayer->pDenseLayer->activation, currentLayer->pDenseLayer->alpha, Xnj);
}

/**********************************************************************************************
Function: dense_layer_backward_calculate
Description: dense网络反向计算
Input: 当前层currentLayer，损失矩阵lossValue
Output: 无
Input_Output: 无
Return: 无
***********************************************************************************************/
VOID dense_layer_backward_calculate(_IN LAYER_NODE *currentLayer, _IN MATRIX *lossValue)
{
    ASSERT(currentLayer != NULL && currentLayer->name == DenseLayer);
    ASSERT(lossValue != NULL);

    INDEX i, j, k;
    REAL sum;
    MATRIX *Xni = currentLayer->pDenseLayer->pXni;
    MATRIX *Wij = currentLayer->pDenseLayer->pWij;
//    MATRIX *Enj = currentLayer->pDenseLayer->pEnj;
    MATRIX *Ynj = currentLayer->pDenseLayer->pYnj;
//    MATRIX *Xnj = currentLayer->pDenseLayer->pXnj;
    // 反向计算需要的转置矩阵
    MATRIX *Xni_T = currentLayer->pDenseLayer->pXni_T;
    MATRIX *Wij_T = currentLayer->pDenseLayer->pWij_T;
    // 权重与偏置更新矩阵
    MATRIX *DEnj = currentLayer->pDenseLayer->pDEnj;
    MATRIX *DWij = currentLayer->pDenseLayer->pDWij;
    MATRIX *DEnk, *Wjk_T;

    if (currentLayer->next->name == HeadLayer)
    {
        network_dactivation(Ynj, currentLayer->pDenseLayer->activation, currentLayer->pDenseLayer->alpha, DEnj);
//        ASSERT(DEnj->rows == lossValue->rows && DEnj->columns == lossValue->columns);
//        ASSERT(Xni->rows == Xni_T->columns && Xni->columns == Xni_T->rows);

        matrix_dot(DEnj, lossValue, DEnj);
        matrix_transpose(Xni, Xni_T);
        matrix_mul(Xni_T, DEnj, DWij);
        matrix_transpose(Wij, Wij_T);
    }
    else
    {
        network_dactivation(Ynj, currentLayer->pDenseLayer->activation, currentLayer->pDenseLayer->alpha, DEnj);

        switch (currentLayer->next->name)
        {
        case DenseLayer:
            DEnk = currentLayer->next->pDenseLayer->pDEnj;
            Wjk_T = currentLayer->next->pDenseLayer->pWij_T;
            break;

        default:
            break;
        }

        // DEnj = DEnk * Wjk_T dot f'(Ynj)
        for (i = 0; i < DEnk->rows; i++)
        {
            for (j = 0; j < Wjk_T->columns; j++)
            {
                sum = 0.0;
                for (k = 0; k < DEnk->columns; k++)
                {
                    sum += DEnk->p[i * DEnk->columns + k] *
                           Wjk_T->p[k * Wjk_T->columns + j];
                }
                DEnj->p[i * Wjk_T->columns + j] =
                    sum * DEnj->p[i * Wjk_T->columns + j];
            }
        }

        // TODO: ASSERT
        matrix_transpose(Xni, Xni_T);
        matrix_mul(Xni_T, DEnj, DWij);
        matrix_transpose(Wij, Wij_T);
    }
}

/**********************************************************************************************
权重更新
***********************************************************************************************/
/**********************************************************************************************
Function: weight_update_sgd
Description: 权重更新方式sgd
Input: 网络表layerList
Output: 无
Input_Output: 无
Return: 无
***********************************************************************************************/
VOID weight_update_sgd(_IN LAYER_LIST *layerList)
{
    ASSERT(layerList != NULL);
    ASSERT(layerList->next != NULL);
    ASSERT(layerList->next->next != NULL);

    LAYER_NODE *headLayer = layerList->next, *currentLayer = NULL;
    REAL alpha = layerList->optParameters->sgdAlpha;

    currentLayer = headLayer->next;
    while (currentLayer != headLayer)
    {
        switch (currentLayer->name)
        {
        case DenseLayer:
            // TODO: ASSERT
            // Wij = Wij - alpha * DWij
            matrix_mul_num(currentLayer->pDenseLayer->pDWij, alpha, currentLayer->pDenseLayer->pTempDWij);
            matrix_sub(currentLayer->pDenseLayer->pWij, currentLayer->pDenseLayer->pTempDWij, currentLayer->pDenseLayer->pWij);

            // Enj = Enj - alpha * DEnj
            matrix_mul_num(currentLayer->pDenseLayer->pDEnj, alpha, currentLayer->pDenseLayer->pTempDEnj);
            matrix_sub(currentLayer->pDenseLayer->pEnj, currentLayer->pDenseLayer->pTempDEnj, currentLayer->pDenseLayer->pEnj);
            break;

        default:
            break;
        }

        currentLayer = currentLayer->next;
    }
}

/**********************************************************************************************
Function: weight_update_momentum
Description: 权重更新方式momentum
Input: 网络表layerList
Output: 无
Input_Output: 无
Return: 无
***********************************************************************************************/
VOID weight_update_momentum(_IN LAYER_LIST *layerList)
{
    ASSERT(layerList != NULL);
    ASSERT(layerList->next != NULL);
    ASSERT(layerList->next->next != NULL);

    LAYER_NODE *headLayer = layerList->next, *currentLayer = NULL;
    REAL alpha = layerList->optParameters->momentumAlpha;
    REAL beta = layerList->optParameters->momentumBeta;

    currentLayer = headLayer->next;
    while (currentLayer != headLayer)
    {
        switch (currentLayer->name)
        {
        case DenseLayer:
            // TODO: ASSERT
            // vWij = alpha * vWij + beta * DWij
            matrix_mul_num(currentLayer->pDenseLayer->pvDWij, alpha, currentLayer->pDenseLayer->pvDWij);
            matrix_mul_num(currentLayer->pDenseLayer->pDWij, beta, currentLayer->pDenseLayer->pTempDWij);
            matrix_add(currentLayer->pDenseLayer->pvDWij, currentLayer->pDenseLayer->pTempDWij, currentLayer->pDenseLayer->pvDWij);
            // Wij = Wij - vWij
            matrix_sub(currentLayer->pDenseLayer->pWij, currentLayer->pDenseLayer->pvDWij, currentLayer->pDenseLayer->pWij);

            // vEnj = alpha * vEnj + beta * DEnj
            matrix_mul_num(currentLayer->pDenseLayer->pvDEnj, alpha, currentLayer->pDenseLayer->pvDEnj);
            matrix_mul_num(currentLayer->pDenseLayer->pDEnj, beta, currentLayer->pDenseLayer->pTempDEnj);
            matrix_add(currentLayer->pDenseLayer->pvDEnj, currentLayer->pDenseLayer->pTempDEnj, currentLayer->pDenseLayer->pvDEnj);
            // Enj = Enj - vEnj
            matrix_sub(currentLayer->pDenseLayer->pEnj, currentLayer->pDenseLayer->pvDEnj, currentLayer->pDenseLayer->pEnj);
            break;

        default:
            break;
        }

        currentLayer = currentLayer->next;
    }
}

/**********************************************************************************************
Function: weight_update_rmsprop
Description: 权重更新方式rmsprop
Input: 网络表layerList
Output: 无
Input_Output: 无
Return: 无
***********************************************************************************************/
VOID weight_update_rmsprop(_IN LAYER_LIST *layerList)
{
    ASSERT(layerList != NULL);
    ASSERT(layerList->next != NULL);
    ASSERT(layerList->next->next != NULL);

    LAYER_NODE *headLayer = layerList->next, *currentLayer = NULL;
    REAL alpha = layerList->optParameters->rmspropAlpha;
    REAL beta = layerList->optParameters->rmspropBeta;
    REAL epsilon = layerList->optParameters->rmspropEpsilon;

    currentLayer = headLayer->next;
    while (currentLayer != headLayer)
    {
        switch (currentLayer->name)
        {
        case DenseLayer:
            // TODO: ASSERT
            // vWij = alpha * vWij + (1 - alpha) * DWij dot DWij
            matrix_mul_num(currentLayer->pDenseLayer->pvDWij, alpha, currentLayer->pDenseLayer->pvDWij);
            matrix_dot(currentLayer->pDenseLayer->pDWij, currentLayer->pDenseLayer->pDWij, currentLayer->pDenseLayer->pTempDWij);
            matrix_mul_num(currentLayer->pDenseLayer->pTempDWij, 1 - alpha, currentLayer->pDenseLayer->pTempDWij);
            matrix_add(currentLayer->pDenseLayer->pvDWij, currentLayer->pDenseLayer->pTempDWij, currentLayer->pDenseLayer->pvDWij);
            // Wij = Wij - beta / (sqrt(vWij + epsilon)) dot DWij
            matrix_add_num(currentLayer->pDenseLayer->pvDWij, epsilon, currentLayer->pDenseLayer->pTempDWij);
            matrix_pow_num(currentLayer->pDenseLayer->pTempDWij, 0.5, currentLayer->pDenseLayer->pTempDWij);
            num_divide_matrix(beta, currentLayer->pDenseLayer->pTempDWij, currentLayer->pDenseLayer->pTempDWij);
            matrix_dot(currentLayer->pDenseLayer->pTempDWij, currentLayer->pDenseLayer->pDWij, currentLayer->pDenseLayer->pTempDWij);
            matrix_sub(currentLayer->pDenseLayer->pWij, currentLayer->pDenseLayer->pTempDWij, currentLayer->pDenseLayer->pWij);

            // vEnj = alpha * vEnj + (1 - alpha) * DEnj dot DEnj
            matrix_mul_num(currentLayer->pDenseLayer->pvDEnj, alpha, currentLayer->pDenseLayer->pvDEnj);
            matrix_dot(currentLayer->pDenseLayer->pDEnj, currentLayer->pDenseLayer->pDEnj, currentLayer->pDenseLayer->pTempDEnj);
            matrix_mul_num(currentLayer->pDenseLayer->pTempDEnj, 1 - alpha, currentLayer->pDenseLayer->pTempDEnj);
            matrix_add(currentLayer->pDenseLayer->pvDEnj, currentLayer->pDenseLayer->pTempDEnj, currentLayer->pDenseLayer->pvDEnj);
            // Enj = Enj - beta / (sqrt(vEnj + epsilon)) dot DEnj
            matrix_add_num(currentLayer->pDenseLayer->pvDEnj, epsilon, currentLayer->pDenseLayer->pTempDEnj);
            matrix_pow_num(currentLayer->pDenseLayer->pTempDEnj, 0.5, currentLayer->pDenseLayer->pTempDEnj);
            num_divide_matrix(beta, currentLayer->pDenseLayer->pTempDEnj, currentLayer->pDenseLayer->pTempDEnj);
            matrix_dot(currentLayer->pDenseLayer->pTempDEnj, currentLayer->pDenseLayer->pDEnj, currentLayer->pDenseLayer->pTempDEnj);
            matrix_sub(currentLayer->pDenseLayer->pEnj, currentLayer->pDenseLayer->pTempDEnj, currentLayer->pDenseLayer->pEnj);
            break;

        default:
            break;
        }

        currentLayer = currentLayer->next;
    }
}

/**********************************************************************************************
Function: weight_update_adam
Description: 权重更新方式adam
Input: 网络表layerList，次数num
Output: 无
Input_Output: 无
Return: 无
***********************************************************************************************/
VOID weight_update_adam(_IN LAYER_LIST *layerList, _IN INTEGER num)
{
    ASSERT(layerList != NULL);
    ASSERT(layerList->next != NULL);
    ASSERT(layerList->next->next != NULL);
    ASSERT(num >= 0);

    STACKS S;
    MATRIX *pmDWijTemp = NULL, *pvDWijTemp = NULL, *pmDEnjTemp = NULL, *pvDEnjTemp = NULL;
    LAYER_NODE *headLayer = layerList->next, *currentLayer = NULL;
    REAL alpha = layerList->optParameters->adamAlpha;
    REAL beta1 = layerList->optParameters->adamBeta1;
    REAL beta2 = layerList->optParameters->adamBeta2;
    // beta1t = beta1 ^ (times + 1)
    // beta2t = beta2 ^ (times + 1)
    REAL beta1t = pow(beta1, num + 1);
    REAL beta2t = pow(beta2, num + 1);
    REAL epsilon = layerList->optParameters->adamEpsilon;
    ERROR_ID errorID = _ERROR_NO_ERROR;

    init_stack(&S);

    currentLayer = headLayer->next;
    while (currentLayer != headLayer)
    {
        switch (currentLayer->name)
        {
        case DenseLayer:
            pmDWijTemp = creat_matrix(currentLayer->pDenseLayer->pDWij->rows, currentLayer->pDenseLayer->pDWij->columns, &errorID, &S);
            pvDWijTemp = creat_matrix(currentLayer->pDenseLayer->pDWij->rows, currentLayer->pDenseLayer->pDWij->columns, &errorID, &S);
            pmDEnjTemp = creat_matrix(currentLayer->pDenseLayer->pDEnj->rows, currentLayer->pDenseLayer->pDEnj->columns, &errorID, &S);
            pvDEnjTemp = creat_matrix(currentLayer->pDenseLayer->pDEnj->rows, currentLayer->pDenseLayer->pDEnj->columns, &errorID, &S);
            if (pmDWijTemp == NULL || pvDWijTemp == NULL || pmDEnjTemp == NULL || pvDEnjTemp == NULL) {
                puts("ERROR_FAILED_TO_ALLOCATE_HEAP_MEMORY");
                goto EXIT;
            }
            
            // mWij = beta1 * mWij + (1 - beta1) * DWij
            matrix_mul_num(currentLayer->pDenseLayer->pmDWij, beta1, currentLayer->pDenseLayer->pmDWij);
            matrix_mul_num(currentLayer->pDenseLayer->pDWij, 1 - beta1, currentLayer->pDenseLayer->pTempDWij);
            matrix_add(currentLayer->pDenseLayer->pmDWij, currentLayer->pDenseLayer->pTempDWij, currentLayer->pDenseLayer->pmDWij);
            // mWij' = mWij / (1 - beta1t)
            matrix_mul_num(currentLayer->pDenseLayer->pmDWij, 1 / (1 - beta1t), pmDWijTemp);
            // vWij = beta2 * vWij + (1 - beta2) * DWij dot DWij
            matrix_mul_num(currentLayer->pDenseLayer->pvDWij, beta2, currentLayer->pDenseLayer->pvDWij);
            matrix_dot(currentLayer->pDenseLayer->pDWij, currentLayer->pDenseLayer->pDWij, currentLayer->pDenseLayer->pTempDWij);
            matrix_mul_num(currentLayer->pDenseLayer->pTempDWij, 1 - beta2, currentLayer->pDenseLayer->pTempDWij);
            matrix_add(currentLayer->pDenseLayer->pvDWij, currentLayer->pDenseLayer->pTempDWij, currentLayer->pDenseLayer->pvDWij);
            // vWij' = vWij / (1 - beta2t)
            matrix_mul_num(currentLayer->pDenseLayer->pvDWij, 1 / (1 - beta2t), pvDWijTemp);
            // Wij = Wij - alpha / (sqrt(vWij' + epsilon)) dot mWij'
            matrix_add_num(pvDWijTemp, epsilon, pvDWijTemp);
            matrix_pow_num(pvDWijTemp, 0.5, pvDWijTemp);
            num_divide_matrix(alpha, pvDWijTemp, pvDWijTemp);
            matrix_dot(pvDWijTemp, pmDWijTemp, pvDWijTemp);
            matrix_sub(currentLayer->pDenseLayer->pWij, pvDWijTemp, currentLayer->pDenseLayer->pWij);

            // mEnj = (beta1 * mEnj + (1 - beta1) * DEnj) / (1 - beta1t)
            matrix_mul_num(currentLayer->pDenseLayer->pmDEnj, beta1, currentLayer->pDenseLayer->pmDEnj);
            matrix_mul_num(currentLayer->pDenseLayer->pDEnj, 1 - beta1, currentLayer->pDenseLayer->pTempDEnj);
            matrix_add(currentLayer->pDenseLayer->pmDEnj, currentLayer->pDenseLayer->pTempDEnj, currentLayer->pDenseLayer->pmDEnj);
            // mEnj' = mEnj / (1 - beta1t)
            matrix_mul_num(currentLayer->pDenseLayer->pmDEnj, 1 / (1 - beta1t), pmDEnjTemp);
            // vEnj = (beta2 * vEnj + (1 - beta2) * DEnj dot DEnj) / (1 - beta2t)
            matrix_mul_num(currentLayer->pDenseLayer->pvDEnj, beta2, currentLayer->pDenseLayer->pvDEnj);
            matrix_dot(currentLayer->pDenseLayer->pDEnj, currentLayer->pDenseLayer->pDEnj, currentLayer->pDenseLayer->pTempDEnj);
            matrix_mul_num(currentLayer->pDenseLayer->pTempDEnj, 1 - beta2, currentLayer->pDenseLayer->pTempDEnj);
            matrix_add(currentLayer->pDenseLayer->pvDEnj, currentLayer->pDenseLayer->pTempDEnj, currentLayer->pDenseLayer->pvDEnj);
            // vEnj' = vEnj / (1 - beta2t)
            matrix_mul_num(currentLayer->pDenseLayer->pvDEnj, 1 / (1 - beta2t), pvDEnjTemp);
            // Enj = Enj - alpha / (sqrt(vEnj' + epsilon)) dot mEnj'
            matrix_add_num(pvDEnjTemp, epsilon, pvDEnjTemp);
            matrix_pow_num(pvDEnjTemp, 0.5, pvDEnjTemp);
            num_divide_matrix(alpha, pvDEnjTemp, pvDEnjTemp);
            matrix_dot(pvDEnjTemp, pmDEnjTemp, pvDEnjTemp);
            matrix_sub(currentLayer->pDenseLayer->pEnj, pvDEnjTemp, currentLayer->pDenseLayer->pEnj);

            break;

        default:
            break;
        }

        currentLayer = currentLayer->next;
    }
    
EXIT:
    free_stack(&S);
}

/**********************************************************************************************
 * 网络初始化
 ***********************************************************************************************/
/**********************************************************************************************
Function: init_hidden_layer
Description: 初始化隐层
Input: 训练数据xTrain，一次训练的量batchSize，网络表layerList，矩阵栈S
Output: 无
Input_Output: 无
Return: 错误号ERROR_ID
***********************************************************************************************/
ERROR_ID init_hidden_layer(_IN MATRIX *xTrain, _IN INTEGER batchSize, _IN LAYER_LIST *layerList, _IN STACKS *S)
{
    ASSERT(xTrain != NULL);
    ASSERT(batchSize > 0 && batchSize <= xTrain->rows);
    ASSERT(layerList != NULL);
    ASSERT(layerList->next != NULL);
    ASSERT(layerList->next->next != NULL);
    ASSERT(S != NULL);

    INTEGER rows, columnsIn, columnsOut;
    LAYER_NODE *headLayer = NULL, *currentLayer = NULL;
    ERROR_ID errorID = _ERROR_NO_ERROR;

    // 初始化隐层矩阵
    rows = batchSize;
    columnsIn = xTrain->columns;
    headLayer = layerList->next;
    currentLayer = headLayer;
    do
    {
        currentLayer = currentLayer->next;

        switch (currentLayer->name)
        {
        case DenseLayer:
            columnsOut = currentLayer->pDenseLayer->units;

            currentLayer->pDenseLayer->pXni = currentLayer->prev->Xnext;
            currentLayer->pDenseLayer->pWij = creat_zero_matrix(columnsIn, columnsOut, &errorID, S);
            init_weights(currentLayer->pDenseLayer->pWij, currentLayer->pDenseLayer->initWeights);
            currentLayer->pDenseLayer->pEnj = creat_zero_matrix(1, columnsOut, &errorID, S);
            currentLayer->pDenseLayer->pYnj = creat_zero_matrix(rows, columnsOut, &errorID, S);
            currentLayer->pDenseLayer->pXnj = creat_zero_matrix(rows, columnsOut, &errorID, S);
            currentLayer->pDenseLayer->pXni_T = creat_zero_matrix(columnsIn, rows, &errorID, S);
            currentLayer->pDenseLayer->pWij_T = creat_zero_matrix(columnsOut, columnsIn, &errorID, S);
            currentLayer->pDenseLayer->pDEnj = creat_zero_matrix(rows, columnsOut, &errorID, S);
            currentLayer->pDenseLayer->pDWij = creat_zero_matrix(columnsIn, columnsOut, &errorID, S);
            switch (layerList->optimizer)
            {
            case Sgd:
                currentLayer->pDenseLayer->pmDEnj = NULL;
                currentLayer->pDenseLayer->pmDWij = NULL;
                currentLayer->pDenseLayer->pvDEnj = NULL;
                currentLayer->pDenseLayer->pvDWij = NULL;
                currentLayer->pDenseLayer->pTempDEnj = creat_zero_matrix(rows, columnsOut, &errorID, S);
                currentLayer->pDenseLayer->pTempDWij = creat_zero_matrix(columnsIn, columnsOut, &errorID, S);
                break;

            case Momentum:
            case Rmsprop:
                currentLayer->pDenseLayer->pmDEnj = NULL;
                currentLayer->pDenseLayer->pmDWij = NULL;
                currentLayer->pDenseLayer->pvDEnj = creat_zero_matrix(rows, columnsOut, &errorID, S);
                currentLayer->pDenseLayer->pvDWij = creat_zero_matrix(columnsIn, columnsOut, &errorID, S);
                currentLayer->pDenseLayer->pTempDEnj = creat_zero_matrix(rows, columnsOut, &errorID, S);
                currentLayer->pDenseLayer->pTempDWij = creat_zero_matrix(columnsIn, columnsOut, &errorID, S);
                break;

            case Adam:
                currentLayer->pDenseLayer->pmDEnj = creat_zero_matrix(rows, columnsOut, &errorID, S);
                currentLayer->pDenseLayer->pmDWij = creat_zero_matrix(columnsIn, columnsOut, &errorID, S);
                currentLayer->pDenseLayer->pvDEnj = creat_zero_matrix(rows, columnsOut, &errorID, S);
                currentLayer->pDenseLayer->pvDWij = creat_zero_matrix(columnsIn, columnsOut, &errorID, S);
                currentLayer->pDenseLayer->pTempDEnj = creat_zero_matrix(rows, columnsOut, &errorID, S);
                currentLayer->pDenseLayer->pTempDWij = creat_zero_matrix(columnsIn, columnsOut, &errorID, S);
                break;

            default:
                break;
            }

            currentLayer->Xnext = currentLayer->pDenseLayer->pXnj;

            columnsIn = columnsOut;
            break;

        default:
            break;
        }
    } while (currentLayer->next != headLayer && errorID == _ERROR_NO_ERROR);

    return errorID;
}

/**********************************************************************************************
Function: init_predict_hidden_layer
Description: 初始化预测的隐层
Input: 训练数据xTrain，一次训练的量batchSize，网络表predictLayerList、trainLayerList，矩阵栈S
Output: 无
Input_Output: 无
Return: 错误号ERROR_ID
***********************************************************************************************/
ERROR_ID init_predict_hidden_layer(_IN INTEGER size, _IN LAYER_LIST *predictLayerList, _IN LAYER_LIST *trainLayerList, _IN STACKS *S)
{
    ASSERT(size > 0);
    ASSERT(predictLayerList != NULL);
    ASSERT(predictLayerList->next != NULL);
    ASSERT(predictLayerList->next->next != NULL);
    ASSERT(trainLayerList != NULL);
    ASSERT(trainLayerList->next != NULL);
    ASSERT(trainLayerList->next->next != NULL);
    ASSERT(S != NULL);

    INTEGER rows, columnsOut;
    LAYER_NODE *headLayer = NULL, *currentPredictLayer = NULL, *currentTrainLayer = NULL;
    ERROR_ID errorID = _ERROR_NO_ERROR;

    // 初始化隐层矩阵
    rows = size;
    headLayer = predictLayerList->next;
    currentPredictLayer = headLayer;
    currentTrainLayer = trainLayerList->next;
    do
    {
        currentPredictLayer = currentPredictLayer->next;
        currentTrainLayer = currentTrainLayer->next;

        switch (currentPredictLayer->name)
        {
        case DenseLayer:
            columnsOut = currentPredictLayer->pDenseLayer->units;

            currentPredictLayer->pDenseLayer->pXni = currentPredictLayer->prev->Xnext;
            currentPredictLayer->pDenseLayer->pWij = currentTrainLayer->pDenseLayer->pWij;
            currentPredictLayer->pDenseLayer->pEnj = currentTrainLayer->pDenseLayer->pEnj;
            currentPredictLayer->pDenseLayer->pYnj = creat_zero_matrix(rows, columnsOut, &errorID, S);
            currentPredictLayer->pDenseLayer->pXnj = creat_zero_matrix(rows, columnsOut, &errorID, S);

            currentPredictLayer->Xnext = currentPredictLayer->pDenseLayer->pXnj;
            break;

        default:
            break;
        }
    } while (currentPredictLayer->next != headLayer && errorID == _ERROR_NO_ERROR);

    return errorID;
}

/**********************************************************************************************
Function: choose_input
Description: 一次训练的数据选择
Input: 训练数据xTrain、yTrain，选择矩阵arr
Output: 选择的训练数据xInput、yOutput
Input_Output: 无
Return: 无
***********************************************************************************************/
VOID choose_input(_IN MATRIX *xTrain, _OUT MATRIX *xInput, _IN MATRIX *yTrain, _OUT MATRIX *yOutput, _IN MATRIX *arr)
{
    ASSERT(xTrain != NULL);
    ASSERT(xInput != NULL);
    ASSERT(yTrain != NULL);
    ASSERT(yOutput != NULL);
    ASSERT(arr != NULL);

    INTEGER i, j, tmp;
    INTEGER row;

    // 数组初始化赋值0~rows-1
    for (i = 0; i < xTrain->rows; i++)
    {
        arr->p[i] = i;
    }

    // 蓄水池抽样法生成随机数组
    //    srand((unsigned int)time(NULL)); //用时间做种，每次产生随机数不一样
    for (i = xInput->rows; i < xTrain->rows; i++)
    {
        j = rand() % (i + 1);

        if (j < xInput->rows)
        {
            tmp = (int)arr->p[j];
            arr->p[j] = arr->p[i];
            arr->p[i] = tmp;
        }
    }

    // 选出batchSize的随机数组
    for (i = 0; i < xInput->rows; i++)
    {
        row = (int)arr->p[i];
        for (j = 0; j < xTrain->columns; j++)
        {
            xInput->p[i * xTrain->columns + j] =
                xTrain->p[row * xTrain->columns + j];
        }
    }

    for (i = 0; i < yOutput->rows; i++)
    {
        row = (int)arr->p[i];
        for (j = 0; j < yTrain->columns; j++)
        {
            yOutput->p[i * yTrain->columns + j] =
                yTrain->p[row * yTrain->columns + j];
        }
    }
}

/**********************************************************************************************
 * 网络计算
 ***********************************************************************************************/
/**********************************************************************************************
Function: network_forward_calculcte
Description: 网络前向计算
Input: 选择的训练数据xInput，网络表layerList
Output: 无
Input_Output: 无
Return: 无
***********************************************************************************************/
VOID network_forward_calculcte(_IN MATRIX *xInput, _IN LAYER_LIST *layerList)
{
    ASSERT(xInput != NULL);
    ASSERT(layerList != NULL);
    ASSERT(layerList->next != NULL);
    ASSERT(layerList->next->next != NULL);

    LAYER_NODE *headLayer = layerList->next, *currentLayer = NULL;

    headLayer->Xnext = xInput;
    switch (headLayer->next->name)
    {
    case DenseLayer:
        headLayer->next->pDenseLayer->pXni = headLayer->Xnext;
        break;

    default:
        break;
    }

    currentLayer = headLayer->next;
    while (currentLayer != headLayer)
    {
        switch (currentLayer->name)
        {
        case DenseLayer:
            dense_layer_forward_calculate(currentLayer);
            break;

        default:
            break;
        }

        currentLayer = currentLayer->next;
    }
}

/**********************************************************************************************
Function: calculate_loss
Description: 计算误差值
Input: 选择的训练数据yOutput，网络表layerList
Output: 误差值lossValue
Input_Output: 无
Return: 无
***********************************************************************************************/
VOID calculate_loss(_IN MATRIX *yOutput, _OUT MATRIX *lossValue, _IN LAYER_LIST *layerList)
{
    ASSERT(yOutput != NULL);
    ASSERT(lossValue != NULL);
    ASSERT(lossValue->rows == yOutput->rows && lossValue->columns == yOutput->columns);
    ASSERT(layerList != NULL);
    ASSERT(layerList->next != NULL);
    ASSERT(layerList->next->next != NULL);

    LAYER_NODE *headLayer = layerList->next, *currentLayer = NULL;

    currentLayer = headLayer->prev;
    switch (layerList->loss)
    {
    case MeanSquaredError:
        // TODO: ASSERT
        matrix_sub(currentLayer->Xnext, yOutput, lossValue);
        matrix_average_broadcast(lossValue, Row);
        break;

    default:
        break;
    }
}

/**********************************************************************************************
Function: network_backward_calculcte
Description: 网络反向计算
Input: 误差值lossValue，网络表layerList
Output: 无
Input_Output: 无
Return: 无
***********************************************************************************************/
VOID network_backward_calculcte(_IN MATRIX *lossValue, _IN LAYER_LIST *layerList)
{
    ASSERT(lossValue != NULL);
    ASSERT(layerList != NULL);
    ASSERT(layerList->next != NULL);
    ASSERT(layerList->next->next != NULL);

    LAYER_NODE *headLayer = layerList->next, *currentLayer = NULL;

    currentLayer = headLayer->prev;
    while (currentLayer != headLayer)
    {
        switch (currentLayer->name)
        {
        case DenseLayer:
            dense_layer_backward_calculate(currentLayer, lossValue);
            break;

        default:
            break;
        }

        currentLayer = currentLayer->prev;
    }
}

/**********************************************************************************************
Function: network_weight_update
Description: 网络权重更新
Input: 网络表layerList，次数num
Output: 无
Input_Output: 无
Return: 无
***********************************************************************************************/
VOID network_weight_update(_IN LAYER_LIST *layerList, _IN INTEGER num)
{
    ASSERT(layerList != NULL);
    ASSERT(layerList->next != NULL);
    ASSERT(layerList->next->next != NULL);
    ASSERT(num >= 0);

    switch (layerList->optimizer)
    {
    case Sgd:
        weight_update_sgd(layerList);
        break;

    case Momentum:
        weight_update_momentum(layerList);
        break;

    case Rmsprop:
        weight_update_rmsprop(layerList);
        break;

    case Adam:
        weight_update_adam(layerList, num);
        break;

    default:
        break;
    }
}
/**********************************************************************************************
 * 网络计算主体结构
 ***********************************************************************************************/
/**********************************************************************************************
Function: network_init
Description: 网络初始化
Input: 指向网络的指针layerList（网络表）
Output: 无
Input_Output: 无
Return: 错误号errorID
***********************************************************************************************/
ERROR_ID network_init(_IN LAYER_LIST *layerList)
{
    LAYER_NODE *headLayer = NULL;
    ERROR_ID errorID = _ERROR_NO_ERROR;

    if (layerList == NULL)
    {
        errorID = _ERROR_INPUT_PARAMETERS_ERROR;
        return errorID;
    }

    // 申请headLayer的空间
    headLayer = (LAYER_NODE *)malloc(sizeof(LAYER_NODE));
    if (headLayer == NULL)
    {
        errorID = _ERROR_FAILED_TO_ALLOCATE_HEAP_MEMORY;
        return errorID;
    }

    // 网络参数
    layerList->optimizer = NullOptimizer;
    layerList->optParameters = NULL;
    layerList->loss = NullLoss;
    layerList->next = headLayer;

    // 循环链表头节点
    headLayer->name = HeadLayer;
    headLayer->nodeID = 0;
    headLayer->Xnext = NULL;
    headLayer->pDenseLayer = NULL;
    headLayer->prev = headLayer;
    headLayer->next = headLayer;

    return errorID;
}

/**********************************************************************************************
Function: add_dense_layer
Description: 增加全连接层
Input: 隐层单元数units，选取的激活函数activation，实数a
(看选取的激活函数是否需要)，初始化权重方式initWeights，指向网络的指针layerList
Output: 无
Input_Output: 无
Return: 错误号errorID
***********************************************************************************************/
ERROR_ID add_dense_layer(_IN INTEGER units, _IN ACTIVATION activation, _IN REAL alpha, _IN INIT_WEIGHTS initWeights, _IN LAYER_LIST *layerList)
{
    DENSE_LAYER *denseLayer = NULL;
    LAYER_NODE *newLayer = NULL, *headLayer = NULL, *prevLayer = NULL;
    ERROR_ID errorID = _ERROR_NO_ERROR;

    if (units <= 0 || alpha < 0 || alpha > 1 || layerList == NULL)
    {
        errorID = _ERROR_INPUT_PARAMETERS_ERROR;
        return errorID;
    }

    if (!is_in_activation_tab(activation) || activation == NullActivation)
    {
        errorID = _ERROR_INPUT_ACTIVATION_ERROR;
        return errorID;
    }

    if (!is_in_init_weights_tab(initWeights) || initWeights == NullInitWeight)
    {
        errorID = _ERROR_INPUT_INIT_WEIGHTS_ERROR;
        return errorID;
    }

    // 申请全连接层和新节点空间
    denseLayer = (DENSE_LAYER *)malloc(sizeof(DENSE_LAYER));
    newLayer = (LAYER_NODE *)malloc(sizeof(LAYER_NODE));
    if (denseLayer == NULL || newLayer == NULL)
    {
        free(denseLayer);
        denseLayer = NULL;
        free(newLayer);
        newLayer = NULL;

        errorID = _ERROR_FAILED_TO_ALLOCATE_HEAP_MEMORY;
        return errorID;
    }

    // 全连接层参数
    denseLayer->units = units;
    denseLayer->activation = activation;
    denseLayer->alpha = alpha;
    denseLayer->initWeights = initWeights;
    // 全连接层矩阵，置为空，网络模型fit时初始化
    // 前向计算矩阵
    denseLayer->pXni = NULL;
    denseLayer->pWij = NULL;
    denseLayer->pEnj = NULL;
    denseLayer->pYnj = NULL;
    denseLayer->pXnj = NULL;
    // 反向计算需要的转置矩阵
    denseLayer->pXni_T = NULL;
    denseLayer->pWij_T = NULL;
    // 权重与偏置更新矩阵
    denseLayer->pDEnj = NULL;
    denseLayer->pDWij = NULL;
    // momentum, rmsprop, adam计算所需的中间矩阵
    denseLayer->pmDEnj = NULL;
    denseLayer->pmDWij = NULL;
    denseLayer->pvDEnj = NULL;
    denseLayer->pvDWij = NULL;
    denseLayer->pTempDEnj = NULL;
    denseLayer->pTempDWij = NULL;

    // 循环链表的头节点和前一个节点
    headLayer = layerList->next;
    prevLayer = headLayer->prev;
    // 新节点的参数
    newLayer->name = DenseLayer;
    newLayer->Xnext = NULL;
    newLayer->nodeID = prevLayer->nodeID + 1;
    newLayer->pDenseLayer = denseLayer;
    // 网络层（循环链表）连接新节点
    newLayer->prev = prevLayer;
    newLayer->next = headLayer;
    prevLayer->next = newLayer;
    headLayer->prev = newLayer;

    return errorID;
}

/**********************************************************************************************
Function: network_compile
Description: 完成网络添加
Input: 优化器optimizer，损失函数loss，指向网络的指针layerList
Output: 无
Input_Output: 无
Return: 错误号errorID
***********************************************************************************************/
ERROR_ID network_compile(_IN OPTIMIZER optimizer, _IN LOSS loss, _IN LAYER_LIST *layerList)
{
    ERROR_ID errorID = _ERROR_NO_ERROR;

    // 输入参数判断
    if (layerList == NULL)
    {
        errorID = _ERROR_INPUT_PARAMETERS_ERROR;
        return errorID;
    }

    if (!is_in_optimizer_tab(optimizer) || optimizer == NullOptimizer)
    {
        errorID = _ERROR_INPUT_OPTIMIZER_ERROR;
        return errorID;
    }

    if (!is_in_loss_tab(loss) || loss == NullLoss)
    {
        errorID = _ERROR_INPUT_LOSS_ERROR;
        return errorID;
    }

    // 优化器参数默认值
    layerList->optParameters = (OPTIMIZER_DEFAULT_PARAMETERS *)malloc(sizeof(OPTIMIZER_DEFAULT_PARAMETERS));
    layerList->optParameters->sgdAlpha = 0.01;
    layerList->optParameters->momentumAlpha = 0.9;
    layerList->optParameters->momentumBeta = 0.01;
    layerList->optParameters->rmspropAlpha = 0.9;
    layerList->optParameters->rmspropBeta = 0.01;
    layerList->optParameters->rmspropEpsilon = 1e-6;
    layerList->optParameters->adamAlpha = 0.001;
    layerList->optParameters->adamBeta1 = 0.9;
    layerList->optParameters->adamBeta2 = 0.999;
    layerList->optParameters->adamEpsilon = 1e-6;

    // 网络参数
    layerList->optimizer = optimizer;
    layerList->loss = loss;

    return errorID;
}

/**********************************************************************************************
Function: network_fit
Description: 网络拟合
Input: 训练数据xTrain，训练值yTrain，一次训练的数据量batchSize，训练次数epoches，损失函数loss，指向网络的指针layerList，矩阵栈S
Output: 无
Input_Output: 无
Return: 错误号errorID
***********************************************************************************************/
ERROR_ID network_fit(_IN MATRIX *xTrain, _IN MATRIX *yTrain, _IN INTEGER batchSize, _IN INTEGER epoches, _IN LAYER_LIST *layerList, _IN STACKS *S)
{
    INTEGER i;
    ERROR_ID errorID = _ERROR_NO_ERROR;

    // 输入参数判断
    if (xTrain == NULL || yTrain == NULL || batchSize <= 0 || epoches <= 0 || layerList == NULL || S == NULL)
    {
        errorID = _ERROR_INPUT_PARAMETERS_ERROR;
        goto EXIT;
    }

    MATRIX *arr = creat_matrix(xTrain->rows, 1, &errorID, S);
    MATRIX *xInput = creat_matrix(batchSize, xTrain->columns, &errorID, S);
    MATRIX *yOutput = creat_matrix(batchSize, yTrain->columns, &errorID, S);
    MATRIX *lossValue = creat_matrix(batchSize, yTrain->columns, &errorID, S);

    if (errorID != _ERROR_NO_ERROR)
    {
        goto EXIT;
    }

    // 初始化隐层矩阵
    srand((unsigned int)time(NULL)); //用时间做种，每次产生随机数不一样
    errorID = init_hidden_layer(xTrain, batchSize, layerList, S);

    for (i = 0; i < epoches; i++)
    {
        // 正向计算
        choose_input(xTrain, xInput, yTrain, yOutput, arr);
        network_forward_calculcte(xInput, layerList);
        // 误差值
        calculate_loss(yOutput, lossValue, layerList);
        // 反向求导
        network_backward_calculcte(lossValue, layerList);
        // 权重更新
        network_weight_update(layerList, i);
    }

EXIT:
    return errorID;
}

/**********************************************************************************************
Function: network_predict
Description: 网络预测
Input: 测试数据xTest，指向网络的指针layerList，错误号errorID，矩阵栈S
Output: 无
Input_Output: 无
Return: 预测值
***********************************************************************************************/
MATRIX *network_predict(_IN MATRIX *xTest, _IN LAYER_LIST *layerList, _IN ERROR_ID *errorID, _IN STACKS *S)
{
    INTEGER rows, columns;
    INDEX i, j;
    MATRIX *yPred = NULL;
    LAYER_LIST predictLayerList;

    // 输入参数判断
    if (*errorID != _ERROR_NO_ERROR)
    {
        goto EXIT;
    }

    if (xTest == NULL || layerList == NULL || S == NULL)
    {
        *errorID = _ERROR_INPUT_PARAMETERS_ERROR;
        goto EXIT;
    }

    LAYER_NODE *headLayer = layerList->next, *currentLayer = NULL;

    // 生成相同的网络
    *errorID = network_init(&predictLayerList);
    currentLayer = headLayer->next;
    while (currentLayer != headLayer && *errorID == _ERROR_NO_ERROR)
    {
        switch (currentLayer->name)
        {
        case DenseLayer:
            *errorID = add_dense_layer(currentLayer->Xnext->columns, currentLayer->pDenseLayer->activation, currentLayer->pDenseLayer->alpha, currentLayer->pDenseLayer->initWeights, &predictLayerList);
            break;

        default:
            break;
        }

        currentLayer = currentLayer->next;
    }
    *errorID = network_compile(layerList->optimizer, layerList->loss, &predictLayerList);

    // 开始计算
    *errorID = init_predict_hidden_layer(xTest->rows, &predictLayerList, layerList, S);
    network_forward_calculcte(xTest, &predictLayerList);

    // 赋值
    rows = predictLayerList.next->prev->Xnext->rows;
    columns = predictLayerList.next->prev->Xnext->columns;
    yPred = creat_matrix(rows, columns, errorID, S);
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < columns; j++)
        {
            yPred->p[i * columns + j] = predictLayerList.next->prev->Xnext->p[i * columns + j];
        }
    }

    free_network(&predictLayerList);

EXIT:
    return yPred;
}

/**********************************************************************************************
Function: free_network
Description: 网络释放
Input: 指向网络的指针layerList
Output: 无
Input_Output: 无
Return: 无
***********************************************************************************************/
VOID free_network(_IN LAYER_LIST *layerList)
{
    if (layerList == NULL)
    {
        return;
    }

    LAYER_NODE *headLayer = layerList->next, *currentLayer = headLayer->next, *nextLayer = NULL;

    while (currentLayer != headLayer)
    {
        nextLayer = currentLayer->next;

        switch (currentLayer->name)
        {
        case DenseLayer:
            free(currentLayer->pDenseLayer);
            currentLayer->pDenseLayer = NULL;
            break;

        default:
            break;
        }

        currentLayer->Xnext = NULL;
        currentLayer->next = NULL;
        currentLayer->prev = NULL;
        free(currentLayer);
        currentLayer = nextLayer;
    }

    headLayer->Xnext = NULL;
    headLayer->next = NULL;
    headLayer->prev = NULL;
    free(headLayer);

    free(layerList->optParameters);
    layerList->optParameters = NULL;
    layerList->optimizer = NullOptimizer;
    layerList->loss = NullLoss;
    layerList->next = NULL;

    // ...
    // 释放其他指针
}