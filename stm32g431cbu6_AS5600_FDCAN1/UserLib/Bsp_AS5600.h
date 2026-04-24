/**
*   @file AS5600.h
*/

#ifndef AS5600_H
#define AS5600_H

#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "stm32g4xx_hal.h"
#include "AS5600_Filter.h"
#include "Bsp_FDCAN.h"
#include "fdcan.h"

//宏定义
#define AS5600_ADDRESS		           0x36		      //AS5600的I2C从机地址

#define AS5600_REG_RAW_ANGLE_H         0x0C           // 原始角度高位
#define AS5600_REG_RAW_ANGLE_L         0x0D           // 原始角度低位

/* 功能常量宏定义 */
#define AS5600_ANGLE_MAX_VAL           4096.0f        // 12位ADC最大值
#define AS5600_ANGLE_TO_DEGREE         360.0f         // 角度转换系数
#define PI                             3.1415926535f  // 圆周率

// AS5600初始化结构体
typedef struct
{
    uint32_t id;                                      //CAN ID
    FDCAN_HandleTypeDef* hfdcan;                      //can句柄
} AS5600_HandleTypDef;

//AS5600数据结构体
typedef struct
{
    AS5600_FilterHandleTypDef *AS5600_FilterData;     //滤波器配置

    uint16_t RawAngle;                                //原始角度
    float Angle_deg;                                  //实际角度值0~360°
    float Angle_rad;                                  //实际弧度值 0 ~ 2π
    float FinalOutput;                                //最终输出

}AS5600_Data;


//对外函数
void AS5600_Init(AS5600_HandleTypDef* AS5600_Data,I2C_HandleTypeDef *hi2c,FDCAN_HandleTypeDef *hfdcan,uint32_t id);
//内部函数
static void AS5600_WriteReg(uint8_t RegAddress, uint8_t Data);
static uint8_t AS5600_ReadReg(uint8_t RegAddress);
static uint8_t AS5600_GetID(void);
void AS5600_GetData(AS5600_Data *AS5600data);

#endif //AS5600_H