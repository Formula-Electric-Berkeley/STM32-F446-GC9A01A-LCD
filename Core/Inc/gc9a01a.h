/*
 * GC9A01A.h
 *
 *  Created on: Jul 15, 2024
 *      Author: TK
 */

#ifndef INC_GC9A01A_H_
#define INC_GC9A01A_H_

#include <stddef.h>
#include <stdint.h>

#include "stm32f4xx_hal.h"



#define GC9A01A_TFTWIDTH 240  ///< Display width in pixels
#define GC9A01A_TFTHEIGHT 240 ///< Display height in pixels

#define GC9A01A_SWRESET 0x01   ///< Software Reset (maybe, not documented)
#define GC9A01A_RDDID 0x04     ///< Read display identification information
#define GC9A01A_RDDST 0x09     ///< Read Display Status
#define GC9A01A_SLPIN 0x10     ///< Enter Sleep Mode
#define GC9A01A_SLPOUT 0x11    ///< Sleep Out
#define GC9A01A_PTLON 0x12     ///< Partial Mode ON
#define GC9A01A_NORON 0x13     ///< Normal Display Mode ON
#define GC9A01A_INVOFF 0x20    ///< Display Inversion OFF
#define GC9A01A_INVON 0x21     ///< Display Inversion ON
#define GC9A01A_DISPOFF 0x28   ///< Display OFF
#define GC9A01A_DISPON 0x29    ///< Display ON
#define GC9A01A_CASET 0x2A     ///< Column Address Set
#define GC9A01A_RASET 0x2B     ///< Row Address Set
#define GC9A01A_RAMWR 0x2C     ///< Memory Write
#define GC9A01A_PTLAR 0x30     ///< Partial Area
#define GC9A01A_VSCRDEF 0x33   ///< Vertical Scrolling Definition
#define GC9A01A_TEOFF 0x34     ///< Tearing Effect Line OFF
#define GC9A01A_TEON 0x35      ///< Tearing Effect Line ON
#define GC9A01A_MADCTL 0x36    ///< Memory Access Control
#define GC9A01A_VSCRSADD 0x37  ///< Vertical Scrolling Start Address
#define GC9A01A_IDLEOFF 0x38   ///< Idle mode OFF
#define GC9A01A_IDLEON 0x39    ///< Idle mode ON
#define GC9A01A_COLMOD 0x3A    ///< Pixel Format Set
#define GC9A01A_CONTINUE 0x3C  ///< Write Memory Continue
#define GC9A01A_TEARSET 0x44   ///< Set Tear Scanline
#define GC9A01A_GETLINE 0x45   ///< Get Scanline
#define GC9A01A_SETBRIGHT 0x51 ///< Write Display Brightness
#define GC9A01A_SETCTRL 0x53   ///< Write CTRL Display
#define GC9A01A1_POWER7 0xA7   ///< Power Control 7
#define GC9A01A_TEWC 0xBA      ///< Tearing effect width control
#define GC9A01A1_POWER1 0xC1   ///< Power Control 1
#define GC9A01A1_POWER2 0xC3   ///< Power Control 2
#define GC9A01A1_POWER3 0xC4   ///< Power Control 3
#define GC9A01A1_POWER4 0xC9   ///< Power Control 4
#define GC9A01A_RDID1 0xDA     ///< Read ID 1
#define GC9A01A_RDID2 0xDB     ///< Read ID 2
#define GC9A01A_RDID3 0xDC     ///< Read ID 3
#define GC9A01A_FRAMERATE 0xE8 ///< Frame rate control
#define GC9A01A_SPI2DATA 0xE9  ///< SPI 2DATA control
#define GC9A01A_INREGEN2 0xEF  ///< Inter register enable 2
#define GC9A01A_GAMMA1 0xF0    ///< Set gamma 1
#define GC9A01A_GAMMA2 0xF1    ///< Set gamma 2
#define GC9A01A_GAMMA3 0xF2    ///< Set gamma 3
#define GC9A01A_GAMMA4 0xF3    ///< Set gamma 4
#define GC9A01A_IFACE 0xF6     ///< Interface control
#define GC9A01A_INREGEN1 0xFE  ///< Inter register enable 1

