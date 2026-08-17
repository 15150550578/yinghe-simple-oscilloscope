#ifndef BUZZER_CONFIG_H
#define BUZZER_CONFIG_H

#include "main.h"
#include "tim.h"

/*
 * ============================================================================
 * 无源蜂鸣器 CubeMX 配置说明（本工程：PC15 + TIM2 软件方波）
 * ============================================================================
 * 背景：
 *   STM32G031 的 PC15 没有定时器 PWM 复用，不能做硬件 PWM。
 *   做法：PC15 配成 GPIO 输出；TIM2 只做周期中断；中断里翻转 PC15，
 *   得到约 2.5 kHz 方波驱动无源蜂鸣器。
 *
 * ---------- 1. 引脚 PC15 ----------
 *   Pinout 视图点击 PC15（可能显示为 PC15-OSC32_OUT）
 *   → 选择 GPIO_Output
 *   → User Label 设为：Buzz_PWM（与下方宏一致；改名需同步改宏）
 *   → GPIO output level：Low
 *   → Mode：Output Push Pull
 *   → Pull：No pull-up and no pull-down
 *   注意：使用 PC15 时不能接外部 32.768 kHz 晶振（LSE）。
 *
 * ---------- 2. 定时器 TIM2（不要开 PWM） ----------
 *   Timers → TIM2
 *   → Clock Source：Internal Clock
 *   → Channel1 等：Disable（不要选 PWM Generation）
 *   → Prescaler：63          （64 MHz / 64 = 1 MHz 计数）
 *   → Counter Period (ARR)：199
 *     中断频率 = 1 MHz / 200 = 5 kHz
 *     每次中断翻转一次 → 方波约 2.5 kHz（适合无源蜂鸣器）
 *
 * ---------- 3. 打开 TIM2 中断 ----------
 *   NVIC Settings（或 System Core → NVIC）
 *   → 勾选 TIM2 global interrupt
 *
 * ---------- 4. 生成代码 ----------
 *   Project → Generate Code
 *   确认生成后：
 *   - main.h 中有 Buzz_PWM_Pin / Buzz_PWM_GPIO_Port（GPIOC, PIN15）
 *   - tim.c 中 MX_TIM2_Init()，且已使能 TIM2_IRQn
 *   - stm32g0xx_it.c 中有 TIM2_IRQHandler → HAL_TIM_IRQHandler(&htim2)
 *
 * ---------- 5. 应用层用法（本模块已封装） ----------
 *   #include "buzzer.h"
 *   Buzz_Init();
 *   Buzz_Beep(200);                    // 响 200 ms
 *   Buzz_BeepTimes(3, 150, 150);       // 嘀三声
 *
 * ---------- 若改成硬件 PWM 引脚（如 PA15=TIM2_CH1）----------
 *   需改 CubeMX 为 PWM，并重写 buzzer.c 为 HAL_TIM_PWM_Start/Stop；
 *   同时修改下方宏与驱动实现。PC15 方案请保持本配置。
 * ============================================================================
 */

/* 无源蜂鸣器：PC15 + TIM2 中断翻转方波（宏对应 CubeMX Label / 外设） */
#define BUZZER_GPIO_PORT    Buzz_PWM_GPIO_Port
#define BUZZER_GPIO_PIN     Buzz_PWM_Pin
#define BUZZER_TIM          (&htim2)
#define BUZZER_TIM_INSTANCE TIM2

#endif /* BUZZER_CONFIG_H */
