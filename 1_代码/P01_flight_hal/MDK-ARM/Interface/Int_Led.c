#include "Int_Led.h"


//led_on函数
void Int_led_on(Led_Struct *led)
{
    HAL_GPIO_WritePin(led->GPIOx, led->Pin, GPIO_PIN_RESET);
}

//led_off函数
void Int_led_off(Led_Struct *led)
{
    HAL_GPIO_WritePin(led->GPIOx, led->Pin, GPIO_PIN_SET);
}

//led_toggle函数
void Int_led_toggle(Led_Struct *led)
{
    HAL_GPIO_TogglePin(led->GPIOx, led->Pin);
}
