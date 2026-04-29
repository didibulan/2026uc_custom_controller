//
// Created by didib on 2026/4/29.
//

#ifndef CASCADE_PID_ALG_PID_H
#define CASCADE_PID_ALG_PID_H

typedef struct {
    float target[2];        // 两次目标值
    float kp;               // 比例增益
    float ki;               // 积分增益
    float kd;               // 微分增益
    float p_out;            // 比例输出
    float i_out;            // 积分输出
    float d_out;            // 微分输出
    float max_output;       // 输出限幅（max）
    float min_output;       // 输出限幅（min）
    float i_max;            // 积分限幅（max）
    float i_min;            // 积分限幅（min）
    float i_sum;            // 积分
    float d_max;            // 微分限幅（max）
    float d_min;            // 微分限幅（min）
    float integral;         // 积分
    float derivative;       // 微分
    float measure[2];       // 两次实际值
    float error[2];         // 两次误差
    float output;           // 输出

    float dead_zone;         // 死区
    //增加：微分先行
} alg_Pid_t;



#endif //CASCADE_PID_ALG_PID_H