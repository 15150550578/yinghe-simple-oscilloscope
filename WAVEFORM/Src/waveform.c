#include "waveform.h"

#include "adc_sample.h"
#include "oled_sh1107.h"

static afe_channel_t s_view_ch = AFE_CH1;

void waveform_set_channel(afe_channel_t ch)
{
  if (ch > AFE_CH2)
  {
    ch = AFE_CH1;
  }
  s_view_ch = ch;
}

afe_channel_t waveform_get_channel(void)
{
  return s_view_ch;
}

static int32_t waveform_full_scale_mv(afe_channel_t ch)
{
  if (afe_get_range(ch) == AFE_RANGE_HI_Z)
  {
    return WAVEFORM_FULL_MV_HI_Z;
  }
  return WAVEFORM_FULL_MV_LO_Z;
}

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

/* 交错缓冲：Ain1=buf[2*i]，Ain2=buf[2*i+1] */
static int32_t waveform_mv_at(const uint16_t *buf, afe_channel_t ch, uint32_t idx)
{
  uint32_t off = (idx * ADC_SAMPLE_CHANNELS) + (uint32_t)ch;
  return afe_code_to_mv(ch, buf[off]);
}

static uint32_t waveform_find_rising_zero(const uint16_t *buf, afe_channel_t ch, uint32_t points)
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
    v0 = waveform_mv_at(buf, ch, i);
    v1 = waveform_mv_at(buf, ch, i + 1U);
    if ((v0 < 0) && (v1 >= 0))
    {
      return i;
    }
  }

  return 0U;
}

void waveform_draw(void)
{
  const uint16_t *buf;
  uint32_t points;
  uint32_t start;
  uint32_t i;
  afe_channel_t ch;
  int32_t full_mv;
  int32_t vin_mv;
  int32_t show_mv;
  uint8_t y0;
  uint8_t y1;
  uint8_t mid_y;
  uint8_t x;

  ch = s_view_ch;
  buf = adc_sample_get_buffer();
  points = adc_sample_get_points();
  if ((buf == 0) || (points < WAVEFORM_DRAW_POINTS))
  {
    return;
  }

  start = waveform_find_rising_zero(buf, ch, points);
  if ((start + WAVEFORM_DRAW_POINTS) > points)
  {
    start = 0U;
  }

  full_mv = waveform_full_scale_mv(ch);
  show_mv = waveform_mv_at(buf, ch, start);

  OLED_Clear();

  if (ch == AFE_CH1)
  {
    OLED_ShowString(0, 0, "CH1", 1, OLED_COLOR_WHITE);
  }
  else
  {
    OLED_ShowString(0, 0, "CH2", 1, OLED_COLOR_WHITE);
  }
  OLED_ShowSignedNum(32, 0, show_mv, 5U, 1U, OLED_COLOR_WHITE);
  OLED_ShowString(80, 0, "mV", 1, OLED_COLOR_WHITE);

  if (afe_get_range(ch) == AFE_RANGE_HI_Z)
  {
    OLED_ShowString(0, 8, "G=-0.5", 1, OLED_COLOR_WHITE);
  }
  else
  {
    OLED_ShowString(0, 8, "G=-0.1", 1, OLED_COLOR_WHITE);
  }
  OLED_ShowString(56, 8, "K1rng", 1, OLED_COLOR_WHITE);

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

  y0 = waveform_mv_to_y(waveform_mv_at(buf, ch, start), full_mv);
  for (i = 1U; i < WAVEFORM_DRAW_POINTS; i++)
  {
    vin_mv = waveform_mv_at(buf, ch, start + i);
    y1 = waveform_mv_to_y(vin_mv, full_mv);
    x = (uint8_t)i;
    OLED_DrawLine((uint8_t)(i - 1U), y0, x, y1, OLED_COLOR_WHITE);
    y0 = y1;
  }

  OLED_Refresh();
}
