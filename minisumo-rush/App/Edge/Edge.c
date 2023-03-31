/*============================================================================*\
*
* Copyright Karol Domanski, All Rights Reserved
* It is not allowed to reproduce or utilize parts of this document in any form
* or by any means, including photocopying and microfilm, without permission in
* written by Karol Domanski.
*
\*============================================================================*/

#include "Edge.h"

/*****************************************************************************
                     PRIVATE STRUCTS / ENUMS / VARIABLES
*****************************************************************************/

#define TASK_NAME "Edge"

static bool sLineDetectedLatch[LINE_SENS_NBR];

static uint32_t sDifferenceBetweenDetectionMillis[LINE_SENS_NBR];
static uint32_t sEstimateLineAngleTimestampMillis;
static uint32_t sRotationAngleAsTimeMillis;
static uint32_t sDifferenceBetweenDetection;

static Edge_MachineState_t sCurrentMachineState;
static Edge_MachineState_t sLastMachineState;

static char * sMachineStateString[] =
{
	"EstimateAngle",
	"ReverseAndRot",
	"Return"
};

/*****************************************************************************
                           INTERFACE IMPLEMENTATION
*****************************************************************************/

void Edge_Handler(void)
{
	switch(sCurrentMachineState)
	{
		case EDGE_MACHINE_STATE_ESTIMATE_ANGLE:
		{
			Line_Output_t line = Line_GetState();
			
			if(line.state[LINE_SENS_FRONT_LEFT] == true
			&& sLineDetectedLatch[LINE_SENS_FRONT_LEFT] == false)
			{
				sLineDetectedLatch[LINE_SENS_FRONT_LEFT] = true;
				
				sDifferenceBetweenDetectionMillis[LINE_SENS_FRONT_LEFT] = Time_GetMillis();
			}
			
			if(line.state[LINE_SENS_FRONT_RIGHT] == true
			&& sLineDetectedLatch[LINE_SENS_FRONT_RIGHT] == false)
			{
				sLineDetectedLatch[LINE_SENS_FRONT_RIGHT] = true;
				
				sDifferenceBetweenDetectionMillis[LINE_SENS_FRONT_RIGHT] = Time_GetMillis();
			}
			
			if(Time_GetMillis() - sEstimateLineAngleTimestampMillis >= EDGE_ESTIMATING_LINE_ANGLE_DELAY_MILLIS)
			{
				sDifferenceBetweenDetection = (uint32_t)abs((int32_t)(
				sDifferenceBetweenDetectionMillis[LINE_SENS_FRONT_LEFT]
				- sDifferenceBetweenDetectionMillis[LINE_SENS_FRONT_RIGHT]));
				
				Edge_SetState(EDGE_MACHINE_STATE_REVERSE_AND_ROTATE);
			}
			
			break;
		}

		case EDGE_MACHINE_STATE_REVERSE_AND_ROTATE:
		{
			if(Time_GetMillis() - sEstimateLineAngleTimestampMillis >= EDGE_REVERSE_TIME_MILLIS)
			{
				if(sLineDetectedLatch[LINE_SENS_FRONT_LEFT] == true
				&& sLineDetectedLatch[LINE_SENS_FRONT_RIGHT] == true)
				{
					Motor_Run(MOTOR_ID_LEFT, (int16_t)-EDGE_ROTATE_AFTER_REVERSAL_SPEED);
					Motor_Run(MOTOR_ID_RIGHT, (int16_t)EDGE_ROTATE_AFTER_REVERSAL_SPEED);
					
					sRotationAngleAsTimeMillis = EDGE_FRONT_LEFT_RIGHT_ROTATE_DURATION_MILLIS;// - sDifferenceBetweenDetection; todo: check it
				}
				else if(sLineDetectedLatch[LINE_SENS_FRONT_LEFT] == true
				&& sLineDetectedLatch[LINE_SENS_FRONT_RIGHT] == false)
				{
					Motor_Run(MOTOR_ID_LEFT, (int16_t)EDGE_ROTATE_AFTER_REVERSAL_SPEED);
					Motor_Run(MOTOR_ID_RIGHT, (int16_t)-EDGE_ROTATE_AFTER_REVERSAL_SPEED);
					
					sRotationAngleAsTimeMillis = EDGE_FRONT_LEFT_ROTATE_DURATION_MILLIS;
				}
				else if(sLineDetectedLatch[LINE_SENS_FRONT_LEFT] == false
				&& sLineDetectedLatch[LINE_SENS_FRONT_RIGHT] == true)
				{
					Motor_Run(MOTOR_ID_LEFT, (int16_t)-EDGE_ROTATE_AFTER_REVERSAL_SPEED);
					Motor_Run(MOTOR_ID_RIGHT, (int16_t)EDGE_ROTATE_AFTER_REVERSAL_SPEED);
					
					sRotationAngleAsTimeMillis = EDGE_FRONT_RIGHT_ROTATE_DURATION_MILLIS;
				}
				else
				{
					Motor_Run(MOTOR_ID_LEFT, (int16_t)-EDGE_ROTATE_AFTER_REVERSAL_SPEED);
					Motor_Run(MOTOR_ID_RIGHT, (int16_t)EDGE_ROTATE_AFTER_REVERSAL_SPEED);
					
					sRotationAngleAsTimeMillis = EDGE_FRONT_LEFT_RIGHT_ROTATE_DURATION_MILLIS;
				}
				
				Edge_SetState(EDGE_MACHINE_STATE_RETURN);
			}
			
			break;
		}
		
		case EDGE_MACHINE_STATE_RETURN:
		{
			if(Time_GetMillis() - sEstimateLineAngleTimestampMillis >= sRotationAngleAsTimeMillis)
			{
				Motor_Run(MOTOR_ID_LEFT, MOTOR_STOP_SPEED);
				Motor_Run(MOTOR_ID_RIGHT, MOTOR_STOP_SPEED);
				
				sLineDetectedLatch[LINE_SENS_FRONT_LEFT] = false;
				sLineDetectedLatch[LINE_SENS_FRONT_RIGHT] = false;
								
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

void Edge_SetState(Edge_MachineState_t machineState)
{
	sLastMachineState = sCurrentMachineState;
	sCurrentMachineState = machineState;
	
	switch(sCurrentMachineState)
	{
		case EDGE_MACHINE_STATE_ESTIMATE_ANGLE:
		{
			sEstimateLineAngleTimestampMillis = Time_GetMillis();
			
			break;
		}
		
		case EDGE_MACHINE_STATE_REVERSE_AND_ROTATE:
		{
			Motor_Run(MOTOR_ID_LEFT, (int16_t)EDGE_REVERSAL_SPEED_FROM_LINE);
			Motor_Run(MOTOR_ID_RIGHT, (int16_t)EDGE_REVERSAL_SPEED_FROM_LINE);
			
			sEstimateLineAngleTimestampMillis = Time_GetMillis();
			
			break;
		}
		
		case EDGE_MACHINE_STATE_RETURN:
		{
			sEstimateLineAngleTimestampMillis = Time_GetMillis();
			
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

/******************************************************************************
                        PRIVATE FUNCTION IMPLEMENTATION
******************************************************************************/