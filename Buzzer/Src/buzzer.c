#include "buzzer.h"

void Buzz_Init(void)
{
  Buzz_Off();
}

void Buzz_On(void)
{
  HAL_TIM_Base_Start_IT(BUZZER_TIM);
}

void Buzz_Off(void)
{
  HAL_TIM_Base_Stop_IT(BUZZER_TIM);
  HAL_GPIO_WritePin(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN, GPIO_PIN_RESET);
}

void Buzz_Beep(uint32_t duration_ms)
{
  Buzz_On();
  HAL_Delay(duration_ms);
  Buzz_Off();
}

void Buzz_BeepTimes(uint8_t times, uint32_t on_ms, uint32_t off_ms)
{
  uint8_t i;

  for (i = 0U; i < times; i++)
  {
    Buzz_Beep(on_ms);
    if (i + 1U < times)
    {
      HAL_Delay(off_ms);
    }
  }
}

/* TIM2 周期中断：翻转 PC15，产生约 2.5 kHz 方波 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == BUZZER_TIM_INSTANCE)
  {
    HAL_GPIO_TogglePin(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN);
  }
}
