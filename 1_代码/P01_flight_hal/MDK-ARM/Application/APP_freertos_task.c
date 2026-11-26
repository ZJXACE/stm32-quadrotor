#include "APP_freertos_task.h"

//内存管理,c语言结构体通常保存在堆里,不会自动垃圾回收,所以我们始终使用一个结构体,不断循环使用
Motor_Struct LEFT_0_motor={.htim = &htim3,.channel = TIM_CHANNEL_1,.speed = 200};
Motor_Struct LEFT_1_motor={.htim = &htim4,.channel = TIM_CHANNEL_4,.speed = 200};
Motor_Struct RIGHT_0_motor={.htim = &htim2,.channel = TIM_CHANNEL_2,.speed = 200};
Motor_Struct RIGHT_1_motor={.htim = &htim1,.channel = TIM_CHANNEL_3,.speed = 200};

//led结构体实现四个led
Led_Struct LEFT_TOP_LED={.GPIOx = LED1_GPIO_Port,.Pin = LED1_Pin};//LED1 = PA12，原来写的 PA0 不是 LED
Led_Struct RIGHT_TOP_LED={.GPIOx = LED2_GPIO_Port,.Pin = LED2_Pin};//LED2 = PA11，原来写的 PA0 不是 LED
Led_Struct RIGHT_BOTTOM_LED={.GPIOx = LED3_GPIO_Port,.Pin = LED3_Pin};//LED3 = PB15，原来写的 PB0 不是 LED
Led_Struct LEFT_BOTTOM_LED={.GPIOx = LED4_GPIO_Port,.Pin = LED4_Pin};//LED4 = PB14，原来写的 PB0 不是 LED

//遥控器连接状态
RemoteControl_Status remote_control_status = REMOTE_CONTROL_CONNECTED;
//飞机飞行状态
Plane_Status plane_state = Error;
//----------------------------------------------------------------------------------------------------------------------------------------------------
//电源管理系统参数
void Int_IP5305T_Task(void *pvParameters);
#define INT_IP5305T_STACK_SIZE 128
#define INT_IP5305T_PRIORITYIO 4
TaskHandle_t INT_IP5305T_hande;
#define POWER_task_period 10000
//----------------------------------------------------------------------------------------------------------------------------------------------------
//电机飞行参数
void Int_Motor_Task(void *pvParameters);
#define INT_MOTOR_STACK_SIZE 128
#define INT_MOTOR_PRIORITYIO 3
TaskHandle_t INT_MOTOR_hande; 
#define MOTOR_task_period 6
//----------------------------------------------------------------------------------------------------------------------------------------------------
//led参数
void Int_Led_Task(void *pvParameters);
#define INT_LED_STACK_SIZE 128
#define INT_LED_PRIORITYIO 1//优先级给1
TaskHandle_t INT_LED_hande;
#define LED_task_period 100

//创建通讯任务
void Comm_Task(void *pvParameters);
#define COMM_STACK_SIZE 128
#define COMM_PRIORITYIO 2
TaskHandle_t comm_hande;
//任务周期
#define COMM_PERIOD 6




//----------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------
//启动 freertos 操作系统

void App_Freertos_Start(void)
{
    //创建电源管理系统任务
    xTaskCreate(Int_IP5305T_Task, "Int_IP5305T_Task", INT_IP5305T_STACK_SIZE, NULL, INT_IP5305T_PRIORITYIO, &INT_IP5305T_hande);

    //创建电机飞行任务
    xTaskCreate(Int_Motor_Task, "Int_Motor_Task", INT_MOTOR_STACK_SIZE, NULL, INT_MOTOR_PRIORITYIO, &INT_MOTOR_hande);

    //创建led任务
    xTaskCreate(Int_Led_Task, "Int_Led_Task", INT_LED_STACK_SIZE, NULL, INT_LED_PRIORITYIO, &INT_LED_hande);

    //创建通讯任务
    xTaskCreate(Comm_Task, "Comm_Task", COMM_STACK_SIZE, NULL, COMM_PRIORITYIO, &comm_hande);

    //启动任务调度器
    vTaskStartScheduler();
}
//通信任务
//创建缓存区
uint8_t Com_buff[TX_PLOAD_WIDTH]={0};


void Comm_Task(void *pvParameters)
{
    //获取当前基准时间
    portTickType lastWakeTime = xTaskGetTickCount();
    while (1)
    {
       
      //1.接收数据到缓存区
     uint8_t res =  Int_SI24R1_RxPacket(Com_buff);

     static uint16_t cnt = 0;
     if (++cnt % 166 == 0) Debug_Printf("waiting...\r\n");   //约每秒一次
     if(res == 0)
     {
        //接收成功
        Debug_Printf("RX: %c%c%c%c%c%c\r\n", Com_buff[0], Com_buff[1], Com_buff[2], Com_buff[3], Com_buff[4], Com_buff[5]);
     }






        //发送完成后恢复到 RX 模式
        // Int_SI24R1_RX_Mode();   //与老师一致：接收模式只在 Init 配一次，循环里不再重配

        //获取基准时间延迟
        vTaskDelayUntil(&lastWakeTime, COMM_PERIOD);
       
        
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
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
//----------------------------------------------------------------------------------------------------------------------------------------------------
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
//----------------------------------------------------------------------------------------------------------------------------------------------------
//led任务
void Int_Led_Task(void *pvParameters)
{
    uint8_t Count = 0;
    portTickType lastWakeTime = xTaskGetTickCount();
    while (1)
    {
        Count++;
        //前面两个灯
       //遥控器连接则前面两个灯常亮
       if(remote_control_status == REMOTE_CONTROL_CONNECTED)
       {
        Int_led_on(&LEFT_TOP_LED);
        Int_led_on(&RIGHT_TOP_LED);
       }
       else//否则前面两个灯熄灭
       {
        Int_led_off(&LEFT_TOP_LED);
        Int_led_off(&RIGHT_TOP_LED);
       }

       //后面两个灯
       //飞机飞行状态为空闲状态
       if(plane_state == IDLE)
       {

        //后面两个灯慢闪，500ms 切换一次
        if(Count % 5 == 0)
        {
            Int_led_toggle(&LEFT_BOTTOM_LED);
            Int_led_toggle(&RIGHT_BOTTOM_LED);
        }
       }
       else if(plane_state == FLIGHT)
       {
        //后面两个灯快闪，200ms 切换一次
        if(Count % 2 == 0)
        {
            Int_led_toggle(&LEFT_BOTTOM_LED);
            Int_led_toggle(&RIGHT_BOTTOM_LED);
        }
       }
       else if(plane_state == Fix_Height)
       {
        //后面两个灯常亮
        Int_led_on(&LEFT_BOTTOM_LED);
        Int_led_on(&RIGHT_BOTTOM_LED);
       }
       else if(plane_state == Error)
       {
        //后面两个灯熄灭
        Int_led_off(&LEFT_BOTTOM_LED);
        Int_led_off(&RIGHT_BOTTOM_LED);
       }
       if(Count==10)
       {
        Count = 0;
       }

        vTaskDelayUntil(&lastWakeTime, LED_task_period);
    }
}
