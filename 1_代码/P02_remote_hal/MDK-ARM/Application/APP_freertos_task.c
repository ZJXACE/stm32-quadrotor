#include "APP_freertos_task.h"



//创建电源管理任务，每 10S 拉低一次电平，使得其继续工作
void Power_Manage_Task(void *pvParameters);
#define POWER_MANAGE_STACK_SIZE 128
#define POWER_MANAGE_PRIORITYIO 4
TaskHandle_t power_manage_hande;
#define POWER_MANAGE_PERIOD 10000
//创建通讯任务
void Comm_Task(void *pvParameters);
#define COMM_STACK_SIZE 128
#define COMM_PRIORITYIO 3
TaskHandle_t comm_hande;
#define COMM_PERIOD 6
//创建按键任务
void Key_Task(void *pvParameters);
#define KEY_STACK_SIZE 128
#define KEY_PRIORITYIO 2
TaskHandle_t key_hande;
#define KEY_PERIOD 20
//创建摇杆任务
void Joystick_Task(void *pvParameters);
#define JOYSTICK_STACK_SIZE 128
#define JOYSTICK_PRIORITYIO 2//和按键任务铜同级别
TaskHandle_t joystick_hande;
#define JOYSTICK_PERIOD 20



//启动freertos操作系统

void App_Freertos_Start(void)
{
    //创建电源管理任务
    xTaskCreate(Power_Manage_Task, "Power_Manage_Task", POWER_MANAGE_STACK_SIZE, NULL, POWER_MANAGE_PRIORITYIO, &power_manage_hande);
   
    //创建通讯任务
    xTaskCreate(Comm_Task, "Comm_Task", COMM_STACK_SIZE, NULL, COMM_PRIORITYIO, &comm_hande);

    //创建按键任务
    xTaskCreate(Key_Task, "Key_Task", KEY_STACK_SIZE, NULL, KEY_PRIORITYIO, &key_hande);

    //创建摇杆任务
    xTaskCreate(Joystick_Task, "Joystick_Task", JOYSTICK_STACK_SIZE, NULL, JOYSTICK_PRIORITYIO, &joystick_hande);

    //启动任务调度器
    vTaskStartScheduler();
}

//摇杆任务
void Joystick_Task(void *pvParameters)
{
    portTickType lastWakeTime = xTaskGetTickCount();
    //初始化摇杆adc
    Init_Joystick_Init();//开启ADC到dma,只需要一次,所以在循环外面部分
    while(1)
    {
        //获取摇杆值,并打印
        App_Process_JoystickData();

        vTaskDelayUntil(&lastWakeTime, JOYSTICK_PERIOD);
    }
}



//按键任务
void Key_Task(void *pvParameters)
{
    portTickType lastWakeTime = xTaskGetTickCount();
    while(1)
    {
       //APP_process_keydata();
       App_Process_KeyData();
      
        

        vTaskDelayUntil(&lastWakeTime, KEY_PERIOD);
}
    }

  



uint8_t Com_buff[TX_PLOAD_WIDTH] = {0};
//通讯任务
void Comm_Task(void *pvParameters)
{
    //获取当前基准时间
    portTickType lastWakeTime = xTaskGetTickCount();
    while (1)
    {
        //1.要发送之前要先进入发送模式
        Int_SI24R1_TX_Mode();
        //2.发送数据
       
        Com_buff[0] = 'h';
        Com_buff[1] = 'i';
        Com_buff[2] = 'l';
        Com_buff[3] = 'o';
        Com_buff[4] = 'w';
        Com_buff[5] = 'r';

        uint8_t r = Int_SI24R1_TxPacket(Com_buff);
       

        //发送完成后恢复到 RX 模式
        Int_SI24R1_RX_Mode();

        //获取基准时间延迟
        vTaskDelayUntil(&lastWakeTime, COMM_PERIOD);
       
        
    }
}



void Power_Manage_Task(void *pvParameters)
{
    //获取当前基准时间
    portTickType lastWakeTime = xTaskGetTickCount();
    while (1)
    {
       //每 10S 拉低一次电平，使得其继续工作
       //用 taskdelayuntil 来进行定时
       vTaskDelayUntil(&lastWakeTime, POWER_MANAGE_PERIOD);

       //执行电源管理任务
       Int_IP5305T_Start();
    }
}
