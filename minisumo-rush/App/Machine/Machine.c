/*============================================================================*\
*
* Copyright Karol Domanski, All Rights Reserved
* It is not allowed to reproduce or utilize parts of this document in any form
* or by any means, including photocopying and microfilm, without permission in
* written by Karol Domanski.
*
\*============================================================================*/

#include "Machine.h"

/*****************************************************************************
                     PRIVATE STRUCTS / ENUMS / VARIABLES
*****************************************************************************/

#define TASK_NAME "Machine"
#define TASK_PERIOD_MILLIS 5U

/*****************************************************************************
                         PRIVATE FUNCTION DECLARATION
*****************************************************************************/

static bool sLastButtonState;
static bool sButtonState;
static bool sBattLow;

static uint8_t sLedIdx;

static uint32_t sDebounceTimeMillis;
static uint32_t sAcceptMillis;

static Machine_MachineState_t sCurrentMachineState;
static Machine_MachineState_t sLastMachineState;

static Machine_LedMatrice_t sMatrice[7] =
{
	{ .red = true,	.blue = false,	.green = false },
	{ .red = true,	.blue = true,	.green = false },
	{ .red = false,	.blue = true,	.green = false },
	{ .red = false,	.blue = true,	.green = true },
	{ .red = false,	.blue = false,	.green = true },
	{ .red = true,	.blue = false,	.green = true },
	{ .red = false,	.blue = true,	.green = false }
};

static char * sMachineStateString[] =
{
	"CheckBattLvl",
	"CheckStartMod",
	"DevStatInd",
	"TacticsSett",
	"WaitForStart",
	"Fight",
	"Stop"
};

/*****************************************************************************
                         PRIVATE FUNCTION DECLARATION
*****************************************************************************/

static void TaskHandler(void);
static void ButtonHandler(void);
static void LedSignal(bool red, bool green, bool blue);

/*****************************************************************************
                           INTERFACE IMPLEMENTATION
*****************************************************************************/

void Machine_Init(void)
{
	Fight_Init();
		
	Scheduler_AddTask(TASK_NAME, TASK_PERIOD_MILLIS, TaskHandler, SCHEDULER_OPLVL_NORMAL);
	
	Terminal_Log("Machine", LOG_OK, "Configured.");	
}

