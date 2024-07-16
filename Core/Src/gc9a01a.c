/*
 * gc9a01a.c
 *
 *  Created on: Jul 15, 2024
 *      Author: TK
 */

#include "gc9a01a.h"



void GC9A01A_init(GC9A01A *tft,
    SPI_HandleTypeDef *hspi,
    GPIO_TypeDef *cs_gpio, uint32_t cs_pin,
    GPIO_TypeDef *dc_gpio, uint32_t dc_pin,
    GPIO_TypeDef *bl_gpio, uint32_t bl_pin,
    GPIO_TypeDef *rst_gpio, uint32_t rst_pin
    ) {
  tft->height = 240;
  tft->width = 240;

  tft->hspi = hspi;
  tft->cs_gpio = cs_gpio;
  tft->cs_pin = cs_pin;
  tft->dc_gpio = dc_gpio;
  tft->dc_pin = dc_pin;
  tft->bl_gpio = bl_gpio;
  tft->bl_pin = bl_pin;
  tft->rst_gpio = rst_gpio;
  tft->rst_pin = rst_pin;


  // digitalWrite(TFT_CS, HIGH); // Deselect
  HAL_GPIO_WritePin(tft->cs_gpio, tft->cs_pin, GPIO_PIN_SET);

  // digitalWrite(TFT_DC, HIGH); // Data mode
  HAL_GPIO_WritePin(tft->dc_gpio, tft->dc_pin, GPIO_PIN_SET);


  // digitalWrite(TFT_RST, LOW);
  HAL_GPIO_WritePin(tft->rst_gpio, tft->rst_pin, GPIO_PIN_RESET);
  HAL_Delay(100);
  // digitalWrite(TFT_RST, HIGH);
  HAL_GPIO_WritePin(tft->rst_gpio, tft->rst_pin, GPIO_PIN_SET);
  HAL_Delay(100);


  uint8_t x, numArgs;
  uint8_t *addr = (uint8_t *)INIT_CMD;
  uint8_t cmd = *addr;

  while (cmd > 0) {
    cmd = *addr;
    addr += 1;
    x = *addr;
    addr += 1;
    numArgs = x & 0x7F;
    GC9A01A_configure_param(tft, cmd, addr, numArgs);
    addr += numArgs;
    if (x & 0x80)
      HAL_Delay(150);
  }

  // Backlight on
  HAL_GPIO_WritePin(tft->bl_gpio, tft->bl_pin, GPIO_PIN_SET);
}

void GC9A01A_configure_param(GC9A01A *tft, uint8_t command, uint8_t *data, uint8_t n_data) {
  GC9A01A_start_spi_transaction(tft);

  GC9A01A_set_spi_datasize(tft, SPI_DATASIZE_8BIT);

  GC9A01A_transmit_command(tft, command);

  // write param value
  HAL_SPI_Transmit(tft->hspi, (uint8_t *)data, n_data, 100);

  GC9A01A_end_spi_transaction(tft);
}

void GC9A01A_set_addr_window(GC9A01A *tft, uint16_t x1, uint16_t y1, uint16_t w, uint16_t h) {
  uint16_t x2 = (x1 + w - 1), y2 = (y1 + h - 1);


  GC9A01A_set_spi_datasize(tft, SPI_DATASIZE_8BIT);
  GC9A01A_transmit_command(tft, GC9A01A_CASET); // Column address set

  GC9A01A_set_spi_datasize(tft, SPI_DATASIZE_16BIT);
  GC9A01A_transmit_data(tft, 1, &x1);
  GC9A01A_transmit_data(tft, 1, &x2);

  GC9A01A_set_spi_datasize(tft, SPI_DATASIZE_8BIT);
  GC9A01A_transmit_command(tft, GC9A01A_RASET); // Row address set

  GC9A01A_set_spi_datasize(tft, SPI_DATASIZE_16BIT);
  GC9A01A_transmit_data(tft, 1, &y1);
  GC9A01A_transmit_data(tft, 1, &y2);

  GC9A01A_set_spi_datasize(tft, SPI_DATASIZE_8BIT);
  GC9A01A_transmit_command(tft, GC9A01A_RAMWR); // Write to RAM
}


void GC9A01A_draw_pixel(GC9A01A *tft, int16_t x, int16_t y, uint16_t color) {
  GC9A01A_start_spi_transaction(tft);

  GC9A01A_set_addr_window(tft, x, y, 1, 1);

  GC9A01A_set_spi_datasize(tft, SPI_DATASIZE_16BIT);
  GC9A01A_transmit_data(tft, 1, &color);

  GC9A01A_end_spi_transaction(tft);
}

void GC9A01A_draw_pixels(GC9A01A *tft, uint16_t x, uint16_t y, uint16_t *color, uint16_t width, uint16_t height) {
  GC9A01A_start_spi_transaction(tft);

  GC9A01A_set_addr_window(tft, x, y, width, height);

  uint16_t *data_ptr = color;

  GC9A01A_set_spi_datasize(tft, SPI_DATASIZE_16BIT);
  for (size_t i = 0; i < height; i += 1) {    // For each scanline...
    GC9A01A_transmit_data(tft, width, data_ptr);           // Push one row
    data_ptr += width;                        // Advance pointer by one full line
  }

  GC9A01A_end_spi_transaction(tft);
}

void GC9A01A_draw_screen(GC9A01A *tft, uint16_t *img) {
  GC9A01A_draw_pixels(tft, 0, 0, img, tft->width, tft->height);
}