// Color definitions
#define GC9A01A_BLACK 0x0000       ///<   0,   0,   0
#define GC9A01A_NAVY 0x000F        ///<   0,   0, 123
#define GC9A01A_DARKGREEN 0x03E0   ///<   0, 125,   0
#define GC9A01A_DARKCYAN 0x03EF    ///<   0, 125, 123
#define GC9A01A_MAROON 0x7800      ///< 123,   0,   0
#define GC9A01A_PURPLE 0x780F      ///< 123,   0, 123
#define GC9A01A_OLIVE 0x7BE0       ///< 123, 125,   0
#define GC9A01A_LIGHTGREY 0xC618   ///< 198, 195, 198
#define GC9A01A_DARKGREY 0x7BEF    ///< 123, 125, 123
#define GC9A01A_BLUE 0x001F        ///<   0,   0, 255
#define GC9A01A_GREEN 0x07E0       ///<   0, 255,   0
#define GC9A01A_CYAN 0x07FF        ///<   0, 255, 255
#define GC9A01A_RED 0xF800         ///< 255,   0,   0
#define GC9A01A_MAGENTA 0xF81F     ///< 255,   0, 255
#define GC9A01A_YELLOW 0xFFE0      ///< 255, 255,   0
#define GC9A01A_WHITE 0xFFFF       ///< 255, 255, 255
#define GC9A01A_ORANGE 0xFD20      ///< 255, 165,   0
#define GC9A01A_GREENYELLOW 0xAFE5 ///< 173, 255,  41
#define GC9A01A_PINK 0xFC18        ///< 255, 130, 198



#define MADCTL_MY 0x80  ///< Bottom to top
#define MADCTL_MX 0x40  ///< Right to left
#define MADCTL_MV 0x20  ///< Reverse Mode
#define MADCTL_ML 0x10  ///< LCD refresh Bottom to top
#define MADCTL_RGB 0x00 ///< Red-Green-Blue pixel order
#define MADCTL_BGR 0x08 ///< Blue-Green-Red pixel order
#define MADCTL_MH 0x04  ///< LCD refresh right to left


typedef struct {
  size_t height;
  size_t width;
  SPI_HandleTypeDef *hspi;
  GPIO_TypeDef      *cs_gpio;
  uint32_t          cs_pin;
  GPIO_TypeDef      *dc_gpio;
  uint32_t          dc_pin;
  GPIO_TypeDef      *bl_gpio;
  uint32_t          bl_pin;
  GPIO_TypeDef      *rst_gpio;
  uint32_t          rst_pin;
} GC9A01A;


static const uint8_t INIT_CMD[] = {
  GC9A01A_INREGEN2, 0,
  0xEB, 1, 0x14,                                                        // ?
  GC9A01A_INREGEN1, 0,
  GC9A01A_INREGEN2, 0,
  0xEB, 1, 0x14,                                                        // ?
  0x84, 1, 0x40,                                                        // ?
  0x85, 1, 0xFF,                                                        // ?
  0x86, 1, 0xFF,                                                        // ?
  0x87, 1, 0xFF,                                                        // ?
  0x88, 1, 0x0A,                                                        // ?
  0x89, 1, 0x21,                                                        // ?
  0x8A, 1, 0x00,                                                        // ?
  0x8B, 1, 0x80,                                                        // ?
  0x8C, 1, 0x01,                                                        // ?
  0x8D, 1, 0x01,                                                        // ?
  0x8E, 1, 0xFF,                                                        // ?
  0x8F, 1, 0xFF,                                                        // ?
  0xB6, 2, 0x00, 0x00,                                                  // ?
  GC9A01A_MADCTL, 1, MADCTL_MX | MADCTL_BGR,
  GC9A01A_COLMOD, 1, 0x05,
  0x90, 4, 0x08, 0x08, 0x08, 0x08,                                      // ?
  0xBD, 1, 0x06,                                                        // ?
  0xBC, 1, 0x00,                                                        // ?
  0xFF, 3, 0x60, 0x01, 0x04,                                            // ?
  GC9A01A1_POWER2, 1, 0x13,
  GC9A01A1_POWER3, 1, 0x13,
  GC9A01A1_POWER4, 1, 0x22,
  0xBE, 1, 0x11,                                                        // ?
  0xE1, 2, 0x10, 0x0E,                                                  // ?
  0xDF, 3, 0x21, 0x0c, 0x02,                                            // ?
  GC9A01A_GAMMA1, 6, 0x45, 0x09, 0x08, 0x08, 0x26, 0x2A,
  GC9A01A_GAMMA2, 6, 0x43, 0x70, 0x72, 0x36, 0x37, 0x6F,
  GC9A01A_GAMMA3, 6, 0x45, 0x09, 0x08, 0x08, 0x26, 0x2A,
  GC9A01A_GAMMA4, 6, 0x43, 0x70, 0x72, 0x36, 0x37, 0x6F,
  0xED, 2, 0x1B, 0x0B,                                                  // ?
  0xAE, 1, 0x77,                                                        // ?
  0xCD, 1, 0x63,                                                        // ?
  // Unsure what this line (from manufacturer's boilerplate code) is
  // meant to do, but users reported issues, seems to work OK without:
  //0x70, 9, 0x07, 0x07, 0x04, 0x0E, 0x0F, 0x09, 0x07, 0x08, 0x03,      // ?
  GC9A01A_FRAMERATE, 1, 0x34,
  0x62, 12, 0x18, 0x0D, 0x71, 0xED, 0x70, 0x70,                         // ?
            0x18, 0x0F, 0x71, 0xEF, 0x70, 0x70,
  0x63, 12, 0x18, 0x11, 0x71, 0xF1, 0x70, 0x70,                         // ?
            0x18, 0x13, 0x71, 0xF3, 0x70, 0x70,
  0x64, 7, 0x28, 0x29, 0xF1, 0x01, 0xF1, 0x00, 0x07,                    // ?
  0x66, 10, 0x3C, 0x00, 0xCD, 0x67, 0x45, 0x45, 0x10, 0x00, 0x00, 0x00, // ?
  0x67, 10, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x01, 0x54, 0x10, 0x32, 0x98, // ?
  0x74, 7, 0x10, 0x85, 0x80, 0x00, 0x00, 0x4E, 0x00,                    // ?
  0x98, 2, 0x3e, 0x07,                                                  // ?
  GC9A01A_TEON, 0,
  GC9A01A_INVON, 0,
  GC9A01A_SLPOUT, 0x80,                                                 // Exit sleep
  GC9A01A_DISPON, 0x80,                                                 // Display on
  0x00                                                                  // End of list
};



