//
// Created by didib on 2026/4/29.
//

#ifndef CASCADE_PID_ALG_PID_H
#define CASCADE_PID_ALG_PID_H

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    // float target[2];        // 两次目标值
    float target_max;       // 目标限幅max
    float target_min;       // 目标限幅min
    float kp;               // 比例增益
    float ki;               // 积分增益
    float kd;               // 微分增益
    // float p_out;            // 比例输出
    // float i_out;            // 积分输出
    // float d_out;            // 微分输出
    float max_output;       // 输出限幅（max）
    float min_output;       // 输出限幅（min）
    float i_max;            // 积分限幅（max）
    float i_min;            // 积分限幅（min）
    // float i_sum;            // 积分
    float d_max;            // 微分限幅（max）
    float d_min;            // 微分限幅（min）
    // float integral;         // 积分
    // float derivative;       // 微分
    // float measure[2];       // 两次实际值
    // float error[2];         // 两次误差
    // float output;           // 输出
    // float last_anlge;       // 上一次角度
    // float angle_sum;        // 角度累积
    float angle_max;        // 角度限幅（max）

    float dead_zone;        // 死区
    //增加：微分先行
} alg_Pid_t;

typedef struct{
    float target_max;       // 目标限幅max
    float target_min;       // 目标限幅min
    float kp;               // 比例增益
    float ki;               // 积分增益
    float kd;               // 微分增益
    float i_max;            // 积分限幅（max）
    float i_min;            // 积分限幅（min）
    float i_sum;            // 积分
    float p_out;            // 比例输出
    float i_out;            // 积分输出
    float d_out;            // 微分输出
    float max_output;       // 输出限幅（max）
    float min_output;       // 输出限幅（min）
    float angle_max;        // 角度限幅（max）
    float output;           // 输出
    float last_anlge;       // 上一次角度
    float angle_sum;        // 角度累积
    float dead_zone;        // 死区

    float measure[2];       // 两次实际值
    float error[2];         // 两次误差
    float target[2];        // 两次目标值

    bool is_enable;          // 使能标志
}PidInstance_s;

//pid注册
PidInstance_s *Pid_Register(alg_Pid_t *config);
//速度环
void Pid_Speed(PidInstance_s *pid, float target, float measure);
//角度环
void Pid_Angle(PidInstance_s *pid, float target, float measure, float MaxSpeed);
//位置控制
float Pid_Calculate(PidInstance_s *angle_pid, PidInstance_s *speed_pid, float target_angle, float measure_angle, float measure_velocoty);

#endif //CASCADE_PID_ALG_PID_H