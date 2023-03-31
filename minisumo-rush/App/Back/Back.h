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
#include "../../Driver/Terminal/Terminal.h"
#include "../../Util/Time/Time.h"
#include "../../Util/Config.h"
#include "../../Driver/Motor/Motor.h"
#include "../Fight/Fight.h"

/*****************************************************************************
                       PUBLIC DEFINES / MACROS / ENUMS
*****************************************************************************/

typedef enum
{
	BACK_MACHINE_STATE_REVERSE
} Back_MachineState_t;

/*****************************************************************************
                         PUBLIC INTERFACE DECLARATION
*****************************************************************************/

void Back_Handler(void);
void Back_SetState(Back_MachineState_t machineState);