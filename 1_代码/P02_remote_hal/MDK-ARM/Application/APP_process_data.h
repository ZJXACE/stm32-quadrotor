#ifndef __APP_PROCESS_DATA_H__
#define __APP_PROCESS_DATA_H__

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"

#include "Int_Joystick.h"
#include"Int_Key.h"
#include"Com_debug.h"

typedef struct //发送给飞机的数据指令包or信号?
{
    //油门
    uint16_t thr;
    //偏航
    uint16_t yaw;
    //俯仰
    uint16_t pitch;
    //滚转
    uint16_t roll;
    //关机
    uint8_t Shutdown;//0:不关机,1:关机
    //定高
    uint16_t Fix_Height;//1定高
}Remote_Data;


extern int16_t key_pitch_offset;
extern int16_t key_roll_offset;
extern Remote_Data remote;


/**
 * @brief 处理按键数据
 * 
 * @param None 
 * @retval None
 */
void App_Process_KeyData(void);

/**
 * @brief 处理摇杆数据
 * 
 * @param None 
 * @retval None
 */
void App_Process_JoystickData(void);


#endif
