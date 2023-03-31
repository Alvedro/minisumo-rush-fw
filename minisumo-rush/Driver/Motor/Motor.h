#pragma once


#include "../../Util/Util.h"
#include "../../Util/Gpio/Gpio.h"
#include "../Terminal/Terminal.h"

typedef struct
{
	volatile uint8_t* portReg;
	Gpio_Port_t port;
	Gpio_Pin_t pin;
} Motor_Config_t;

typedef enum
{
	MOTOR_ID_LEFT,
	MOTOR_ID_RIGHT
} Motor_Id_t;

void Motor_Init(void);
void Motor_Run(Motor_Id_t id, int16_t speed);