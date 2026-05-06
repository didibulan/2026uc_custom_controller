//
// Created by didib on 2026/4/29.
//

//PID控制器注册函数

#include "alg_portbag.h"
#include "alg_Pid.h"
#include "FreeRTOS.h"
#include "robot_config.h"
#include <string.h>

//pid注册函数
PidInstance_s *Pid_Register(alg_Pid_t *config){
    if (config == NULL) return NULL;

    PidInstance_s *Pid_Instance = (PidInstance_s *)user_malloc(sizeof(PidInstance_s));
    if (Pid_Instance == NULL) return NULL;
    memset(Pid_Instance, 0, sizeof(PidInstance_s));

    Pid_Instance->kp = config->kp;
    Pid_Instance->ki = config->ki;
    Pid_Instance->kd = config->kd;
    Pid_Instance->angle_max = config->angle_max;
    if (config->i_max > config->max_output) Pid_Instance->i_max = config->max_output;
    else Pid_Instance->i_max = config->i_max;
    Pid_Instance->max_output = config->max_output;
    Pid_Instance->dead_zone = config->dead_zone;
    //微分先行，变速积分（未实现）
    Pid_Instance->is_enable = true;

    return Pid_Instance;
}
static void Pid_AngleProtect(PidInstance_s *Pid){
    float half_angle = Pid->angle_max / 2.0f;
    if (Pid->target[0] - Pid->measure[0] > half_angle) Pid->target[0] -= Pid->angle_max;
    else if (Pid->target[0] - Pid->measure[0] < -half_angle) Pid->target[0] += Pid->angle_max;
}
//速度环
void Pid_Speed(PidInstance_s *pid, float target, float measure){
    //检查pid是否正确使能
    if (pid == NULL || !pid->is_enable) return;
    //前置工作
    pid->target[0] = target;
    pid->measure[0] = measure;
    //过零保护
    //pid计算
    pid->error[0] = pid->target[0] - pid->measure[0];
    //判断死区
    //计算比例项
    pid->p_out = pid->kp * pid->error[0];
    //积分项(带限幅)
    pid->i_sum += pid->error[0];
    pid->i_out = constrainf(pid->ki * pid->i_sum, pid->i_max, pid->i_min);
    //微分项
    pid->d_out = pid->kd * (pid->error[0] - pid->error[1]);
    //输出（带限幅）
    pid->output = constrainf(pid->p_out + pid->i_out + pid->d_out, pid->max_output, pid->min_output);
    //善后工作
    pid->target[1] = pid->target[0];
    pid->measure[1] = pid->measure[0];
    pid->error[1] = pid->error[0];
}
//角度环
void Pid_Angle(PidInstance_s *pid, float target, float measure, float MaxSpeed){
    //检查pid是否正确使能
    if (pid == NULL || !pid->is_enable) return;
    //前置工作
    pid->target[0] = target;
    pid->measure[0] = measure;
    //输入角度限制
    constrainf(pid->target[0], pid->target_max, pid->target_min);
    //pid计算
    pid->error[0] = pid->target[0] - pid->measure[0];
    pid->i_sum += pid->error[0];
    constrainf(pid->i_sum, pid->i_max, pid->i_min);
    //比例项
    pid->p_out = pid->kp * pid->error[0];
    //积分项
    pid->i_out = pid->ki * pid->i_sum;
    //微分项
    pid->d_out = pid->kd * (pid->error[0] - pid->error[1]);
    //输出
    pid->output = pid->p_out + pid->i_out + pid->d_out;
    //最大速度限制
    if (pid->output > MaxSpeed) pid->output = MaxSpeed;
    else if (pid->output < -MaxSpeed) pid->output = -MaxSpeed;
    //善后工作
    pid->target[1] = pid->target[0];
    pid->measure[1] = pid->measure[0];
    pid->error[1] = pid->error[0];
}
//位置控制
float Pid_Calculate(PidInstance_s *angle_pid, PidInstance_s *speed_pid, float target_angle, float measure_angle, float measure_velocoty){
    if (angle_pid == NULL || speed_pid == NULL) return 0.0f;
    if (!angle_pid->is_enable || !speed_pid->is_enable) return 0.0f;

    //角度保护
    Pid_AngleProtect(angle_pid);
    //角度环计算，输出目标速度
    Pid_Angle(angle_pid, target_angle, measure_angle, speed_pid->max_output);
    //角度环输出作为速度环输入
    Pid_Speed(speed_pid, angle_pid->output, measure_velocoty);
    return speed_pid->output;
}