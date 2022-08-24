/*******************************************************************************
 *     File Name :                        init_weights.c
 *     Abstract Description :             权重初始化方式
 *     Warring :                          need srand((unsigned int)time(NULL))
 *     todo :                             other init_weights function
 *                                        ...
 *******************************************************************************/

#include "init_weights.h"

/**********************************************************************************************
 * 判断输入参数是否在 enum 表中
 ***********************************************************************************************/
FLAG is_in_init_weights_tab(INIT_WEIGHTS activation)
{
    FLAG flag = false;

    switch (activation)
    {
#define INIT_WEIGHTS_MACRO(a, b) case a:
        INIT_WEIGHTS_TAB
#undef INIT_WEIGHTS_MACRO

        flag = true;
        break;

    default:
        break;
    }

    return flag;
}

/**********************************************************************************************
Function: uniform_distribution
Description: 生成[a,b]的均匀分布
Input: 范围a, b (a<b)
Output: 无
Input_Output: 无
Return: [a,b]区间的数
***********************************************************************************************/
REAL uniform_distribution(_IN REAL a, _IN REAL b)
{
    REAL seed = rand() / (RAND_MAX + 1.0);
    return a + (b - a) * seed;
}

/**********************************************************************************************
Function: normal_distribution
Description: 生成均值为mu，方差为sigma的正态分布
Input: 均值mu，方差sigma
Output: 无
Input_Output: 无
Return: 均值为mu，方差为sigma的正态分布的值
***********************************************************************************************/
REAL normal_distribution(_IN REAL mu, _IN REAL sigma)
{
    // Box Muller法生成正态分布
    REAL u1 = uniform_distribution(0, 1.0);
    REAL u2 = uniform_distribution(0, 1.0);
    REAL rho = sqrt(-2 * log(u2));
    REAL theta = 2.0 * PI * u1;
    REAL x = rho * cos(theta);

    return (mu + sigma * x);
}

/**********************************************************************************************
Function: xavier_uniform_distribution
Description: 生成xavier的均匀分布
Input: 权重行列数nIn, nOut
Output: 无
Input_Output: 无
Return: xavier的均匀分布的值
***********************************************************************************************/
REAL xavier_uniform_distribution(_IN INTEGER nIn, _IN REAL nOut)
{
    return uniform_distribution(-sqrt(6.0 / (nIn + nOut)), sqrt(6.0 / (nIn + nOut)));
}

/**********************************************************************************************
Function: xavier_normal_distribution
Description: 生成xavier的正态分布
Input: 权重行列数nIn, nOut
Output: 无
Input_Output: 无
Return: xavier的正态分布的值
***********************************************************************************************/
REAL xavier_normal_distribution(_IN INTEGER nIn, _IN REAL nOut)
{
    return normal_distribution(0, sqrt(2.0 / (nIn + nOut)));
}

/**********************************************************************************************
Function: he_uniform_distribution
Description: 生成he的均匀分布
Input: 权重行列数nIn, nOut
Output: 无
Input_Output: 无
Return: he的均匀分布的值
***********************************************************************************************/
REAL he_uniform_distribution(_IN INTEGER nIn, _IN REAL nOut)
{
    return uniform_distribution(-sqrt(3.0 / 2.0 / (nIn + nOut)), sqrt(3.0 / 2.0 / (nIn + nOut)));
}

/**********************************************************************************************
Function: he_normal_distribution
Description: 生成he的正态分布
Input: 权重行列数nIn, nOut
Output: 无
Input_Output: 无
Return: he的正态分布的值
***********************************************************************************************/
REAL he_normal_distribution(_IN INTEGER nIn, _IN REAL nOut)
{
    return normal_distribution(0, sqrt(2.0 / nIn));
}