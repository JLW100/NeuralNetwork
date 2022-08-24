#ifndef MAIN_ACTIVATION_H
#define MAIN_ACTIVATION_H

#include "base.h"

/**********************************************************************************************
Description: 激活函数
***********************************************************************************************/
#define ACTIVATION_TAB                                                         \
    ACTIVATION_MACRO(Identity, 0)                                              \
    ACTIVATION_MACRO(Sigmoid, 1)                                               \
    ACTIVATION_MACRO(Tanh, 2)                                                  \
    ACTIVATION_MACRO(Relu, 3)                                                  \
    ACTIVATION_MACRO(PRelu, 4)                                                 \
    ACTIVATION_MACRO(Elu, 5)                                                   \
    ACTIVATION_MACRO(NullActivation, -1)

typedef enum {
#define ACTIVATION_MACRO(a, b) a = b,
    ACTIVATION_TAB
#undef ACTIVATION_MACRO
} ACTIVATION;

/**********************************************************************************************
 * 判断输入参数是否在 enum 表中
 ***********************************************************************************************/
FLAG is_in_activation_tab(ACTIVATION activation);

/**********************************************************************************************
Function: identity
Description: f(x) = x
Input: x
Output: 无
Input_Output: 无
Return: f(x)
***********************************************************************************************/
REAL identity(_IN REAL x);

/**********************************************************************************************
Function: d_identity
Description: f'(x) = 1
Input: 无
Output: 无
Input_Output: 无
Return: f'(x)
***********************************************************************************************/
REAL d_identity(void);

/**********************************************************************************************
Function: sigmoid
Description: f(x) = 1 / (1 + e^(-x))
Input: x
Output: 无
Input_Output: 无
Return: f(x)
***********************************************************************************************/
REAL sigmoid(_IN REAL x);

/**********************************************************************************************
Function: d_sigmoid
Description: f'(x) = f(x)(1 - f(x)), f(x) = sigmoid(x)
Input: x
Output: 无
Input_Output: 无
Return: f'(x)
***********************************************************************************************/
REAL d_sigmoid(_IN REAL x);

/**********************************************************************************************
Function: tanh
Description: f(x) = 2 / (1 + e^(-2x)) - 1 = 2sigmoid(2x) - 1
Input: x
Output: 无
Input_Output: 无
Return: f(x)
***********************************************************************************************/
REAL tanh(_IN REAL x);

/**********************************************************************************************
Function: d_tanh
Description: f'(x) = 1 - f(x)^2, f(x) = tanh(x)
Input: x
Output: 无
Input_Output: 无
Return: f'(x)
***********************************************************************************************/
REAL d_tanh(_IN REAL x);

/**********************************************************************************************
Function: relu
Description: f(x) = {0, x<0; x, x>=0}
Input: x
Output: 无
Input_Output: 无
Return: f(x)
***********************************************************************************************/
REAL relu(_IN REAL x);

/**********************************************************************************************
Function: d_relu
Description: f'(x) = {0, x<0; 1, x>=0}
Input: x
Output: 无
Input_Output: 无
Return: f'(x)
***********************************************************************************************/
REAL d_relu(_IN REAL x);

/**********************************************************************************************
Function: prelu
Description: f(x) = {ax, x<0; x, x>=0}
Input: a, x
Output: 无
Input_Output: 无
Return: f(x)
***********************************************************************************************/
REAL prelu(_IN REAL a, _IN REAL x);

/**********************************************************************************************
Function: d_prelu
Description: f'(x) = {a, x<0; 1, x>=0}
Input: x
Output: 无
Input_Output: 无
Return: f'(x)
***********************************************************************************************/
REAL d_prelu(_IN REAL a, _IN REAL x);

/**********************************************************************************************
Function: elu
Description: f(x) = {a(e^x - 1), x<0; x, x>=0}
Input: a, x
Output: 无
Input_Output: 无
Return: f(x)
***********************************************************************************************/
REAL elu(_IN REAL a, _IN REAL x);

/**********************************************************************************************
Function: d_elu
Description: f'(x) = {f(x)+a, x<0; 1, x>=0}, f(x) = elu(x)
Input: x
Output: 无
Input_Output: 无
Return: f'(x)
***********************************************************************************************/
REAL d_elu(_IN REAL a, _IN REAL x);

#endif // MAIN_ACTIVATION_H