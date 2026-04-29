//
// Created by didib on 2026/4/29.
//

//PID控制器注册函数

#include "alg_Pid.h"
//限制函数
static float Pid_Limit(float input, float max, float min){
    if (input < min) return min;
    else if (input > max) return max;
    else return input;
}
//速度环
void Pid_Speed(alg_Pid_t *pid, float target, float measure){
    //检查pid是否正确使能
    //前置工作
    pid->target[0] = target;
    pid->measure[0] = measure;
    //过零保护
    //pid计算
    pid->error[0] = pid->target[0] - pid->measure[0];
    //判断死区
    //计算比例项
    pid->p_out = pid->kp * pid->error[0];
    //积分项(带积分限幅)
    pid->i_sum += pid->error[0];
    pid->i_out = Pid_Limit(pid->ki * pid->i_sum, pid->i_max, pid->i_min);
    //微分项
    pid->d_out = pid->kd * (pid->error[0] - pid->error[1]);
    //输出（带限幅）
    pid->output = Pid_Limit(pid->p_out + pid->i_out + pid->d_out, pid->max_output, pid->min_output);
    //善后工作
    pid->target[1] = pid->target[0];
    pid->measure[1] = pid->measure[0];
    pid->error[1] = pid->error[0];
}
//角度环
void Pid_Angle(){
    //检查pid是否正确使能
    //前置工作
    //pid计算
    //比例项
    //积分项
    //微分项
    //善后工作
}
//过零保护（写在电机功能实现里）