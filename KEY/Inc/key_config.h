#ifndef KEY_CONFIG_H
#define KEY_CONFIG_H

#include "main.h"

/*
 * 按键由 CubeMX 配置（.ioc → MX_GPIO_Init）：
 *   Key1 = PA4，GPIO_Input，Pull-up → 切换当前显示通道量程（SW1/SW2）
 *   Key2 = PA5，GPIO_Input，Pull-up → 切换显示 Ain1 / Ain2
 * KEY_Init() 只初始化消抖状态，不再重复配 GPIO。
 */

#define KEY1_Pin                 Key1_Pin
#define KEY1_GPIO_Port           Key1_GPIO_Port
#define KEY2_Pin                 Key2_Pin
#define KEY2_GPIO_Port           Key2_GPIO_Port

#define KEY_PRESSED_LEVEL        GPIO_PIN_RESET
#define KEY_DEBOUNCE_MS          (30U)

#endif /* KEY_CONFIG_H */
