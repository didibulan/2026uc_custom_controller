/**
*   @file Bsp_AS5600.c
*/

#include "Bsp_AS5600.h"

// 静态保存I2C句柄，防止意外修改
static I2C_HandleTypeDef *s_AS5600_I2C;

/**
 * @brief  BSP初始化
 * @param
 * @retval None
 */
void AS5600_Init(AS5600_HandleTypDef* AS5600_Init,I2C_HandleTypeDef *hi2c,FDCAN_HandleTypeDef *hfdcan,uint32_t id)
{
    AS5600_Init->hfdcan = hfdcan;
    AS5600_Init->id = id;
    s_AS5600_I2C = hi2c;
}

/**
  * 函    数：AS5600读寄存器
  * 参    数：RegAddress 寄存器地址
  * 返 回 值：读取寄存器的数据
  */
static uint8_t AS5600_ReadReg(uint8_t RegAddress)
{
    uint8_t Data = 0 ;
    HAL_I2C_Mem_Read(s_AS5600_I2C,AS5600_ADDRESS<<1,RegAddress,I2C_MEMADD_SIZE_8BIT,&Data,1,100);

    return Data;
}

/**
  * 函    数：AS5600写寄存器
  * 参    数：RegAddress 寄存器地址
  * 参    数：Data 要写入寄存器的数据，范围：0x00~0xFF
  * 返 回 值：HAL状态，HAL_OK表示成功，其他表示失败
  */
static void AS5600_WriteReg(uint8_t RegAddress, uint8_t Data)
{
    HAL_I2C_Mem_Write(s_AS5600_I2C, AS5600_ADDRESS << 1, RegAddress, I2C_MEMADD_SIZE_8BIT, &Data, 1, 100);
}


/**
  * 函    数：AS5600获取ID号
  * 参    数：无
  * 返 回 值：AS5600的ID号
  */
static uint8_t AS5600_GetID(void)
{
    return AS5600_ReadReg(0x00);		//返回AS5600 ID寄存器的值
}


/**
  * 函    数：AS5600获取角度数据
  * 参    数：RawAngle 原始角度值(0~4095)
  * 参    数：Angle 实际角度值(0~360.0°)
  * 返 回 值：无
  */
static void AS5600_GetData(AS5600_Data *AS5600data)
{
    uint8_t DataH,DataL;

    //每次读取之前清除ErrorCode
    // if (s_AS5600_I2C->ErrorCode !=0 )//清除全部ErrorCode，慎用
    if (s_AS5600_I2C->ErrorCode==HAL_I2C_ERROR_TIMEOUT)
    {
        __HAL_I2C_CLEAR_FLAG(s_AS5600_I2C,I2C_FLAG_BERR);
        s_AS5600_I2C->ErrorCode = HAL_I2C_ERROR_NONE;
    }

    //接收并处理数据
    DataH = AS5600_ReadReg(AS5600_REG_RAW_ANGLE_H);         //读取角度高八位
    DataL = AS5600_ReadReg(AS5600_REG_RAW_ANGLE_L);         //读取角度低八位
    AS5600data->RawAngle = (uint16_t)DataH << 8 | DataL;    //原始数据

    //角度制换算
    AS5600data->Angle_deg = (float)AS5600data->RawAngle * AS5600_ANGLE_TO_DEGREE / AS5600_ANGLE_MAX_VAL;
    //弧度制换算
    AS5600data->Angle_rad = (float)AS5600data->RawAngle * 2 * PI / AS5600_ANGLE_MAX_VAL;

    //滤波处理
    AS5600_Filter(AS5600data->AS5600_FilterData,AS5600data->RawAngle,AS5600data->FinalOutput);

}

/**
  * 函    数：AS5600获取角度数据打包发送至can总线
  * 参    数：无
  * 返 回 值：无
  */
void AS5600_DataTransmit(AS5600_HandleTypDef *AS5600_Init,AS5600_Data *AS5600data)
{

    AS5600_GetData(AS5600data);

    uint8_t TxBuffer[8]={0};

    memcpy(TxBuffer,&AS5600data->FinalOutput,sizeof(float));

    FDCAN_Transmit(AS5600_Init->hfdcan,TxBuffer,AS5600_Init->id);
}