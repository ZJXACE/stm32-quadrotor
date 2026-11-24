#include "Int_Motor.h"


//设置每一个电机的转速
/**
 * @brief  传入的参数是比较值,最大为1000,默认值为200
 * @param speed 电机转速
 * @retval None
 * */
void Int_Motor_SetSpeed(Motor_Struct *motor)
{
    if(motor->speed>1000)
    {
        Debug_Printf("the speed too high\n");
        return;
    }
    //设置速度,用到设置占空比,函数:_HAL_TIM_set_compare
    __HAL_TIM_SET_COMPARE(motor->htim, motor->channel, motor->speed);//因为要用到句柄,通道,速度,所以.h文件里的结构体要存这三
    
}






//电机启动函数
void Int_Motor_Start(Motor_Struct *motor)
{
    //启动电机函数是:HAL_TIM_PWM_Start1
    HAL_TIM_PWM_Start(motor->htim, motor->channel);
    
}


