#include "Int_Key.h"

/**
 * @brief 获取是哪个按键被按下的返回值
 * @param 无
 * @retval 按键的状态
 */
KEY_type Int_Key_Get(void)
{
    //先对上下左右四个小按键进行判断(上方两个按键,需要判断长按还是短按的逻辑)
    if(HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin) == GPIO_PIN_RESET)
    {
        //1.电弧按键消抖,但是注意,这个按键任务是rtos里面的,所以要用taskdelay,而不是,hal_delay
        vTaskDelay(5);
        if(HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin) == GPIO_PIN_RESET)
        {
            //2.由于比如我按键检测每次10ms,而人体按下大概至少100ms以上,
            //所以很容易一次按下动作返回多个值,所以需要每次抬起才计数一次
            while(HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin) == GPIO_PIN_RESET);//3.如果一直按下,则一直等待
            //返回按键值
            return KEY_UP;
        }
    }
    else if(HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port, KEY_DOWN_Pin) == GPIO_PIN_RESET)
    {
        //1.电弧按键消抖,但是注意,这个按键任务是rtos里面的,所以要用taskdelay,而不是,hal_delay
        vTaskDelay(5);
        if(HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port, KEY_DOWN_Pin) == GPIO_PIN_RESET)
        {
            while(HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port, KEY_DOWN_Pin) == GPIO_PIN_RESET);//3.如果一直按下,则一直等待
            //返回按键值
            return KEY_DOWN;
        }
    }
    else if(HAL_GPIO_ReadPin(KEY_LEFT_GPIO_Port, KEY_LEFT_Pin) == GPIO_PIN_RESET)
    {
        //1.电弧按键消抖,但是注意,这个按键任务是rtos里面的,所以要用taskdelay,而不是,hal_delay
        vTaskDelay(5);
        if(HAL_GPIO_ReadPin(KEY_LEFT_GPIO_Port, KEY_LEFT_Pin) == GPIO_PIN_RESET)
        {
            while(HAL_GPIO_ReadPin(KEY_LEFT_GPIO_Port, KEY_LEFT_Pin) == GPIO_PIN_RESET);//3.如果一直按下,则一直等待
            //返回按键值
            return KEY_LEFT;
        }
    }
    else if(HAL_GPIO_ReadPin(KEY_RIGHT_GPIO_Port, KEY_RIGHT_Pin) == GPIO_PIN_RESET)
    {
        //1.电弧按键消抖,但是注意,这个按键任务是rtos里面的,所以要用taskdelay,而不是,hal_delay
        vTaskDelay(5);
        if(HAL_GPIO_ReadPin(KEY_RIGHT_GPIO_Port, KEY_RIGHT_Pin) == GPIO_PIN_RESET)
        {
            while(HAL_GPIO_ReadPin(KEY_RIGHT_GPIO_Port, KEY_RIGHT_Pin) == GPIO_PIN_RESET);//3.如果一直按下,则一直等待
            //返回按键值
            return KEY_RIGHT;
        }
    }
//左上按键
    else if(HAL_GPIO_ReadPin(KEY_LEFT_X_GPIO_Port, KEY_LEFT_X_Pin) == GPIO_PIN_RESET)
    {
        //1.电弧按键消抖,但是注意,这个按键任务是rtos里面的,所以要用taskdelay,而不是,hal_delay
        vTaskDelay(5);
        if(HAL_GPIO_ReadPin(KEY_LEFT_X_GPIO_Port, KEY_LEFT_X_Pin) == GPIO_PIN_RESET)
        {
            while(HAL_GPIO_ReadPin(KEY_LEFT_X_GPIO_Port, KEY_LEFT_X_Pin) == GPIO_PIN_RESET);//3.如果一直按下,则一直等待
            //返回按键值
            return KEY_LEFT_X;
        }
    }
//右上按键
    else if(HAL_GPIO_ReadPin(KEY_RIGHT_X_GPIO_Port, KEY_RIGHT_X_Pin) == GPIO_PIN_RESET)
    {
       //从此时开始计时,如果超过1S,则认为是长按
       TickType_t Count1=xTaskGetTickCount();
       //防止电弧抖动
       vTaskDelay(5);
       if(HAL_GPIO_ReadPin(KEY_RIGHT_X_GPIO_Port, KEY_RIGHT_X_Pin) == GPIO_PIN_RESET)
       {
        while(HAL_GPIO_ReadPin(KEY_RIGHT_X_GPIO_Port, KEY_RIGHT_X_Pin) == GPIO_PIN_RESET)//3.如果一直按下,则一直等待
        {
            vTaskDelay(1);
        }
        TickType_t Count2=xTaskGetTickCount();
        if(Count2-Count1>1000)
        {
            //认作长按按键
            return KEY_RIGHT_X_LONG;
        }
        else
        {
            //认作短按按键
            return KEY_RIGHT_X;
        }

       }
    }

    return KEY_NONE;
}






