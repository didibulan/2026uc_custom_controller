/**
*   @file AS5600_Filter.c
*/

#include "AS5600_Filter.h"
#include "string.h"
#include <stdlib.h>

/**
 * @brief AS5600一阶低通滤波函数（带角度跳变保护）
 * @param
 * @return 滤波后的平滑角度值（单位：rad）
 * @note
 */
static float AS5600_SetLPF(float new_value,float alpha)
{
    static float output = 0.0f;

    //计算本次差值和上一次差值之间的差，防止过滤跳变
    float delta = ((new_value - output) > 0)?(new_value - output) : -(new_value-output);

    if (delta > PI)
    {
        output = new_value;
    }else
    {
        output = alpha * new_value +(1.0f - alpha) * output;
    }
    return output;
}

/**
 * @brief AS5600中值滤波函数
 * @param
 * @return 滤波后的平滑角度值（单位：rad）
 * @note 中值滤波
 */
static float AS5600_SetMedian(float new_value,uint8_t window_size)
{
    static float *buffer = NULL;            //缓冲区
    static uint8_t current_window = 0;      //窗口大小
    static int index = 0;                   //索引

    if (window_size != current_window){
        //释放内存，重新申请
        if (buffer != NULL){
            free(buffer);
        }
        buffer=(float *)malloc(sizeof(float)*window_size);
        if (buffer == NULL){
            //分配失败
        }
        current_window = window_size;
        index = 0;
    }

    //更新缓冲区
    buffer[index] = new_value;
    index = (index + 1) % current_window;

    //固定长度的临时数组
    float sorted[20];
    if (current_window >= 20)return new_value;
    //冒泡排序
    memcpy(sorted, buffer, sizeof(float)*current_window);
    for (int i = 0; i < current_window-1; i++) {
        for (int j = 0; j < current_window-i-1; j++) {
            if (sorted[j] > sorted[j+1]) {
                float temp = sorted[j];
                sorted[j] = sorted[j+1];
                sorted[j+1] = temp;
            }
        }
    }
    //返回中值
    return  sorted[current_window/2];
}

void AS5600_Filter(AS5600_FilterHandleTypDef *AS5600_FilterData,uint16_t RawAngle,float FinalOutput)
{
    if (AS5600_FilterData->Filter_mode == FILTER_MODE_NONE)
    {
        FinalOutput = RawAngle;

    }else if (AS5600_FilterData->Filter_mode == FILTER_MODE_LPF)
    {
        FinalOutput = AS5600_SetLPF(RawAngle,AS5600_FilterData->alpha);
    }
    else if (AS5600_FilterData->Filter_mode == FILTER_MODE_MEDIAN)
    {
        FinalOutput = AS5600_SetMedian(RawAngle,AS5600_FilterData->window_size);
    }
}