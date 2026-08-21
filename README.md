# 硬禾学堂简易示波器项目

基于 **硬禾学堂 Instru_G031**（STM32G031）的简易示波器 / 测量学习工程。

## 当前功能

- 双通道 ADC 采集（TIM1 触发 + DMA，默认约 200 kHz/通道）
- 模拟前端量程：高阻 G≈-0.5 / 低阻 G≈-0.1，码值换算为对地电压（mV）
- OLED 单通道波形：128 点折线、0 V 中线、过零上升沿触发
- **Key1**：切换**当前显示通道**量程
- **Key2**：在 Ain1 / Ain2 波形显示之间切换
- PB0 PWM 输出（默认约 50% 占空比，可用于联调）

尚未实现：触发电平旋钮、时基菜单、双通道同屏、自动测量（Vpp/频率）等。

## 使用说明

### 1. 准备与烧录

1. 用 ST-Link 连接板卡 SWD，USB 供电。
2. 编译并下载固件（见下方「编译」）。
3. 上电后 OLED 先短暂提示 `K1:Rng K2:CH`，随后进入波形界面。

### 2. 接线

| 信号 | 板卡丝印 / 引脚 | 说明 |
|------|-----------------|------|
| 通道 1 输入 | Ain1（PA1） | 对地输入被测信号 |
| 通道 2 输入 | Ain2（PA7） | 对地输入被测信号 |
| 地 | GND | 与信号源共地 |

建议先用幅度适中的信号（低阻档约可达十余伏量级；小信号可切高阻档）。输入勿超过板卡前端设计范围。

### 3. 按键操作

| 按键 | 引脚 | 作用 |
|------|------|------|
| **Key1** | PA4 | 切换**当前显示通道**的量程：`G=-0.1`（低阻，大量程）↔ `G=-0.5`（高阻，更灵敏） |
| **Key2** | PA5 | 切换波形显示通道：**CH1（Ain1）↔ CH2（Ain2）** |

顶栏含义示例：

- `CH1` / `CH2`：当前显示通道
- 中间数字：触发点附近瞬时电压（mV）
- `G=-0.1` 或 `G=-0.5`：当前通道前端增益档

波形区水平中线为 **0 V**；超出满幅（高阻约 ±3.3 V，低阻约 ±16.5 V）会钳在上下边界。

### 4. 推荐操作步骤

1. 信号接到 Ain1，共地。
2. 若波形太小：按 **Key1** 切到 `G=-0.5`；若削顶/饱和：切回 `G=-0.1`。
3. 看第二路时：信号接 Ain2，按 **Key2** 切到 `CH2`，再用 Key1 调该通道量程。
4. 波形左右晃动：多为未过零或直流偏置较大，本版触发固定为过 0 V 上升沿。

### 5. 电压换算说明（简述）

前端为以 Bias 为基准的反相比例关系：

\[
V_{ADC} \approx V_{Bias} - |G|\cdot(V_{AIN}-V_{Bias})
\quad\Rightarrow\quad
V_{AIN} = V_{Bias} + \frac{V_{ADC}-V_{Bias}}{G}
\]

- 高阻档（并联 1 MΩ）：\(G=-0.5\)
- 低阻档（并联 110 kΩ）：\(G\approx-0.1\)
- 默认 \(V_{Bias}=1.65\,\mathrm{V}\)（可用软件 `afe_set_bias_mv()` 校准）

## 硬件

- MCU：STM32G031G8Ux
- 板卡：T&M DIY Kit Based on STM32G031（EETree / 硬禾学堂）
- 显示：SPI OLED 128×128（SH1107）
- 模拟输入：Ain1 / Ain2
- 按键：Key1 / Key2；另有蜂鸣器、PWM、旋钮等（部分未接入业务）

## 工程结构

| 目录/文件 | 说明 |
|-----------|------|
| `Core/` | CubeMX 生成代码（业务写在 `USER CODE`） |
| `ADC_SAMPLE/` | TIM1 触发 + ADC DMA 采样 |
| `AFE/` | 量程开关 SW1/SW2 + 电压换算 |
| `WAVEFORM/` | OLED 波形绘制与通道选择 |
| `KEY/` | Key1/Key2 消抖 |
| `OLED/` | SH1107 驱动 |
| `PWM_OUT/` | PB0 PWM |
| `Buzzer/` | 蜂鸣器驱动（暂未在主流程使用） |
| `*.ioc` | CubeMX 工程 |
| `CMakeLists.txt` | CMake 构建入口 |

## 开发环境

- STM32CubeMX
- arm-none-eabi-gcc
- CMake
- VS Code / Cursor + Cortex-Debug（可选）

## 编译

```bash
cmake --preset Debug
cmake --build --preset Debug
```

产物一般在 `build/Debug/`。

## 关键引脚

| 功能 | 引脚 |
|------|------|
| Ain1（ADC） | PA1 |
| Ain2（ADC） | PA7 |
| Key1 / Key2 | PA4 / PA5 |
| 量程 SW1 / SW2 | PA8 / PC6 |
| PWM_OUT | PB0（TIM3_CH3） |
| OLED SPI | PB5 DC、PB6 RES、PB7 DIN、PB8 CLK |

具体以原理图与 `.ioc` 为准。

## 许可证

学习与实验用途。板卡与资料归属硬禾学堂 / EETree 及相关作者。
