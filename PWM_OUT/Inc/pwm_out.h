#ifndef PWM_OUT_H
#define PWM_OUT_H

#include <stdint.h>
#include "pwm_out_config.h"

void PWM_OUT_Init(void);                      /* 启动 PB0 PWM，默认占空比 0% */
void PWM_OUT_SetDuty(uint8_t duty_percent);   /* 设置占空比，参数范围 0~100 */
void PWM_OUT_Stop(void);                      /* 停止 PWM 输出 */

#endif /* PWM_OUT_H */
