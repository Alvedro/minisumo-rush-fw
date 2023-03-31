/*============================================================================*\
*
* Copyright Karol Domanski, All Rights Reserved
* It is not allowed to reproduce or utilize parts of this document in any form
* or by any means, including photocopying and microfilm, without permission in
* written by Karol Domanski.
*
\*============================================================================*/

#include "Time.h"
#include "../../Driver/Battery/Battery.h"

/*****************************************************************************
                     PRIVATE STRUCTS / ENUMS / VARIABLES
*****************************************************************************/

static uint32_t sTimer1Millis;

/*****************************************************************************
                           INTERFACE IMPLEMENTATION
*****************************************************************************/

ISR(TIMER1_COMPA_vect)
{
	sTimer1Millis++;
}

void Time_Init(void)
{
	uint32_t ctcMatchOverflow;

	ctcMatchOverflow = ((F_CPU / 1000U) / 8U);

	TCCR1B |= (1U << WGM12) | (1U << CS11);

	OCR1AH = (ctcMatchOverflow >> 8U);
	OCR1AL = ctcMatchOverflow;

	TIMSK |= (1 << OCIE1A);

	sei();
}

uint32_t Time_GetMillis(void)
{
	uint32_t millis;

	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		millis = sTimer1Millis;
	}
	
	return millis;
}