#include "Motor.h"

#define MOTOR_LEFT_PWM_PORT GPIO_PORT_B
#define MOTOR_LEFT_PWM_PIN GPIO_PIN_3
#define MOTOR_LEFT_DIR_A_PORT GPIO_PORT_A
#define MOTOR_LEFT_DIR_A_PIN GPIO_PIN_0
#define MOTOR_LEFT_DIR_B_PORT GPIO_PORT_A
#define MOTOR_LEFT_DIR_B_PIN GPIO_PIN_1

#define MOTOR_RIGHT_PWM_PORT GPIO_PORT_D
#define MOTOR_RIGHT_PWM_PIN GPIO_PIN_7
#define MOTOR_RIGHT_DIR_A_PORT GPIO_PORT_C
#define MOTOR_RIGHT_DIR_A_PIN GPIO_PIN_4
#define MOTOR_RIGHT_DIR_B_PORT GPIO_PORT_C
#define MOTOR_RIGHT_DIR_B_PIN GPIO_PIN_5

#define MOTOR_NBR_OF_MOTORS 2U
#define MOTOR_NBR_OF_DRIVER_OUTPUTS 3U

typedef enum
{
	PIN_PWM,
	PIN_DIR_A,
	PIN_DIR_B	
} MotDriverPinout_t;

typedef enum
{
	MOTOR_DIR_FORWARD,
	MOTOR_DIR_BACKWARD
} Motor_Direction_t;

static Motor_Config_t sMotor[MOTOR_NBR_OF_MOTORS][MOTOR_NBR_OF_DRIVER_OUTPUTS] =
{
	{
		{ .port =  MOTOR_LEFT_PWM_PORT,		.pin = MOTOR_LEFT_PWM_PIN},
		{ .port =  MOTOR_LEFT_DIR_A_PORT,	.pin = MOTOR_LEFT_DIR_A_PIN},
		{ .port =  MOTOR_LEFT_DIR_B_PORT,	.pin = MOTOR_LEFT_DIR_B_PIN}
	},
	{
		{ .port =  MOTOR_RIGHT_PWM_PORT,	.pin = MOTOR_RIGHT_PWM_PIN},
		{ .port =  MOTOR_RIGHT_DIR_A_PORT,	.pin = MOTOR_RIGHT_DIR_A_PIN},
		{ .port =  MOTOR_RIGHT_DIR_B_PORT,	.pin = MOTOR_RIGHT_DIR_B_PIN}
	}
};

static inline void SetMotorDirection(Motor_Id_t id, Motor_Direction_t dir);

void Motor_Init(void)
{
	for(uint8_t motIdx = 0U; motIdx < MOTOR_NBR_OF_MOTORS; motIdx++)
	{
		for(uint8_t pinIdx = 0U; pinIdx < MOTOR_NBR_OF_DRIVER_OUTPUTS; pinIdx++)
		{
			Gpio_Init(sMotor[motIdx][pinIdx].port, sMotor[motIdx][pinIdx].pin, GPIO_TYPE_OUTPUT);
		}
	}
	
	sMotor[MOTOR_ID_LEFT][PIN_PWM].portReg = &PORTB;
	sMotor[MOTOR_ID_LEFT][PIN_DIR_A].portReg = &PORTA;
	sMotor[MOTOR_ID_LEFT][PIN_DIR_B].portReg = &PORTA;
	
	sMotor[MOTOR_ID_RIGHT][PIN_PWM].portReg = &PORTD;
	sMotor[MOTOR_ID_RIGHT][PIN_DIR_A].portReg = &PORTC;
	sMotor[MOTOR_ID_RIGHT][PIN_DIR_B].portReg = &PORTC;
		
	TCCR0 &= ~(1 << CS01) & ~(1 << CS02) & ~(1 << COM00) & ~(1 << FOC0);
	TCCR0 |= (1 << CS00) | (1 << COM01) | (1 << WGM00) | (1 << WGM01);
		
	TCCR2 &=  ~(1 << CS22) & ~(1 << CS21) & ~(1 << COM20) & ~(1 << FOC2);
	TCCR2 |= (1 << CS20) | (1 << WGM20) | (1 << WGM21) | (1 << COM21);
	
	Terminal_Log("Motor", LOG_OK, "Configured.");
}

static inline void SetMotorDirection(Motor_Id_t id, Motor_Direction_t dir)
{
	if(dir == MOTOR_DIR_FORWARD)
	{
		*sMotor[id][PIN_DIR_A].portReg &= ~(1 << sMotor[id][PIN_DIR_A].pin);
		*sMotor[id][PIN_DIR_B].portReg |= (1 << sMotor[id][PIN_DIR_B].pin);
	}
	else
	{
		*sMotor[id][PIN_DIR_A].portReg |= (1 << sMotor[id][PIN_DIR_A].pin);
		*sMotor[id][PIN_DIR_B].portReg &= ~(1 << sMotor[id][PIN_DIR_B].pin);	
	}
}

void Motor_Run(Motor_Id_t id, int16_t speed)
{
	if(speed >= 255)
	{
		speed = 255;
	}
	else if(speed <= -255)
	{
		speed = -255;
	}
	
	if(speed > 0)
	{
		SetMotorDirection(id, MOTOR_DIR_FORWARD);
		
		if(id == MOTOR_ID_LEFT)
		{
			OCR0 = (uint8_t)speed;
		}
		else
		{
			OCR2 = (uint8_t)speed;	
		}
	}
	else if(speed < 0)
	{
		SetMotorDirection(id, MOTOR_DIR_BACKWARD);
		
		if(id == MOTOR_ID_LEFT)
		{
			OCR0 = (uint8_t)abs(speed);
		}
		else
		{
			OCR2 = (uint8_t)abs(speed);
		}
	}
	else
	{
		if(id == MOTOR_ID_LEFT)
		{
			OCR0 = 0U;
		}
		else
		{
			OCR2 = 0U;
		}
	}
}