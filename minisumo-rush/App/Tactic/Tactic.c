/*============================================================================*\
*
* Copyright Karol Domanski, All Rights Reserved
* It is not allowed to reproduce or utilize parts of this document in any form
* or by any means, including photocopying and microfilm, without permission in
* written by Karol Domanski.
*
\*============================================================================*/

#include "Tactic.h"

/*****************************************************************************
                     PRIVATE STRUCTS / ENUMS / VARIABLES
*****************************************************************************/

#define TASK_NAME "Tactic"

static bool sIsTestMode;

static uint32_t sFightTacticTimestampMillis;

static Tactic_MachineState_t sCurrentMachineState;
static Tactic_MachineState_t sLastMachineState;

static char * sMachineStateString[] =
{
	"RotatingCcw",
	"RotateCcwAndGo",
	"FullRotateGo",
	"RotateCwAndGo",
	"RotatingCw",
	"RotateAndWait",
	"Go",
	"Standby"
};

/*****************************************************************************
                         PRIVATE FUNCTION DECLARATION
*****************************************************************************/

static void StopExecutingTacticWhenOponentDetected(void);

/*****************************************************************************
                           INTERFACE IMPLEMENTATION
*****************************************************************************/

void Tactic_Handler(void)
{
	switch(sCurrentMachineState)
	{
		case TACTIC_MACHINE_STATE_ROTATING_CCW:
		{
			StopExecutingTacticWhenOponentDetected();
			
			//todo: timeout for rotating in place?
			
			break;
		}
		
		case TACTIC_MACHINE_STATE_ROTATE_CCW_AND_GO:
		{
			if(Time_GetMillis() - sFightTacticTimestampMillis >= TACTIC_ROTATE_AND_GO_CCW_ROTATING_TIME_MILLIS)
			{
				//todo: hard break before?
				
				if(Tactic_IsTestMode() == true)
				{
					Motor_Run(MOTOR_ID_LEFT, MOTOR_STOP_SPEED);
					Motor_Run(MOTOR_ID_RIGHT, MOTOR_STOP_SPEED);
					
					while(1)
					{
						//dummy
					}
				}
				else
				{
					Motor_Run(MOTOR_ID_LEFT, TACTIC_SPEED_AFTER_ROTATATION);
					Motor_Run(MOTOR_ID_RIGHT, TACTIC_SPEED_AFTER_ROTATATION);
				}

				Fight_SetState(FIGHT_MACHINE_STATE_LOOP);
			}
			
			StopExecutingTacticWhenOponentDetected();
			
			break;
		}
		
		case TACTIC_MACHINE_STATE_FULL_ROTATE_AND_GO:
		{
			if(Time_GetMillis() - sFightTacticTimestampMillis >= TACTIC_ROTATE_AND_GO_ROTATING_TIME_MILLIS)
			{
				//todo: hard break before?
				
				if(Tactic_IsTestMode() == true)
				{
					Motor_Run(MOTOR_ID_LEFT, MOTOR_STOP_SPEED);
					Motor_Run(MOTOR_ID_RIGHT, MOTOR_STOP_SPEED);
					
					while(1)
					{
						//dummy
					}
				}
				else
				{
					Motor_Run(MOTOR_ID_LEFT, TACTIC_SPEED_AFTER_ROTATATION);
					Motor_Run(MOTOR_ID_RIGHT, TACTIC_SPEED_AFTER_ROTATATION);
				}
				
				Fight_SetState(FIGHT_MACHINE_STATE_LOOP);
			}
			
			break;
		}
		
		case TACTIC_MACHINE_STATE_ROTATE_CW_AND_GO:
		{
			if(Time_GetMillis() - sFightTacticTimestampMillis >= TACTIC_ROTATE_AND_GO_CW_ROTATING_TIME_MILLIS)
			{
				//todo: hard break before?
				
				if(Tactic_IsTestMode() == true)
				{
					Motor_Run(MOTOR_ID_LEFT, MOTOR_STOP_SPEED);
					Motor_Run(MOTOR_ID_RIGHT, MOTOR_STOP_SPEED);
					
					while(1)
					{
						//dummy
					}
				}
				else
				{
					Motor_Run(MOTOR_ID_LEFT, TACTIC_SPEED_AFTER_ROTATATION);
					Motor_Run(MOTOR_ID_RIGHT, TACTIC_SPEED_AFTER_ROTATATION);
				}
				
				Fight_SetState(FIGHT_MACHINE_STATE_LOOP);
			}
			
			StopExecutingTacticWhenOponentDetected();
			
			break;
		}
		
		case TACTIC_MACHINE_STATE_ROTATING_CW:
		{
			StopExecutingTacticWhenOponentDetected();
			
			//todo: timeout for rotating in place?
			
			break;
		}
		
		case TACTIC_MACHINE_STATE_ROTATE_AND_WAIT:
		{
			if(Time_GetMillis() - sFightTacticTimestampMillis >= TACTIC_ROTATE_AND_WAIT_ROTATING_TIME_MILLIS)
			{
				Motor_Run(MOTOR_ID_LEFT, MOTOR_STOP_SPEED);
				Motor_Run(MOTOR_ID_RIGHT, MOTOR_STOP_SPEED);
				
				if(Tactic_IsTestMode() == false)
				{
					Tactic_SetState(TACTIC_MACHINE_STATE_STANDBY);	
				}
			}
			
			break;
		}
		
		case TACTIC_MACHINE_STATE_GO:
		{
			Fight_SetState(FIGHT_MACHINE_STATE_LOOP);
					
			break;
		}

		case TACTIC_MACHINE_STATE_STANDBY:
		{
			if(Time_GetMillis() - sFightTacticTimestampMillis >= MAXIMUM_TIME_WITHOUT_MOVEMENT_MILLIS)
			{
				Motor_Run(MOTOR_ID_LEFT, TACTIC_SPEED_AFTER_ROTATATION);
				Motor_Run(MOTOR_ID_RIGHT, TACTIC_SPEED_AFTER_ROTATATION);
				
				Fight_SetState(FIGHT_MACHINE_STATE_LOOP);
			}
			
			StopExecutingTacticWhenOponentDetected();
			
			break;
		}
		
		default:
		{
			break;
		}
	}
}

