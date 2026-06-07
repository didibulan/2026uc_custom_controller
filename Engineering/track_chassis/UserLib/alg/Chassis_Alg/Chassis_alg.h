//
// Created by didib on 2026/5/31.
//

#ifndef CASCADE_PID_CHASSIS_ALG_H
#define CASCADE_PID_CHASSIS_ALG_H

#include "alg_portbag.h"
#include "dm_motor.h"
#include "robot_config.h"

//底盘类型枚举
typedef enum {
    Omni_Wheel = 0,     //全向
    Mecanum_Wheel = 1,  //麦轮
    Steering_Wheel = 2, //舵轮
}CarType;

//底盘速度结构体
typedef struct
{

    float Vx;//x
    float Vy;//y
    float Vw;//角速度

}Chassis_Speed;

//底盘固定参数
//全向，舵轮
typedef struct{
    float wheel_radius;                     //< 轮子半径(m)
    float chassis_radius;                   //< 底盘旋转半径(m)
    float chassis_steering_zero[4];         ///<舵向电机初始值(rad)
    float chassis_steering_normal[4];       ///<舵向电机正常值(rad)
    float Steering_Ratio;                   //< 舵组功率分配系数
}Chassis_Omni_Steering_Message_s;

//麦轮
typedef struct
{

    float length_a;//a半长
    float length_b;//b半宽
    float radius_s;//s轮半径

}Chassis_Mecanum__Message_s;

//底盘初始化
typedef struct
{
    CarType cartyp;
    Chassis_Speed chassis_speed;
    Chassis_Omni_Steering_Message_s omni_steering_message;
    Chassis_Mecanum__Message_s mecanum_message;

}ChassisInitConfig_s;

//底盘实例
typedef struct
{
    CarType cartyp;
    Chassis_Speed chassis_speed;
    Chassis_Omni_Steering_Message_s omni_steering_message;
    Chassis_Mecanum__Message_s mecanum_message;
    float power_limitation[4];//功率限制数组
    float out_speed[4];//输出速度
    float out_angle[4];//输出角度

    // DjiMotor

}ChassisInstance_s;

#endif //CASCADE_PID_CHASSIS_ALG_H