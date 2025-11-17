#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

void Log(int severity, const char* logMessage, ...)
{
    va_list args;
    va_start(args, logMessage);
    
    char* severityLookup[6] = {"Debug", "Trace", "Info", "Warn", "Error", "Fatal"};

    char logFormat[100];
    
    strcat(logFormat, severityLookup[severity]);
    strcat(logFormat, ": ");
    strcat(logFormat, logMessage);

    vprintf(logFormat, args);

    va_end(args);
}

void Debug   (const char* logMessage, ...) { va_list args; va_start(args, logMessage); Log(0,    logMessage, args); va_end(args); }
void Trace   (const char* logMessage, ...) { va_list args; va_start(args, logMessage); Log(1,    logMessage, args); va_end(args); }
void Info    (const char* logMessage, ...) { va_list args; va_start(args, logMessage); Log(2,    logMessage, args); va_end(args); }
void Warn    (const char* logMessage, ...) { va_list args; va_start(args, logMessage); Log(3,    logMessage, args); va_end(args); }
void Error   (const char* logMessage, ...) { va_list args; va_start(args, logMessage); Log(4,    logMessage, args); va_end(args); }
void Fatal   (const char* logMessage, ...) { va_list args; va_start(args, logMessage); Log(5,    logMessage, args); va_end(args); }