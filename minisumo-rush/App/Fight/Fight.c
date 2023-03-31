/*============================================================================*\
*
* Copyright Karol Domanski, All Rights Reserved
* It is not allowed to reproduce or utilize parts of this document in any form
* or by any means, including photocopying and microfilm, without permission in
* written by Karol Domanski.
*
\*============================================================================*/

#include "Fight.h"

/*****************************************************************************
                     PRIVATE STRUCTS / ENUMS / VARIABLES
*****************************************************************************/

#define TASK_NAME "Fight"

#define INT_RESOLUTION_MULTIPLIER 10

static uint8_t sActiveSensorsCnt;

static uint32_t sBackSensorActivityTimeMillis;
static uint32_t sLastCourseTimeoutMillis;
static uint32_t sFrontTargettingTimeoutMillis;

static int32_t sCourse;
static int32_t sLastCourse;
static int32_t sDefaultFightSpeed;

static Fight_MachineState_t sCurrentMachineState;
static Fight_MachineState_t sLastMachineState;

static PID_Instance_t sPid;

static char * sMachineStateString[] =
{
	"ExecuteTactic",
	"Loop",
	"Line",
	"Back"
};

static int8_t sDistanceSensorsFrontWeight[DIST_SENS_FRONT_NBR] =
{
	DISTANCE_SENSOR_LEFT_WEIGHT,
	DISTANCE_SENSOR_LEFT_DIAG_WEIGHT,
	DISTANCE_SENSOR_FRONT_WEIGHT,
	DISTANCE_SENSOR_RIGHT_DIAG_WEIGHT,
	DISTANCE_SENSOR_RIGHT_WEIGHT
};

/*****************************************************************************
                         PRIVATE FUNCTION DECLARATION
*****************************************************************************/

static void Loop(void);

/*****************************************************************************
                           INTERFACE IMPLEMENTATION
*****************************************************************************/

void Fight_Init(void)
{
	sPid.scale = INT_RESOLUTION_MULTIPLIER;

	sPid.setpoint = 0;
	sPid.difference = 0;
	sPid.lastError = 0;
	sPid.derivative = 0;
	sPid.integral = 0;
	sPid.proportional = 0;
	sPid.derivativeOut = 0;
	sPid.integralOut = 0;
	sPid.proportionalOut = 0;
	
	sPid.kpVal = (int32_t)FIGHT_PID_KP_VALUE;
	sPid.kiVal = (int32_t)FIGHT_PID_KI_VALUE;
	sPid.kdVal = (int32_t)FIGHT_PID_KD_VALUE;
	
	sPid.integralWindup = (int32_t)FIGHT_PID_INTEGRAL_WINDUP;

	sPid.outputMin = (int32_t)FIGHT_PID_OUTPUT_MIN; //(int32_t)FIGHT_PID_OUTPUT_MIN - (int32_t)FIGHT_MOTOR_DEFAULT_SPEED;
	sPid.outputMax = (int32_t)FIGHT_PID_OUTPUT_MAX; //(int32_t)FIGHT_PID_OUTPUT_MAX - (int32_t)FIGHT_MOTOR_DEFAULT_SPEED;
	
	sDefaultFightSpeed = (int32_t)FIGHT_MOTOR_DEFAULT_SPEED;
}

void Fight_Handler(void)
{
	switch(sCurrentMachineState)
	{
		case FIGHT_MACHINE_STATE_EXECUTE_TACTIC:
		{
			Tactic_Handler();
			
			break;
		}
		
		case FIGHT_MACHINE_STATE_LOOP:
		{
			Loop();
			
			if(Distance_GetNbrOfActFrontSensors() == 0)
			{
				Distance_Output_t distance = Distance_GetFrontStateArray();
				Line_Output_t line = Line_GetState();
				
				if(distance.back[DIST_SENS_BACK] == false)
				{
					sBackSensorActivityTimeMillis = Time_GetMillis();
				}
								
				if(distance.back[DIST_SENS_BACK] == true)
				{
					if(Time_GetMillis() - sBackSensorActivityTimeMillis >= BACK_SENSOR_BACK_ACTIVITY_TIME_MILLIS)
					{
						Back_SetState(BACK_MACHINE_STATE_REVERSE);
						Fight_SetState(FIGHT_MACHINE_STATE_BACK);
					}
				}
				else if(line.state[LINE_SENS_FRONT_LEFT] == true
				|| line.state[LINE_SENS_FRONT_RIGHT] == true)
				{
					Edge_SetState(EDGE_MACHINE_STATE_ESTIMATE_ANGLE);
					Fight_SetState(FIGHT_MACHINE_STATE_LINE);
				}		
			}
						
			break;
		}
		
		case FIGHT_MACHINE_STATE_LINE:
		{
			Edge_Handler();
			
			break;
		}
		
		case FIGHT_MACHINE_STATE_BACK:
		{
			Back_Handler();
			
			break;
		}
		
		default:
		{
			break;
		}
	}
}

