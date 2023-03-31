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
#include "../Terminal/Terminal.h"
#include "../../App/Tactic/Tactic.h"

#define BUTTON_PORT GPIO_PORT_C
#define BUTTON_PIN GPIO_PIN_0

/*****************************************************************************
                         PUBLIC INTERFACE DECLARATION
*****************************************************************************/
void EnterDebug(void);
void Button_Init(void);
bool Button_GetState(void);