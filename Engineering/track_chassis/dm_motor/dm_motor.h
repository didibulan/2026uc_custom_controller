//
// Created by didib on 2026/4/30.
//

#ifndef CASCADE_PID_DM_MOTOR_H
#define CASCADE_PID_DM_MOTOR_H

#include "Bsp_fdcan.h"
#include "alg_Pid.h"

#define FLOAT_MAX 16777210.0f

typedef enum {
    J4310 = 0,
    J4340 = 1,
    S3519 = 2,
    J8006 = 3,
    J6248 = 4,
}DmMotorType_e;

/**
*@brief DM电机PID模式选择枚举，目前只能用于MIT模式的力矩参数下
*/
typedef enum {
    DM_POSITION,     // 位置控制模式
    DM_VELOCITY,     // 速度控制模式
    DM_POSVEL,       // 速度位置控制模式
}DmMotorControlMode_e;

/**
 * @brief DM电机状态枚举
 */
typedef enum {
    DM_DISABLE = 0x00,      // 失能
    DM_ENABLE = 0x01,       // 使能
    ENCODER_NOT_CALIBRATED = 0x02, // 编码器未校准
    OVERVOLTAGE = 0x08,     // 超压
    UNDERVOLTAGE = 0x09,    // 欠压
    OVERCURRENT = 0x0A,     // 过电流
    MOS_OVERTEMP = 0x0B,    //MOS 过热
    ROTOR_OVERTEMP = 0x0C,  // 电机线圈过热
    COMM_LOST = 0x0D,       // 通讯丢失
    OVERLOAD = 0x0E         // 过载
} Error_code_e;

/**
 * @brief DM电机命令枚举
 */
typedef enum {
    DM_CMD_MOTOR_ENABLE = 0,    // 使能
    DM_CMD_MOTOR_DISABLE = 1,    // 失能
    DM_CMD_ZERO_POSITION = 2, // 保存位置零点
    DM_CMD_CLEAR_ERROR = 3    // 清除电机错误
}DmMotorMode_e;

typedef struct{
    float pos_max;
    float vel_max;
    float tor_max;
    float kp_max;
    float kd_max;
    float kp_int;
    float kd_int;
}DmMotorParameter_s;

typedef struct{
    int pos_int;
    int vel_int;
    int tor_int;
    float last_position;    // 电机输出上次角度(-PI~PI)
    float last_out_position;// 电机输出上次角度(-PI~PI)
    float position;         // 当前角度，没有归一化的角度
    float out_position;     // 当前角度(-PI~PI)
    float out_velocity;     // 当前速度
    float torque;           // 当前扭矩(Kg·m^2·s^(-2))
    float tem_mos;          // 驱动上MOS的温度(°C)
    float tem_rotor;        // 电机内部线圈的温度(°C)

    float total_angle;      // 电机累计旋转总角度
}DmMotorMessage_s;

typedef struct {
    char* topic_name;
    DmMotorType_e type; // 电机类型
    DmMotorParameter_s parameters;
    DmMotorControlMode_e control_mode; // 电机控制模式

    CanInitConfig_s can_config; // 电机CAN配置

    alg_Pid_t angle_pid_config; // 角度控制PID配置
    alg_Pid_t velocity_pid_config; // 速度控制PID配置
}DmMotorInitConfig_s;

typedef struct {
    char* topic_name;
    DmMotorType_e type; // 电机类型
    DmMotorParameter_s parameters;
    CanInstance_s *can_instance; // 电机CAN实例
    DmMotorControlMode_e control_mode; // 电机控制模式
    PidInstance_s *angle_pid; // 角度控制PID
    PidInstance_s *velocity_pid; // 速度控制PID
    Error_code_e motor_state;  // 电机状态

    DmMotorMessage_s message; // 电机反馈数据

    float target_position;  // 电机目标角度(-PI~PI)
    float target_velocity;  // 电机目标速度(rpm)
    float output;
}DmMotorInstance_s;

DmMotorInstance_s *Motor_DM_Register(DmMotorInitConfig_s *config);
bool Motor_Dm_Control(DmMotorInstance_s *motor, float target);
bool Motor_Dm_Cmd(DmMotorInstance_s *motor, DmMotorMode_e cmd);
bool Motor_Dm_Mit_Control(const DmMotorInstance_s *motor, const float pos, const float vel, const float tor);
bool Motor_Dm_Pos_Vel_Control(const DmMotorInstance_s *motor, float pos, float vel);
bool Motor_Dm_Transmit(const DmMotorInstance_s *motor);

bool Enable_Joint_Motors(DmMotorInstance_s **instance,uint8_t num);
bool ZeroPoint_Mark(DmMotorInstance_s *instance);

#endif //CASCADE_PID_DM_MOTOR_H