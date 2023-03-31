/*============================================================================*\
*
* Copyright Karol Domanski, All Rights Reserved
* It is not allowed to reproduce or utilize parts of this document in any form
* or by any means, including photocopying and microfilm, without permission in
* written by Karol Domanski.
*
\*============================================================================*/

#include "Back.h"

/*****************************************************************************
                     PRIVATE STRUCTS / ENUMS / VARIABLES
*****************************************************************************/

#define TASK_NAME "Back"

static uint32_t sFullRotationTimeMillis;

static Back_MachineState_t sCurrentMachineState;
static Back_MachineState_t sLastMachineState;

static char * sMachineStateString[] =
{
	"Reverse"
};

/*****************************************************************************
                           INTERFACE IMPLEMENTATION
*****************************************************************************/

void Back_Handler(void)
{
	switch(sCurrentMachineState)
	{
		case BACK_MACHINE_STATE_REVERSE:
		{
			if(Time_GetMillis() - sFullRotationTimeMillis >= BACK_HALF_TURN_DURATION_TIME_MILLIS)
			{
				Motor_Run(MOTOR_ID_LEFT, MOTOR_STOP_SPEED);
				Motor_Run(MOTOR_ID_RIGHT, MOTOR_STOP_SPEED);
				
				Fight_SetState(FIGHT_MACHINE_STATE_LOOP);	
			}
			
			break;
		}
		
		default:
		{
			break;
		}	
	}
}

void Back_SetState(Back_MachineState_t machineState)
{
	sLastMachineState = sCurrentMachineState;
	sCurrentMachineState = machineState;
	
	switch(sCurrentMachineState)
	{
		case BACK_MACHINE_STATE_REVERSE:
		{
			Motor_Run(MOTOR_ID_LEFT, -BACK_HALF_TURN_ROTATE_SPEED);
			Motor_Run(MOTOR_ID_RIGHT, BACK_HALF_TURN_ROTATE_SPEED);
								
			sFullRotationTimeMillis = Time_GetMillis();
			
			break;
		}
		
		default:
		{
			break;
		}
	}
	
	Terminal_Log(TASK_NAME, LOG_INFO, "Change state from [%s] to [%s].",
	sMachineStateString[sLastMachineState],
	sMachineStateString[sCurrentMachineState]);
}