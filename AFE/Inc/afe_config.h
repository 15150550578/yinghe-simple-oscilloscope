#ifndef AFE_CONFIG_H
#define AFE_CONFIG_H

#include "main.h"

/*
 * ============================================================================
 * 模拟前端（AFE）换算 — Instru_G031
 * ============================================================================
 *
 * Ain 与 ADC_IN 以 Bias 为基准的反相比例关系（SW 切换反馈并联电阻）：
 *
 *   高阻档（接入 R21=1M，|G|=0.5）：
 *     V_ADC ≈ V_Bias - 0.5 * (V_AIN - V_Bias)
 *
 *   低阻档（接入 R22=110k，|G|≈0.1）：
 *     V_ADC ≈ V_Bias - 0.1 * (V_AIN - V_Bias)
 *
 * 反解输入电压：
 *   V_AIN = V_Bias + (V_ADC - V_Bias) / G
 *   G = -0.5（高阻/灵敏）或 G = -0.1（低阻/大量程）
 *
 * SW1=PA8 → Ain1；SW2=PC6 → Ain2。两路算法相同。
 * 若实测档位反了，交换 AFE_SW_LEVEL_HI_Z / LO_Z。
 * ============================================================================
 */

#define AFE_ADC_MAX_CODE           (4095U)
#define AFE_VREF_MV                (3300)   /* VDDA ≈ 3.3 V，单位 mV */
#define AFE_VBIAS_MV_DEFAULT       (1650)   /* Bias 默认 1.65 V */

/* G = num/den：高阻 -0.5，低阻 -0.1 */
#define AFE_GAIN_HI_Z_NUM          (-1)     /* G = -1/2 */
#define AFE_GAIN_HI_Z_DEN          (2)
#define AFE_GAIN_LO_Z_NUM          (-1)     /* G = -1/10 */
#define AFE_GAIN_LO_Z_DEN          (10)

/* 兼容旧宏名 */
#define AFE_GAIN_X1_NUM            AFE_GAIN_HI_Z_NUM
#define AFE_GAIN_X1_DEN            AFE_GAIN_HI_Z_DEN
#define AFE_GAIN_X5_NUM            AFE_GAIN_LO_Z_NUM
#define AFE_GAIN_X5_DEN            AFE_GAIN_LO_Z_DEN

/* SW 电平：高阻=灵敏，低阻=大量程（可按实测对调） */
#define AFE_SW_LEVEL_HI_Z          GPIO_PIN_SET
#define AFE_SW_LEVEL_LO_Z          GPIO_PIN_RESET
#define AFE_SW_LEVEL_X1            AFE_SW_LEVEL_HI_Z
#define AFE_SW_LEVEL_X5            AFE_SW_LEVEL_LO_Z

#define AFE_SW1_GPIO_Port          SW1_GPIO_Port
#define AFE_SW1_Pin                SW1_Pin
#define AFE_SW2_GPIO_Port          SW2_GPIO_Port
#define AFE_SW2_Pin                SW2_Pin

#endif /* AFE_CONFIG_H */
