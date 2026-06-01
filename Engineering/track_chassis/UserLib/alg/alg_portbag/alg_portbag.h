//
// Created by didib on 2026/5/6.
//

#ifndef CASCADE_PID_ALG_PORTBAG_H
#define CASCADE_PID_ALG_PORTBAG_H

#define PI               3.14159265358979f
#define EPS              1e-3f

//角度归一化PI~-PI
#define rad_format(Ang) loop_float_constrain((Ang), -PI, PI)

//符号函数
float signf(float x);
//浮点限幅函数
float constrainf(float input, float max, float min);
//循环限幅函数/用于角度归一化
float loop_float_constrain(float Input, float minValue, float maxValue);
//浮点转化为无符号整型
int float_to_uint(const float x_float, const float x_min, const float x_max, const int bits);
//无符号整型转化为浮点数
float uint_to_float(const int x_int, const float x_min, const float x_max, const int bits);

float  Angle_Normalize (float angle);

#endif //CASCADE_PID_ALG_PORTBAG_H