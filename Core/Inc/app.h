/*
 * app.h
 *
 *  Created on: Jul 13, 2024
 *      Author: TK
 */

#ifndef INC_APP_H_
#define INC_APP_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "stm32f4xx_hal.h"
#include "gc9a01a.h"



// http://elm-chan.org/junk/32bit/binclude.html
#define INCLUDE_FILE(section, filename, symbol) asm (\
    ".section "#section"\n"                   /* Change section */\
    ".balign 4\n"                             /* Word alignment */\
    ".global "#symbol"_start\n"               /* Export the object start address */\
    ".global "#symbol"_data\n"                /* Export the object address */\
    #symbol"_start:\n"                        /* Define the object start address label */\
    #symbol"_data:\n"                         /* Define the object label */\
    ".incbin \""filename"\"\n"                /* Import the file */\
    ".global "#symbol"_end\n"                 /* Export the object end address */\
    #symbol"_end:\n"                          /* Define the object end address label */\
    ".balign 4\n"                             /* Word alignment */\
    ".section \".text\"\n")                   /* Restore section */



// Chip select
#define TFT1_CS_GPIO   GPIOB
#define TFT1_CS_PIN    GPIO_PIN_6

// Data/command
#define TFT1_DC_GPIO   GPIOA
#define TFT1_DC_PIN    GPIO_PIN_8

// Backlight control
#define TFT1_BL_GPIO   GPIOC
#define TFT1_BL_PIN    GPIO_PIN_7

#define TFT1_RST_GPIO  GPIOA
#define TFT1_RST_PIN   GPIO_PIN_9


// Chip select
#define TFT2_CS_GPIO   GPIOA
#define TFT2_CS_PIN    GPIO_PIN_11

// Data/command
#define TFT2_DC_GPIO   GPIOB
#define TFT2_DC_PIN    GPIO_PIN_1

// Backlight control
#define TFT2_BL_GPIO   GPIOB
#define TFT2_BL_PIN    GPIO_PIN_12

#define TFT2_RST_GPIO  GPIOB
#define TFT2_RST_PIN   GPIO_PIN_2



void APP_init();

void APP_main();


#endif /* INC_APP_H_ */
