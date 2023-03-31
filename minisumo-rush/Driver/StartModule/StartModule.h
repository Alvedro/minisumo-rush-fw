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
#include "../../App/Machine/Machine.h"
#include "../../Util/Config.h"

/*****************************************************************************
                       PUBLIC DEFINES / MACROS / ENUMS
*****************************************************************************/

#define STARTMODULE_PORT GPIO_PORT_B
#define STARTMODULE_PIN GPIO_PIN_2

/*****************************************************************************
                         PUBLIC INTERFACE DECLARATION
*****************************************************************************/

void StartModule_Set(void);
void StartModule_Init(void);
bool StartModule_HasWrongState(void);
void StartModule_InitIrq(void);