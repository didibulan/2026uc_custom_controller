//
// Created by didib on 2026/4/30.
//
#include "App_ChassisTask.h"
#include "dm_motor.h"
#include "cmsis_os.h"
//变量定义
float measure[4] = {0};//当前值（其实就是DmMotorParameter_s里的值）
extern DmMotorInstance_s *motors_chassis[4];

void App_ChassisTask(void const* argument){
    osDelay(1);
}
