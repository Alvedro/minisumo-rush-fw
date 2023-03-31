/*============================================================================*\
*
* Copyright Karol Domanski, All Rights Reserved
* It is not allowed to reproduce or utilize parts of this document in any form
* or by any means, including photocopying and microfilm, without permission in
* written by Karol Domanski.
*
\*============================================================================*/

#include "Gpio.h"

/*****************************************************************************
                           INTERFACE IMPLEMENTATION
*****************************************************************************/

void Gpio_Init(Gpio_Port_t port, Gpio_Pin_t pin, Gpio_Type_t type)
{
	if(port > GPIO_PORT_D)
	{
		return;
	}
	
	if(pin > GPIO_PIN_7)
	{
		return;
	}
	
	if(type > GPIO_TYPE_OUTPUT)
	{
		return;
	}
			
    switch(type)
    {
		case GPIO_TYPE_INPUT:
		{
			switch(port)
			{
				case GPIO_PORT_A:
				{
					DDRA = DDRA & (~(1 << pin));
					
					break;
				}
				
				case GPIO_PORT_B:
				{
					DDRB = DDRB & (~(1 << pin));
					
					break;
				}

				case GPIO_PORT_C:
				{
					DDRC = DDRC & (~(1 << pin));
					
					break;
				}
				
				case GPIO_PORT_D:
				{
					DDRD = DDRD & (~(1 << pin));
					
					break;
				}
				
				default:
				{
					break;
				}
			}
						
			break;	
		}
		
		case GPIO_TYPE_INPUT_PULL_UP:
		{
			switch(port)
			{
				case GPIO_PORT_A:
				{
					DDRA = DDRA & (~(1 << pin));
					PORTA = PORTA | (1 << pin);
								
					break;
				}
							
				case GPIO_PORT_B:
				{
					DDRB = DDRB & (~(1 << pin));
					PORTB = PORTB | (1 << pin);
								
					break;
				}

				case GPIO_PORT_C:
				{
					DDRC = DDRC & (~(1 << pin));
					PORTC = PORTC | (1 << pin);
								
					break;
				}
							
				case GPIO_PORT_D:
				{
					DDRD = DDRD & (~(1 << pin));
					PORTD = PORTD | (1 << pin);
								
					break;
				}
							
				default:
				{
					break;
				}
			}
			
			break;
		}
				
		case GPIO_TYPE_OUTPUT:
		{
			switch(port)
			{
				case GPIO_PORT_A:
				{
					DDRA = DDRA | (1 << pin);
					
				break;
				}
				
				case GPIO_PORT_B:
				{
					DDRB = DDRB | (1 << pin);
					
				break;
				}

				case GPIO_PORT_C:
				{
					DDRC = DDRC | (1 << pin);
					
				break;
				}
				
				case GPIO_PORT_D:
				{
					DDRD = DDRD | (1 << pin);
					
				break;
				}
				
				default:
				{
					break;
				}
			}
			break;
		}
		
		default:
		{
			break;
		}
    }
}

void Gpio_WritePin(Gpio_Port_t port, Gpio_Pin_t pin, bool state)
{
	if(pin > GPIO_PIN_7)
	{
		return;
	}
	
    switch(port)
    {
	    case GPIO_PORT_A:
		{
			if(state == true)
			{
				PORTA |= (1 << pin);
			}
			else
			{
				PORTA &= ~(1 << pin);
			}
			
			break;		
		}
		
	    case GPIO_PORT_B:
	    {
		    if(state == true)
		    {
			    PORTB |= (1 << pin);
		    }
		    else
		    {
			    PORTB &= ~(1 << pin);
		    }
		    
		    break;
	    }

	    case GPIO_PORT_C:
	    {
		    if(state == true)
		    {
			    PORTC |= (1 << pin);
		    }
		    else
		    {
			    PORTC &= ~(1 << pin);
		    }
		    
		    break;
	    }
		
		case GPIO_PORT_D:
		{
			if(state == true)
			{
				PORTD |= (1 << pin);
			}
			else
			{
				PORTD &= ~(1 << pin);
			}
			
			break;
		}
		
		default:
		{
			break;
		}
    }
}

void Gpio_TogglePin(Gpio_Port_t port, Gpio_Pin_t pin)
{
	if(pin > GPIO_PIN_7)
	{
		return;
	}
	
	switch(port)
	{
		case GPIO_PORT_A:
		{
			PORTA ^= (1 << pin);
			
			break;
		}
		
		case GPIO_PORT_B:
		{
			PORTB ^= (1 << pin);
			
			break;
		}

		case GPIO_PORT_C:
		{
			PORTC ^= (1 << pin);
			
			break;
		}
		
		case GPIO_PORT_D:
		{
			PORTD ^= (1 << pin);
			
			break;
		}
		
		default:
		{
			break;
		}
	}
}

bool Gpio_ReadPin(Gpio_Port_t port, Gpio_Pin_t pin)
{
	bool state = false;
	
	if(pin > GPIO_PIN_7)
	{
		return state;
	}
		
	switch(port)
	{
		case GPIO_PORT_A:
		{
			state = PINA & (1 << pin);
			
			break;
		}
		
		case GPIO_PORT_B:
		{
			state = PINB & (1 << pin);
			
			break;
		}

		case GPIO_PORT_C:
		{
			state = PINC & (1 << pin);
			
			break;
		}
		
		case GPIO_PORT_D:
		{
			state = PIND & (1 << pin);
			
			break;
		}
		
		default:
		{
			state = false;
			
			break;
		}
	}
	
	return state;
}