/*============================================================================*\
*
* Copyright Karol Domanski, All Rights Reserved
* It is not allowed to reproduce or utilize parts of this document in any form
* or by any means, including photocopying and microfilm, without permission in
* written by Karol Domanski.
*
\*============================================================================*/

#include "Terminal.h"

/*****************************************************************************
                     PRIVATE STRUCTS / ENUMS / VARIABLES
*****************************************************************************/

#define BAUD_PRESCALLER (((F_CPU / (TERMINAL_BAUDRATE * 16))) - 1)

static Terminal_LogLevel_t sLogLevel;

static const char* sTerminal_ColorLevelString[] =
{
	RTT_CTRL_TEXT_CYAN"Debug"RTT_CTRL_TEXT_WHITE,
	RTT_CTRL_TEXT_WHITE" Info"RTT_CTRL_TEXT_WHITE,
	RTT_CTRL_TEXT_GREEN"   Ok"RTT_CTRL_TEXT_WHITE,
	RTT_CTRL_TEXT_MAGENTA" Warn"RTT_CTRL_TEXT_WHITE,
	RTT_CTRL_TEXT_RED"Error"RTT_CTRL_TEXT_WHITE,
	RTT_CTRL_TEXT_RED"Fatal"RTT_CTRL_TEXT_WHITE
};

/*****************************************************************************
                         PRIVATE FUNCTION DECLARATION
*****************************************************************************/

static void UsartSendByte(unsigned char data);
static void UsartSendString(char* stringPtr);

void Terminal_SetLevel(Terminal_LogLevel_t logLevel)
{
	sLogLevel = logLevel;	
}

Terminal_LogLevel_t Terminal_GetLevel(void)
{
	return sLogLevel;
}
void Terminal_Init(void)
{
	UBRRH = (uint8_t)(BAUD_PRESCALLER>>8);
	UBRRL = (uint8_t)(BAUD_PRESCALLER);
	UCSRB = (1<<RXEN)|(1<<TXEN);
	UCSRC = (1<<UCSZ0)|(1<<UCSZ1)|(1<<URSEL);
	
	if(sLogLevel != LOG_NONE)
	{
// 		UsartSendString("Rush starting...\r\n");	
	}
}

void Terminal_Log(const char *callerName, Terminal_LogLevel_t level, const char *szFormat, ...)
{
	if(level < sLogLevel)
	{
		return;
	}
	
	uint32_t timestamp = Time_GetMillis();
	
	char timestampBuff[8];
	sprintf(timestampBuff, "[%8lu] ", (uint32_t)timestamp);
	UsartSendString(timestampBuff);
	
	char levelBuff[5];
	sprintf(levelBuff, "[%s] ", sTerminal_ColorLevelString[level]);
	UsartSendString(levelBuff);
	
	char callerNameBuff[16];
	sprintf(callerNameBuff, "[%.16s] : ", callerName);
	UsartSendString((char *)callerNameBuff);
	
	char buffer[128];
	va_list args;
	va_start (args, szFormat);
	vsnprintf (buffer, 255, szFormat, args);
	UsartSendString((char *)buffer);
	va_end (args);
	
	char *eol = "\r\n";
	UsartSendString((char *)eol);
}

static void UsartSendByte(unsigned char data)
{
	while(!(UCSRA & (1 << UDRE)));
	UDR = data;
}

static void UsartSendString(char* stringPtr)
{
	while(*stringPtr != 0x00){
		UsartSendByte(*stringPtr);
	stringPtr++;}
}