#include "key.h"

typedef struct
{
  GPIO_TypeDef *port;
  uint16_t pin;
  uint8_t stable;          /* 1=松开，0=按下 */
  uint8_t last_raw;
  uint32_t last_change_ms;
  uint8_t click;
} key_slot_t;

static key_slot_t s_keys[2];

static void key_slot_init(key_slot_t *slot, GPIO_TypeDef *port, uint16_t pin)
{
  slot->port = port;
  slot->pin = pin;
  slot->stable = 1U;
  slot->last_raw = 1U;
  slot->last_change_ms = HAL_GetTick();
  slot->click = 0U;
}

static void key_slot_poll(key_slot_t *slot)
{
  uint8_t raw;
  uint8_t pressed;
  uint32_t now;

  raw = (HAL_GPIO_ReadPin(slot->port, slot->pin) == KEY_PRESSED_LEVEL) ? 0U : 1U;
  now = HAL_GetTick();

  if (raw != slot->last_raw)
  {
    slot->last_raw = raw;
    slot->last_change_ms = now;
  }
  else if ((now - slot->last_change_ms) >= KEY_DEBOUNCE_MS)
  {
    if (raw != slot->stable)
    {
      pressed = (slot->stable != 0U) && (raw == 0U);
      slot->stable = raw;
      if (pressed != 0U)
      {
        slot->click = 1U;
      }
    }
  }
}

void KEY_Init(void)
{
  /* GPIO 已由 MX_GPIO_Init() 配置为上拉输入 */
  key_slot_init(&s_keys[KEY_ID_1], KEY1_GPIO_Port, KEY1_Pin);
  key_slot_init(&s_keys[KEY_ID_2], KEY2_GPIO_Port, KEY2_Pin);
}

uint8_t KEY_TakeClick(key_id_t id)
{
  uint8_t out;

  if ((id != KEY_ID_1) && (id != KEY_ID_2))
  {
    return 0U;
  }

  key_slot_poll(&s_keys[id]);
  out = s_keys[id].click;
  s_keys[id].click = 0U;
  return out;
}
