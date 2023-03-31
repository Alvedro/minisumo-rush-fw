#pragma once

#include "../../Util/Util.h"
#include "../../Util/Gpio/Gpio.h"
#include "../../Driver/Terminal/Terminal.h"
#include "../../Driver/Scheduler/Scheduler.h"
#include "../../Util/Config.h"

#define LINE_SENSOR_FRONT_LEFT_PORT GPIO_PORT_C
#define LINE_SENSOR_FRONT_LEFT_PIN GPIO_PIN_3

#define LINE_SENSOR_FRONT_RIGHT_PORT GPIO_PORT_B
#define LINE_SENSOR_FRONT_RIGHT_PIN GPIO_PIN_0

#define LINE_SENSOR_BACK_LEFT_PORT GPIO_PORT_C
#define LINE_SENSOR_BACK_LEFT_PIN GPIO_PIN_2

#define LINE_SENSOR_BACK_RIGHT_PORT GPIO_PORT_B
#define LINE_SENSOR_BACK_RIGHT_PIN GPIO_PIN_4

typedef enum
{
	LINE_SENS_FRONT_LEFT,
	LINE_SENS_FRONT_RIGHT,
	LINE_SENS_BACK_LEFT,
	LINE_SENS_BACK_RIGHT,
	LINE_SENS_NBR
} Line_SensorId_t;

typedef struct
{
	bool state[LINE_SENS_NBR];
} Line_Output_t;

typedef struct
{
	volatile uint8_t* portReg;
	Gpio_Port_t port;
	Gpio_Pin_t pin;
	bool enable;
} Line_Sensor_t;

void Line_Init(void);
Line_Output_t Line_GetState(void);