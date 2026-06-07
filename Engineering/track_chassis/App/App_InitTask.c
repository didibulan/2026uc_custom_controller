//
// Created by didib on 2026/4/30.
//
#include "App_InitTask.h"
#include "dm_motor.h"
#include "cmsis_os.h"

DmMotorInstance_s *motors_chassis[4] = {};
DmMotorInstance_s *motor_chassis0 = NULL;
DmMotorInstance_s *motor_chassis1 = NULL;
DmMotorInstance_s *motor_chassis2 = NULL;
DmMotorInstance_s *motor_chassis3 = NULL;

bool istrackInitialized = false;

DmMotorInitConfig_s  motor_chassis0_init = {
    .topic_name = "motor_chassis0",
    .type = J4310,
    .parameters = {
        .pos_max = 0.0f,
        .vel_max = 0.0f,
        .tor_max = 0.0f,
        .kp_max = 0.0f,
        .kd_max = 0.0f,
        .kp_int = 0.0f,
        .kd_int = 0.0f,
    },
    .control_mode = DM_POSITION,
    .can_config = {
        .fdcan_mode = FDCAN_BIT_SWITCHING_MODE,
        // .fdcan_mode = CAN_STANDARD_MODE,
        .DLC = FDCAN_DLC_BYTES_8,
        .can_number = 1,
        .tx_id = 0x09,
        .rx_id = 0x19,
    },
    .angle_pid_config = {
        .target_max = 0.0f,
        .target_min = 0.0f,
        .kp = 0.0f,
        .ki = 0.0f,
        .kd = 0.0f,
        .max_output = 0.0f,
        .min_output = 0.0f,
        .i_max = 0.0f,
        .i_min = 0.0f,
        .d_max = 0.0f,
        .d_min = 0.0f,
        .angle_max = 0.0f,
        .dead_zone = 0.0f,
    },
    .velocity_pid_config = {
        .target_max = 0.0f,
        .target_min = 0.0f,
        .kp = 0.0f,
        .ki = 0.0f,
        .kd = 0.0f,
        .max_output = 0.0f,
        .min_output = 0.0f,
        .i_max = 0.0f,
        .i_min = 0.0f,
        .d_max = 0.0f,
        .d_min = 0.0f,
        .angle_max = 0.0f,
        .dead_zone = 0.0f,
    }
};

DmMotorInitConfig_s  motor_chassis1_init = {
    .topic_name = "motor_chassis1",
    .type = J4310,
    .parameters = {
        .pos_max = 0.0f,
        .vel_max = 0.0f,
        .tor_max = 0.0f,
        .kp_max = 0.0f,
        .kd_max = 0.0f,
        .kp_int = 0.0f,
        .kd_int = 0.0f,
    },
    .control_mode = DM_POSITION,
    .can_config = {
        // .fdcan_mode = FDCAN_BIT_SWITCHING_MODE,
        .fdcan_mode = CAN_STANDARD_MODE,
        .DLC = FDCAN_DLC_BYTES_8,
        .can_number = 1,
        .tx_id = 0x07,
        .rx_id = 0x17,
    },
    .angle_pid_config = {
        .target_max = 0.0f,
        .target_min = 0.0f,
        .kp = 0.0f,
        .ki = 0.0f,
        .kd = 0.0f,
        .max_output = 0.0f,
        .min_output = 0.0f,
        .i_max = 0.0f,
        .i_min = 0.0f,
        .d_max = 0.0f,
        .d_min = 0.0f,
        .angle_max = 0.0f,
        .dead_zone = 0.0f,
    },
    .velocity_pid_config = {
        .target_max = 0.0f,
        .target_min = 0.0f,
        .kp = 0.0f,
        .ki = 0.0f,
        .kd = 0.0f,
        .max_output = 0.0f,
        .min_output = 0.0f,
        .i_max = 0.0f,
        .i_min = 0.0f,
        .d_max = 0.0f,
        .d_min = 0.0f,
        .angle_max = 0.0f,
        .dead_zone = 0.0f,
    }
};

