#include "waveform.h"

#include "adc_sample.h"
#include "afe.h"
#include "oled_sh1107.h"

static int32_t waveform_full_scale_mv(void)
{
  if (afe_get_range(AFE_CH1) == AFE_RANGE_HI_Z)
  {
    return WAVEFORM_FULL_MV_HI_Z;
  }
  return WAVEFORM_FULL_MV_LO_Z;
}

/* Vin(mV) → 波形区 y，0 V 在中线，超出钳位 */
static uint8_t waveform_mv_to_y(int32_t vin_mv, int32_t full_mv)
{
  int32_t mid;
  int32_t half_h;
  int32_t y;
  int32_t y_min;
  int32_t y_max;

  mid = (int32_t)WAVEFORM_AREA_Y + ((int32_t)WAVEFORM_AREA_H / 2);
  half_h = (int32_t)WAVEFORM_AREA_H / 2;

  if (full_mv <= 0)
  {
    full_mv = WAVEFORM_FULL_MV_LO_Z;
  }

  /* y 向下增大，电压向上画：中线减去偏移 */
  y = mid - ((vin_mv * half_h) / full_mv);

  y_min = (int32_t)WAVEFORM_AREA_Y;
  y_max = (int32_t)WAVEFORM_AREA_Y + (int32_t)WAVEFORM_AREA_H - 1;
  if (y < y_min)
  {
    y = y_min;
  }
  else if (y > y_max)
  {
    y = y_max;
  }

  return (uint8_t)y;
}

/* 交错缓冲中取 Ain1：buf[2*i] */
static int32_t waveform_ch1_mv_at(const uint16_t *buf, uint32_t idx)
{
  return afe_code_to_mv(AFE_CH1, buf[idx * ADC_SAMPLE_CHANNELS]);
}

/* 在前 TRIG_SEARCH_MAX 点内找上升沿过 0 mV；失败返回 0 */
static uint32_t waveform_find_rising_zero(const uint16_t *buf, uint32_t points)
{
  uint32_t search_n;
  uint32_t i;
  int32_t v0;
  int32_t v1;

  search_n = WAVEFORM_TRIG_SEARCH_MAX;
  if (search_n > (points - WAVEFORM_DRAW_POINTS))
  {
    if (points <= WAVEFORM_DRAW_POINTS)
    {
      return 0U;
    }
    search_n = points - WAVEFORM_DRAW_POINTS;
  }

  for (i = 0U; i + 1U < search_n; i++)
  {
    v0 = waveform_ch1_mv_at(buf, i);
    v1 = waveform_ch1_mv_at(buf, i + 1U);
    if ((v0 < 0) && (v1 >= 0))
    {
      return i;
    }
  }

  return 0U;
}

void waveform_draw_ch1(void)
{
  const uint16_t *buf;
  uint32_t points;
  uint32_t start;
  uint32_t i;
  int32_t full_mv;
  int32_t vin_mv;
  int32_t show_mv;
  uint8_t y0;
  uint8_t y1;
  uint8_t mid_y;
  uint8_t x;

  buf = adc_sample_get_buffer();
  points = adc_sample_get_points();
  if ((buf == 0) || (points < WAVEFORM_DRAW_POINTS))
  {
    return;
  }

  start = waveform_find_rising_zero(buf, points);
  if ((start + WAVEFORM_DRAW_POINTS) > points)
  {
    start = 0U;
  }

  full_mv = waveform_full_scale_mv();
  show_mv = waveform_ch1_mv_at(buf, start);

  OLED_Clear();

  /* 顶栏 */
  OLED_ShowString(0, 0, "CH1", 1, OLED_COLOR_WHITE);
  OLED_ShowSignedNum(32, 0, show_mv, 5U, 1U, OLED_COLOR_WHITE);
  OLED_ShowString(80, 0, "mV", 1, OLED_COLOR_WHITE);
  if (afe_get_range(AFE_CH1) == AFE_RANGE_HI_Z)
  {
    OLED_ShowString(0, 8, "G=-0.5", 1, OLED_COLOR_WHITE);
  }
  else
  {
    OLED_ShowString(0, 8, "G=-0.1", 1, OLED_COLOR_WHITE);
  }

  /* 波形区边框与 0 V 中线 */
  OLED_DrawRect((uint8_t)WAVEFORM_AREA_X,
                (uint8_t)WAVEFORM_AREA_Y,
                (uint8_t)WAVEFORM_AREA_W,
                (uint8_t)WAVEFORM_AREA_H,
                OLED_COLOR_WHITE);
  mid_y = (uint8_t)((int32_t)WAVEFORM_AREA_Y + ((int32_t)WAVEFORM_AREA_H / 2));
  OLED_DrawLine((uint8_t)(WAVEFORM_AREA_X + 1U),
                mid_y,
                (uint8_t)(WAVEFORM_AREA_X + WAVEFORM_AREA_W - 2U),
                mid_y,
                OLED_COLOR_WHITE);

  /* 折线：x = 0..127 对应触发后采样点 */
  y0 = waveform_mv_to_y(waveform_ch1_mv_at(buf, start), full_mv);
  for (i = 1U; i < WAVEFORM_DRAW_POINTS; i++)
  {
    vin_mv = waveform_ch1_mv_at(buf, start + i);
    y1 = waveform_mv_to_y(vin_mv, full_mv);
    x = (uint8_t)i;
    OLED_DrawLine((uint8_t)(i - 1U), y0, x, y1, OLED_COLOR_WHITE);
    y0 = y1;
  }

  OLED_Refresh();
}
