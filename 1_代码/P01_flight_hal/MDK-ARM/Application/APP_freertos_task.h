#ifndef __APP_FREERTOS_TASK_H__
#define __APP_FREERTOS_TASK_H__

#include "FreeRTOS.h"
#include "task.h"
#include "com_debug.h"
#include "Int_IP5305T.h"
#include "Int_Motor.h"
#include "Int_Led.h"
#include "Com_config.h"
#include "Int_SI24R1.h"
/**
 * @brief  启动freertos操作
 * @retval None
 */
void App_Freertos_Start(void);





#endif
