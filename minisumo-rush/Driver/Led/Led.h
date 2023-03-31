/*============================================================================*\
*
* Copyright Karol Domanski, All Rights Reserved
* It is not allowed to reproduce or utilize parts of this document in any form
* or by any means, including photocopying and microfilm, without permission in
* written by Karol Domanski.
*
\*============================================================================*/

#pragma once

#include "../../Util/Util.h"
#include "../../Util/Gpio/Gpio.h"
#include "../../Driver/Terminal/Terminal.h"
#include "../../Util/Time/Time.h"

/*****************************************************************************
                       PUBLIC DEFINES / MACROS / ENUMS
*****************************************************************************/

#define LED_SHORT_BLINK_PERIOD_MILLIS 100U

#define LED_RED_PORT GPIO_PORT_D
#define LED_RED_PIN GPIO_PIN_6

#define LED_BLUE_PORT GPIO_PORT_D
#define LED_BLUE_PIN GPIO_PIN_3

#define LED_GREEN_PORT GPIO_PORT_D
#define LED_GREEN_PIN GPIO_PIN_4

typedef enum
{
	LED_COLOR_RED,
	LED_COLOR_GREEN,
	LED_COLOR_BLUE
} Led_Colors_t;

/*****************************************************************************
                         PUBLIC INTERFACE DECLARATION
*****************************************************************************/

void Led_Init(void);
void Led_Set(Led_Colors_t color, bool state);
void Led_FlashAndBlink(Led_Colors_t ledColor, uint16_t flashTimeMillis, uint16_t blinkTimeMillis, uint8_t nbrOfBlinks);
void Led_ShortBlinkRed(void);
void Led_ShortBlinkGreen(void);
void Led_ShortBlinkBlue(void);