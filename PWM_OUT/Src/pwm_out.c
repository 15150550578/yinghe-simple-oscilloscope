#include "pwm_out.h"

void PWM_OUT_Init(void)
{
  HAL_TIM_PWM_Start(PWM_OUT_TIM, PWM_OUT_TIM_CHANNEL);
  PWM_OUT_SetDuty(0U);
}

void PWM_OUT_SetDuty(uint8_t duty_percent)
{
  uint32_t period;
  uint32_t pulse;

  if (duty_percent > 100U)
  {
    duty_percent = 100U;
  }

  period = __HAL_TIM_GET_AUTORELOAD(PWM_OUT_TIM);
  pulse = ((uint32_t)duty_percent * (period + 1U)) / 100U;
  __HAL_TIM_SET_COMPARE(PWM_OUT_TIM, PWM_OUT_TIM_CHANNEL, pulse);
}

void PWM_OUT_Stop(void)
{
  HAL_TIM_PWM_Stop(PWM_OUT_TIM, PWM_OUT_TIM_CHANNEL);
  __HAL_TIM_SET_COMPARE(PWM_OUT_TIM, PWM_OUT_TIM_CHANNEL, 0U);
}
