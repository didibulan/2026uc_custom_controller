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

#endif //CASCADE_PID_ALG_PORTBAG_H