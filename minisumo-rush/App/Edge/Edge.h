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
#include "../../Driver/Line/Line.h"
#include "../../Util/Time/Time.h"
#include "../../Driver/Led/Led.h"
#include "../../Driver/Motor/Motor.h"
#include "../Fight/Fight.h"

/*****************************************************************************
                       PUBLIC DEFINES / MACROS / ENUMS
*****************************************************************************/

typedef enum
{
	EDGE_MACHINE_STATE_ESTIMATE_ANGLE,
	EDGE_MACHINE_STATE_REVERSE_AND_ROTATE,
	EDGE_MACHINE_STATE_RETURN
} Edge_MachineState_t;

/*****************************************************************************
                         PUBLIC INTERFACE DECLARATION
*****************************************************************************/

void Edge_Handler(void);
void Edge_SetState(Edge_MachineState_t machineState);