/*============================================================================*\
*
* Copyright Karol Domanski, All Rights Reserved
* It is not allowed to reproduce or utilize parts of this document in any form
* or by any means, including photocopying and microfilm, without permission in
* written by Karol Domanski.
*
\*============================================================================*/

#include "Led.h"

/*****************************************************************************
                           INTERFACE IMPLEMENTATION
*****************************************************************************/

void Led_Init(void)
{
    Gpio_Init(LED_RED_PORT, LED_RED_PIN, GPIO_TYPE_OUTPUT);
    Gpio_Init(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_TYPE_OUTPUT);
    Gpio_Init(LED_BLUE_PORT, LED_BLUE_PIN, GPIO_TYPE_OUTPUT);
	
	Led_Set(LED_COLOR_RED, false);
	Led_Set(LED_COLOR_GREEN, false);
	Led_Set(LED_COLOR_BLUE, false);
	
	Terminal_Log("Led", LOG_OK, "Configured.");
}

void Led_Set(Led_Colors_t color, bool state)
{
	if(color > LED_COLOR_BLUE)
	{
		return;
	}
	
	switch(color)
	{
		case LED_COLOR_RED:
		{
			Gpio_WritePin(LED_RED_PORT, LED_RED_PIN, state);
			
			break;
		} 
		
		case LED_COLOR_GREEN:
		{
			Gpio_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, state);
			
			break;
		}
		
		case LED_COLOR_BLUE:
		{
			Gpio_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, state);
			
			break;
		}
		
		default:
		{
			break;
		}
	}
}

void Led_FlashAndBlink(Led_Colors_t ledColor, uint16_t flashTimeMillis, uint16_t blinkTimeMillis, uint8_t nbrOfBlinks)
{
	static bool flag;
	static uint8_t blinksCnt;
	static uint32_t timestampFlashTimeMillis;
	static uint32_t timestampBlinkTimeMillis;
	
	uint32_t currentMillis = Time_GetMillis();
	if(flag == false)
	{
		currentMillis = flashTimeMillis;
		flag = true;
	}
	
	if(currentMillis - timestampFlashTimeMillis >= flashTimeMillis)
	{
		if(blinksCnt < nbrOfBlinks * 2U)
		{
			if(Time_GetMillis() - timestampBlinkTimeMillis >= blinkTimeMillis)
			{
				if (blinksCnt % 2 == 0)
				{
					Led_Set(ledColor, true);
				}
				else
				{
					Led_Set(ledColor, false);
				}
				
				blinksCnt++;				
				timestampBlinkTimeMillis = Time_GetMillis();
			}
		}
		else
		{
			blinksCnt = 0U;
			timestampFlashTimeMillis = Time_GetMillis();
		}
	}
}

void Led_ShortBlinkRed(void)
{
	static uint32_t millis;
	
	if(Time_GetMillis() - millis > LED_SHORT_BLINK_PERIOD_MILLIS)
	{
		Gpio_TogglePin(LED_RED_PORT, LED_RED_PIN);
		
		millis = Time_GetMillis();
	}
}

void Led_ShortBlinkGreen(void)
{
	static uint32_t millis;
	
	if(Time_GetMillis() - millis > LED_SHORT_BLINK_PERIOD_MILLIS)
	{
		Gpio_TogglePin(LED_GREEN_PORT, LED_GREEN_PIN);
		
		millis = Time_GetMillis();
	}
}

void Led_ShortBlinkBlue(void)
{
	static uint32_t millis;
	
	if(Time_GetMillis() - millis > LED_SHORT_BLINK_PERIOD_MILLIS)
	{
		Gpio_TogglePin(LED_BLUE_PORT, LED_BLUE_PIN);
		
		millis = Time_GetMillis();
	}
}