static inline uint16_t color565(uint8_t red, uint8_t green, uint8_t blue) {
  return ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | (blue >> 3);
}



static inline void swap_bytes(size_t len, uint16_t *dest, uint16_t *src) {
  for (size_t i = 0; i < len; i += 1) {
    dest[i] = __builtin_bswap16(src[i]);
  }
}


static inline void GC9A01A_set_spi_datasize(GC9A01A *tft, uint32_t data_size) {
  tft->hspi->Init.DataSize = data_size;
  WRITE_REG(tft->hspi->Instance->CR1, ((tft->hspi->Init.Mode & (SPI_CR1_MSTR | SPI_CR1_SSI)) |
                                    (tft->hspi->Init.Direction & (SPI_CR1_RXONLY | SPI_CR1_BIDIMODE)) |
                                    (tft->hspi->Init.DataSize & SPI_CR1_DFF) |
                                    (tft->hspi->Init.CLKPolarity & SPI_CR1_CPOL) |
                                    (tft->hspi->Init.CLKPhase & SPI_CR1_CPHA) |
                                    (tft->hspi->Init.NSS & SPI_CR1_SSM) |
                                    (tft->hspi->Init.BaudRatePrescaler & SPI_CR1_BR_Msk) |
                                    (tft->hspi->Init.FirstBit  & SPI_CR1_LSBFIRST) |
                                    (tft->hspi->Init.CRCCalculation & SPI_CR1_CRCEN)));
}


static inline void GC9A01A_start_spi_transaction(GC9A01A *tft) {
  HAL_GPIO_WritePin(tft->cs_gpio, tft->cs_pin, GPIO_PIN_RESET);
}

static inline void GC9A01A_end_spi_transaction(GC9A01A *tft) {
  HAL_GPIO_WritePin(tft->cs_gpio, tft->cs_pin, GPIO_PIN_SET);
}

static inline void GC9A01A_transmit_command(GC9A01A *tft, uint8_t command) {
  // set DC pin to LOW, enter command mode
  HAL_GPIO_WritePin(tft->dc_gpio, tft->dc_pin, GPIO_PIN_RESET);

  // send command
  HAL_SPI_Transmit(tft->hspi, &command, 1, 100);

  // set DC pin to HIGH, return to data mode
  HAL_GPIO_WritePin(tft->dc_gpio, tft->dc_pin, GPIO_PIN_SET);
}

static inline void GC9A01A_transmit_data(GC9A01A *tft, size_t n, uint16_t *data) {
  HAL_SPI_Transmit(tft->hspi, (uint8_t *)data, n, 100);
}



void GC9A01A_init(GC9A01A *tft,
    SPI_HandleTypeDef *hspi,
    GPIO_TypeDef *cs_gpio, uint32_t cs_pin,
    GPIO_TypeDef *dc_gpio, uint32_t dc_pin,
    GPIO_TypeDef *bl_gpio, uint32_t bl_pin,
    GPIO_TypeDef *rst_gpio, uint32_t rst_pin
    );

void GC9A01A_configure_param(GC9A01A *tft, uint8_t command, uint8_t *data, uint8_t n_data);

void GC9A01A_set_addr_window(GC9A01A *tft, uint16_t x1, uint16_t y1, uint16_t w, uint16_t h);

void GC9A01A_draw_pixel(GC9A01A *tft, int16_t x, int16_t y, uint16_t color);

void GC9A01A_draw_pixels(GC9A01A *tft, uint16_t x, uint16_t y, uint16_t *color, uint16_t width, uint16_t height);

void GC9A01A_draw_screen(GC9A01A *tft, uint16_t *img);



#endif /* INC_GC9A01A_H_ */
