#ifndef ADC_SAMPLE_CONFIG_H
#define ADC_SAMPLE_CONFIG_H

#include "main.h"
#include "adc.h"
#include "tim.h"

/*
 * ============================================================================
 * ADC + DMA + TIM1 触发 — CubeMX 配置说明（本工程）
 * ============================================================================
 *
 * 硬件：
 *   PA1 = Ain1 = ADC1_IN1
 *   PA7 = Ain2 = ADC1_IN7
 *   TIM1 仅作内部触发源（不占用引脚）
 *   DMA1_Channel1 ← ADC1
 *
 * ---------- 1. DMA ----------
 *   System Core → DMA → Add
 *   Request：ADC1
 *   Direction：Peripheral To Memory
 *   Priority：High
 *   Mode：Circular
 *   Data Width：Half Word / Half Word
 *   NVIC：勾选 DMA1 channel 1 interrupt
 *
 * ---------- 2. ADC1 ----------
 *   Analog → ADC1
 *   IN1 / IN7：IN Single-ended
 *   Parameter Settings：
 *     Scan Conversion Mode：Enable
 *     Continuous Conversion Mode：Disable
 *     DMA Continuous Requests：Enable
 *     External Trigger Conversion Source：Timer 1 Trigger Out Event 2
 *     External Trigger Conversion Edge：Rising edge
 *     Number Of Conversion：2
 *     Rank1 = Channel 1，Rank2 = Channel 7
 *     Sampling Time Common1：39.5 Cycles
 *
 * ---------- 3. TIM1（触发源，无引脚输出） ----------
 *   Timers → TIM1
 *   Clock Source：Internal Clock
 *   Prescaler：0
 *   Counter Period：319
 *     触发频率 = 64 MHz / (319+1) = 200 kHz（每通道采样率）
 *   Trigger Event Selection TRGO2：Update Event
 *   （不要占用 OLED/PWM 用到的引脚）
 *
 * ---------- 4. 初始化顺序 ----------
 *   MX_GPIO_Init → MX_DMA_Init → ... → MX_ADC1_Init → MX_TIM1_Init
 *   必须先 MX_DMA_Init，再 MX_ADC1_Init
 *
 * ---------- 5. 缓冲格式 ----------
 *   DMA 交错存放：[Ain1, Ain2, Ain1, Ain2, ...]
 *   点数 ADC_SAMPLE_POINTS 为每通道采样点数
 * ============================================================================
 */

/* 每通道采样点数（总 DMA 长度 = POINTS * CHANNELS） */
#define ADC_SAMPLE_POINTS           (256U)
#define ADC_SAMPLE_CHANNELS         (2U)
#define ADC_SAMPLE_BUF_LEN          (ADC_SAMPLE_POINTS * ADC_SAMPLE_CHANNELS)

/* 默认采样率 200 kHz（与 TIM1 ARR=319 对应，系统时钟 64 MHz） */
#define ADC_SAMPLE_RATE_HZ_DEFAULT  (200000U)

#define ADC_SAMPLE_ADC              (&hadc1)
#define ADC_SAMPLE_TRIG_TIM         (&htim1)

#endif /* ADC_SAMPLE_CONFIG_H */
