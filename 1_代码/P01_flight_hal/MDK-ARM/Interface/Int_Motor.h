#ifndef __INT_MOTOR_H__
#define __INT_MOTOR_H__

#include "main.h"
#include "tim.h"
#include "com_debug.h"

//电机结构体
typedef struct
{
    //存一下句柄
    TIM_HandleTypeDef *htim;
    //存一下通道
    uint16_t channel;
    //存一下速度
    uint16_t speed;
}Motor_Struct;



//设置每一个电机的转速
/**
 * @brief  传入的参数是比较值,最大为1000,默认值为200
 * @param speed 电机转速
 * @retval None
 * */
void Int_Motor_SetSpeed(Motor_Struct *motor);//因为我们需要知道是哪个电机,所以我们创建一个电机的结构体,传入结构体指针


//电机启动函数
void Int_Motor_Start(Motor_Struct *motor);

//电机关闭函数


#endif
