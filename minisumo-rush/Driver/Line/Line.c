#include "Line.h"

#define TASK_NAME "Line"
#define TASK_PERIOD_MILLIS 5U

static Line_Sensor_t sSensorsFront[LINE_SENS_NBR] =
{
	{ .port = LINE_SENSOR_FRONT_LEFT_PORT,	.pin = LINE_SENSOR_FRONT_LEFT_PIN,	.enable = LINE_SENSOR_FRONT_LEFT_ENABLE },
	{ .port = LINE_SENSOR_FRONT_RIGHT_PORT,	.pin = LINE_SENSOR_FRONT_RIGHT_PIN,	.enable =  LINE_SENSOR_FRONT_RIGHT_ENABLE },
	{ .port = LINE_SENSOR_BACK_LEFT_PORT,	.pin = LINE_SENSOR_BACK_LEFT_PIN,	.enable =  LINE_SENSOR_BACK_LEFT_ENABLE },
	{ .port = LINE_SENSOR_BACK_RIGHT_PORT,	.pin = LINE_SENSOR_BACK_RIGHT_PIN,	.enable =  LINE_SENSOR_BACK_RIGHT_ENABLE }
};

static Line_Output_t sSensorsOutput;

/*****************************************************************************
                         PRIVATE FUNCTION DECLARATION
*****************************************************************************/

static void TaskHandler(void);

/*****************************************************************************
                           INTERFACE IMPLEMENTATION
*****************************************************************************/

void Line_Init(void)
{
	for(uint8_t idx = 0U; idx < LINE_SENS_NBR; idx++)
	{
		Gpio_Init(sSensorsFront[idx].port, sSensorsFront[idx].pin, GPIO_TYPE_INPUT);
	}
	
	sSensorsFront[LINE_SENS_FRONT_LEFT].portReg = &PINC;
	sSensorsFront[LINE_SENS_FRONT_RIGHT].portReg = &PINB;
	sSensorsFront[LINE_SENS_BACK_LEFT].portReg = &PINC;
	sSensorsFront[LINE_SENS_BACK_RIGHT].portReg = &PINB;
	
	Scheduler_AddTask(TASK_NAME, TASK_PERIOD_MILLIS, TaskHandler, SCHEDULER_OPLVL_NORMAL);
	
	Terminal_Log(TASK_NAME, LOG_OK, "Configured.");
}

Line_Output_t Line_GetState(void)
{
	return sSensorsOutput;
}

/******************************************************************************
                        PRIVATE FUNCTION IMPLEMENTATION
******************************************************************************/

static void TaskHandler(void)
{	
	for(uint8_t idx = 0U; idx < LINE_SENS_NBR; idx++)
	{
		sSensorsOutput.state[idx] = !(*sSensorsFront[idx].portReg & (1 << sSensorsFront[idx].pin))
		* (uint8_t)sSensorsFront[idx].enable;
	}
}