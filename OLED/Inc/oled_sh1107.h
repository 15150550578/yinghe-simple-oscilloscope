#ifndef OLED_SH1107_H
#define OLED_SH1107_H

#include <stdint.h>
#include "oled_config.h"

void OLED_Init(void);                                                       /* 初始化 OLED */
void OLED_Clear(void);                                                      /* 清空显存（不自动刷屏） */
void OLED_Fill(uint8_t color);                                              /* 全屏填充指定颜色 */
void OLED_SetPixel(uint8_t x, uint8_t y, uint8_t color);                    /* 画一个像素点 */
void OLED_Refresh(void);                                                    /* 将显存内容刷新到屏幕 */
void OLED_DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color); /* 画直线 */
void OLED_DrawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);     /* 画矩形框 */
void OLED_FillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);     /* 画实心矩形 */
void OLED_ShowChar(uint8_t x, uint8_t y, char ch, uint8_t size, uint8_t color);    /* 显示单个字符 */
void OLED_ShowString(uint8_t x, uint8_t y, const char *str, uint8_t size, uint8_t color); /* 显示字符串 */
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t color); /* 显示无符号整数 */
void OLED_ShowSignedNum(uint8_t x, uint8_t y, int32_t num, uint8_t len, uint8_t size, uint8_t color); /* 显示有符号整数 */
void OLED_ShowHexNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t color); /* 显示十六进制数 */
void OLED_ShowFloat(uint8_t x, uint8_t y, float val, uint8_t intLen, uint8_t fracLen, uint8_t size, uint8_t color); /* 显示浮点数 */

#endif /* OLED_SH1107_H */
