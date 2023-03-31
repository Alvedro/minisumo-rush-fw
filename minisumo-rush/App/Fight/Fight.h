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
#include "../../Util/Config.h"
#include "../../Driver/Scheduler/Scheduler.h"
#include "../../Driver/Distance/Distance.h"
#include "../../Driver/Led/Led.h"
#include "../../Driver/Terminal/Terminal.h"
#include "../../Driver/PID/PID.h"
#include "../../Driver/Motor/Motor.h"
#include "../../Driver/Line/Line.h"
#include "../../App/Tactic/Tactic.h"
#include "../Edge/Edge.h"
#include "../Back/Back.h"

/*****************************************************************************
                       PUBLIC DEFINES / MACROS / ENUMS
*****************************************************************************/

typedef enum
{
	FIGHT_MACHINE_STATE_EXECUTE_TACTIC,
	FIGHT_MACHINE_STATE_LOOP,
	FIGHT_MACHINE_STATE_LINE,
	FIGHT_MACHINE_STATE_BACK
} Fight_MachineState_t;

/*****************************************************************************
                         PUBLIC INTERFACE DECLARATION
*****************************************************************************/

void Fight_Init(void);
void Fight_Handler(void);
void Fight_SetState(Fight_MachineState_t machineState);