#include "oled_sh1107.h"
#include "oled_port.h"
#include "oled_font.h"

#include <stdio.h>
#include <string.h>

static uint8_t OLED_GRAM[OLED_WIDTH * OLED_PAGES];

static void OLED_WriteInitCommands(const uint8_t *cmds, uint16_t len)
{
  for (uint16_t i = 0U; i < len; i++)
  {
    OLED_WriteCmd(cmds[i]);
  }
}

void OLED_Init(void)
{
  static const uint8_t init[] = {
      0xAE, 0xD5, 0x51, 0x20, 0x81, 0x4F, 0xAD, 0x8A,
      0xA0, 0xC0, 0xDC, 0x00, 0xD3, 0x60, 0xD9, 0x22,
      0xDB, 0x35, 0xA8, 0x3F, 0xA4, 0xA6,
  };
  static const uint8_t init_128x128[] = {
      0xD3, 0x00, 0xA8, 0x7F,
  };

  OLED_Port_Reset();
  OLED_WriteInitCommands(init, (uint16_t)sizeof(init));
  OLED_WriteInitCommands(init_128x128, (uint16_t)sizeof(init_128x128));
  HAL_Delay(100);
  OLED_WriteCmd(0xAF);

  OLED_Clear();
  OLED_Refresh();
}

void OLED_Clear(void)
{
  (void)memset(OLED_GRAM, 0x00, sizeof(OLED_GRAM));
}

void OLED_Fill(uint8_t color)
{
  uint8_t fill = (color != OLED_COLOR_BLACK) ? 0xFFU : 0x00U;
  (void)memset(OLED_GRAM, fill, sizeof(OLED_GRAM));
}

void OLED_SetPixel(uint8_t x, uint8_t y, uint8_t color)
{
  if ((x >= OLED_WIDTH) || (y >= OLED_HEIGHT))
  {
    return;
  }

  uint16_t index = (uint16_t)(y / 8U) * OLED_WIDTH + x;
  uint8_t bit = (uint8_t)(1U << (y & 0x07U));

  if (color != OLED_COLOR_BLACK)
  {
    OLED_GRAM[index] |= bit;
  }
  else
  {
    OLED_GRAM[index] &= (uint8_t)(~bit);
  }
}

void OLED_Refresh(void)
{
  for (uint8_t page = 0U; page < OLED_PAGES; page++)
  {
    OLED_WriteCmd((uint8_t)(0xB0U + page));
    OLED_WriteCmd(0x00U);
    OLED_WriteCmd(0x10U);
    OLED_WriteDataBuffer(&OLED_GRAM[(uint16_t)page * OLED_WIDTH], OLED_WIDTH);
  }
}

static int16_t OLED_Abs16(int16_t value)
{
  return (value < 0) ? (int16_t)(-value) : value;
}

void OLED_DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color)
{
  int16_t dx = (int16_t)x1 - (int16_t)x0;
  int16_t dy = (int16_t)y1 - (int16_t)y0;
  int16_t sx = (dx >= 0) ? 1 : -1;
  int16_t sy = (dy >= 0) ? 1 : -1;
  int16_t err = OLED_Abs16(dx) - OLED_Abs16(dy);
  int16_t cx = (int16_t)x0;
  int16_t cy = (int16_t)y0;

  dx = OLED_Abs16(dx);
  dy = OLED_Abs16(dy);

  while (1)
  {
    OLED_SetPixel((uint8_t)cx, (uint8_t)cy, color);

    if ((cx == (int16_t)x1) && (cy == (int16_t)y1))
    {
      break;
    }

    int16_t e2 = err * 2;
    if (e2 > -dy)
    {
      err -= dy;
      cx += sx;
    }
    if (e2 < dx)
    {
      err += dx;
      cy += sy;
    }
  }
}

void OLED_DrawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color)
{
  if ((w == 0U) || (h == 0U))
  {
    return;
  }

  uint8_t x2 = (uint8_t)(x + w - 1U);
  uint8_t y2 = (uint8_t)(y + h - 1U);

  OLED_DrawLine(x, y, x2, y, color);
  OLED_DrawLine(x, y2, x2, y2, color);
  OLED_DrawLine(x, y, x, y2, color);
  OLED_DrawLine(x2, y, x2, y2, color);
}

void OLED_FillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color)
{
  if ((w == 0U) || (h == 0U))
  {
    return;
  }

  for (uint8_t row = y; row < (uint8_t)(y + h); row++)
  {
    for (uint8_t col = x; col < (uint8_t)(x + w); col++)
    {
      OLED_SetPixel(col, row, color);
    }
  }
}

