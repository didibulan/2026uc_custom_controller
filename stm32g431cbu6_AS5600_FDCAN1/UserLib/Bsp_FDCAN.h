/**
*   @file Bsp_FDCAN.h
*/

#ifndef BSP_FDCAN_H
#define BSP_FDCAN_H

#include <stdint.h>
#include "fdcan.h"

typedef struct
{
    FDCAN_HandleTypeDef *hcan;
    FDCAN_TxHeaderTypeDef Header;
    uint8_t *Data;
}FDCAN_TxFrame_s;

typedef struct
{
    FDCAN_HandleTypeDef *hcan;
    FDCAN_RxHeaderTypeDef Header;
    uint8_t Data[8];
}FDCAN_RxFrame_s;

void FDCAN_Init(void);
void FDCAN_Transmit(FDCAN_HandleTypeDef *hfdcan,uint8_t *data,uint32_t id);

#endif //BSP_FDCAN_H