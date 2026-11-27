#ifndef __INT_JOYSTICK_H__
#define __INT_JOYSTICK_H__

#include "main.h"
#include "adc.h"
//四个ad通道代表四个值
typedef struct
{
    int16_t thr;//油门
    int16_t yaw;//偏航角
    int16_t pitch;//俯仰角
    int16_t roll;//横滚角

}Joystick_struct;

/**
 * @brief 初始化ADC摇杆
 * 
 * @param None 
 * @retval None
 */
void Init_Joystick_Init(void);

/**
 * @brief 获取ADC数据,保存到结构体地址中
 * 
 * @param joystick 摇杆结构体指针
 * @retval None
 */
void Init_Joystick_Get(Joystick_struct *joystick);

#endif
