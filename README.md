# STM32 四轴无人机 · 从零开始

一个边学边做的 STM32 四轴无人机项目：**飞控固件 + 硬件资料 + 开发日志**。

主控是 **STM32F103C8T6**，工程用 **STM32CubeMX + HAL 库**生成，用 **Keil MDK-ARM** 编译，在 **VS Code** 里改代码。
每天的进展都写进 [开发日志](docs/开发日志/)，包括"今天学会了什么"和"踩了哪些坑"——这对以后回看特别有用。

## 目前进度

- [x] CubeMX 工程骨架（SWD 调试口、8 MHz 外部晶振、72 MHz 主频）
- [x] Keil 编译通过，能生成 `.hex`
- [x] USART2 串口**发送**（115200 8N1），串口助手能看到 `Hello World!`
- [ ] USART2 串口**接收**（中断方式 + 回显）
- [ ] `printf` 重定向到串口
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

## 目录结构

```
四轴无人机项目/
├── 1_代码/
│   ├── P01_flight_hal/              # CubeMX 工程（真正在写的代码）
│   │   ├── Core/                    # 自己写的代码：main.c / usart.c / gpio.c
│   │   ├── Drivers/                 # ST 官方 HAL 库 + CMSIS（不要手改）
│   │   ├── MDK-ARM/                 # Keil 工程（.uvprojx）
│   │   └── P01_flight_hal.ioc       # CubeMX 配置，改引脚/时钟就改它
│   └── FreeRtos/Prot/               # FreeRTOS 移植目录（预留，还没放东西）
├── 硬件原理图/                       # 原理图 PDF
├── docs/开发日志/                    # 每天的开发记录
└── README.md
```

## 开发环境

| 工具 | 版本 |
| --- | --- |
| STM32CubeMX | 6.18.1 |
| STM32Cube 固件包 | FW_F1 V1.8.7 |
| Keil MDK-ARM | V5.32（TargetToolchain = MDK-ARM V5.32） |
| 编辑器 | VS Code + Keil Assistant 插件 |
| 下载器 | ST-Link（SWD） |

## 编译与烧录

1. Keil 打开 `1_代码/P01_flight_hal/MDK-ARM/P01_flight_hal.uvprojx`
2. `F7` 编译（Build），产物在 `MDK-ARM/P01_flight_hal/P01_flight_hal.hex`
3. `F8` 下载（Download）到板子，然后按复位键运行

> 想用 VS Code：装 **Keil Assistant** 插件，把 `.uvprojx` 路径填进去，就能在 VS Code 里编译 / 下载，代码用 VS Code 编辑。

## 串口怎么用

- 接线：PA2(TX) → USB-TTL 的 RX，PA3(RX) → USB-TTL 的 TX，GND 对 GND（**别接 5V 到 3.3V 上**）
- 串口助手：波特率 **115200**、8 数据位、无校验、1 停止位、无流控
- 代码里发一条字符串：

```c
HAL_UART_Transmit(&huart2, (uint8_t *)"Hello World!\r\n", 13, 0xFFFF);
```

## 提交约定

- 每天一次提交，commit message 用 `类型: 中文说明`，例如 `feat(uart): 增加串口中断接收`
  - `feat` 新功能 / `fix` 修 bug / `docs` 文档 / `chore` 杂项
- 只提交"人写的东西"：源码、`.ioc`、`.uvprojx`、文档、原理图
- 不提交编译产物（`.o` `.axf` `.hex`）和本机配置文件（见 [`.gitignore`](.gitignore)）

## 更新日志

| 日期 | 内容 |
| --- | --- |
| 2026-11-21 | 用 CubeMX 建工程，配置时钟 72 MHz，打通 USART2 发送 → [日志](docs/开发日志/2026-11-21.md) |
