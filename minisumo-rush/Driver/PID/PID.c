#include "PID.h"

int32_t ComputePidOutput(PID_Instance_t *pid, int32_t input)
{
	pid->difference = pid->setpoint - input;
	
	pid->proportional = pid->difference;
	pid->integral += pid->difference;
	pid->derivative = pid->difference - pid->lastError;
	
	if (pid->integral >= pid->integralWindup)
	{
		pid->integral = pid->integralWindup;
	}
	else if (pid->integral <= -pid->integralWindup)
	{
		pid->integral = -pid->integralWindup;
	}
	
	pid->proportionalOut = pid->kpVal * pid->proportional;
	pid->integralOut = pid->kiVal * pid->integral;
	pid->derivativeOut = pid->kdVal * pid->derivative;
	
	int32_t output = (pid->proportionalOut
		+ pid->integralOut
		+ pid->derivativeOut) / pid->scale;
	
	pid->lastError = pid->difference;

	if (output >= pid->outputMax)
	{
		output = pid->outputMax;
	}
	else if (output <= pid->outputMin)
	{
		output = pid->outputMin;
	}
		
	return output;
}