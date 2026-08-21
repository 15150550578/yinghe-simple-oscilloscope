#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <stdint.h>
#include "waveform_config.h"
#include "afe.h"

/**
 * @brief  选择要显示的通道（AFE_CH1 / AFE_CH2）
 */
void waveform_set_channel(afe_channel_t ch);

/**
 * @brief  当前显示通道
 */
afe_channel_t waveform_get_channel(void);

/**
 * @brief  绘制当前选中通道的波形（含顶栏）
 */
void waveform_draw(void);

#endif /* WAVEFORM_H */
