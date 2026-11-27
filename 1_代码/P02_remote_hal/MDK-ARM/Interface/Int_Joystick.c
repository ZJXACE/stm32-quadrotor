#include "Int_Joystick.h"

//四个通道的数组
uint16_t adc_buff[4];

/**
 * @brief 初始化ADC摇杆
 * 
 * @param None 
 * @retval None
 */
void Init_Joystick_Init(void)
{
    //开启hal库中adc函数
    HAL_ADC_Start_DMA(&hadc1,(uint32_t *)adc_buff,4);

}


/**
 * @brief 获取ADC数据,保存到结构体地址中
 * 
 * @param joystick 摇杆结构体指针
 * @retval None
 */
void Init_Joystick_Get(Joystick_struct *joystick)
{
    //cpu不参与,直接从数组中取数据,对应通道取的数据是自己在cubdemx设置通道里时候设置的,记住顺序写的什么
    joystick->thr = adc_buff[0];
    joystick->yaw = adc_buff[1];
    joystick->pitch = adc_buff[2];
    joystick->roll = adc_buff[3];
}
