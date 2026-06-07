//
// Created by didib on 2026/4/30.
//
#include "Bsp_fdcan.h"

#include <sys/types.h>

static uint8_t idx1;
static CanInstance_s *fdcan1_instance[FDCAN_MAX_REGISTER_CNT];
static uint8_t idx2;
static CanInstance_s *fdcan2_instance[FDCAN_MAX_REGISTER_CNT];
static uint8_t idx3;
static CanInstance_s *fdcan3_instance[FDCAN_MAX_REGISTER_CNT];

//fdcan初始化标志
static bool fdcan_init_flag = false;
//接收帧
FDCAN_RxFrame_TypeDef *FDCAN_RxFIFO0Frame;
FDCAN_RxFrame_TypeDef *FDCAN_RxFIFO1Frame;
//过滤器配置
FDCAN_FilterTypeDef FDCAN_FIFO0_Filter = {
    .IdType = FDCAN_STANDARD_ID,
    .FilterIndex = 0, // 过滤器编号
    .FilterType = FDCAN_FILTER_MASK, // 过滤器 Mask 模式 关乎到 ID1ID2 的配置
    .FilterConfig = FDCAN_FILTER_TO_RXFIFO0,
    .FilterID1 = 0x00000000, // 过滤器 ID1，只要 ID2 配置为 0x00000000，就不会过滤任何 ID
    .FilterID2 = 0x00000000, // 过滤器 ID2
};
FDCAN_FilterTypeDef FDCAN_FIFO1_Filter = {
    .IdType = FDCAN_STANDARD_ID,
    .FilterIndex = 0, // 过滤器编号
    .FilterType = FDCAN_FILTER_MASK, // 过滤器 Mask 模式 关乎到 ID1ID2 的配置
    .FilterConfig = FDCAN_FILTER_TO_RXFIFO1,
    .FilterID1 = 0x00000000, // 过滤器 ID1，只要 ID2 配置为 0x00000000，就不会过滤任何 ID
    .FilterID2 = 0x00000000, // 过滤器 ID2
};
//私有函数
//初始化过滤器配置
static void FDCAN_Filter_Init(FDCAN_HandleTypeDef *fdcan_handle){
    HAL_FDCAN_ConfigFilter(fdcan_handle, &FDCAN_FIFO0_Filter);
    HAL_FDCAN_ConfigFilter(fdcan_handle, &FDCAN_FIFO1_Filter);
}
//初始化can服务
static void FDCAN_Service_Init(FDCAN_HandleTypeDef *fdcan_handle){
    // 拒绝接收匹配不成功的标准 ID 和扩展 ID, 不接受远程帧
    HAL_FDCAN_ConfigGlobalFilter(fdcan_handle, FDCAN_REJECT, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE);
    while (HAL_FDCAN_Start(fdcan_handle) != HAL_OK);
    HAL_FDCAN_ActivateNotification(fdcan_handle, FDCAN_IT_RX_FIFO0_NEW_MESSAGE | FDCAN_IT_RX_FIFO1_NEW_MESSAGE, 0);
}
static FDCAN_HandleTypeDef *Select_FDCAN_Handle(const uint8_t can_num){
    switch (can_num){
    case 1:
        return &hfdcan1;
    case 2:
        return &hfdcan2;
    case 3:
        return &hfdcan3;
    default:
        return NULL;
    }
}
//初始化can模块
static void Fdcan_Init(uint8_t can_num){
    if (!fdcan_init_flag){
        FDCAN_Filter_Init(Select_FDCAN_Handle(can_num));
        FDCAN_Service_Init(Select_FDCAN_Handle(can_num));
        fdcan_init_flag = true;
    }
}
//检查注册配置的有效性
static bool FDCAN_Register_Check(const CanInitConfig_s *config){
    if (config == NULL) return false;
    if (config->topic_name == NULL) return false;
    if (config->can_number == 0 || config->can_number > 3) return false;
    if (config->tx_id == 0 || config->rx_id == 0) return false;
    return true;
}
//对外函数
//注册一个can实例
CanInstance_s *Can_Register(CanInitConfig_s* config){
    if (!fdcan_init_flag) Fdcan_Init(config->can_number);
    if (!FDCAN_Register_Check(config)) return NULL;

    CanInstance_s *instance = malloc(sizeof(CanInstance_s));
    if (instance == NULL) return NULL;

    if (config->fdcan_mode != FDCAN_BIT_SWITCHING_MODE){
        config->fdcan_mode = CAN_STANDARD_MODE;
        config->DLC = FDCAN_DLC_BYTES_8;
    }

    memset(instance, 0, sizeof(CanInstance_s));
    instance->topic_name = config->topic_name;
    instance->tx_id = config->tx_id;

    instance->tx_conf.Identifier = config->tx_id;

    instance->tx_conf.IdType = FDCAN_STANDARD_ID;//标准id
    instance->tx_conf.TxFrameType = FDCAN_DATA_FRAME;
    instance->tx_conf.DataLength = config->DLC;
    instance->tx_conf.ErrorStateIndicator = FDCAN_ESI_ACTIVE;//这个啥用？
    instance->rx_id = config->rx_id;
    instance->parent_ptr = config->parent_ptr;
    instance->can_module_callback = config->can_module_callback;
    instance->can_handle = Select_FDCAN_Handle(config->can_number);
    if (config->fdcan_mode == FDCAN_BIT_SWITCHING_MODE){
        instance->tx_conf.BitRateSwitch = FDCAN_BRS_ON;
        instance->tx_conf.FDFormat = FDCAN_FD_CAN;
    }else{
        instance->tx_conf.BitRateSwitch = FDCAN_BRS_OFF;
        instance->tx_conf.FDFormat = FDCAN_CLASSIC_CAN;
    }

    if (instance->can_handle == &hfdcan1) fdcan1_instance[idx1++] = instance;
    if (instance->can_handle == &hfdcan2) fdcan2_instance[idx2++] = instance;
    if (instance->can_handle == &hfdcan3) fdcan3_instance[idx3++] = instance;

    return instance;
}
//发送数据（自定义发送内容）
bool Can_Transmit_External_Tx_Buff( CanInstance_s *instance, uint8_t *tx_buff){
    if (HAL_FDCAN_AddMessageToTxFifoQ(instance->can_handle, &instance->tx_conf, tx_buff) == HAL_OK){
        return true;
    }return false;
}
//发送函数，can实例中的内容
bool Can_Transmit(CanInstance_s *instance){
    if (HAL_FDCAN_AddMessageToTxFifoQ(instance->can_handle, &instance->tx_conf, instance->tx_buff) == HAL_OK){
        return true;
    }return false;
}
//解析实际字节数
static uint8_t FDCAN_GetRxLen(uint8_t DataLength) {
    switch (DataLength) {
    case FDCAN_DLC_BYTES_0: return 0;
    case FDCAN_DLC_BYTES_1: return 1;
    case FDCAN_DLC_BYTES_2: return 2;
    case FDCAN_DLC_BYTES_3: return 3;
    case FDCAN_DLC_BYTES_4: return 4;
    case FDCAN_DLC_BYTES_5: return 5;
    case FDCAN_DLC_BYTES_6: return 6;
    case FDCAN_DLC_BYTES_7: return 7;
    case FDCAN_DLC_BYTES_8: return 8;
    case FDCAN_DLC_BYTES_12: return 12;
    case FDCAN_DLC_BYTES_16: return 16;
    case FDCAN_DLC_BYTES_20: return 20;
    case FDCAN_DLC_BYTES_24: return 24;
    case FDCAN_DLC_BYTES_32: return 32;
    case FDCAN_DLC_BYTES_48: return 48;
    case FDCAN_DLC_BYTES_64: return 64;
    default: return 0;
    }
}
//FDCAN接收FIFO中断回调函数
static void FDCAN_RxFifoCallback(const FDCAN_RxFrame_TypeDef *FDCAN_RxFIFOxFrame, const uint8_t idx, CanInstance_s **instance){
    if (idx == 0) return;
    for (uint8_t i = 0; i < idx; i++){
        if (FDCAN_RxFIFOxFrame->Header.Identifier == instance[i]->rx_id){
            if (instance[i]->can_module_callback != NULL){
                instance[i]->rx_len = FDCAN_GetRxLen(FDCAN_RxFIFOxFrame->Header.DataLength);
                memcpy(instance[i]->rx_buff, FDCAN_RxFIFOxFrame->rx_buff,instance[i]->rx_len);
                instance[i]->cnt++;
                if (instance[i]->cnt >= 0xFFFF) instance[i]->cnt = 0;
                instance[i]->can_module_callback(instance[i]);
            }else break;
        }
    }
}
//中断回调函数
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs){
    if (RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE){
        HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &FDCAN_RxFIFO0Frame->Header, FDCAN_RxFIFO0Frame->rx_buff);

        if (hfdcan == &hfdcan1) FDCAN_RxFifoCallback(FDCAN_RxFIFO0Frame,idx1,fdcan1_instance);
        if (hfdcan == &hfdcan2) FDCAN_RxFifoCallback(FDCAN_RxFIFO0Frame,idx2,fdcan2_instance);
        if (hfdcan == &hfdcan3) FDCAN_RxFifoCallback(FDCAN_RxFIFO0Frame,idx3,fdcan3_instance);
    }
}