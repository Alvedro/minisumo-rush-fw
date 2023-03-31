/*============================================================================*\
*
* Copyright Karol Domanski, All Rights Reserved
* It is not allowed to reproduce or utilize parts of this document in any form
* or by any means, including photocopying and microfilm, without permission in
* written by Karol Domanski.
*
\*============================================================================*/

#include "Scheduler.h"
#include "../../Driver/Terminal/Terminal.h"

/*****************************************************************************
                     PRIVATE STRUCTS / ENUMS / VARIABLES
*****************************************************************************/

typedef struct __attribute__((__packed__))
{
  Scheduler_TaskParamType_t taskParam;
  uint32_t nextExecTime;
  uint32_t taskExecTime;
  bool taskEnable;
} Scheduler_TaskStructType_t;

typedef struct __attribute__((__packed__)) 
{
	uint32_t activeTaskNbr;
	Scheduler_OperatingLevel_t OperatingLevel;
	Scheduler_TaskStructType_t taskParamTable[SCHEDULER_TASK_MAX_NBR];
} Scheduler_SystemParamType_t;

static Scheduler_SystemParamType_t sSystemScheduler;

static uint32_t sSchedulerExecutionTimeMillis;;

/*****************************************************************************
                           INTERFACE IMPLEMENTATION
*****************************************************************************/

Scheduler_ErrorType_t Scheduler_AddTask(const char* taskName, uint32_t taskPeriod, void(*handler)(void), Scheduler_OperatingLevel_t operatingLvl)
{
	uint32_t taskIdx;
	
	for(taskIdx = 0U; sSystemScheduler.taskParamTable[taskIdx].taskParam.functionHandler != NULL; taskIdx++)
	{
		if (taskIdx >= SCHEDULER_TASK_MAX_NBR)
		{
			Terminal_Log("Scheduler", LOG_FATAL, "Cannot add task, Max tasks number.");
			
			return SCHEDULER_E_MAX_TASK_NBR;
		}
	}

	sSystemScheduler.taskParamTable[taskIdx].taskEnable = true;
	sSystemScheduler.taskParamTable[taskIdx].nextExecTime = (Time_GetMillis() + taskPeriod);
	sSystemScheduler.taskParamTable[taskIdx].taskParam.name = taskName;
	sSystemScheduler.taskParamTable[taskIdx].taskParam.periodms = taskPeriod;
	sSystemScheduler.taskParamTable[taskIdx].taskParam.functionHandler = handler;
	sSystemScheduler.taskParamTable[taskIdx].taskParam.operatingLevel = operatingLvl;

	sSystemScheduler.activeTaskNbr++;
	
	Terminal_Log("Scheduler", LOG_INFO, "Task added: %s.", taskName);

	return SCHEDULER_E_OK;
}

Scheduler_ErrorType_t Scheduler_TaskEnable(char *taskName, bool enable)
{
  for(uint32_t taskIdx = 0U; taskIdx < sSystemScheduler.activeTaskNbr; taskIdx++)
  {
    if (!strcmp(sSystemScheduler.taskParamTable[taskIdx].taskParam.name, (char*)taskName))
    {
      sSystemScheduler.taskParamTable[taskIdx].taskEnable = enable;
	  
	  Terminal_Log("Scheduler", LOG_INFO, "%s task %s.", taskName, enable ? "enabled" : "disabled");
	  
      return SCHEDULER_E_OK;
    }
  }

  return SCHEDULER_E_UNRECOGNIZED_NAME;
}

Scheduler_ErrorType_t Scheduler_TaskPostpone(char *taskName, uint32_t postponeTimeMillis)
{
	for(uint32_t taskIdx = 0U; taskIdx < sSystemScheduler.activeTaskNbr; taskIdx++)
	{
		if(!strcmp(sSystemScheduler.taskParamTable[taskIdx].taskParam.name, (char*)taskName))
		{
			sSystemScheduler.taskParamTable[taskIdx].nextExecTime = Time_GetMillis() + postponeTimeMillis;
			
			return SCHEDULER_E_OK;
		}
	}

	return SCHEDULER_E_UNRECOGNIZED_NAME;
}

void Scheduler_SetOperatingLevel(Scheduler_OperatingLevel_t opLvl)
{
  sSystemScheduler.OperatingLevel = opLvl;
}

void Scheduler_Init(void)
{
	for(uint32_t taskIdx = 0; taskIdx < SCHEDULER_TASK_MAX_NBR; taskIdx++)
	{
		sSystemScheduler.taskParamTable[taskIdx].nextExecTime = 0;
		sSystemScheduler.taskParamTable[taskIdx].taskParam.name = NULL;
		sSystemScheduler.taskParamTable[taskIdx].taskParam.periodms = 0;
		sSystemScheduler.taskParamTable[taskIdx].taskParam.priority = 0;
		sSystemScheduler.taskParamTable[taskIdx].taskParam.functionHandler = NULL;
	}

	sSystemScheduler.activeTaskNbr = 0U;
	sSystemScheduler.OperatingLevel = SCHEDULER_OPLVL_NORMAL;
	
	Terminal_Log("Scheduler", LOG_OK, "Configured.");
}

void Scheduler_Loop(void)
{
	uint32_t timestamp = Time_GetMillis();
	
	for(uint32_t taskIdx = 0U; taskIdx < sSystemScheduler.activeTaskNbr; taskIdx++)
	{
		if(sSystemScheduler.taskParamTable[taskIdx].taskParam.operatingLevel <= sSystemScheduler.OperatingLevel
			&& sSystemScheduler.taskParamTable[taskIdx].taskEnable == true
			&& sSystemScheduler.taskParamTable[taskIdx].taskParam.functionHandler != NULL)
		{
			if(Time_GetMillis() >= sSystemScheduler.taskParamTable[taskIdx].nextExecTime)
			{
				sSystemScheduler.taskParamTable[taskIdx].nextExecTime = Time_GetMillis()
					+ sSystemScheduler.taskParamTable[taskIdx].taskParam.periodms;
				
				uint32_t execTimeMillis = Time_GetMillis();
				sSystemScheduler.taskParamTable[taskIdx].taskParam.functionHandler();
				sSystemScheduler.taskParamTable[taskIdx].taskExecTime = Time_GetMillis() - execTimeMillis;
			}
		}
	}
	
	sSchedulerExecutionTimeMillis = Time_GetMillis() - timestamp;
}

uint32_t Scheduler_GetExecutionTimeMillis(void)
{
	return sSchedulerExecutionTimeMillis;
}