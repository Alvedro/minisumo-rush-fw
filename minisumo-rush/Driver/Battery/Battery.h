#pragma once

#include "../../Util/Util.h"
#include "../Terminal/Terminal.h"
#include "../Scheduler/Scheduler.h"
#include "../../Util/Config.h"

#define BATTERY_ADC_PIN PA3

#define BATTERY_ADC_VREF_VOLT 5.0F
#define BATTERY_ADC_RESOLUTION 1023.0F
#define BATTERY_VOLTAGE_DIVIDER_TOP_RESISTOR_OHM 10000.0F
#define BATTERY_VOLTAGE_DIVIDER_BOTTOM_RESISTOR_OHM 4700.0F

typedef struct
{
	uint8_t intPart;
	uint8_t decPartFirstDig;
	uint8_t decPartSecDig;
	uint16_t adcRaw;
	float adcVoltage;
	bool initMeasureDone;
} Battery_Voltage_t;

void Battery_Init(void);
Battery_Voltage_t Battery_GetVoltage(void);