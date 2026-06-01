//
// Created by didib on 2026/4/30.
//
#include "App_ChassisTask.h"
#include "dm_motor.h"
#include "cmsis_os.h"

//循环使能（调试）
// #define CIRCULAR_ENABLE

//电机零点标定
#define ZERO_POINT_MARK

//变量定义
float measure[4] = {0};//当前值（其实就是DmMotorParameter_s里的值）
extern DmMotorInstance_s *motors_chassis[4];
extern bool istrackInitialized;

void App_ChassisTask(void const* argument){
    while (!istrackInitialized) vTaskDelay(10);

#ifdef CIRCULAR_ENABLE
    while (1)
    {
        Enable_Joint_Motors(motors_chassis,4);
        osDelay(1);
    }
#endif

#ifdef ZERO_POINT_MARK
    ZeroPoint_Mark(motors_chassis[0]);
#endif

    Enable_Joint_Motors(motors_chassis,4);
    while (1)
    {

    }

}
