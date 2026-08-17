#include "oled_port.h"
#include "main.h"
#include "spi.h"

#define OLED_SPI_TIMEOUT  100U

void OLED_Port_Reset(void)
{
  HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_RESET);
  HAL_Delay(10);
  HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_SET);
  HAL_Delay(10);
}

void OLED_WriteCmd(uint8_t cmd)
{
  HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET);
  (void)HAL_SPI_Transmit(&hspi2, &cmd, 1U, OLED_SPI_TIMEOUT);
}

void OLED_WriteData(uint8_t data)
{
  HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET);
  (void)HAL_SPI_Transmit(&hspi2, &data, 1U, OLED_SPI_TIMEOUT);
}

void OLED_WriteDataBuffer(const uint8_t *buf, uint16_t len)
{
  if ((buf == NULL) || (len == 0U))
  {
    return;
  }

  HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET);
  (void)HAL_SPI_Transmit(&hspi2, (uint8_t *)buf, len, OLED_SPI_TIMEOUT);
}
