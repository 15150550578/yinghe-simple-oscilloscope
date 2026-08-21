#include "afe.h"

static afe_range_t s_range[2] = {AFE_RANGE_LO_Z, AFE_RANGE_LO_Z};
static int32_t s_bias_mv = AFE_VBIAS_MV_DEFAULT;

static void afe_apply_sw(afe_channel_t ch, afe_range_t range)
{
  GPIO_PinState level = (range == AFE_RANGE_HI_Z) ? AFE_SW_LEVEL_HI_Z : AFE_SW_LEVEL_LO_Z;

  if (ch == AFE_CH1)
  {
    HAL_GPIO_WritePin(AFE_SW1_GPIO_Port, AFE_SW1_Pin, level);
  }
  else
  {
    HAL_GPIO_WritePin(AFE_SW2_GPIO_Port, AFE_SW2_Pin, level);
  }
}

void afe_init(void)
{
  s_bias_mv = AFE_VBIAS_MV_DEFAULT;
  s_range[0] = AFE_RANGE_LO_Z;
  s_range[1] = AFE_RANGE_LO_Z;
  afe_apply_sw(AFE_CH1, AFE_RANGE_LO_Z);
  afe_apply_sw(AFE_CH2, AFE_RANGE_LO_Z);
}

int32_t afe_set_range(afe_channel_t ch, afe_range_t range)
{
  if ((ch > AFE_CH2) || ((range != AFE_RANGE_HI_Z) && (range != AFE_RANGE_LO_Z)))
  {
    return AFE_ERR_PARAM;
  }

  s_range[ch] = range;
  afe_apply_sw(ch, range);
  return AFE_OK;
}

afe_range_t afe_get_range(afe_channel_t ch)
{
  if (ch > AFE_CH2)
  {
    return AFE_RANGE_LO_Z;
  }
  return s_range[ch];
}

void afe_set_bias_mv(int32_t bias_mv)
{
  if (bias_mv < 500)
  {
    bias_mv = 500;
  }
  else if (bias_mv > 2800)
  {
    bias_mv = 2800;
  }
  s_bias_mv = bias_mv;
}

int32_t afe_get_bias_mv(void)
{
  return s_bias_mv;
}

int32_t afe_code_to_mv(afe_channel_t ch, uint16_t adc_code)
{
  int32_t vadc_mv;
  int32_t delta_mv;
  int32_t g_num;
  int32_t g_den;

  if (ch > AFE_CH2)
  {
    return 0;
  }
  if (adc_code > AFE_ADC_MAX_CODE)
  {
    adc_code = (uint16_t)AFE_ADC_MAX_CODE;
  }

  /* V_ADC(mV) = code * 3300 / 4095 */
  vadc_mv = ((int32_t)adc_code * AFE_VREF_MV) / (int32_t)AFE_ADC_MAX_CODE;
  delta_mv = vadc_mv - s_bias_mv;

  if (s_range[ch] == AFE_RANGE_HI_Z)
  {
    /* 高阻档 R21：G = -0.5 */
    g_num = AFE_GAIN_HI_Z_NUM;
    g_den = AFE_GAIN_HI_Z_DEN;
  }
  else
  {
    /* 低阻档 R22：G = -0.1 */
    g_num = AFE_GAIN_LO_Z_NUM;
    g_den = AFE_GAIN_LO_Z_DEN;
  }

  /*
   * V_ADC ≈ V_Bias - |G| * (V_AIN - V_Bias)
   * 反解：V_AIN = V_Bias + (V_ADC - V_Bias) / G
   *       G = num/den（num 为负）
   */
  return s_bias_mv + (delta_mv * g_den) / g_num;
}

float afe_code_to_volt(afe_channel_t ch, uint16_t adc_code)
{
  return ((float)afe_code_to_mv(ch, adc_code)) / 1000.0f;
}
