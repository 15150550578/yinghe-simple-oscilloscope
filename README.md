# 硬禾学堂简易示波器项目

基于 **硬禾学堂 Instru_G031**（STM32G031）的简易示波器 / 测量学习工程。

## 硬件

- MCU：STM32G031G8Ux
- 板卡：T&M DIY Kit Based on STM32G031（EETree / 硬禾学堂）
- 显示：SPI OLED 128×128（SH1107）
- 模拟输入：Ain1 / Ain2（目标量程约 2 mVpp～30 Vpp，带宽 DC～50 kHz）
- 其他：蜂鸣器、PWM 信号输出、旋钮与按键

## 工程结构

| 目录/文件 | 说明 |
|-----------|------|
| `Core/` | CubeMX 生成的核心代码（用户逻辑写在 `USER CODE` 区） |
| `Drivers/` | STM32 HAL / CMSIS |
| `OLED/` | OLED 驱动 |
| `Buzzer/` | 蜂鸣器驱动 |
| `PWM_OUT/` | PWM 输出相关 |
| `*.ioc` | CubeMX 工程配置 |
| `CMakeLists.txt` | CMake 构建入口 |

## 开发环境

- STM32CubeMX
- arm-none-eabi-gcc
- CMake
- VS Code / Cursor + Cortex-Debug（可选）

## 编译

在工程根目录使用 CMake Preset（或本地已配置的 Debug 构建）：

```bash
cmake --preset Debug
cmake --build --preset Debug
```

产物一般在 `build/Debug/` 下。

## 关键引脚（示波器采集相关）

| 功能 | 引脚 |
|------|------|
| Ain1（ADC） | PA1 |
| Ain2（ADC） | PA7 |
| 量程开关 SW1 / SW2 | PA8 / PC6 |
| 偏置 PWM（DCOut） | PA6 → TIM3_CH1 |

具体以原理图与 `.ioc` 为准。

## 许可证

学习与实验用途。板卡与资料归属硬禾学堂 / EETree 及相关作者。
