#ifndef KEY_H
#define KEY_H

#include <stdint.h>
#include "key_config.h"

typedef enum
{
  KEY_ID_1 = 0,
  KEY_ID_2 = 1
} key_id_t;

/**
 * @brief  初始化按键消抖状态（GPIO 由 MX_GPIO_Init 完成）
 */
void KEY_Init(void);

/**
 * @brief  检测指定按键单击（按下沿，带消抖）；读到一次后清边沿
 * @retval 1=发生单击，0=无
 */
uint8_t KEY_TakeClick(key_id_t id);

#endif /* KEY_H */
