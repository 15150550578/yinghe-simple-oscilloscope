#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>
#include "buzzer_config.h"

void Buzz_Init(void);                                              /* 初始化蜂鸣器（默认静音） */
void Buzz_On(void);                                                /* 开始发声 */
void Buzz_Off(void);                                               /* 停止发声 */
void Buzz_Beep(uint32_t duration_ms);                              /* 响 duration_ms 毫秒后停止 */
void Buzz_BeepTimes(uint8_t times, uint32_t on_ms, uint32_t off_ms); /* 连续嘀 times 声 */

#endif /* BUZZER_H */
