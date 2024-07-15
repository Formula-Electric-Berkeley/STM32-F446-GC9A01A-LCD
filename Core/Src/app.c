/*
 * app.c
 *
 *  Created on: Jul 13, 2024
 *      Author: TK
 */

#include "app.h"


extern SPI_HandleTypeDef hspi1;
extern UART_HandleTypeDef huart2;


// load the weight data block from the model.bin file
INCLUDE_FILE(".rodata", "./img.bin", image);
extern uint8_t image_data[];
extern size_t image_start[];
extern size_t image_end[];


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


static inline void swap_bytes(size_t len, uint16_t *dest, uint16_t *src) {
  for (size_t i = 0; i < len; i += 1) {
    dest[i] = __builtin_bswap16(src[i]);
  }
}


static inline void set_spi_datasize(SPI_HandleTypeDef *hspi, uint32_t data_size) {
  hspi1.Init.DataSize = data_size;
  WRITE_REG(hspi->Instance->CR1, ((hspi->Init.Mode & (SPI_CR1_MSTR | SPI_CR1_SSI)) |
                                    (hspi->Init.Direction & (SPI_CR1_RXONLY | SPI_CR1_BIDIMODE)) |
                                    (hspi->Init.DataSize & SPI_CR1_DFF) |
                                    (hspi->Init.CLKPolarity & SPI_CR1_CPOL) |
                                    (hspi->Init.CLKPhase & SPI_CR1_CPHA) |
                                    (hspi->Init.NSS & SPI_CR1_SSM) |
                                    (hspi->Init.BaudRatePrescaler & SPI_CR1_BR_Msk) |
                                    (hspi->Init.FirstBit  & SPI_CR1_LSBFIRST) |
                                    (hspi->Init.CRCCalculation & SPI_CR1_CRCEN)));
}


static inline void start_spi_transaction() {
  HAL_GPIO_WritePin(TFT_CS_GPIO, TFT_CS_PIN, GPIO_PIN_RESET);
}

static inline void end_spi_transaction() {
  HAL_GPIO_WritePin(TFT_CS_GPIO, TFT_CS_PIN, GPIO_PIN_SET);
}

static inline void transmit_command(uint8_t command) {
  // set DC pin to LOW, enter command mode
  HAL_GPIO_WritePin(TFT_DC_GPIO, TFT_DC_PIN, GPIO_PIN_RESET);

  // send command
  HAL_SPI_Transmit(&hspi1, &command, 1, 100);

  // set DC pin to HIGH, return to data mode
  HAL_GPIO_WritePin(TFT_DC_GPIO, TFT_DC_PIN, GPIO_PIN_SET);
}

static inline void transmit_data(size_t n, uint16_t *data) {
  HAL_SPI_Transmit(&hspi1, (uint8_t *)data, n, 100);
}


void configure_param(uint8_t command, uint8_t *data, uint8_t n_data) {
  start_spi_transaction();

  set_spi_datasize(&hspi1, SPI_DATASIZE_8BIT);

  transmit_command(command);

  // write param value
  HAL_SPI_Transmit(&hspi1, (uint8_t *)data, n_data, 100);

  end_spi_transaction();
}


void set_addr_window(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h) {
  uint16_t x2 = (x1 + w - 1), y2 = (y1 + h - 1);

  set_spi_datasize(&hspi1, SPI_DATASIZE_8BIT);
  transmit_command(GC9A01A_CASET); // Column address set

  set_spi_datasize(&hspi1, SPI_DATASIZE_16BIT);
  transmit_data(1, &x1);
  transmit_data(1, &x2);

  set_spi_datasize(&hspi1, SPI_DATASIZE_8BIT);
  transmit_command(GC9A01A_RASET); // Row address set

  set_spi_datasize(&hspi1, SPI_DATASIZE_16BIT);
  transmit_data(1, &y1);
  transmit_data(1, &y2);

  set_spi_datasize(&hspi1, SPI_DATASIZE_8BIT);
  transmit_command(GC9A01A_RAMWR); // Write to RAM
}


void draw_pixel(int16_t x, int16_t y, uint16_t color) {
  start_spi_transaction();

  set_addr_window(x, y, 1, 1);

  set_spi_datasize(&hspi1, SPI_DATASIZE_16BIT);
  transmit_data(1, &color);

  end_spi_transaction();
}

void draw_pixels(uint16_t x, uint16_t y, uint16_t *color, uint16_t width, uint16_t height) {
  start_spi_transaction();

  set_addr_window(x, y, width, height);

  uint16_t *data_ptr = color;

  set_spi_datasize(&hspi1, SPI_DATASIZE_16BIT);
  for (size_t i = 0; i < height; i += 1) {    // For each scanline...
    transmit_data(width, data_ptr);           // Push one row
    data_ptr += width;                        // Advance pointer by one full line
  }

  end_spi_transaction();
}

void draw_screen(uint16_t *img) {
  draw_pixels(0, 0, img, WIDTH, HEIGHT);
}




uint8_t counter;


void APP_init() {
  char str[128];

  sprintf(str, "GC9A01A Test!\n");
  HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen(str), 100);

  // digitalWrite(TFT_CS, HIGH); // Deselect
  HAL_GPIO_WritePin(TFT_CS_GPIO, TFT_CS_PIN, GPIO_PIN_SET);

  // digitalWrite(TFT_DC, HIGH); // Data mode
  HAL_GPIO_WritePin(TFT_DC_GPIO, TFT_DC_PIN, GPIO_PIN_SET);


  // digitalWrite(TFT_RST, LOW);
  HAL_GPIO_WritePin(TFT_RST_GPIO, TFT_RST_PIN, GPIO_PIN_RESET);
  HAL_Delay(100);
  // digitalWrite(TFT_RST, HIGH);
  HAL_GPIO_WritePin(TFT_RST_GPIO, TFT_RST_PIN, GPIO_PIN_SET);
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
    configure_param(cmd, addr, numArgs);
    addr += numArgs;
    if (x & 0x80)
      HAL_Delay(150);
  }

//  digitalWrite(TFT_BL, HIGH); // Backlight on
  HAL_GPIO_WritePin(TFT_BL_GPIO, TFT_BL_PIN, GPIO_PIN_SET);


  for (size_t r = 0; r < HEIGHT; r += 1) {
    for (size_t c = 0; c < WIDTH; c += 1) {
      draw_pixel(c, r, 0xFFFF);
    }
  }
}


void APP_main() {
//  char str[128];
//  sprintf(str, "Done!\n");
//  HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen(str), 100);


  draw_screen((uint16_t *)image_data);

//
//  for (int i=0; i<120; i+=1) {
//    draw_pixel(120, i, color565(0, 128, 255));
//  }
//  for (int i=0; i<120; i+=1) {
//    draw_pixel(120, i, color565(255, 128, 0));
//  }


  counter += 1;
}
