#include "Battery.h"

#define TASK_NAME "Battery"
#define TASK_PERIOD_MILLIS 10U

static uint8_t sAvgCnt;

static float sAdcToBattVoltageFactor;
static float sAvg;
static float sAvgOut;

static Battery_Voltage_t sVoltage;

static void TaskHandler(void);
static void Precalculation(void);

void Battery_Init(void)
{
    ADMUX |= (1 << REFS0);
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    ADCSRA |= (1 << ADEN);
	ADCSRA |= (1 << ADSC);
	ADMUX |= (1 << MUX0) | (1 << MUX1);
	DDRA &= ~(1 << BATTERY_ADC_PIN);
	
	Precalculation();
	
	Scheduler_AddTask(TASK_NAME, TASK_PERIOD_MILLIS, TaskHandler, SCHEDULER_OPLVL_NORMAL);
		
	Terminal_Log("Battery", LOG_OK, "Configured.");
}

Battery_Voltage_t Battery_GetVoltage(void)
{
	uint16_t intPart = (uint16_t)sAvgOut;
	uint16_t decimalPart = round((float)(sAvgOut - intPart) * 100.0F);

	if(decimalPart == 100)
	{
		intPart += 1;
		decimalPart = 0;
	}

	int decimalPartFirstDigit = decimalPart / 10;
	int decimalPartSecondDigit = decimalPart % 10;

	sVoltage.intPart = intPart;
	sVoltage.decPartFirstDig = decimalPartFirstDigit;
	sVoltage.decPartSecDig = decimalPartSecondDigit;
	sVoltage.adcRaw = ADCW;
	sVoltage.adcVoltage = sAvgOut;
	
	return sVoltage;
}

static void TaskHandler(void)
{
	ADMUX &= 0xF0;
	ADMUX |= 3;
	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC));
	
	float voltage = ADCW / sAdcToBattVoltageFactor;
	
	sAvg += voltage;
	sAvgCnt++;
	
	if(sAvgCnt >= BATTERY_AVG_NBR_OF_SAMPLES)
	{
		sAvgOut = sAvg / (float)sAvgCnt;
		sAvgCnt = 0;
		sAvg = 0.0F;
		sVoltage.initMeasureDone = true;
	}
}

static void Precalculation(void)
{
	float adcResolution = BATTERY_ADC_RESOLUTION / BATTERY_ADC_VREF_VOLT;	
	float voltageDividerFactor = (BATTERY_VOLTAGE_DIVIDER_TOP_RESISTOR_OHM + BATTERY_VOLTAGE_DIVIDER_BOTTOM_RESISTOR_OHM)
		/ BATTERY_VOLTAGE_DIVIDER_BOTTOM_RESISTOR_OHM;
	
	sAdcToBattVoltageFactor = adcResolution / voltageDividerFactor;
}
