/*============================================================================*\
*
* Copyright Karol Domanski, All Rights Reserved
* It is not allowed to reproduce or utilize parts of this document in any form
* or by any means, including photocopying and microfilm, without permission in
* written by Karol Domanski.
*
\*============================================================================*/

#include "Distance.h"

/*****************************************************************************
                     PRIVATE STRUCTS / ENUMS / VARIABLES
*****************************************************************************/

#define TASK_NAME "Distance"
#define TASK_PERIOD_MILLIS 5U

static Distance_Sensor_t sSensorsFront[DIST_SENS_FRONT_NBR] =
{
	{ .port = DISTANCE_SENSOR_LEFT_PORT,		.pin = DISTANCE_SENSOR_LEFT_PIN,		.enable = DISTANCE_SENSOR_LEFT_ENABLE },
	{ .port = DISTANCE_SENSOR_LEFT_DIAG_PORT,	.pin = DISTANCE_SENSOR_LEFT_DIAG_PIN,	.enable = DISTANCE_SENSOR_LEFT_DIAG_ENABLE },
	{ .port = DISTANCE_SENSOR_FRONT_PORT,		.pin = DISTANCE_SENSOR_FRONT_PIN,		.enable = DISTANCE_SENSOR_FRONT_ENABLE },
	{ .port = DISTANCE_SENSOR_RIGHT_DIAG_PORT,	.pin = DISTANCE_SENSOR_RIGHT_DIAG_PIN,	.enable = DISTANCE_SENSOR_RIGHT_DIAG_ENABLE },
	{ .port = DISTANCE_SENSOR_RIGHT_PORT,		.pin = DISTANCE_SENSOR_RIGHT_PIN,		.enable = DISTANCE_SENSOR_RIGHT_ENABLE }
};

static Distance_Sensor_t sSensorsBack[DIST_SENS_BACK_NBR] =
{
	{ .port = DISTANCE_SENSOR_BACK_PORT,		.pin = DISTANCE_SENSOR_BACK_PIN,		.enable = DISTANCE_SENSOR_BACK_ENABLE },
};

static Distance_Output_t sSensorsOutput;

/*****************************************************************************
                         PRIVATE FUNCTION DECLARATION
*****************************************************************************/

static void TaskHandler(void);

/*****************************************************************************
                           INTERFACE IMPLEMENTATION
*****************************************************************************/

void Distance_Init(void)
{
	for(uint8_t idx = 0U; idx < DIST_SENS_FRONT_NBR; idx++)
	{
		Gpio_Init(sSensorsFront[idx].port, sSensorsFront[idx].pin, GPIO_TYPE_INPUT);
	}
	Gpio_Init(sSensorsBack[DIST_SENS_BACK].port, sSensorsBack[DIST_SENS_BACK].pin, GPIO_TYPE_INPUT);
	
	sSensorsFront[DIST_SENS_LEFT].portReg = &PINC;
	sSensorsFront[DIST_SENS_LEFT_DIAG].portReg = &PINA;
	sSensorsFront[DIST_SENS_FRONT].portReg = &PINA;
	sSensorsFront[DIST_SENS_RIGHT_DIAG].portReg = &PINA;
	sSensorsFront[DIST_SENS_RIGHT].portReg = &PINA;
	sSensorsBack[DIST_SENS_BACK].portReg = &PIND;
	
	Scheduler_AddTask(TASK_NAME, TASK_PERIOD_MILLIS, TaskHandler, SCHEDULER_OPLVL_NORMAL);
	
	Terminal_Log("Distance", LOG_OK, "Configured.");
}

Distance_Output_t Distance_GetFrontStateArray(void)
{
	return sSensorsOutput;
}

uint8_t Distance_GetNbrOfActFrontSensors(void)
{
	uint8_t activeSensors = ((uint8_t)sSensorsOutput.front[DIST_SENS_LEFT] * (uint8_t)sSensorsBack[DIST_SENS_LEFT].enable)
	+ ((uint8_t)sSensorsOutput.front[DIST_SENS_LEFT_DIAG] * (uint8_t)sSensorsBack[DIST_SENS_LEFT_DIAG].enable)
	+ ((uint8_t)sSensorsOutput.front[DIST_SENS_FRONT] * (uint8_t)sSensorsBack[DIST_SENS_FRONT].enable)
	+ ((uint8_t)sSensorsOutput.front[DIST_SENS_RIGHT_DIAG] * (uint8_t)sSensorsBack[DIST_SENS_RIGHT_DIAG].enable)
	+ ((uint8_t)sSensorsOutput.front[DIST_SENS_RIGHT] * (uint8_t)sSensorsBack[DIST_SENS_RIGHT].enable);
	
	return activeSensors;
}

/******************************************************************************
                        PRIVATE FUNCTION IMPLEMENTATION
******************************************************************************/

static void TaskHandler(void)
{	
	for(uint8_t idx = 0U; idx < DIST_SENS_FRONT_NBR; idx++)
	{
		sSensorsOutput.front[idx] =
			!(*sSensorsFront[idx].portReg
			& (1 << sSensorsFront[idx].pin))
			* (uint8_t)sSensorsFront[idx].enable;
	}
	
	sSensorsOutput.back[DIST_SENS_BACK] = !(*sSensorsBack[DIST_SENS_BACK].portReg
		& (1 << sSensorsBack[DIST_SENS_BACK].pin))
		* (uint8_t)sSensorsBack[DIST_SENS_BACK].enable;
}