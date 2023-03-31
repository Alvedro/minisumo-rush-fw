/*============================================================================*\
*
* Copyright Karol Domanski, All Rights Reserved
* It is not allowed to reproduce or utilize parts of this document in any form
* or by any means, including photocopying and microfilm, without permission in
* written by Karol Domanski.
*
\*============================================================================*/

#pragma once

#include "../Util.h"

/*****************************************************************************
                       PUBLIC DEFINES / MACROS / ENUMS
*****************************************************************************/

typedef enum
{
	GPIO_PIN_0,
	GPIO_PIN_1,
	GPIO_PIN_2,
	GPIO_PIN_3,
	GPIO_PIN_4,
	GPIO_PIN_5,
	GPIO_PIN_6,
	GPIO_PIN_7
} Gpio_Pin_t;

typedef enum
{
	GPIO_PORT_A,
	GPIO_PORT_B,
	GPIO_PORT_C,
	GPIO_PORT_D
} Gpio_Port_t;

typedef enum
{
	GPIO_TYPE_INPUT,
	GPIO_TYPE_INPUT_PULL_UP,
	GPIO_TYPE_OUTPUT
} Gpio_Type_t;

/*****************************************************************************
                         PUBLIC INTERFACE DECLARATION
*****************************************************************************/

void Gpio_Init(Gpio_Port_t port, Gpio_Pin_t pin, Gpio_Type_t type);
void Gpio_WritePin(Gpio_Port_t port, Gpio_Pin_t pin, bool state);
void Gpio_TogglePin(Gpio_Port_t port, Gpio_Pin_t pin);
bool Gpio_ReadPin(Gpio_Port_t port, Gpio_Pin_t pin);