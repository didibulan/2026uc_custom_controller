/**
*   @file AS5600_Filter.h
*/

#ifndef AS5600_FILTER_H
#define AS5600_FILTER_H

#include <stdint.h>


#define MEDIAN_WINDOW_SIZE      5                // 中值滤波窗口大小
#define PI                      3.1415926535f    // 圆周率

/**
*@brief AS5600滤波模式选择
*/
typedef enum
{
    FILTER_MODE_NONE,   //原始输出
    FILTER_MODE_LPF,    //一阶低通
    FILTER_MODE_MEDIAN, //中值滤波
}Filter_Mode;

typedef struct
{
    Filter_Mode Filter_mode;
    float alpha;
    uint8_t window_size;

}AS5600_FilterHandleTypDef;

//外部函数
void AS5600_Filter(AS5600_FilterHandleTypDef *AS5600_FilterData,uint16_t RawAngle,float FinalOutput);
//滤波算法（内部调用）
static float AS5600_SetLPF(float new_value, float alpha) ;
static float AS5600_SetMedian(float new_value,uint8_t window_size);

#endif //AS5600_FILTER_H