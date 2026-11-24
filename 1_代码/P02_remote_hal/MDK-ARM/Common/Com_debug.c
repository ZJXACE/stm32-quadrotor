/*
 * @Author: ZJXACE 284203290+ZJXACE@users.noreply.github.com
 * @Date: 2026-11-21 22:27:47
 * @Description: 调试打印模块：把 printf 重定向到 USART2
 *
 * Copyright (c) 2026 by ZJXACE, All Rights Reserved.
 */
#include "Com_debug.h"

/* 重写 fputc：printf 每输出一个字符，就通过 USART2 发出去 */
int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 1000);
    return ch;
}
