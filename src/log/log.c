#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#pragma region      ANSI Colour Codes 
// Codes for terminal colours taken from: https://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c
#define ANSI_COLOR_RED     "\33[31m"
#define ANSI_COLOR_GREEN   "\33[32m"
#define ANSI_COLOR_YELLOW  "\33[33m"
#define ANSI_COLOR_BLUE    "\33[34m"
#define ANSI_COLOR_MAGENTA "\33[35m"
#define ANSI_COLOR_CYAN    "\33[36m"
#define ANSI_COLOR_GRAY    "\33[90m"
#define ANSI_COLOR_FATAL   "\33[41m"
#define ANSI_COLOR_RESET   "\33[0m"
#pragma endregion

#define PrefixLength 7

#pragma region      Private Function Declarations 
char* FormatLogMessage(enum LogSeverity severity, const char* logMessage);
#pragma endregion

void Debug(const char* logMessage, ...) 
{
    
    va_list args; va_start(args, logMessage); 
    char* logFormat = FormatLogMessage(debug, logMessage);
    #ifndef NDEBUG
    vprintf(logFormat, args);
    va_end(args);
    #endif
}

void Trace(const char* logMessage, ...) 
{ 
    va_list args; va_start(args, logMessage); 
    char* logFormat = FormatLogMessage(trace, logMessage);
    vprintf(logFormat, args);
    va_end(args); 
}

void Info(const char* logMessage, ...) 
{ 
    va_list args; va_start(args, logMessage); 
    char* logFormat = FormatLogMessage(info, logMessage);
    vprintf(logFormat, args);
    va_end(args); 
}

void Warn(const char* logMessage, ...) 
{ 
    va_list args; va_start(args, logMessage); 
    char* logFormat = FormatLogMessage(warn, logMessage);
    vprintf(logFormat, args);
    va_end(args); 
}

void Error(const char* logMessage, ...) 
{ 
    va_list args; va_start(args, logMessage); 
    char* logFormat = FormatLogMessage(error, logMessage);
    vprintf(logFormat, args);
    va_end(args); 
}

void Fatal(const char* logMessage, ...) 
{ 
    va_list args; va_start(args, logMessage); 
    char* logFormat = FormatLogMessage(fatal, logMessage);
    vprintf(logFormat, args);
    va_end(args); 
}
char* FormatLogMessage(enum LogSeverity severity, const char* logMessage)
{

    char* severityLookup[severityCount] =       {"Debug", "Trace", "Info", "Warn", "Error", "Fatal", "Off"};
    char* severityColourLookup[severityCount] = {   ANSI_COLOR_BLUE, ANSI_COLOR_GRAY, ANSI_COLOR_GREEN, ANSI_COLOR_YELLOW, ANSI_COLOR_RED, ANSI_COLOR_FATAL, ANSI_COLOR_RESET};

    int logFormatLength = PrefixLength + strlen(logMessage);
    char* logFormat = calloc(logFormatLength, sizeof(char));
    
    // Generate Log Format
    strcat(logFormat, severityColourLookup[severity]);
    strcat(logFormat, severityLookup[severity]);
    strcat(logFormat, ANSI_COLOR_RESET);
    strcat(logFormat, ": ");
    strcat(logFormat, logMessage);

    char* result = logFormat;


    return result;
}