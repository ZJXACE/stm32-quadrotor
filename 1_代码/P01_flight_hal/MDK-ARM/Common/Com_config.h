#ifndef __COM_CONFIG_H__
#define __COM_CONFIG_H__

//遥控器连接状态
typedef enum
{
    REMOTE_CONTROL_CONNECTED = 0,
    REMOTE_CONTROL_DISCONNECTED = 1,
}RemoteControl_Status;

//飞机飞行状态
typedef enum
{
    IDLE = 0,//空闲状态
    FLIGHT = 1,//飞行状态
    Fix_Height = 2,//固定高度状态
    Error = 3,//错误状态
}Plane_Status;



#endif

