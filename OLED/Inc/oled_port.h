#ifndef OLED_PORT_H
#define OLED_PORT_H

#include <stdint.h>

void OLED_Port_Reset(void);
void OLED_WriteCmd(uint8_t cmd);
void OLED_WriteData(uint8_t data);
void OLED_WriteDataBuffer(const uint8_t *buf, uint16_t len);

#endif /* OLED_PORT_H */
