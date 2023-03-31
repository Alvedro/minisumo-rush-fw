/*============================================================================*\
*
* Copyright Karol Domanski, All Rights Reserved
* It is not allowed to reproduce or utilize parts of this document in any form
* or by any means, including photocopying and microfilm, without permission in
* written by Karol Domanski.
*
\*============================================================================*/

#include "Util/Util.h"
#include "Util/Gpio/Gpio.h"
#include "Driver/Led/Led.h"
#include "Driver/StartModule/StartModule.h"
#include "Util/Time/Time.h"
#include "Driver/Scheduler/Scheduler.h"
#include "Driver/Distance/Distance.h"
#include "App/Machine/Machine.h"
#include "Driver/Button/Button.h"
#include "Driver/Motor/Motor.h"
#include "Driver/Battery/Battery.h"
#include "Driver/Terminal/Terminal.h"
#include "Driver/Line/Line.h"

/*****************************************************************************
                     PRIVATE STRUCTS / ENUMS / VARIABLES
*****************************************************************************/

#define TASK_NAME "Main"

/*****************************************************************************
                           INTERFACE IMPLEMENTATION
*****************************************************************************/

int main(void)
{
	EnterDebug();
	
	Terminal_Init();
	Time_Init();
	Scheduler_Init();
	
	Led_Init();
	StartModule_Init();
	Distance_Init();
	Line_Init();
	Motor_Init();
	Button_Init();
	Battery_Init();
	Machine_Init();

	Terminal_Log(TASK_NAME, LOG_OK, "Super loop...");
	
    while(1) 
    {
		Scheduler_Loop();
    }
}

