#ifndef ADC_SAMPLE_H
#define ADC_SAMPLE_H

#include <stdint.h>
#include "adc_sample_config.h"

/* 错误码：0 成功，负值失败 */
#define ADC_SAMPLE_OK               (0)
#define ADC_SAMPLE_ERR_PARAM        (-1)
#define ADC_SAMPLE_ERR_HAL          (-2)
#define ADC_SAMPLE_ERR_STATE        (-3)

/**
 * @brief  ADC 校准（启动采样前调用一次）
 * @retval ADC_SAMPLE_OK / ADC_SAMPLE_ERR_HAL
 */
int32_t adc_sample_calibrate(void);

/**
 * @brief  启动 TIM1 触发 + ADC DMA 循环采样
 * @retval ADC_SAMPLE_OK / 错误码
 */
int32_t adc_sample_start(void);

/**
 * @brief  停止采样（关 TIM1 触发与 ADC DMA）
 * @retval ADC_SAMPLE_OK / 错误码
 */
int32_t adc_sample_stop(void);

/**
 * @brief  修改 TIM1 触发频率（采样率，单位 Hz）
 * @note   须在 adc_sample_start 之前调用；范围建议 1kHz~1MHz
 * @param  sample_rate_hz 每通道采样率
 * @retval ADC_SAMPLE_OK / ADC_SAMPLE_ERR_PARAM
 */
int32_t adc_sample_set_rate(uint32_t sample_rate_hz);

/**
 * @brief  获取 DMA 缓冲首地址（交错：Ain1,Ain2,...）
 */
const uint16_t *adc_sample_get_buffer(void);

/**
 * @brief  每通道采样点数
 */
uint32_t adc_sample_get_points(void);

/**
 * @brief  DMA 半传输完成标志（读后软件清 0）
 */
uint8_t adc_sample_take_half_flag(void);

/**
 * @brief  DMA 全传输完成标志（读后软件清 0）
 */
uint8_t adc_sample_take_full_flag(void);

#endif /* ADC_SAMPLE_H */
