#ifndef __INT_LED_H__
#define __INT_LED_H__

#include "main.h"
/*
 *首要任务是,1.led_on  2.led_off  3.led_toggle
 */

 //对象有四个灯,创键一个结构体
 typedef struct
 {
    GPIO_TypeDef *GPIOx;
    uint16_t Pin;
 }Led_Struct;

 void Int_led_on(Led_Struct *led);
 void Int_led_off(Led_Struct *led);
 void Int_led_toggle(Led_Struct *led);


#endif

