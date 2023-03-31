#pragma once

#include "../../Util/Util.h"
#include "../../Util/Time/Time.h"
#include "../../Util/Config.h"

typedef enum
{
	LOG_DEBUG,
	LOG_INFO,
	LOG_OK,
	LOG_WARNING,
	LOG_ERROR,
	LOG_FATAL,
	LOG_NONE
} Terminal_LogLevel_t;

#define RTT_CTRL_TEXT_BLACK           "\x1B[2;30m"
#define RTT_CTRL_TEXT_RED             "\x1B[2;31m"
#define RTT_CTRL_TEXT_GREEN           "\x1B[2;32m"
#define RTT_CTRL_TEXT_YELLOW          "\x1B[2;33m"
#define RTT_CTRL_TEXT_BLUE            "\x1B[2;34m"
#define RTT_CTRL_TEXT_MAGENTA         "\x1B[2;35m"
#define RTT_CTRL_TEXT_CYAN            "\x1B[2;36m"
#define RTT_CTRL_TEXT_WHITE           "\x1B[2;37m"

void Terminal_SetLevel(Terminal_LogLevel_t logLevel);
Terminal_LogLevel_t Terminal_GetLevel(void);
void Terminal_Init(void);
void Terminal_Log(const char *callerName, Terminal_LogLevel_t level, const char *szFormat, ...);

