#include "APP_process_data.h"

 //定义微调偏移量
 int16_t key_pitch_offset;//俯仰角微调//往前正,
 int16_t key_roll_offset;//滚转微调 //往右正
 
//Joystick_struct joystick[4]={0};
Remote_Data remote={0};//状态结构体
/**
 * @brief 处理按键数据
 * 
 * @param None 
 * @retval None
 */
void App_Process_KeyData(void)//按键数据处理实际上就是让按键具有特殊含义,比如普通四个按键影响pitch和roll,left_x关机,right_x短按定高,长按校准摇杆
{
        KEY_type key = Int_Key_Get();    
       if(key==KEY_UP)
       {
        //向前飞微调,俯仰角
        key_pitch_offset+=10;
       }
       else if(key==KEY_DOWN)
       {
        //向后飞微调,俯仰角
        key_pitch_offset-=10;
       }
       else if(key==KEY_LEFT)
       {
        //向左飞调,滚转
        key_roll_offset-=10;
       }
       else if(key==KEY_RIGHT)
       {
        //向右飞调,滚转
        key_roll_offset+=10;
       }
       else if(key==KEY_LEFT_X)
       {
        //遥控关机,让状态结构体,关机结构体出来
        remote.Shutdown=1;
       }
       else if(key==KEY_RIGHT_X)
       {
        //实现定高
        remote.Fix_Height=1;
       }
       else if(key==KEY_RIGHT_X_LONG)
       {
        //实现校准摇杆,要等到摇杆写完之后才能完成______________________
       }
       
}
      

/**
 * @brief 处理摇杆数据
 * 
 * @param None 
 * @retval None
 */
void App_Process_JoystickData(void)
{
    //获取摇杆各个值,并打印
    //周期性地获取这个adc值
       // Init_Joystick_Get(joystick);

    //2.处理范围和极性
    //2.1处理范围从0到4095到0到1000  2.2处理极性,上推是1000,下推是0,用1000去减掉就可以了
   // joystick->thr =1000-(joystick->thr * 1000 / 4095);
    //joystick->yaw =1000-(joystick->yaw * 1000 / 4095);
   // joystick->pitch =1000-(joystick->pitch * 1000 / 4095);
   // joystick->roll =1000-(joystick->roll * 1000 / 4095);
    
   // Debug_Printf(":%d,%d,%d,%d\n", joystick->thr, joystick->yaw, joystick->pitch, joystick->roll);
}

