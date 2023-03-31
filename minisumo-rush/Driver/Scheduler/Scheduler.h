/*============================================================================*\
*
* Copyright Karol Domanski, All Rights Reserved
* It is not allowed to reproduce or utilize parts of this document in any form
* or by any means, including photocopying and microfilm, without permission in
* written by Karol Domanski.
*
\*============================================================================*/

#pragma once

#include "../../Util/Util.h"
#include "../../Util/Time/Time.h"

/*****************************************************************************
                       PUBLIC DEFINES / MACROS / ENUMS
*****************************************************************************/

#define SCHEDULER_TASK_MAX_NBR 10U

typedef enum
{
	SCHEDULER_OPLVL_SLEEP,
	SCHEDULER_OPLVL_SUSPEND,
	SCHEDULER_OPLVL_NORMAL
} Scheduler_OperatingLevel_t;

typedef enum
{
	SCHEDULER_E_OK,
	SCHEDULER_E_MAX_TASK_NBR,
	SCHEDULER_E_UNRECOGNIZED_NAME,
	SCHEDULER_E_BUSY
} Scheduler_ErrorType_t;

typedef struct __attribute__((__packed__)) 
{
  void (*functionHandler)(void);
  uint32_t periodms;
  uint8_t priority;
  const char* name;
  Scheduler_OperatingLevel_t operatingLevel;
} Scheduler_TaskParamType_t;

/*****************************************************************************
                         PUBLIC INTERFACE DECLARATION
*****************************************************************************/

Scheduler_ErrorType_t Scheduler_AddTask(const char* taskName, uint32_t taskPeriod, void(*handler)(void), Scheduler_OperatingLevel_t operatingLvl);
Scheduler_ErrorType_t Scheduler_TaskEnable(char *taskName, bool enable);
Scheduler_ErrorType_t Scheduler_TaskPostpone(char *taskName, uint32_t postponeTimeMillis);
void Scheduler_SetOperatingLevel(Scheduler_OperatingLevel_t opLvl);
extern void Scheduler_Init(void);
extern void Scheduler_Loop(void);
uint32_t Scheduler_GetExecutionTimeMillis(void);