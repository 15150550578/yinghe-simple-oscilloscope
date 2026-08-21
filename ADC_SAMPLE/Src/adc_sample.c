#include "adc_sample.h"

/* 静态 DMA 缓冲：交错存放 Ain1/Ain2，禁止动态分配 */
static uint16_t s_adc_dma_buf[ADC_SAMPLE_BUF_LEN];

static volatile uint8_t s_half_ready = 0U;
static volatile uint8_t s_full_ready = 0U;
static volatile uint8_t s_running = 0U;

int32_t adc_sample_calibrate(void)
{
  if (HAL_ADCEx_Calibration_Start(ADC_SAMPLE_ADC) != HAL_OK)
  {
    return ADC_SAMPLE_ERR_HAL;
  }
  return ADC_SAMPLE_OK;
}

int32_t adc_sample_set_rate(uint32_t sample_rate_hz)
{
  uint32_t tim_clk;
  uint32_t arr;

  if ((sample_rate_hz < 1000U) || (sample_rate_hz > 1000000U))
  {
    return ADC_SAMPLE_ERR_PARAM;
  }
  if (s_running != 0U)
  {
    return ADC_SAMPLE_ERR_STATE;
  }

  /* APB1 定时器时钟：本工程 APB1 不分频，TIM 时钟 = 64 MHz */
  tim_clk = HAL_RCC_GetPCLK1Freq();
  if (tim_clk == 0U)
  {
    return ADC_SAMPLE_ERR_HAL;
  }

  arr = (tim_clk / sample_rate_hz);
  if (arr < 2U)
  {
    return ADC_SAMPLE_ERR_PARAM;
  }
  arr = arr - 1U;

  __HAL_TIM_SET_PRESCALER(ADC_SAMPLE_TRIG_TIM, 0U);
  __HAL_TIM_SET_AUTORELOAD(ADC_SAMPLE_TRIG_TIM, arr);
  /* 立刻装载影子寄存器 */
  ADC_SAMPLE_TRIG_TIM->Instance->EGR = TIM_EGR_UG;

  return ADC_SAMPLE_OK;
}

int32_t adc_sample_start(void)
{
  if (s_running != 0U)
  {
    return ADC_SAMPLE_ERR_STATE;
  }

  s_half_ready = 0U;
  s_full_ready = 0U;

  if (HAL_ADC_Start_DMA(ADC_SAMPLE_ADC,
                        (uint32_t *)s_adc_dma_buf,
                        ADC_SAMPLE_BUF_LEN) != HAL_OK)
  {
    return ADC_SAMPLE_ERR_HAL;
  }

  if (HAL_TIM_Base_Start(ADC_SAMPLE_TRIG_TIM) != HAL_OK)
  {
    (void)HAL_ADC_Stop_DMA(ADC_SAMPLE_ADC);
    return ADC_SAMPLE_ERR_HAL;
  }

  s_running = 1U;
  return ADC_SAMPLE_OK;
}

int32_t adc_sample_stop(void)
{
  (void)HAL_TIM_Base_Stop(ADC_SAMPLE_TRIG_TIM);
  (void)HAL_ADC_Stop_DMA(ADC_SAMPLE_ADC);
  s_running = 0U;
  s_half_ready = 0U;
  s_full_ready = 0U;
  return ADC_SAMPLE_OK;
}

const uint16_t *adc_sample_get_buffer(void)
{
  return s_adc_dma_buf;
}

uint32_t adc_sample_get_points(void)
{
  return ADC_SAMPLE_POINTS;
}

uint8_t adc_sample_take_half_flag(void)
{
  uint8_t flag = s_half_ready;
  s_half_ready = 0U;
  return flag;
}

uint8_t adc_sample_take_full_flag(void)
{
  uint8_t flag = s_full_ready;
  s_full_ready = 0U;
  return flag;
}

/* HAL 弱回调：ISR 内仅置标志，禁止阻塞 */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
  if (hadc->Instance == ADC1)
  {
    s_half_ready = 1U;
  }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  if (hadc->Instance == ADC1)
  {
    s_full_ready = 1U;
  }
}