void Machine_SetState(Machine_MachineState_t machineState)
{
	sLastMachineState = sCurrentMachineState;
	sCurrentMachineState = machineState;
	
	switch(sCurrentMachineState)
	{
		case MACHINE_STATE_WAIT_FOR_START:
		{
			StartModule_InitIrq();
			
			break;
		}
		
		case MACHINE_STATE_TACTICS_SETTING:
		{
			Scheduler_TaskEnable("Battery", false);
			
			break;
		}
		
		case MACHINE_STATE_FIGHT:
		{
			Tactic_SetState(sLedIdx);
			
			Led_Set(LED_COLOR_RED, false);
			Led_Set(LED_COLOR_GREEN, true);
			Led_Set(LED_COLOR_BLUE, false);
			
			break;
		}
		
		case MACHINE_STATE_STOP:
		{
			Motor_Run(MOTOR_ID_LEFT, 0U);
			Motor_Run(MOTOR_ID_RIGHT, 0U);
			
			Led_Set(LED_COLOR_RED, true);
			Led_Set(LED_COLOR_GREEN, false);
			Led_Set(LED_COLOR_BLUE, false);
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

static void TaskHandler(void)
{
	switch(sCurrentMachineState)
	{
		case MACHINE_STATE_CHECK_BATTERY_LEVEL:
		{
			ButtonHandler();
				
			if(sLedIdx >= 1)
			{
				Led_Set(LED_COLOR_RED, false);
				Led_Set(LED_COLOR_GREEN, false);
				Led_Set(LED_COLOR_BLUE, false);
					
				sLedIdx = 0U;
				sLastButtonState = false;
				sButtonState = false;
				sDebounceTimeMillis = 0U;
					
				Machine_SetState(MACHINE_STATE_CHECK_START_MODULE);
					
				break;
			}
				
			Battery_Voltage_t voltage = Battery_GetVoltage();
			if(voltage.initMeasureDone == true)
			{
				if(voltage.adcVoltage < BATTERY_LOW_BATTERY_LEVEL_VOLT
				&& voltage.adcVoltage > BATTERY_CRITICAL_LOW_BATTERY_LEVEL_VOLT)
				{
					sBattLow = true;
						
					Led_Set(LED_COLOR_RED, false);
					Led_Set(LED_COLOR_GREEN, false);
					Led_Set(LED_COLOR_BLUE, false);
						
					Machine_SetState(MACHINE_STATE_CHECK_START_MODULE);
				}
				else if(voltage.adcVoltage <= BATTERY_CRITICAL_LOW_BATTERY_LEVEL_VOLT)
				{
					sBattLow = true;
						
					Led_FlashAndBlink(LED_COLOR_RED, 1000U, 100U, 5U);
				}
				else
				{
					sBattLow = false;
						
					Led_Set(LED_COLOR_RED, false);
					Led_Set(LED_COLOR_GREEN, false);
					Led_Set(LED_COLOR_BLUE, false);
						
					Machine_SetState(MACHINE_STATE_CHECK_START_MODULE);
				}
			}
			break;
		}
			
		case MACHINE_STATE_CHECK_START_MODULE:
		{
			ButtonHandler();
				
			if(sLedIdx >= 1)
			{
				Led_Set(LED_COLOR_RED, false);
				Led_Set(LED_COLOR_GREEN, false);
				Led_Set(LED_COLOR_BLUE, false);
					
				sLedIdx = 0U;
				sLastButtonState = false;
				sButtonState = false;
				sDebounceTimeMillis = 0U;
					
				Machine_SetState(MACHINE_STATE_DEVICE_STATUS_INDICATION);
					
				break;
			}
				
			if(StartModule_HasWrongState() == true)
			{
				Led_ShortBlinkRed();
				Led_ShortBlinkBlue();
				Led_ShortBlinkGreen();
			}
			else if(Tactic_IsTestMode() == true)
			{
				Led_ShortBlinkRed();
// 				Led_ShortBlinkBlue();	
				Led_ShortBlinkGreen();
			}
			else
			{
				Machine_SetState(MACHINE_STATE_DEVICE_STATUS_INDICATION);
			}
			break;
		}
			
		case MACHINE_STATE_DEVICE_STATUS_INDICATION:
		{
			ButtonHandler();
				
			Distance_Output_t distance = Distance_GetFrontStateArray();
			Line_Output_t line = Line_GetState();
			Battery_Voltage_t voltage = Battery_GetVoltage();

			Terminal_Log("Machine", LOG_DEBUG, "<line: fl:%d fr:%d bl:%d br:%d> <dist: l:%d ld:%d f:%d rd:%d r:%d [b:%d]> <batt: %d.%d%dV %d> <start: %d> <exTime: %dms>",
			line.state[LINE_SENS_FRONT_LEFT],
			line.state[LINE_SENS_FRONT_RIGHT],
			line.state[LINE_SENS_BACK_LEFT],
			line.state[LINE_SENS_BACK_RIGHT],
			distance.front[DIST_SENS_LEFT],
			distance.front[DIST_SENS_LEFT_DIAG],
			distance.front[DIST_SENS_FRONT],
			distance.front[DIST_SENS_RIGHT_DIAG],
			distance.front[DIST_SENS_RIGHT],
			distance.back[DIST_SENS_BACK],
			voltage.intPart,
			voltage.decPartFirstDig,
			voltage.decPartSecDig,
			voltage.adcRaw,
			Gpio_ReadPin(STARTMODULE_PORT, STARTMODULE_PIN),
			Scheduler_GetExecutionTimeMillis());

			if(sLedIdx >= 1)
			{
				Led_Set(LED_COLOR_RED, false);
				Led_Set(LED_COLOR_GREEN, false);
				Led_Set(LED_COLOR_BLUE, false);
					
				sLedIdx = 0U;
				sLastButtonState = false;
				sButtonState = false;
				sDebounceTimeMillis = 0U;
					
				Machine_SetState(MACHINE_STATE_TACTICS_SETTING);
					
				break;
			}
				
			if(sBattLow == true)
			{
				Led_ShortBlinkRed();
					
				Distance_Output_t distance = Distance_GetFrontStateArray();
				if(distance.front[DIST_SENS_LEFT] == true
				|| distance.front[DIST_SENS_LEFT_DIAG] == true
				|| distance.front[DIST_SENS_FRONT] == true
				|| distance.front[DIST_SENS_RIGHT_DIAG] == true
				|| distance.front[DIST_SENS_RIGHT] == true
				|| distance.back[DIST_SENS_BACK] == true)
				{
					Led_Set(LED_COLOR_BLUE, true);
				}
				else
				{
					Led_Set(LED_COLOR_BLUE, false);
				}
					
				Line_Output_t line = Line_GetState();
				if(line.state[LINE_SENS_FRONT_LEFT] == true
				|| line.state[LINE_SENS_FRONT_RIGHT] == true
				|| line.state[LINE_SENS_BACK_LEFT] == true
				|| line.state[LINE_SENS_BACK_RIGHT] == true)
				{
					Led_Set(LED_COLOR_GREEN, true);
				}
				else
				{
					Led_Set(LED_COLOR_GREEN, false);
				}
			}
			else
			{
				Led_ShortBlinkGreen();
					
				Distance_Output_t distance = Distance_GetFrontStateArray();
				if(distance.front[DIST_SENS_LEFT] == true
				|| distance.front[DIST_SENS_LEFT_DIAG] == true
				|| distance.front[DIST_SENS_FRONT] == true
				|| distance.front[DIST_SENS_RIGHT_DIAG] == true
				|| distance.front[DIST_SENS_RIGHT] == true
				|| distance.back[DIST_SENS_BACK] == true)
				{
					Led_Set(LED_COLOR_BLUE, true);
				}
				else
				{
					Led_Set(LED_COLOR_BLUE, false);
				}
					
				Line_Output_t line = Line_GetState();
				if(line.state[LINE_SENS_FRONT_LEFT] == true
				|| line.state[LINE_SENS_FRONT_RIGHT] == true
				|| line.state[LINE_SENS_BACK_LEFT] == true
				|| line.state[LINE_SENS_BACK_RIGHT] == true)
				{
					Led_Set(LED_COLOR_RED, true);
				}
				else
				{
					Led_Set(LED_COLOR_RED, false);
				}
			}
			break;
		}
			
		case MACHINE_STATE_TACTICS_SETTING:
		{
			LedSignal(
			sMatrice[sLedIdx].red,
			sMatrice[sLedIdx].green,
			sMatrice[sLedIdx].blue
			);
				
			ButtonHandler();
				
			break;
		}
			
		case MACHINE_STATE_WAIT_FOR_START:
		{
			Led_ShortBlinkBlue();
			
			break;
		}
			
		case MACHINE_STATE_FIGHT:
		{
			Fight_Handler();
				
			break;
		}
			
		case MACHINE_STATE_STOP:
		{
			Motor_Run(MOTOR_ID_LEFT, 0);
			Motor_Run(MOTOR_ID_RIGHT, 0);
			
			while(1)
			{
				//robot has to be restarted
			}	
			
			break;
		}
	}
}		

static void LedSignal(bool red, bool green, bool blue)
{
	Led_Set(LED_COLOR_RED, red);
	Led_Set(LED_COLOR_GREEN, green);
	Led_Set(LED_COLOR_BLUE, blue);
}

static void ButtonHandler(void)
{
	bool currentState = Button_GetState();
	
	if(currentState != sLastButtonState)
	{
		sDebounceTimeMillis = Time_GetMillis();
	}
	
	if(currentState == false)
	{
		sAcceptMillis = Time_GetMillis();	
	}
	
	if(Time_GetMillis() - sAcceptMillis > MACHINE_BUTTON_ACCEPT_TIME_MILLIS
		&& sCurrentMachineState > MACHINE_STATE_DEVICE_STATUS_INDICATION)
	{
		Led_Set(LED_COLOR_RED, false);
		Led_Set(LED_COLOR_GREEN, false);
		Led_Set(LED_COLOR_BLUE, false);
		
		Machine_SetState(MACHINE_STATE_WAIT_FOR_START);
		
		return;
	}
	
	if(Time_GetMillis() - sDebounceTimeMillis > MACHINE_BUTTON_DEBOUNCE_MILLIS)
	{
		if(currentState != sButtonState)
		{
			sButtonState = currentState;
			
			if(sButtonState == false)
			{
				sAcceptMillis = Time_GetMillis();
				
				sLedIdx++;
				
				if(sLedIdx >= 7U)
				{
					sLedIdx = 0U;
				}
			}
		}
	}
	
	sLastButtonState = currentState;
}

