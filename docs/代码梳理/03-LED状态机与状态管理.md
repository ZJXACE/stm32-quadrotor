# 03 · LED 状态机与状态管理

> 讲"四盏灯怎么表达状态"的代码实现。设计思路见 [学习记录 2026-11-25](../学习记录/2026-11-25.md)。

## 一、涉及的文件

| 文件 | 职责 |
| --- | --- |
| `Core/Inc/main.h` | CubeMX 生成的 LED 引脚别名（`LED1_Pin` / `LED1_GPIO_Port` …） |
| `Core/Src/gpio.c` | 四个 LED 引脚初始化（推挽输出、初始高电平 = 熄灭） |
| `MDK-ARM/Common/Com_config.h` | 状态枚举：`RemoteControl_Status`、`Plane_Status` |
| `MDK-ARM/Interface/Int_Led.c/.h` | 灯的驱动：`Led_Struct` + `on/off/toggle` |
| `MDK-ARM/Application/APP_freertos_task.c` | 灯对象定义、全局状态变量、`Int_Led_Task` 状态机 |

## 二、硬件映射

| 逻辑名 | 引脚 | 位置 | 点亮电平 |
| --- | --- | --- | --- |
| `LEFT_TOP_LED`（LED1） | PA12 | 左上 | 低电平 |
| `RIGHT_TOP_LED`（LED2） | PA11 | 右上 | 低电平 |
| `RIGHT_BOTTOM_LED`（LED3） | PB15 | 右下 | 低电平 |
| `LEFT_BOTTOM_LED`（LED4） | PB14 | 左下 | 低电平 |

> GPIO 初始化时全部写高电平（熄灭），所以 `Int_led_on()` 里是 `GPIO_PIN_RESET`。

## 三、数据结构

```c
// 一盏灯 = 端口 + 引脚
typedef struct { GPIO_TypeDef *GPIOx; uint16_t Pin; } Led_Struct;

// 两套状态
typedef enum { REMOTE_CONTROL_CONNECTED, REMOTE_CONTROL_DISCONNECTED } RemoteControl_Status;
typedef enum { IDLE, FLIGHT, Fix_Height, Error } Plane_Status;
```

四个灯对象在 `APP_freertos_task.c` 里定义：

```c
Led_Struct LEFT_TOP_LED     = { .GPIOx = LED1_GPIO_Port, .Pin = LED1_Pin };
Led_Struct RIGHT_TOP_LED    = { .GPIOx = LED2_GPIO_Port, .Pin = LED2_Pin };
Led_Struct RIGHT_BOTTOM_LED = { .GPIOx = LED3_GPIO_Port, .Pin = LED3_Pin };
Led_Struct LEFT_BOTTOM_LED  = { .GPIOx = LED4_GPIO_Port, .Pin = LED4_Pin };

RemoteControl_Status remote_control_status = REMOTE_CONTROL_CONNECTED;  // 调试期写死
Plane_Status         plane_state           = Error;                     // 上电默认熄灭
```

## 四、状态 → 灯效对照

| 状态变量 | 取值 | 前两个灯 | 后两个灯 |
| --- | --- | --- | --- |
| `remote_control_status` | `CONNECTED` | 常亮 | — |
| `remote_control_status` | `DISCONNECTED` | 熄灭 | — |
| `plane_state` | `IDLE` | — | 慢闪 500ms |
| `plane_state` | `FLIGHT` | — | 快闪 200ms |
| `plane_state` | `Fix_Height` | — | 常亮 |
| `plane_state` | `Error` | — | 熄灭 |

## 五、调用链

```
Int_Led_Task（每 100ms）
   │
   ├─ 读 remote_control_status ──► Int_led_on/off(&LEFT_TOP_LED, &RIGHT_TOP_LED)
   │                                     │
   ├─ 读 plane_state，Count 取余 ──► Int_led_toggle/on/off(&LEFT_BOTTOM_LED, ...)
   │                                     │
   └─ vTaskDelayUntil(&lastWakeTime, 100) │
                                          ▼
                             HAL_GPIO_WritePin(端口, 引脚, SET/RESET)
                                          │
                                          ▼
                                     GPIOx->BSRR / BRR
```

## 六、节奏算法："一个节拍 + 取余分频"

任务基准周期 100ms（`LED_task_period = 100`，1 tick = 1ms），用 `Count` 计数实现不同闪烁频率：

| 想要的周期 | 分频系数 | 代码 | 实际周期 |
| --- | --- | --- | --- |
| 100ms | 1 | `Count % 1 == 0` | 100ms |
| 200ms | 2 | `Count % 2 == 0` | 200ms |
| 500ms | 5 | `Count % 5 == 0` | 500ms |
| 1000ms | 10 | `Count % 10 == 0` | 1000ms |

`Count` 每 10 拍归零：

```c
if (Count == 10) Count = 0;
```

**注意耦合**：取余系数和归零值都是相对"100ms 节拍"的。如果哪天把 `LED_task_period` 改成 50，所有闪烁周期会**减半**，必须同步改系数。

## 七、怎么新增一种状态

以"降落中 `LANDING`"为例：

1. `Com_config.h` 里 `Plane_Status` 加一行 `LANDING,`
2. `Int_Led_Task` 里加一个分支（决定灯效）
3. 在"谁改状态"的地方写入 `plane_state = LANDING;`
4. 编译 → 串口打日志确认状态切换
5. 在 [开发日志](../开发日志/) 与 [学习记录](../学习记录/) 里补一句

## 八、已知问题

| # | 问题 | 影响 | 建议 |
| --- | --- | --- | --- |
| 1 | `remote_control_status` 写死 `CONNECTED` | 前两个灯永远常亮，无法验证断开效果 | 遥控链路做好后改由通信任务写入 |
| 2 | `plane_state` 初值 `Error` | 上电后后两个灯熄灭，容易被误认为"灯坏了" | 想看上电灯效改 `IDLE`；或上电先亮 1 秒再进 `Error` |
| 3 | `if / else if` 无 `else` 兜底 | 状态值异常时灯效保持上一次 | 末尾加 `else { 全灭; Debug_Printf }` |
| 4 | `Count` 是 `uint8_t` 且手动归零 10 | 改周期时容易忘记同步 | 归零值用宏 `#define LED_COUNT_MAX 10` |
| 5 | 灯效逻辑写在任务里 | 以后加灯或改逻辑会越来越长 | 可抽成 `Int_Led_Update(status, plane_state)` 放到 Interface 层 |
