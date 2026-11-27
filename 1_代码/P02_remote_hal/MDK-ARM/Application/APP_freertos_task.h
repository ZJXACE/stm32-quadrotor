#ifndef __APP_FREERTOS_TASK_H__
#define __APP_FREERTOS_TASK_H__

#include "FreeRTOS.h"
#include "task.h"
#include "com_debug.h"
#include "Int_IP5305T.h"
#include "Int_SI24R1.h"
#include "APP_process_data.h"
#include "Int_Joystick.h"



/**
 * @brief  启动freertos操作
 * @retval None
 */
void App_Freertos_Start(void);





#endif
