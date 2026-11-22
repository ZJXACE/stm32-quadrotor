#ifndef __COM_DEBUG_H__
#define __COM_DEBUG_H__

#include <stdio.h>
#include <string.h>
#include "main.h"
#include "usart.h"

/* ------------------------------------------------------------------
 * 为什么要有 DEBUG_ENABLE 这个开关？
 *
 * "Hello world!\r\n" 一共 13 个字节，串口 8N1 下 1 个字节约 10 bit，
 * 115200 波特率发 1 个字节约 87us，13 个字节约 1.1ms。
 * 在飞控主循环里每打印一次就卡住 1ms 以上，严重影响实时性，
 * 所以调试打印必须能一键关掉（改成 0 即可，编译器会把这些代码全部删掉）。
 * ------------------------------------------------------------------ */
#define DEBUG_ENABLE 1

#ifdef DEBUG_ENABLE

/* Keil 里 __FILE__ 是 "..\Core\Src\main.c" 这种完整路径，
 * 用 strrchr 从后往前找最后一个分隔符，只留文件名，打印更简短 */
#define FILE_NAME   (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define FILE_NAME_  (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : \
                     (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__))

/* 打印格式：[main.c:92] 你的内容 */
#define Debug_Printf(format, ...)  printf("[%s:%d]" format "\r\n", FILE_NAME_, __LINE__, ##__VA_ARGS__)

#else

#define Debug_Printf(format, ...)

#endif

#endif /* __COM_DEBUG_H__ */
