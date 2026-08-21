#ifndef WAVEFORM_CONFIG_H
#define WAVEFORM_CONFIG_H

#include "oled_config.h"
#include "adc_sample_config.h"

/*
 * Ain1 波形显示布局（128×128 SH1107）
 *   顶栏 y=0..15：CH1 瞬时 mV
 *   波形区 y=16..127：边框、0V 中线、折线
 * 横轴 128 列；触发：前半缓冲找上升沿过 0 mV，再取 128 点
 * 纵轴固定满幅：高阻 ±3300 mV，低阻 ±16500 mV
 */

#define WAVEFORM_STATUS_H          (16U)
#define WAVEFORM_AREA_X            (0U)
#define WAVEFORM_AREA_Y            (WAVEFORM_STATUS_H)
#define WAVEFORM_AREA_W            (OLED_WIDTH)
#define WAVEFORM_AREA_H            (OLED_HEIGHT - WAVEFORM_STATUS_H)

#define WAVEFORM_DRAW_POINTS       (128U)
#define WAVEFORM_TRIG_SEARCH_MAX   (128U)

/* 固定满幅（相对 0 V），单位 mV */
#define WAVEFORM_FULL_MV_HI_Z      (3300)
#define WAVEFORM_FULL_MV_LO_Z      (16500)

/* 主循环每 N 次全缓冲才刷一次屏 */
#define WAVEFORM_UI_DIV            (20U)

#endif /* WAVEFORM_CONFIG_H */
