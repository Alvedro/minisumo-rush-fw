/*============================================================================*\
*
* Copyright Karol Domanski, All Rights Reserved
* It is not allowed to reproduce or utilize parts of this document in any form
* or by any means, including photocopying and microfilm, without permission in
* written by Karol Domanski.
*
\*============================================================================*/

#include "Button.h"

/*****************************************************************************
                           INTERFACE IMPLEMENTATION
*****************************************************************************/

void EnterDebug(void)
{
	Gpio_Init(BUTTON_PORT, BUTTON_PIN, GPIO_TYPE_INPUT_PULL_UP);
	
	if(Button_GetState() == true)
	{
		Tactic_RunTestMode();
		
		Terminal_SetLevel(LOG_DEBUG);
	}
	else
	{
		Terminal_SetLevel(LOG_NONE);
	}
	
	while(Button_GetState() == true);
}

void Button_Init(void)
{
	Gpio_Init(BUTTON_PORT, BUTTON_PIN, GPIO_TYPE_INPUT_PULL_UP);
	
	Terminal_Log("Button", LOG_OK, "Configured.");
}

bool Button_GetState(void)
{
	bool state = !Gpio_ReadPin(BUTTON_PORT, BUTTON_PIN);
	
	return state;
}