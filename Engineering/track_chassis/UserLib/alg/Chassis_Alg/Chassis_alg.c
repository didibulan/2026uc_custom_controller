//
// Created by didib on 2026/5/31.
//
#include "Chassis_alg.h"

#include "portable.h"
//检查初始化合法性
static bool Chassis_Judgement(const ChassisInitConfig_s *chassis_initconfig)
{
    // 全向轮/舵轮底盘必须设置旋转半径
    if ((chassis_initconfig->cartyp == Omni_Wheel || chassis_initconfig->cartyp
        == Steering_Wheel) && chassis_initconfig->omni_steering_message.chassis_radius == 0) return false;
    // 麦轮底盘必须设置几何尺寸
    if (chassis_initconfig->cartyp == Mecanum_Wheel &&
        (chassis_initconfig->mecanum_message.length_a == 0 || chassis_initconfig->mecanum_message.length_b == 0
            || chassis_initconfig->mecanum_message.radius_s ==0)) return false;
    return true;
}
//注册并初始化底盘实例
ChassisInstance_s *Chassis_Register(const ChassisInitConfig_s *Chassis_config)
{
    if (!Chassis_Judgement(Chassis_config)) return NULL;

    ChassisInstance_s *Chassis_Instance = (ChassisInstance_s *)pvPortMalloc(sizeof(ChassisInstance_s));
    if (Chassis_Instance == NULL) return NULL;
    memset(Chassis_Instance, 0, sizeof(ChassisInstance_s));

    Chassis_Instance->cartyp = Chassis_config->cartyp;
    Chassis_Instance->mecanum_message = Chassis_config->mecanum_message;
    Chassis_Instance->omni_steering_message = Chassis_config->omni_steering_message;
    Chassis_Instance->chassis_speed = Chassis_config->chassis_speed;
    Chassis_Instance->power_limitation = ;

    return Chassis_Instance;
}