DmMotorInitConfig_s  motor_chassis2_init = {
    .topic_name = "motor_chassis2",
    .type = J4310,
    .parameters = {
        .pos_max = 0.0f,
        .vel_max = 0.0f,
        .tor_max = 0.0f,
        .kp_max = 0.0f,
        .kd_max = 0.0f,
        .kp_int = 0.0f,
        .kd_int = 0.0f,
    },
    .control_mode = DM_POSITION,
    .can_config = {
        .fdcan_mode = FDCAN_BIT_SWITCHING_MODE,
        .DLC = FDCAN_DLC_BYTES_8,
        .can_number = 1,
        .tx_id = 0x06,
        .rx_id = 0x16,
    },
    .angle_pid_config = {
        .target_max = 0.0f,
        .target_min = 0.0f,
        .kp = 0.0f,
        .ki = 0.0f,
        .kd = 0.0f,
        .max_output = 0.0f,
        .min_output = 0.0f,
        .i_max = 0.0f,
        .i_min = 0.0f,
        .d_max = 0.0f,
        .d_min = 0.0f,
        .angle_max = 0.0f,
        .dead_zone = 0.0f,
    },
    .velocity_pid_config = {
        .target_max = 0.0f,
        .target_min = 0.0f,
        .kp = 0.0f,
        .ki = 0.0f,
        .kd = 0.0f,
        .max_output = 0.0f,
        .min_output = 0.0f,
        .i_max = 0.0f,
        .i_min = 0.0f,
        .d_max = 0.0f,
        .d_min = 0.0f,
        .angle_max = 0.0f,
        .dead_zone = 0.0f,
    }
};

DmMotorInitConfig_s  motor_chassis3_init = {
    .topic_name = "motor_chassis3",
    .type = J4310,
    .parameters = {
        .pos_max = 12.5f,
        .vel_max = 30.0f,
        .tor_max = 10.0f,
        .kp_max = 500.0f,
        .kd_max = 5.0f,
        .kp_int = 0.0f,
        .kd_int = 0.0f,
    },
    .control_mode = DM_POSITION,
    .can_config = {
        .fdcan_mode = FDCAN_BIT_SWITCHING_MODE,
        // .fdcan_mode = CAN_STANDARD_MODE,
        .DLC = FDCAN_DLC_BYTES_8,
        .can_number = 1,
        .tx_id = 0x01,
        .rx_id = 0x11,
    },
    .angle_pid_config = {
        .target_max = 0.0f,
        .target_min = 0.0f,
        .kp = 6.8f,
        .ki = 0.0f,
        .kd = 0.0f,
        .max_output = 0.0f,
        .min_output = 0.0f,
        .i_max = 0.0f,
        .i_min = 0.0f,
        .d_max = 0.0f,
        .d_min = 0.0f,
        .angle_max = 0.0f,
        .dead_zone = 0.0f,
    },
    .velocity_pid_config = {
        .target_max = 3.7f,
        .target_min = 0.15f,
        .kp = 0.0f,
        .ki = 0.0f,
        .kd = 0.0f,
        .max_output = 0.0f,
        .min_output = 0.0f,
        .i_max = 0.15f,
        .i_min = 0.0f,
        .d_max = 0.0f,
        .d_min = 0.0f,
        .angle_max = 0.0f,
        .dead_zone = 0.0f,
    }
};
void App_Init(void const* argument){
    while (motor_chassis0 == NULL) motor_chassis0 = Motor_DM_Register(&motor_chassis0_init);
    while (motor_chassis1 == NULL) motor_chassis1 = Motor_DM_Register(&motor_chassis1_init);
    while (motor_chassis2 == NULL) motor_chassis2 = Motor_DM_Register(&motor_chassis2_init);
    while (motor_chassis3 == NULL) motor_chassis3 = Motor_DM_Register(&motor_chassis3_init);

    motors_chassis[0] = motor_chassis0;
    motors_chassis[1] = motor_chassis1;
    motors_chassis[2] = motor_chassis2;
    motors_chassis[3] = motor_chassis3;

    istrackInitialized = true;
    //删除初始化任务
    vTaskDelete(NULL);
}