void Fight_SetState(Fight_MachineState_t machineState)
{
	sLastMachineState = sCurrentMachineState;
	sCurrentMachineState = machineState;
	
	switch(sCurrentMachineState)
	{
		case FIGHT_MACHINE_STATE_EXECUTE_TACTIC:
		{
			break;
		}
		
		case FIGHT_MACHINE_STATE_LOOP:
		{
			sBackSensorActivityTimeMillis = Time_GetMillis();	
			
			break;
		}
		
		case FIGHT_MACHINE_STATE_LINE:
		{
			break;
		}
		
		case FIGHT_MACHINE_STATE_BACK:
		{
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

static void Loop(void)
{
	Distance_Output_t output = Distance_GetFrontStateArray();
	
	if(output.front[DIST_SENS_FRONT] == false)
	{
		sFrontTargettingTimeoutMillis = Time_GetMillis();	
	}
	
	sActiveSensorsCnt = (uint8_t)output.front[DIST_SENS_LEFT]
	+ (uint8_t)output.front[DIST_SENS_LEFT_DIAG]
	+ (uint8_t)output.front[DIST_SENS_FRONT]
	+ (uint8_t)output.front[DIST_SENS_RIGHT_DIAG]
	+ (uint8_t)output.front[DIST_SENS_RIGHT];
	
	sCourse = (int32_t)((uint8_t)output.front[DIST_SENS_LEFT] * sDistanceSensorsFrontWeight[DIST_SENS_LEFT])
	+ (int32_t)((uint8_t)output.front[DIST_SENS_LEFT_DIAG] * sDistanceSensorsFrontWeight[DIST_SENS_LEFT_DIAG])
	+ (int32_t)((uint8_t)output.front[DIST_SENS_FRONT] * sDistanceSensorsFrontWeight[DIST_SENS_FRONT])
	+ (int32_t)((uint8_t)output.front[DIST_SENS_RIGHT_DIAG] * sDistanceSensorsFrontWeight[DIST_SENS_RIGHT_DIAG])
	+ (int32_t)((uint8_t)output.front[DIST_SENS_RIGHT] * sDistanceSensorsFrontWeight[DIST_SENS_RIGHT]);

	if(sActiveSensorsCnt > 0U)
	{
		sCourse = (sCourse * (int32_t)sPid.scale) / (int32_t)sActiveSensorsCnt;
		sLastCourse = sCourse;
		
		sLastCourseTimeoutMillis = Time_GetMillis();
	}
	else
	{
		if(Time_GetMillis() - sLastCourseTimeoutMillis >= FIGHT_USING_LAST_COURSE_EXECUTION_TIME_MILLIS)
		{
			sCourse = 0;
			sPid.integral = 0;
			sPid.derivative = 0;
			sPid.lastError = 0;
		}
		else
		{
			sCourse = sLastCourse;	
		}	
	}
	
	if(sActiveSensorsCnt > 0U
		&& sCourse == 0)
	{
		sDefaultFightSpeed = (int32_t)FIGHT_MAX_ATTACK_SPEED;
		sPid.integral = 0;
		sPid.derivative = 0;
		sPid.lastError = 0;
	}
	else if(Time_GetMillis() - sFrontTargettingTimeoutMillis >= FIGHT_FRONT_TARGETTING_TIMEOUT_MILLIS)
	{
		sDefaultFightSpeed = (int32_t)FIGHT_MAX_ATTACK_SPEED;
		sCourse = 0;
		sPid.integral = 0;
		sPid.derivative = 0;
		sPid.lastError = 0;	
	}
	else
	{
		sDefaultFightSpeed = (int32_t)FIGHT_MOTOR_DEFAULT_SPEED;
	}
	
	int32_t pidOut = ComputePidOutput(&sPid, sCourse);

	Terminal_Log(TASK_NAME, LOG_OK, "L: %ld R: %ld Err: %ld DefSp: %ld PidOut: %ld", sDefaultFightSpeed + pidOut, sDefaultFightSpeed - pidOut, sCourse, sDefaultFightSpeed, pidOut);
	
// 	Motor_Run(MOTOR_ID_LEFT, 0);
// 	Motor_Run(MOTOR_ID_RIGHT, 0);

	Motor_Run(MOTOR_ID_LEFT, (int16_t)(sDefaultFightSpeed + pidOut));
	Motor_Run(MOTOR_ID_RIGHT, (int16_t)(sDefaultFightSpeed - pidOut));
}

//sDefaultFightSpeed set it to 255 when sensors see