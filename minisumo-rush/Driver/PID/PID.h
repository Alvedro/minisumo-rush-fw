#pragma once

#include "../../Util/Util.h"

typedef struct
{
	int32_t kpVal;
	int32_t kiVal; 
	int32_t kdVal;
	
	int32_t setpoint;
	int32_t difference;
	int32_t lastError;
	
	int32_t proportional;
	int32_t integral;
	int32_t derivative;
	
	int32_t proportionalOut;
	int32_t integralOut;
	int32_t derivativeOut;
	
	int32_t integralWindup;
	int32_t outputMin;
	int32_t outputMax;
	
	int32_t scale;
} PID_Instance_t;

int32_t ComputePidOutput(PID_Instance_t *pid, int32_t input);