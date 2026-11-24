#include "APP_freertos_task.h"

//创建电源管理任务,每10S拉低一次电平,使得其继续工程
void Power_Manage_Task(void *pvParameters);
#define POWER_MANAGE_STACK_SIZE 128
#define POWER_MANAGE_PRIORITYIO 4
TaskHandle_t power_manage_hande;



//启动freertos操作系统

void App_Freertos_Start(void)
{
    //创建电源管理任务
    xTaskCreate(Power_Manage_Task, "Power_Manage_Task", POWER_MANAGE_STACK_SIZE, NULL, POWER_MANAGE_PRIORITYIO, &power_manage_hande);
   
    // 启动任务调度器
    vTaskStartScheduler();
}


void Power_Manage_Task(void *pvParameters)
{
    //获取当前基准时间
    portTickType lastWakeTime = xTaskGetTickCount();
    while (1)
    {
       //每10S拉低一次电平,使得其继续工程
       //用taskdelayuntil来进行定时
       vTaskDelayUntil(&lastWakeTime, 10000);

       //执行电源管理任务
       Int_IP5305T_Start();
    }
}
