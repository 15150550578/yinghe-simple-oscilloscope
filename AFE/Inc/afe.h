#ifndef AFE_H
#define AFE_H

#include <stdint.h>
#include "afe_config.h"

/* 错误码：0 成功，负值失败 */
#define AFE_OK                     (0)
#define AFE_ERR_PARAM              (-1)

/**
 * 前端档位：
 *   AFE_RANGE_HI_Z / X1：接入 R21，G=-0.5，灵敏度高
 *   AFE_RANGE_LO_Z / X5：接入 R22，G=-0.1，量程大
 */
typedef enum
{
  AFE_RANGE_HI_Z = 0,
  AFE_RANGE_LO_Z = 1,
  AFE_RANGE_X1 = AFE_RANGE_HI_Z,
  AFE_RANGE_X5 = AFE_RANGE_LO_Z
} afe_range_t;

typedef enum
{
  AFE_CH1 = 0,   /* Ain1 / SW1 / Bias1 */
  AFE_CH2 = 1    /* Ain2 / SW2 / Bias2 */
} afe_channel_t;

/**
 * @brief  初始化 AFE：两通道默认低阻档（大量程），偏置用默认值
 */
void afe_init(void);

/**
 * @brief  设置通道档位，并驱动对应 SW 引脚
 * @retval AFE_OK / AFE_ERR_PARAM
 */
int32_t afe_set_range(afe_channel_t ch, afe_range_t range);

/**
 * @brief  读取当前软件记录的档位
 */
afe_range_t afe_get_range(afe_channel_t ch);

/**
 * @brief  在高阻/低阻两档之间切换指定通道
 * @retval 切换后的档位；通道非法时返回 AFE_RANGE_LO_Z
 */
afe_range_t afe_toggle_range(afe_channel_t ch);

/**
 * @brief  两通道同步切换量程（示波器常用）
 * @retval 切换后的档位
 */
afe_range_t afe_toggle_range_both(void);

/**
 * @brief  设置偏置电压（mV）；Bias1/Bias2 接近时可共用
 */
void afe_set_bias_mv(int32_t bias_mv);

/**
 * @brief  当前偏置（mV）
 */
int32_t afe_get_bias_mv(void);

/**
 * @brief  ADC 码值 → Ain 输入电压（mV，对地）
 * @note   V_AIN = V_Bias + (V_ADC - V_Bias) / G
 */
int32_t afe_code_to_mv(afe_channel_t ch, uint16_t adc_code);

/**
 * @brief  ADC 码值 → Ain 输入电压（V）
 */
float afe_code_to_volt(afe_channel_t ch, uint16_t adc_code);

#endif /* AFE_H */
