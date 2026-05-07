//
// Created by didib on 2026/4/30.
//
#include "App_ChassisTask.h"
#include "dm_motor.h"
#include "cmsis_os.h"

//循环使能（调试）
#define ENABLE_TRANSMIT
//变量定义
float measure[4] = {0};//当前值（其实就是DmMotorParameter_s里的值）
extern DmMotorInstance_s *motors_chassis[4];
extern bool istrackInitialized;

void App_ChassisTask(void const* argument){
    while (!istrackInitialized) osDelay(10);

#ifdef ENABLE_TRANSMIT
    for (int i = 0; i < 4; i++){
        Enable_Chassis_Motors(motors_chassis[i]);
        osDelay(1);
    }
#endif

}
