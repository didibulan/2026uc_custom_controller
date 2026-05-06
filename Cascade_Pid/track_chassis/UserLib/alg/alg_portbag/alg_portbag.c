//
// Created by didib on 2026/5/6.
//
#include "alg_portbag.h"
//符号函数
float signf(float x){
    if (x > 0) return 1.0f;
    else if (x < 0) return -1.0f;
    else return 0.0f;
}

//浮点限幅函数
float constrainf(float input, float max, float min){
    if (input < min) return min;
    else if (input > max) return max;
    else return input;
}

//循环限幅函数/用于角度归一化
float loop_float_constrain(float Input, float minValue, float maxValue){
    if (maxValue < minValue) return Input;
    if (Input > maxValue){
        float len = maxValue - minValue;
        while (Input > maxValue) Input -= len;
    }
    else if (Input < minValue){
        float len = maxValue - minValue;
        while (Input < minValue) Input += len;
    }
    return Input;
}