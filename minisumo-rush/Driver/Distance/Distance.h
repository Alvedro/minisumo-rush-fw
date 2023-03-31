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
#include "../../Util/Gpio/Gpio.h"
#include "../../Driver/Terminal/Terminal.h"
#include "../Scheduler/Scheduler.h"
#include "../../Util/Config.h"

/*****************************************************************************
                       PUBLIC DEFINES / MACROS / ENUMS
*****************************************************************************/

#define DISTANCE_SENSOR_LEFT_PORT GPIO_PORT_C
#define DISTANCE_SENSOR_LEFT_PIN GPIO_PIN_6

#define DISTANCE_SENSOR_LEFT_DIAG_PORT GPIO_PORT_A
#define DISTANCE_SENSOR_LEFT_DIAG_PIN GPIO_PIN_5

#define DISTANCE_SENSOR_FRONT_PORT GPIO_PORT_A
#define DISTANCE_SENSOR_FRONT_PIN GPIO_PIN_2

#define DISTANCE_SENSOR_RIGHT_DIAG_PORT GPIO_PORT_A
#define DISTANCE_SENSOR_RIGHT_DIAG_PIN GPIO_PIN_6

#define DISTANCE_SENSOR_RIGHT_PORT GPIO_PORT_A
#define DISTANCE_SENSOR_RIGHT_PIN GPIO_PIN_7

#define DISTANCE_SENSOR_BACK_PORT GPIO_PORT_D
#define DISTANCE_SENSOR_BACK_PIN GPIO_PIN_2

typedef enum
{
	DIST_SENS_LEFT,
	DIST_SENS_LEFT_DIAG,
	DIST_SENS_FRONT,
	DIST_SENS_RIGHT_DIAG,
	DIST_SENS_RIGHT,
	DIST_SENS_FRONT_NBR
} Distance_SensorFrontId_t;

typedef enum
{
	DIST_SENS_BACK,
	DIST_SENS_BACK_NBR
} Distance_SensorBackId_t;

typedef struct
{
	volatile uint8_t* portReg;
	Gpio_Port_t port;
	Gpio_Pin_t pin;
	bool enable;
} Distance_Sensor_t;

typedef struct
{
	bool front[DIST_SENS_FRONT_NBR];
	bool back[DIST_SENS_BACK_NBR];
} Distance_Output_t;

/*****************************************************************************
                         PUBLIC INTERFACE DECLARATION
*****************************************************************************/

void Distance_Init(void);
Distance_Output_t Distance_GetFrontStateArray(void);
uint8_t Distance_GetNbrOfActFrontSensors(void);