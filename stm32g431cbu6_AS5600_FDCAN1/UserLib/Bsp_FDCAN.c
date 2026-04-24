/**
*   @file Bsp_FDCAN.c
*/

#include "Bsp_FDCAN.h"


//stm32g431cbu6开了一路fdcan
FDCAN_RxFrame_s FDCAN_RxFrame = {
    .hcan = &hfdcan1
};

void FDCAN_Init()
{
    FDCAN_FilterTypeDef filter_config1;

    filter_config1.IdType = FDCAN_STANDARD_ID;
    filter_config1.FilterIndex = 0;
    filter_config1.FilterType = FDCAN_FILTER_MASK;
    filter_config1.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    filter_config1.FilterID1 = 0x00000000;
    filter_config1.FilterID2 = 0x00000000;

    //按照特定配置接收帧
    HAL_FDCAN_ConfigFilter(&hfdcan1, &filter_config1);

    //拒绝未配置帧
    HAL_FDCAN_ConfigGlobalFilter(&hfdcan1,FDCAN_ACCEPT_IN_RX_FIFO0, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE);

    HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);

    HAL_FDCAN_Start(&hfdcan1);
}

void FDCAN_Transmit(FDCAN_HandleTypeDef *hfdcan,uint8_t *data,uint32_t id)
{
    FDCAN_TxFrame_s frame = {
        .hcan = hfdcan,
        .Header.IdType = FDCAN_STANDARD_ID,
        .Header.TxFrameType = FDCAN_DATA_FRAME,
        .Header.DataLength = 8,
        .Header.ErrorStateIndicator = FDCAN_ESI_ACTIVE,
        .Header.BitRateSwitch = FDCAN_BRS_OFF,
        .Header.TxEventFifoControl = FDCAN_NO_TX_EVENTS,
        .Header.MessageMarker = 0,
        .Header.Identifier = id,
        .Data = data,
    };

    HAL_FDCAN_AddMessageToTxFifoQ(frame.hcan,&frame.Header,frame.Data);
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
    HAL_FDCAN_GetRxMessage(hfdcan,FDCAN_RX_FIFO0, &FDCAN_RxFrame.Header, FDCAN_RxFrame.Data);
}