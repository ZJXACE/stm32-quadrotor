#include "APP_freertos_task.h"

//内存管理,c语言结构体通常保存在堆里,不会自动垃圾回收,所以我们始终使用一个结构体,不断循环使用
Motor_Struct LEFT_0_motor={.htim = &htim3,.channel = TIM_CHANNEL_1,.speed = 200};
Motor_Struct LEFT_1_motor={.htim = &htim4,.channel = TIM_CHANNEL_4,.speed = 200};
Motor_Struct RIGHT_0_motor={.htim = &htim2,.channel = TIM_CHANNEL_2,.speed = 200};
Motor_Struct RIGHT_1_motor={.htim = &htim1,.channel = TIM_CHANNEL_3,.speed = 200};



//电源管理系统参数
void Int_IP5305T_Task(void *pvParameters);
#define INT_IP5305T_STACK_SIZE 128
#define INT_IP5305T_PRIORITYIO 4
TaskHandle_t INT_IP5305T_hande;

//电机飞行参数
void Int_Motor_Task(void *pvParameters);
#define INT_MOTOR_STACK_SIZE 128
#define INT_MOTOR_PRIORITYIO 3
TaskHandle_t INT_MOTOR_hande; 



//定义任务的周期
#define POWER_task_period 10000
#define MOTOR_task_period 6

//----------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------
//启动freertos操作系统

void App_Freertos_Start(void)
{
    //创建电源管理系统任务
    xTaskCreate(Int_IP5305T_Task, "Int_IP5305T_Task", INT_IP5305T_STACK_SIZE, NULL, INT_IP5305T_PRIORITYIO, &INT_IP5305T_hande);

    //创建电机飞行任务
    xTaskCreate(Int_Motor_Task, "Int_Motor_Task", INT_MOTOR_STACK_SIZE, NULL, INT_MOTOR_PRIORITYIO, &INT_MOTOR_hande);

    // 启动任务调度器
    vTaskStartScheduler();
}

//电机启动任务
void Int_Motor_Task(void *pvParameters)
{
    portTickType lastWakeTime = xTaskGetTickCount();
    while (1)
    {
        //1.设置电机转速


        //2.启动电机
        //Int_Motor_Start(&RIGHT_1_motor);
        //Int_Motor_Start(&RIGHT_0_motor);

        vTaskDelayUntil(&lastWakeTime, MOTOR_task_period);
    }
}




//电源管理系统任务
void Int_IP5305T_Task(void *pvParameters)
{
    portTickType lastWakeTime = xTaskGetTickCount();
    while (1)
    {
       
        
        vTaskDelayUntil(&lastWakeTime, POWER_task_period);

        Int_IP5305T_Start();

     }    
}

