#ifndef MAIN_INIT_WEIGHTS_H
#define MAIN_INIT_WEIGHTS_H

#include "base.h"

/**********************************************************************************************
Description: 初始化权重方法
***********************************************************************************************/
#define INIT_WEIGHTS_TAB                 \
    INIT_WEIGHTS_MACRO(Uniform01, 0)     \
    INIT_WEIGHTS_MACRO(Uniform11, 1)     \
    INIT_WEIGHTS_MACRO(Normal, 2)        \
    INIT_WEIGHTS_MACRO(XavierUniform, 3) \
    INIT_WEIGHTS_MACRO(XavierNormal, 4)  \
    INIT_WEIGHTS_MACRO(HeUniform, 5)     \
    INIT_WEIGHTS_MACRO(HeNormal, 6)      \
    INIT_WEIGHTS_MACRO(NullInitWeight, -1)

typedef enum
{
#define INIT_WEIGHTS_MACRO(a, b) a = b,
    INIT_WEIGHTS_TAB
#undef INIT_WEIGHTS_MACRO
} INIT_WEIGHTS;

/**********************************************************************************************
 * 判断输入参数是否在 enum 表中，编辑器会检测，可去除
 ***********************************************************************************************/
FLAG is_in_init_weights_tab(INIT_WEIGHTS activation);

/**********************************************************************************************
Function: uniform_distribution
Description: 生成[a,b]的均匀分布
Input: 范围a, b (a<b)
Output: 无
Input_Output: 无
Return: [a,b]区间的数
***********************************************************************************************/
REAL uniform_distribution(_IN REAL a, _IN REAL b);

/**********************************************************************************************
Function: normal_distribution
Description: 生成均值为mu，方差为sigma的正态分布
Input: 均值mu，方差sigma
Output: 无
Input_Output: 无
Return: 均值为mu，方差为sigma的正态分布的值
***********************************************************************************************/
REAL normal_distribution(_IN REAL mu, _IN REAL sigma);

/**********************************************************************************************
Function: xavier_uniform_distribution
Description: 生成xavier的均匀分布
Input: 权重行列数nIn, nOut
Output: 无
Input_Output: 无
Return: xavier的均匀分布的值
***********************************************************************************************/
REAL xavier_uniform_distribution(_IN INTEGER nIn, _IN REAL nOut);

/**********************************************************************************************
Function: xavier_normal_distribution
Description: 生成xavier的正态分布
Input: 权重行列数nIn, nOut
Output: 无
Input_Output: 无
Return: xavier的正态分布的值
***********************************************************************************************/
REAL xavier_normal_distribution(_IN INTEGER nIn, _IN REAL nOut);

/**********************************************************************************************
Function: he_uniform_distribution
Description: 生成he的均匀分布
Input: 权重行列数nIn, nOut
Output: 无
Input_Output: 无
Return: he的均匀分布的值
***********************************************************************************************/
REAL he_uniform_distribution(_IN INTEGER nIn, _IN REAL nOut);

/**********************************************************************************************
Function: he_normal_distribution
Description: 生成he的正态分布
Input: 权重行列数nIn, nOut
Output: 无
Input_Output: 无
Return: he的正态分布的值
***********************************************************************************************/
REAL he_normal_distribution(_IN INTEGER nIn, _IN REAL nOut);
#endif // MAIN_INIT_WEIGHTS_H