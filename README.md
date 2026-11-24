# STM32 四轴无人机 · 从零开始

一个边学边做的 STM32 四轴无人机项目：**飞控固件 + 遥控端固件 + 硬件资料 + 学习笔记 + 开发日志**。

主控是 **STM32F103C8T6**，工程用 **STM32CubeMX + HAL 库**生成，跑 **FreeRTOS**，用 **Keil MDK-ARM** 编译，在 **VS Code** 里改代码。

## 文档导航

| 我想看… | 去哪 |
| --- | --- |
| 今天/那天做了啥 | [开发日志](docs/开发日志/) |
| 学到了什么、踩过哪些坑 | [学习记录](docs/学习记录/) |
| 代码是怎么跑起来的、调用链 | [代码梳理](docs/代码梳理/) |
| 截图、芯片资料、别人的经验 | [2_笔记](2_笔记/) |
| 原理图 | [硬件原理图](硬件原理图/) |

## 目前进度

- [x] CubeMX 工程骨架（SWD 调试口、8 MHz 外部晶振、72 MHz 主频）
- [x] USART2 串口发送（115200 8N1）+ `Com_debug` 调试打印模块
- [x] 移植 FreeRTOS V202212.01，**调度器已跑起来**，两个任务正常运行
- [x] 四路 PWM 输出（TIM1/2/3/4 各一路），初始占空比 20%
- [x] IP5305T 电源保持（每 10s 拉低一次按键脉冲，防止整机断电）
- [x] 工程分层：Application / Interface / Common
- [x] 第二个工程 `P02_remote_hal`（遥控端，USART1）
- [ ] 四个电机按占空比稳定转起来（驱动函数已写好，任务里还没调用）
- [ ] USART2 串口**接收**（中断 + FreeRTOS 队列）
- [ ] 读取 IMU（MPU6050）
- [ ] 姿态解算 + PID
- [ ] 遥控信号收发（Si24R1）

## 硬件

| 项目 | 说明 |
| --- | --- |
| 主控 | STM32F103C8T6（Cortex-M3，64 KB Flash / 20 KB RAM，LQFP48） |
| 时钟 | 外部 8 MHz 晶振（HSE）× PLL 9 = **72 MHz**，APB1 = 36 MHz（定时器时钟 72 MHz） |
| 调试接口 | SWD：PA13 = SWDIO，PA14 = SWCLK（ST-Link）。**必须选 Serial Wire**，否则 PB4 无法当 PWM 用 |
| 调试串口 | USART2：PA2 = TX，PA3 = RX，115200 8N1；遥控端用 USART1：PA9/PA10 |
| 电机 PWM | RIGHT_0 = PA1(TIM2_CH2)、RIGHT_1 = PA10(TIM1_CH3)、LEFT_0 = PB4(TIM3_CH1)、LEFT_1 = PB9(TIM4_CH4) |
| 电源管理 | IP5305T，`POWER_KEY` = PB2（飞控）/ PB15（遥控），推挽输出、初始高电平 |
| 关键器件 | MPU6050（六轴）、Si24R1（2.4G）、VL53L4CD（激光测高）、AMS1117-3.3 |

## 目录结构

```
四轴无人机项目/
├── 1_代码/
│   ├── P01_flight_hal/              # 飞控（主工程）
│   │   ├── Core/                    # CubeMX 生成：main.c / tim.c / gpio.c / usart.c / FreeRTOSConfig.h
│   │   ├── Drivers/                 # ST 官方 HAL + CMSIS（不手改）
│   │   ├── MDK-ARM/
│   │   │   ├── Application/         # 应用层：建任务、定周期
│   │   │   ├── Interface/           # 硬件接口层：Int_Motor、Int_IP5305T
│   │   │   ├── Common/              # 公共层：Com_debug 调试打印
│   │   │   └── P01_flight_hal.uvprojx
│   │   └── P01_flight_hal.ioc       # CubeMX 配置
│   ├── P02_remote_hal/              # 遥控端（结构同上）
│   └── FreeRtos/                    # FreeRTOS V202212.01 源码
│       ├── Source/ include/         # 内核
│       └── Protable/                # 移植层：port.c（Cortex-M3）、heap_4.c
├── 2_笔记/                           # 按日期 + 主题归档的学习笔记
├── 硬件原理图/
├── docs/
│   ├── 开发日志/                     # 每天做了什么、编译结果
│   ├── 学习记录/                     # 学习感悟 + 踩坑记录
│   └── 代码梳理/                     # 工程结构、调用链、已知问题
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

1. Keil 打开 `1_代码/P01_flight_hal/MDK-ARM/P01_flight_hal.uvprojx`（遥控端是 `P02_remote_hal`）
2. `F7` 编译，产物在 `MDK-ARM/P01_flight_hal/P01_flight_hal.hex`
3. `F8` 下载，按复位键运行

命令行编译（不开 Keil 界面也行）：

```powershell
& 'C:\Keil_v5\UV4\UV4.exe' -b 'MDK-ARM\P01_flight_hal.uvprojx' -j0 -o build.log
```

## 关键参数速查

| 参数 | 值 | 说明 |
| --- | --- | --- |
| 主频 | 72 MHz | HSE 8M × PLL9 |
| `configTICK_RATE_HZ` | 1000 | 1 tick = 1ms |
| `configTOTAL_HEAP_SIZE` | 12 KB | FreeRTOS 堆（总 RAM 20KB） |
| PWM 计数时钟 | 18 MHz | 72M ÷ (PSC+1)，PSC = 4-1 |
| PWM 频率 | 18 kHz | 18MHz ÷ (ARR+1)，ARR = 1000-1 |
| 初始占空比 | 20% | Pulse = 200 |

## 提交约定

- 每天一次提交，commit message 用 `类型(范围): 中文说明`
  - `feat` 新功能 / `fix` 修 bug / `docs` 文档 / `chore` 杂项
- 只提交"人写的东西"：源码、`.ioc`、`.uvprojx`、文档、原理图、笔记
- 不提交编译产物（`.o` `.axf` `.hex`）和本机配置（见 [`.gitignore`](.gitignore)）
- 文件编码统一 UTF-8（见 [`.editorconfig`](.editorconfig)），避免中文注释乱码

## 更新日志

| 日期 | 内容 |
| --- | --- |
| 2026-11-21 | 用 CubeMX 建工程，配置时钟 72 MHz，打通 USART2 发送 → [日志](docs/开发日志/2026-11-21.md) |
| 2026-11-22 | 移植 FreeRTOS、写 `Com_debug` 调试模块、归档学习笔记 → [日志](docs/开发日志/2026-11-22.md) |
| 2026-11-24 | 电源保持任务、四路 PWM、电机驱动封装、FreeRTOS 调度器跑起来；新建遥控端工程 → [日志](docs/开发日志/2026-11-24.md) · [学习记录](docs/学习记录/2026-11-24.md) |
