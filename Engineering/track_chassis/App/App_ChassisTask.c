//
// Created by didib on 2026/4/30.
//
#include "App_ChassisTask.h"
#include "dm_motor.h"
#include "cmsis_os.h"
#include "alg_Pid.h"

//循环使能（调试）
// #define CIRCULAR_ENABLE

//电机零点标定
// #define ZERO_POINT_MARK

//变量定义
float measure_angle[4] = {0};//当前值（其实就是DmMotorParameter_s里的值）
float measure_velocoty[4] = {0};
float torque[4] = {0};
static float target_q[4] = {0.f, 0.f, 0.f, 1.57f};
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

    osDelay(1);
    Motor_Dm_Cmd(motors_chassis[3],DM_CMD_MOTOR_ENABLE);
    Motor_Dm_Transmit(motors_chassis[3]);

    while (1)
    {
        // Motor_Dm_Cmd(motors_chassis[3],DM_CMD_MOTOR_ENABLE);
        // Motor_Dm_Transmit(motors_chassis[3]);
        measure_angle[3] = motors_chassis[3]->message.out_position;
        measure_velocoty[3] = motors_chassis[3]->message.out_velocity;
        torque[3] = Pid_Calculate(motors_chassis[3]->angle_pid,motors_chassis[3]->velocity_pid,
            target_q[3],measure_angle[3],measure_velocoty[3]);

        Motor_Dm_Mit_Control(motors_chassis[3], 0.f, 0.f,torque[3]);
        Motor_Dm_Transmit(motors_chassis[3]);
    }

    Enable_Joint_Motors(motors_chassis,4);
    while (1)
    {

    }

}