void Tactic_SetState(Tactic_MachineState_t machineState)
{
	sLastMachineState = sCurrentMachineState;
	sCurrentMachineState = machineState;
	
	switch(sCurrentMachineState)
	{
		case TACTIC_MACHINE_STATE_ROTATING_CCW:
		{
			Motor_Run(MOTOR_ID_LEFT, -TACTIC_ROTATING_SPEED);
			Motor_Run(MOTOR_ID_RIGHT, TACTIC_ROTATING_SPEED);
				
			break;
		}
		
		case TACTIC_MACHINE_STATE_ROTATE_CCW_AND_GO:
		{
			Motor_Run(MOTOR_ID_LEFT, -TACTIC_ROTATING_AND_GO_SPEED);
			Motor_Run(MOTOR_ID_RIGHT, TACTIC_ROTATING_AND_GO_SPEED);
			
			sFightTacticTimestampMillis = Time_GetMillis();	
			
			break;
		}
		
		case TACTIC_MACHINE_STATE_FULL_ROTATE_AND_GO:
		{
			Motor_Run(MOTOR_ID_LEFT, -TACTIC_ROTATING_AND_GO_SPEED);
			Motor_Run(MOTOR_ID_RIGHT, TACTIC_ROTATING_AND_GO_SPEED);
			
			sFightTacticTimestampMillis = Time_GetMillis();
			
			break;
		}
		
		case TACTIC_MACHINE_STATE_ROTATE_CW_AND_GO:
		{
			Motor_Run(MOTOR_ID_LEFT, TACTIC_ROTATING_AND_GO_SPEED);
			Motor_Run(MOTOR_ID_RIGHT, -TACTIC_ROTATING_AND_GO_SPEED);
			
			sFightTacticTimestampMillis = Time_GetMillis();	
			
			break;
		}
		
		case TACTIC_MACHINE_STATE_ROTATING_CW:
		{
			Motor_Run(MOTOR_ID_LEFT, TACTIC_ROTATING_AND_GO_SPEED);
			Motor_Run(MOTOR_ID_RIGHT, -TACTIC_ROTATING_AND_GO_SPEED);
			
			break;
		}
		
		case TACTIC_MACHINE_STATE_ROTATE_AND_WAIT:
		{
			Motor_Run(MOTOR_ID_LEFT, -TACTIC_ROTATE_AND_WAIT_SPEED);
			Motor_Run(MOTOR_ID_RIGHT, TACTIC_ROTATE_AND_WAIT_SPEED);
			
			sFightTacticTimestampMillis = Time_GetMillis();	
			
			break;
		}
		
		case TACTIC_MACHINE_STATE_GO:
		{
			Motor_Run(MOTOR_ID_LEFT, TACTIC_SPEED_AFTER_ROTATATION);
			Motor_Run(MOTOR_ID_RIGHT, TACTIC_SPEED_AFTER_ROTATATION);
			
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

void Tactic_RunTestMode(void)
{
	sIsTestMode	= true;
}

bool Tactic_IsTestMode(void)
{
	return sIsTestMode;
}

/******************************************************************************
                        PRIVATE FUNCTION IMPLEMENTATION
******************************************************************************/

static void StopExecutingTacticWhenOponentDetected(void)
{
	Distance_Output_t distance = Distance_GetFrontStateArray();
	if(distance.front[DIST_SENS_LEFT] == true
	|| distance.front[DIST_SENS_LEFT_DIAG] == true
	|| distance.front[DIST_SENS_FRONT] == true
	|| distance.front[DIST_SENS_RIGHT_DIAG] == true
	|| distance.front[DIST_SENS_RIGHT] == true)
	{
		//todo: hard break before?
		
		Fight_SetState(FIGHT_MACHINE_STATE_LOOP);
	}
}