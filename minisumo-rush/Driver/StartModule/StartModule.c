/*============================================================================*\
*
* Copyright Karol Domanski, All Rights Reserved
* It is not allowed to reproduce or utilize parts of this document in any form
* or by any means, including photocopying and microfilm, without permission in
* written by Karol Domanski.
*
\*============================================================================*/

#include "StartModule.h"
#include "../../Util/Gpio/Gpio.h"
#include "../Led/Led.h"

/*****************************************************************************
                     PRIVATE STRUCTS / ENUMS / VARIABLES
*****************************************************************************/

static bool sStartModuleWrongState;
static Machine_MachineState_t sMachineStateFightStop = MACHINE_STATE_FIGHT;

/*****************************************************************************
                         PRIVATE FUNCTION DECLARATION
*****************************************************************************/

static void EnableInt2(void);
static void DisableInt2(void);
static void ClearInt2Flag(void);
// static void SetInt2AsFallingEdge(void);
static void SetInt2AsRisingEdge(void);
static void EnableGlobalInt(void);
static void CheckStartModuleState(void);

/*****************************************************************************
                           INTERFACE IMPLEMENTATION
*****************************************************************************/

void StartModule_Init(void)
{
	Gpio_Init(STARTMODULE_PORT, STARTMODULE_PIN, GPIO_TYPE_INPUT_PULL_UP);
	
	_delay_ms(START_MODULE_TIME_TO_STARTUP_MILLIS);
	
	CheckStartModuleState();
					
	Terminal_Log("StartModule", LOG_OK, "Configured.");
}

void StartModule_InitIrq(void)
{
	DisableInt2();
	SetInt2AsRisingEdge();
	ClearInt2Flag();
	EnableInt2();
	EnableGlobalInt();	
}

ISR(INT2_vect)
{
	GICR &= ~(1 << INT2);
	MCUCSR &= ~(1 << ISC2);
	GIFR |= 1 << INTF2;
	GICR |= (1 << INT2);
	sei();
	
	Machine_SetState(sMachineStateFightStop++);
}

bool StartModule_HasWrongState(void)
{
	return sStartModuleWrongState;
}

/******************************************************************************
                        PRIVATE FUNCTION IMPLEMENTATION
******************************************************************************/

static void CheckStartModuleState(void)
{
	if(Gpio_ReadPin(STARTMODULE_PORT, STARTMODULE_PIN))
	{
		sStartModuleWrongState = true;
	}
	else
	{
		sStartModuleWrongState = false;
	}
}

static void EnableInt2(void)
{
	GICR |= (1 << INT2);
}

static void DisableInt2(void)
{
	GICR &= ~(1 << INT2);
}

static void ClearInt2Flag(void)
{
	GIFR |= 1 << INTF2;
}

// static void SetInt2AsFallingEdge(void)
// {
// 	MCUCSR &= ~(1 << ISC2);
// }

static void SetInt2AsRisingEdge(void)
{	
	MCUCSR |= 1 << ISC2;		
}

static void EnableGlobalInt(void)
{
	sei();
}
