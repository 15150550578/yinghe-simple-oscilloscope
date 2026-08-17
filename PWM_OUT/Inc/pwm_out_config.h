#ifndef PWM_OUT_CONFIG_H
#define PWM_OUT_CONFIG_H

#include "main.h"
#include "tim.h"

/*
 * ============================================================================
 * PWM_OUT CubeMX 配置说明（本工程：PB0 = TIM3_CH3）
 * ============================================================================
 * 硬件：
 *   PB0  → TIM3_CH3 硬件 PWM 输出
 *   User Label：PWM_OUT（生成宏 PWM_OUT_Pin / PWM_OUT_GPIO_Port）
 *
 * ---------- 1. 引脚 PB0 ----------
 *   Pinout 视图点击 PB0
 *   → 选择 TIM3_CH3
 *   → User Label 设为：PWM_OUT（与下方宏一致；改名需同步改宏）
 *
 * ---------- 2. 定时器 TIM3 ----------
 *   Timers → TIM3
 *   → Clock Source：Internal Clock
 *   → Channel3：PWM Generation CH3
 *   参数（当前工程已生成）：
 *     - Prescaler：63          （64 MHz / 64 = 1 MHz）
 *     - Counter Period (ARR)：99
 *       频率 f = 1 MHz / 100 = 10 kHz
 *     - Pulse（CH3 初始）：0   （占空比 0%，由软件再设）
 *
 *   占空比换算（PWM_OUT_SetDuty 内部使用）：
 *     Pulse = duty_percent * (ARR + 1) / 100
 *     例：duty=50、ARR=99 → Pulse=50 → 约 50%
 *
 * ---------- 3. 生成代码 ----------
 *   Project → Generate Code
 *   确认生成后：
 *   - main.h 中有 PWM_OUT_Pin / PWM_OUT_GPIO_Port（GPIOB, PIN0）
 *   - tim.c 中 MX_TIM3_Init() 配置了 TIM_CHANNEL_3
 *   - HAL_TIM_MspPostInit 中有 PB0 ------> TIM3_CH3（AF1_TIM3）
 *
 * ---------- 4. 工程侧（CMake，非 CubeMX） ----------
 *   根目录 CMakeLists.txt 需包含：
 *     PWM_OUT/Src/pwm_out.c
 *     以及 include 路径 PWM_OUT/Inc
 *
 * ---------- 5. 应用层用法 ----------
 *   #include "pwm_out.h"
 *   PWM_OUT_Init();           // 启动 PWM，默认 0%
 *   PWM_OUT_SetDuty(50);      // 占空比 50%（参数范围 0~100）
 *   PWM_OUT_Stop();           // 停止输出
 *
 * ---------- 注意 ----------
 *   - 必须先 PWM_OUT_Init()（内部 HAL_TIM_PWM_Start），只改占空比不会自动启动
 *   - TIM3 的 PSC/ARR 同时影响该定时器所有通道频率；各通道占空比独立
 *   - TIM2 已被蜂鸣器占用，PB0 请使用 TIM3_CH3，不要改用 TIM2
 * ============================================================================
 */

#define PWM_OUT_TIM           (&htim3)
#define PWM_OUT_TIM_CHANNEL   TIM_CHANNEL_3

#endif /* PWM_OUT_CONFIG_H */
