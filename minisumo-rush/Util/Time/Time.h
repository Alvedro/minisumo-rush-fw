/*============================================================================*\
*
* Copyright Karol Domanski, All Rights Reserved
* It is not allowed to reproduce or utilize parts of this document in any form
* or by any means, including photocopying and microfilm, without permission in
* written by Karol Domanski.
*
\*============================================================================*/

#pragma once

#include "../Util.h"

/*****************************************************************************
                       PUBLIC DEFINES / MACROS / ENUMS
*****************************************************************************/

ISR(TIMER1_COMPA_vect);

/*****************************************************************************
                         PUBLIC INTERFACE DECLARATION
*****************************************************************************/

void Time_Init(void);
uint32_t Time_GetMillis(void);