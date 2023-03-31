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
#include "../../Driver/Terminal/Terminal.h"
#include "../../Driver/Motor/Motor.h"
#include "../../Driver/Distance/Distance.h"
#include "../Fight/Fight.h"

/*****************************************************************************
                       PUBLIC DEFINES / MACROS / ENUMS
*****************************************************************************/

typedef enum
{
	TACTIC_MACHINE_STATE_ROTATING_CCW,
	TACTIC_MACHINE_STATE_ROTATE_CCW_AND_GO,
	TACTIC_MACHINE_STATE_FULL_ROTATE_AND_GO,
	TACTIC_MACHINE_STATE_ROTATE_CW_AND_GO,
	TACTIC_MACHINE_STATE_ROTATING_CW,
	TACTIC_MACHINE_STATE_ROTATE_AND_WAIT,
	TACTIC_MACHINE_STATE_GO,
	TACTIC_MACHINE_STATE_STANDBY
} Tactic_MachineState_t;

/*****************************************************************************
                         PUBLIC INTERFACE DECLARATION
*****************************************************************************/

void Tactic_Handler(void);
void Tactic_SetState(Tactic_MachineState_t machineState);
void Tactic_RunTestMode(void);
bool Tactic_IsTestMode(void);