#ifndef OLED_CONFIG_H
#define OLED_CONFIG_H

#include "main.h"

/*
 * ============================================================================
 * SH1107 OLED CubeMX 配置说明（本工程：SPI2 4 线软件控制 DC/RES）
 * ============================================================================
 * 硬件接口（与 main.h 中 Label 一致）：
 *   PB8  OLED_CLK  → SPI2_SCK
 *   PB7  OLED_DIN  → SPI2_MOSI
 *   PB5  OLED_DC   → GPIO_Output（命令/数据选择，0=命令，1=数据）
 *   PB6  OLED_RES  → GPIO_Output（复位，低有效）
 * 屏规格：SH1107，128×128；驱动见 OLED/Src，底层收发见 oled_port.c（hspi2）。
 *
 * ---------- 1. 使能 SPI2 ----------
 *   Connectivity → SPI2
 *   → Mode：Transmit Only Master（或 Master，本工程为只发不收）
 *   → Hardware NSS：Disable（本屏未用片选脚，或板级常接有效）
 *   Parameter Settings 建议：
 *     - Frame Format：Motorola
 *     - Data Size：8 Bits
 *     - First Bit：MSB First
 *     - Clock Polarity (CPOL)：Low
 *     - Clock Phase (CPHA)：1 Edge（即 Mode 0；若花屏可试 2 Edge）
 *     - Baud Rate Prescaler：16
 *       （本工程约 4 MBits/s；过快可能导致花屏，可改为 32/64）
 *
 * ---------- 2. 引脚与 User Label ----------
 *   Pinout 中确认并锁定：
 *   → PB8：SPI2_SCK，User Label = OLED_CLK
 *   → PB7：SPI2_MOSI，User Label = OLED_DIN
 *   → PB5：GPIO_Output，User Label = OLED_DC
 *     Level=Low，Push Pull，No pull，Speed 按默认即可
 *   → PB6：GPIO_Output，User Label = OLED_RES
 *     Level=Low 或 High 均可（上电后由 OLED_Port_Reset 拉低再拉高）
 *
 *   改 Label 后需同步检查 oled_port.c 使用的宏：
 *   OLED_DC_Pin / OLED_RES_Pin / OLED_DC_GPIO_Port / OLED_RES_GPIO_Port
 *   以及 SPI 句柄是否仍为 hspi2（若改用 SPI1 需改 oled_port.c）。
 *
 * ---------- 3. 生成代码 ----------
 *   Project → Generate Code
 *   确认生成后：
 *   - main.h 中有 OLED_CLK / OLED_DIN / OLED_DC / OLED_RES 宏
 *   - spi.c 中有 MX_SPI2_Init()，main 里已调用
 *   - gpio.c 中 DC、RES 已初始化为输出
 *
 * ---------- 4. 工程侧（CMake，非 CubeMX） ----------
 *   根目录 CMakeLists.txt 需包含：
 *     OLED/Src/oled_port.c
 *     OLED/Src/oled_sh1107.c
 *     OLED/Src/oled_font.c
 *     以及 include 路径 OLED/Inc
 *
 * ---------- 5. 应用层用法 ----------
 *   #include "oled_sh1107.h"
 *   OLED_Init();
 *   OLED_Clear();
 *   OLED_ShowString(0, 0, "Hello", 1, OLED_COLOR_WHITE);
 *   OLED_Refresh();   // 显存改完后必须刷新才显示
 *
 * ---------- 常见问题 ----------
 *   - 无显示：查接线、3.3V/GND、RES 复位时序、SPI 模式/速率
 *   - 花屏/乱码：降低 SPI 分频；核对 CPOL/CPHA；确认是 SH1107 而非 SSD1306
 *   - 只有半边：检查分辨率宏 OLED_WIDTH/OLED_HEIGHT 与屏是否匹配
 * ============================================================================
 */

#define OLED_WIDTH   128U
#define OLED_HEIGHT  128U
#define OLED_PAGES   (OLED_HEIGHT / 8U)

#define OLED_COLOR_BLACK  0U
#define OLED_COLOR_WHITE  1U

#endif /* OLED_CONFIG_H */
