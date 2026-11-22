# STM32 四轴无人机 · 从零开始

一个边学边做的 STM32 四轴无人机项目：**飞控固件 + 硬件资料 + 学习笔记 + 开发日志**。

主控是 **STM32F103C8T6**，工程用 **STM32CubeMX + HAL 库**生成，用 **Keil MDK-ARM** 编译，在 **VS Code** 里改代码。
每天的进展都写进 [开发日志](docs/开发日志/)，零散的学习资料截图收在 [2_笔记](2_笔记/) 里。

## 目前进度

- [x] CubeMX 工程骨架（SWD 调试口、8 MHz 外部晶振、72 MHz 主频）
- [x] Keil 编译通过，能生成 `.hex`
- [x] USART2 串口**发送**（115200 8N1），串口助手能看到 `Hello World!`
- [x] 移植 FreeRTOS V202212.01（源码入工程、编译通过，**尚未启动调度器**）
- [x] `Com_debug` 调试打印模块（`printf` 重定向到 USART2 + `Debug_Printf` 带文件名行号）
- [ ] USART2 串口**接收**（中断方式 + 回显）
- [ ] FreeRTOS 启动调度器，建第一个任务
- [ ] 定时器 PWM 驱动电调
- [ ] 读取 IMU（陀螺仪 / 加速度计）
- [ ] 姿态解算 + PID
- [ ] 遥控信号接收

## 硬件

| 项目 | 说明 |
| --- | --- |
| 主控 | STM32F103C8T6（Cortex-M3，64 KB Flash / 20 KB RAM，LQFP48） |
| 时钟 | 外部 8 MHz 晶振（HSE）× PLL 9 = **72 MHz** 主频，APB1 = 36 MHz |
| 调试接口 | SWD：PA13 = SWDIO，PA14 = SWCLK（ST-Link） |
| 串口 | USART2：PA2 = TX，PA3 = RX，115200 8N1，无流控，接 USB 转 TTL |
| 原理图 | [`硬件原理图/`](硬件原理图/)（PDF_STM32无人机） |
| 关键器件 | MPU6050（六轴）、Si24R1（2.4G）、VL53L4CD（激光测高）、AMS1117-3.3、IP5305T（充电管理） |

## 目录结构

```
四轴无人机项目/
├── 1_代码/
│   ├── P01_flight_hal/              # CubeMX 工程（真正在写的代码）
│   │   ├── Core/                    # 自己写的代码：main.c / usart.c / gpio.c / FreeRTOSConfig.h
│   │   ├── Drivers/                 # ST 官方 HAL 库 + CMSIS（不要手改）
│   │   ├── MDK-ARM/                 # Keil 工程（.uvprojx）、Common/调试模块
│   │   └── P01_flight_hal.ioc       # CubeMX 配置，改引脚/时钟就改它
│   └── FreeRtos/                    # FreeRTOS V202212.01 源码
│       ├── Source/                  # tasks.c / queue.c / list.c ...
│       ├── include/                 # 头文件
│       └── Protable/                # 移植层：port.c（Cortex-M3）、heap_4.c
├── 2_笔记/                           # 按日期 + 主题归档的学习笔记（截图/文档）
├── 硬件原理图/                       # 原理图 PDF
├── docs/开发日志/                    # 每天的开发记录
└── README.md
```

## 开发环境

| 工具 | 版本 |
| --- | --- |
| STM32CubeMX | 6.18.1 |
| STM32Cube 固件包 | FW_F1 V1.8.7 |
| FreeRTOS | V202212.01（手动移植） |
| Keil MDK-ARM | V5.32（ARMCC V5.06 update 5，已勾选 MicroLIB） |
| 编辑器 | VS Code + Keil Assistant 插件 |
| 下载器 | ST-Link（SWD） |

## 编译与烧录

1. Keil 打开 `1_代码/P01_flight_hal/MDK-ARM/P01_flight_hal.uvprojx`
2. `F7` 编译（Build），产物在 `MDK-ARM/P01_flight_hal/P01_flight_hal.hex`
3. `F8` 下载（Download）到板子，然后按复位键运行

> 不想开 Keil 的话，命令行也能编译（我就是这么验证的）：
> ```powershell
> & 'C:\Keil_v5\UV4\UV4.exe' -b 'MDK-ARM\P01_flight_hal.uvprojx' -j0 -o build.log
> ```

## 串口怎么用

- 接线：PA2(TX) → USB-TTL 的 RX，PA3(RX) → USB-TTL 的 TX，GND 对 GND（**别接 5V 到 3.3V 上**）
- 串口助手：波特率 **115200**、8 数据位、无校验、1 停止位、无流控
- 代码里发一条字符串：

```c
HAL_UART_Transmit(&huart2, (uint8_t *)"Hello World!\r\n", 13, 0xFFFF);
```

- 用调试打印（需要先 `#include "Com_debug.h"`，并在 Keil 里勾选 **Use MicroLIB**）：

```c
Debug_Printf("电机 %d 转速 %d", motor_id, speed);   // 输出：[main.c:92]电机 0 转速 1200
```

## 提交约定

- 每天一次提交，commit message 用 `类型: 中文说明`，例如 `feat(uart): 增加串口中断接收`
  - `feat` 新功能 / `fix` 修 bug / `docs` 文档 / `chore` 杂项
- 只提交"人写的东西"：源码、`.ioc`、`.uvprojx`、文档、原理图、笔记
- 不提交编译产物（`.o` `.axf` `.hex`）和本机配置文件（见 [`.gitignore`](.gitignore)）

## 更新日志

| 日期 | 内容 |
| --- | --- |
| 2026-11-21 | 用 CubeMX 建工程，配置时钟 72 MHz，打通 USART2 发送 → [日志](docs/开发日志/2026-11-21.md) |
| 2026-11-22 | 移植 FreeRTOS、写 `Com_debug` 调试模块、归档 10-23 至 11-22 的学习笔记 → [日志](docs/开发日志/2026-11-22.md) |
