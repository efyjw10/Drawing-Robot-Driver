#include "logger.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma region      ANSI Colour Codes 
// Codes for terminal colours taken from: https://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c
#define ANSI_COLOUR_RED     "\33[31m"
#define ANSI_COLOUR_GREEN   "\33[32m"
#define ANSI_COLOUR_YELLOW  "\33[33m"
#define ANSI_COLOUR_BLUE    "\33[34m"
#define ANSI_COLOUR_MAGENTA "\33[35m"
#define ANSI_COLOUR_CYAN    "\33[36m"
#define ANSI_COLOUR_GREY    "\33[90m"
#define ANSI_COLOUR_FATAL   "\33[41m"
#define ANSI_COLOUR_RESET   "\33[0m"
#pragma endregion

#define MAXLEN 255

char* FormatString(const char* fmt, enum LogLevel severity);

void LogInit()
{
    FILE* logFile = fopen(logPath, "w");
    fclose(logFile);
}

void FileLog(const char* log)
{
    FILE* logFile = fopen(logPath, "a");
    if (logFile == NULL) logFile = fopen(logPath, "w");
    if (logFile == NULL) return;

    fputs(log, logFile);
    fclose(logFile);

    Debug(log);
}

void Log(enum LogLevel severity, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    fmt = FormatString(fmt, severity);
    vprintf(fmt, args);
    va_end(args);   
}

char* FormatString(const char* fmt, enum LogLevel severity)
{
    char* SeverityNames[] = {"Trace", "Debug", "Info", "Warn", "Error", "Fatal", "Off"};
    char* SeverityColours[] = {ANSI_COLOUR_GREY, ANSI_COLOUR_BLUE, ANSI_COLOUR_GREEN, ANSI_COLOUR_YELLOW, ANSI_COLOUR_RED, ANSI_COLOUR_MAGENTA, ANSI_COLOUR_RESET};
    
    
    char* severityName = SeverityNames[severity];
    char* severityColour = SeverityColours[severity];

    char* offColour = SeverityColours[OFF];

    int logLength = strlen(severityColour) + strlen(severityName) + strlen(": ") + strlen(offColour) + strlen(fmt);

    char* formatString = calloc(logLength, sizeof(char));

    strcat(formatString, severityColour);
    strcat(formatString, severityName);
    strcat(formatString, ": ");
    strcat(formatString, offColour);
    strcat(formatString, fmt);

    return formatString;
}