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
#include "../../Driver/Button/Button.h"
#include "../../Util/Time/Time.h"
#include "../../Driver/Led/Led.h"
#include "../../Driver/Scheduler/Scheduler.h"
#include "../../Driver/Terminal/Terminal.h"
#include "../../Driver/Distance/Distance.h"
#include "../../Driver/Line/Line.h"
#include "../../Driver/Battery/Battery.h"
#include "../../Driver/StartModule/StartModule.h"
#include "../../Driver/Motor/Motor.h"
#include "../../Driver/PID/PID.h"
#include "../Fight/Fight.h"
#include "../Tactic/Tactic.h"

/*****************************************************************************
                       PUBLIC DEFINES / MACROS / ENUMS
*****************************************************************************/

#define MACHINE_BUTTON_ACCEPT_TIME_MILLIS 1000U
#define MACHINE_BUTTON_DEBOUNCE_MILLIS 50U

typedef enum
{
	MACHINE_STATE_CHECK_BATTERY_LEVEL,
	MACHINE_STATE_CHECK_START_MODULE,
	MACHINE_STATE_DEVICE_STATUS_INDICATION,
	MACHINE_STATE_TACTICS_SETTING,
	MACHINE_STATE_WAIT_FOR_START,
	MACHINE_STATE_FIGHT,
	MACHINE_STATE_STOP
} Machine_MachineState_t;

typedef struct
{
	bool red;
	bool green;
	bool blue;
} Machine_LedMatrice_t;

typedef enum
{
	ROTATE_CW,
	ROTATE_CCW
} Machine_Rotate_t;

typedef enum
{
	TURN_AND_GO,
	TURN_AND_STOP
} Machine_TurnAction_t;

typedef struct
{
	Machine_Rotate_t rotate;
	uint16_t angleDeg;
	Machine_TurnAction_t action;
} Machine_StartPositionAction_t;

/*****************************************************************************
                         PUBLIC INTERFACE DECLARATION
*****************************************************************************/

void Machine_Init(void);
void Machine_SetState(Machine_MachineState_t state);