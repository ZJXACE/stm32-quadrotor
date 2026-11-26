#ifndef __INT_KEY_H__
#define __INT_KEY_H__

#include "main.h"
#include"FreeRTOS.h"
#include"task.h"

//创建枚举,去记录按键谁是谁
typedef enum
{
    KEY_NONE = 0,
    KEY_UP = 1,
    KEY_DOWN = 2,
    KEY_LEFT = 3,
    KEY_RIGHT = 4,

    //左边短按关机,右边短按定高,长按校准摇杆
    KEY_LEFT_X = 5,
    KEY_RIGHT_X = 6,
  //右边的长按功能
    KEY_RIGHT_X_LONG = 7,
  
}KEY_type;

/**
 * @brief 获取是哪个按键被按下的返回值
 * @param 无
 * @retval 按键的状态
 */
KEY_type Int_Key_Get(void);




#endif