static void OLED_DrawCharPixel(uint8_t x, uint8_t y, uint8_t pixel, uint8_t color)
{
  if (pixel != 0U)
  {
    OLED_SetPixel(x, y, color);
  }
}

void OLED_ShowChar(uint8_t x, uint8_t y, char ch, uint8_t size, uint8_t color)
{
  const uint8_t *font = OLED_Font8x16_Get(ch);
  uint8_t charWidth = (uint8_t)(OLED_FONT8X16_WIDTH * size);
  uint8_t charHeight = (uint8_t)(OLED_FONT8X16_HEIGHT * size);

  if (((uint16_t)x + charWidth > OLED_WIDTH) || ((uint16_t)y + charHeight > OLED_HEIGHT))
  {
    return;
  }

  for (uint8_t row = 0U; row < OLED_FONT8X16_HEIGHT; row++)
  {
    uint8_t line = font[row];
    for (uint8_t col = 0U; col < OLED_FONT8X16_WIDTH; col++)
    {
      uint8_t pixel = (uint8_t)((line >> (7U - col)) & 0x01U);
      if (size == 1U)
      {
        OLED_DrawCharPixel((uint8_t)(x + col), (uint8_t)(y + row), pixel, color);
      }
      else
      {
        for (uint8_t sy = 0U; sy < size; sy++)
        {
          for (uint8_t sx = 0U; sx < size; sx++)
          {
            OLED_DrawCharPixel((uint8_t)(x + (col * size) + sx),
                               (uint8_t)(y + (row * size) + sy),
                               pixel,
                               color);
          }
        }
      }
    }
  }
}

void OLED_ShowString(uint8_t x, uint8_t y, const char *str, uint8_t size, uint8_t color)
{
  uint8_t cursorX = x;
  uint8_t cursorY = y;
  uint8_t charWidth = (uint8_t)(OLED_FONT8X16_WIDTH * size);
  uint8_t charHeight = (uint8_t)(OLED_FONT8X16_HEIGHT * size);

  if (str == NULL)
  {
    return;
  }

  while (*str != '\0')
  {
    if (*str == '\n')
    {
      cursorX = x;
      cursorY = (uint8_t)(cursorY + charHeight);
      str++;
      continue;
    }

    if ((uint16_t)cursorX + charWidth > OLED_WIDTH)
    {
      cursorX = x;
      cursorY = (uint8_t)(cursorY + charHeight);
    }

    if ((uint16_t)cursorY + charHeight > OLED_HEIGHT)
    {
      break;
    }

    OLED_ShowChar(cursorX, cursorY, *str, size, color);
    cursorX = (uint8_t)(cursorX + charWidth);
    str++;
  }
}

void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t color)
{
  char buf[12];
  char format[8];

  if (len >= (uint8_t)sizeof(buf))
  {
    len = (uint8_t)(sizeof(buf) - 1U);
  }

  (void)snprintf(format, sizeof(format), "%%0%uu", len);
  (void)snprintf(buf, sizeof(buf), format, (unsigned int)num);
  OLED_ShowString(x, y, buf, size, color);
}

void OLED_ShowSignedNum(uint8_t x, uint8_t y, int32_t num, uint8_t len, uint8_t size, uint8_t color)
{
  char buf[16];
  char format[8];

  if (len >= (uint8_t)(sizeof(buf) - 2U))
  {
    len = (uint8_t)(sizeof(buf) - 3U);
  }

  (void)snprintf(format, sizeof(format), "%%+0%dld", len);
  (void)snprintf(buf, sizeof(buf), format, (long)num);
  OLED_ShowString(x, y, buf, size, color);
}

void OLED_ShowHexNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t color)
{
  char buf[12];
  char format[8];

  if (len >= (uint8_t)sizeof(buf))
  {
    len = (uint8_t)(sizeof(buf) - 1U);
  }

  (void)snprintf(format, sizeof(format), "%%0%uX", len);
  (void)snprintf(buf, sizeof(buf), format, (unsigned int)num);
  OLED_ShowString(x, y, buf, size, color);
}

void OLED_ShowFloat(uint8_t x, uint8_t y, float val, uint8_t intLen, uint8_t fracLen, uint8_t size, uint8_t color)
{
  char buf[20];
  char format[12];

  (void)snprintf(format, sizeof(format), "%% %u.%uf", intLen, fracLen);
  (void)snprintf(buf, sizeof(buf), format, (double)val);
  OLED_ShowString(x, y, buf, size, color);
}
