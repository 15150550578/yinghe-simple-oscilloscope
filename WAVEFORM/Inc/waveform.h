#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <stdint.h>
#include "waveform_config.h"

/**
 * @brief  根据当前 DMA 缓冲绘制 Ain1 波形（含顶栏 mV）
 * @note   内部完成清屏、触发定位、折线绘制；调用后需已可 Refresh
 */
void waveform_draw_ch1(void);

#endif /* WAVEFORM_H */
