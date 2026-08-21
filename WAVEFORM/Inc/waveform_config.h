#ifndef WAVEFORM_CONFIG_H
#define WAVEFORM_CONFIG_H

#include "oled_config.h"
#include "adc_sample_config.h"

/*
 * 单通道波形显示（Key1 切量程，Key2 切 Ain1/Ain2 显示）
 *   顶栏 y=0..15：通道名、瞬时 mV、当前档位 G
 *   波形区 y=16..127：边框、0V 中线、折线
 * 横轴 128 列；触发：上升沿过 0 mV
 * 纵轴：高阻 ±3300 mV，低阻 ±16500 mV（跟当前显示通道档位）
 */

#define WAVEFORM_STATUS_H          (16U)
#define WAVEFORM_AREA_X            (0U)
#define WAVEFORM_AREA_Y            (WAVEFORM_STATUS_H)
#define WAVEFORM_AREA_W            (OLED_WIDTH)
#define WAVEFORM_AREA_H            (OLED_HEIGHT - WAVEFORM_STATUS_H)

#define WAVEFORM_DRAW_POINTS       (128U)
#define WAVEFORM_TRIG_SEARCH_MAX   (128U)

#define WAVEFORM_FULL_MV_HI_Z      (3300)
#define WAVEFORM_FULL_MV_LO_Z      (16500)

#define WAVEFORM_UI_DIV            (20U)

#endif /* WAVEFORM_CONFIG_H */
